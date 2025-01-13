/*
  ---------------------------------
  IMPORTANT: Configuration Reminder
  ---------------------------------
  
  Before running this code, make sure to check the "secrets.h" file
  for important configuration details such as Wi-Fi credentials and 
  Firebase settings.

  The "secrets.h" file should include:
  - Your Wi-Fi SSID and Password
  - Your Firebase Realtime Database URL
  - (OPTIONAL) Firebase Authentication Token

  Ensure that "secrets.h" is properly configured and includes the correct
  information for your project. Failure to do so may result in connection
  errors or incorrect behavior of your application.

  Note: The "secrets.h" file should be located in the same directory as
  this sketch.
*/

#include "secrets.h"
#include <Firebase.h>
#include <NTPClient.h>
#include <Wire.h>
#include <AM2320.h>
#include <WiFiUdp.h>

Firebase fb(REFERENCE_URL, AUTH_TOKEN);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
AM2320 airSensor;

const byte pumpSpeedControl = D1;
const byte pumpDirection = D3;
const byte fanSpeedControl = D2;
const byte fanDirection = D4;

unsigned long startMillis;
unsigned long currentMillis;
int timeBetweenFetches = 5 * 1000;

const int earthSensorPin = 0;
const int numberOfValuesInAvrege = 3;
const int earthInteruptAt = 5;
const int humInteruptAt = 5;
const int tempInteruptAt = 3;

int earthArray[numberOfValuesInAvrege];
int humArray[numberOfValuesInAvrege];
int tempArray[numberOfValuesInAvrege];

int earthValue;
int humValue;
int tempValue;

bool fanIsOn = false;
bool fanManualOverride = false;
int fanSpeed = 0;
int fanManualSpeed = 0;
int fanTurnOnAtHum = 0;
int fanTurnOnAtTemp = 0;

bool pumpIsOn = false;
bool pumpManualOverride = false;
int pumpSpeed = 0;
int pumpManualSpeed = 0;
int pumpTurnOnAtSoil = 0;
int pumpTimeOn = 30 * 1000;

bool trapDoorIsOpen = false;
bool trapDoorManualOverride = false;
int trapDoorTurnOnAtHum = 0;
int trapDoorTurnOnAtTemp = 0;

int maxSpeed = 1020;

int i = 0;

String isoDate = "";

enum class DataType {
  earth,
  hum,
  temp,
};

void firebaseFetch() {
  fanManualOverride = fb.getBool("control/fan/manualOverride");
  fanManualSpeed = fb.getInt("control/fan/manualSpeed");
  fanTurnOnAtHum = fb.getInt("control/fan/turnOnAtHum");
  fanTurnOnAtTemp = fb.getInt("control/fan/turnOnAtTemp");

  pumpManualOverride = fb.getBool("control/pump/manualOverride");
  pumpManualSpeed = fb.getInt("control/pump/speed");
  pumpTurnOnAtSoil = fb.getInt("control/pump/turnOnAtSoil");

  trapDoorManualOverride = fb.getBool("control/trapDoor/manualOverride");
  trapDoorTurnOnAtHum = fb.getInt("control/trapDoor/turnOnAtHum");
  trapDoorTurnOnAtTemp = fb.getInt("control/trapDoor/turnOnAtTemp");
}

void motorSetup() {
  pinMode(pumpSpeed, OUTPUT);
  pinMode(pumpDirection, HIGH);

  pinMode(fanSpeed, OUTPUT);
  pinMode(fanDirection, HIGH);
}

int getTemp() {
  if(airSensor.measure()) {
    return airSensor.getTemperature();
  }
  else {
    switch (airSensor.getErrorCode()) {
    case 1:
      Serial.println("ERR: Sensor is offline");
      break;
    case 2:
      Serial.println("ERR: CRC validation failed");
      break;
    }
  }
  Serial.println("ERR: Unknown error getting temperature");
  return 0;
}

int getHum() {
  if(airSensor.measure()) {
    return airSensor.getHumidity();
  }
  else {
    switch (airSensor.getErrorCode()) {
    case 1:
      Serial.println("ERR: Sensor is offline");
      break;
    case 2:
      Serial.println("ERR: CRC validaion failed");
      break;
    }
  }
  Serial.println("ERR: Unknown error getting humidity");
  return 0;
}

void timeSetup() {
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}

void updateTime() {
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();

    struct tm ptm;
    gmtime_r(&epochTime, &ptm);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02dZ",
             ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday,
             timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());

    isoDate = String(buffer);
}

int calculateAverage(const int numberOfValuesInAverage, const int Values[]) {
  int sum = 0;
  for (int i = 0; i < numberOfValuesInAverage; i++){
    sum += Values[i];
  }
  return sum / numberOfValuesInAverage;
}

void shiftArrayRight(int array[], const int size) {
  for (int i = size - 1; i > 0; i--) {
    array[i] = array[i - 1];
  }
}

int readEarthSensor() {
  int rawSensorValue = analogRead(earthSensorPin);
  int value = map(rawSensorValue, 0, 1023, 100, -1);
  return value;
}

int earthInterupt(const int latestEarthValue) {
  shiftArrayRight(earthArray, numberOfValuesInAvrege);
  earthArray[0] = latestEarthValue;
  return earthArray[0];
}

int humInterupt(const int latestHumValue) {
  shiftArrayRight(humArray, numberOfValuesInAvrege);
  humArray[0] = latestHumValue;
  return humArray[0];
}

int tempInterupt(const int latestTempValue) {
  shiftArrayRight(tempArray, numberOfValuesInAvrege);
  tempArray[0] = latestTempValue;
  return tempArray[0];
}

bool triggerEarthInterupt(const int latestEarthValue, const int interuptAt) {
  if (abs(latestEarthValue - earthArray[0]) >= interuptAt) {
    return true;
  }
  return false;
}

bool triggerHumInterupt(const int latestHumValue, const int interuptAt) {
  if (abs(latestHumValue - humArray[0]) >= interuptAt) {
    return true;
  }
  return false;
}

bool triggerTempInterupt(const int latestTempValue, const int interuptAt) {
  if (abs(latestTempValue - tempArray[0]) >= interuptAt) {
    return true;
  }
  return false;
}

int firebaseUpload(DataType dataType) {
  switch (dataType) {
  case DataType::earth:
    Serial.println("has uploaded earthData");
  return fb.setInt(("sensor/earth/" + isoDate + "/soil").c_str(), earthValue);
  case DataType::hum:
    Serial.println("has uploaded humData");
  return fb.setInt(("sensor/air/hum/" + isoDate + "/moist").c_str(), humValue);
  case DataType::temp:
    Serial.println("has uploaded tempData");
  return fb.setInt(("sensor/air/temp/" + isoDate + "/C").c_str(), tempValue);
  };
  return 0;
}

void wifiSetup() {
  WiFi.disconnect();
  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println();
}

void hardwereSetup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(D5, D6);
  motorSetup();
}

void checkIfAirSensorIsOnline() {
  if(airSensor.measure()) {
    // Serial.println("Sensor is online");
    return;
  }
  switch (airSensor.getErrorCode()) {
  case 1:
    Serial.println("ERR: Sensor is offline");
    delay(1000);
    break;
  case 2:
    Serial.println("ERR: CRC validaion failed");
    delay(1000);
    break;
  }
}

void softwereSetup() {
  wifiSetup();
  timeSetup();
  startMillis = millis();
}

void setup() {
  hardwereSetup();
  softwereSetup();
  // analogWrite(pumpSpeedControl, maxSpeed);
}

void pump() {
  if (pumpManualOverride) {
    if (pumpManualSpeed > 0) {
      analogWrite(pumpSpeedControl, pumpManualSpeed);
      pumpIsOn = true;
    }
    else {
      analogWrite(pumpSpeedControl, 0);
      pumpIsOn = false;
    }
  }
  else if (earthValue >= pumpTurnOnAtSoil) {
    Serial.println("Pump is on");
    analogWrite(pumpSpeedControl, maxSpeed);
    pumpIsOn = true;
    // analogWrite(pumpSpeed, 0);
  }
  else {
    analogWrite(pumpSpeedControl, 0);
    pumpIsOn = false;
  }
}

void fan() {
  if (fanManualOverride) {
    if (fanManualSpeed > 0) {
      Serial.println("Fan is on");
      analogWrite(fanSpeedControl, fanManualSpeed);
      fanIsOn = true;
      fb.setBool("control/fan/isOn", fanIsOn);
    }
    else {
      analogWrite(fanSpeed, 0);
      fanIsOn = false;
      fb.setBool("control/fan/isOn", fanIsOn);
    }
  }
  else if (humValue >= fanTurnOnAtHum || tempValue >= fanTurnOnAtTemp) {
      Serial.println("Fan is on");
      analogWrite(fanSpeedControl, maxSpeed);
      fanIsOn = true;
      fb.setBool("control/fan/isOn", fanIsOn);
    }
  else {
    analogWrite(fanSpeedControl, 0);
    fanIsOn = false;
    fb.setBool("control/fan/isOn", fanIsOn);
  }
}

void loop() {
  currentMillis = millis();
  checkIfAirSensorIsOnline();
  earthValue = readEarthSensor();
  humValue = getHum();
  tempValue = getTemp();
  if (currentMillis - startMillis >= timeBetweenFetches || i == 0) {
    startMillis = currentMillis;
    firebaseFetch();
  }
  if(triggerEarthInterupt(earthValue, earthInteruptAt)) {
    digitalWrite(LED_BUILTIN, HIGH);
    updateTime();
    shiftArrayRight(earthArray, numberOfValuesInAvrege);
    earthArray[0] = earthValue;
    firebaseUpload(DataType::earth);
    digitalWrite(LED_BUILTIN, LOW);
  }
  if(triggerHumInterupt(humValue, humInteruptAt)) {
    digitalWrite(LED_BUILTIN, HIGH);
    updateTime();
    shiftArrayRight(humArray, numberOfValuesInAvrege);
    humArray[0] = humValue;
    firebaseUpload(DataType::hum);
    digitalWrite(LED_BUILTIN, LOW);
  }
  if(triggerTempInterupt(tempValue, tempInteruptAt)) {
    digitalWrite(LED_BUILTIN, HIGH);
    updateTime();
    shiftArrayRight(tempArray, numberOfValuesInAvrege);
    tempArray[0] = tempValue;
    firebaseUpload(DataType::temp);
    digitalWrite(LED_BUILTIN, LOW);
  }
  fan();
  pump();
  
  i++;
  delay(100);
}