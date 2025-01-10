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

const int earthSensorPin = 0;
const int numberOfValuesInAvrege = 3;
const int earthInteruptAt = 5;
const int humInteruptAt = 5;
const int tempInteruptAt = 3;

int earthArray[numberOfValuesInAvrege];
int humArray[numberOfValuesInAvrege];
int tempArray[numberOfValuesInAvrege];

String isoDate = "";

enum class DataType {
  earth,
  hum,
  temp,
};

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
  // Serial.print("earthSensor: ");
  // Serial.println(value);
  return value;
}

int earthInterupt(const int latestEarthValue) {
  shiftArrayRight(earthArray, numberOfValuesInAvrege);
  earthArray[0] = latestEarthValue;
  return calculateAverage(numberOfValuesInAvrege, earthArray);
}

int humInterupt(const int latestHumValue) {
  shiftArrayRight(humArray, numberOfValuesInAvrege);
  humArray[0] = latestHumValue;
  return calculateAverage(numberOfValuesInAvrege, humArray);
}

int tempInterupt(const int latestTempValue) {
  shiftArrayRight(tempArray, numberOfValuesInAvrege);
  tempArray[0] = latestTempValue;
  return calculateAverage(numberOfValuesInAvrege, tempArray);
}

bool triggerEarthInterupt(const int latestEarthValue, const int interuptAt) {
  if (abs(latestEarthValue - earthArray[0]) >= interuptAt) {
    return true;
  }
  return false;
}

bool triggerHumInterupt(const int latestHumValue, const int interuptAt) {
  if (abs(latestHumValue - humArray[0] >= interuptAt)) {
    return true;
  }
  return false;
}

bool triggerTempInterupt(const int latestTempValue, const int interuptAt) {
  if (abs(latestTempValue - tempArray[0] >= interuptAt)) {
    return true;
  }
  return false;
}

int firebaseUpload(DataType dataType) {
  switch (dataType) {
  case DataType::earth:
    Serial.println("has uploaded earthData");
  return fb.setInt(("sensor/earth/" + isoDate + "/soil").c_str(), readEarthSensor());
  case DataType::hum:
    Serial.println("has uploaded humData");
  return fb.setInt(("sensor/air/hum/" + isoDate + "/moist").c_str(), getHum());
  case DataType::temp:
    Serial.println("has uploaded tempData");
  return fb.setInt(("sensor/air/temp/" + isoDate + "/C").c_str(), getTemp());
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
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(D5, D6);
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
}

void setup() {
  hardwereSetup();
  softwereSetup();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  checkIfAirSensorIsOnline();
  if(triggerEarthInterupt(readEarthSensor(), earthInteruptAt)) {
    updateTime();
    firebaseUpload(DataType::earth);
  }
  if(triggerHumInterupt(getHum(), humInteruptAt)) {
    updateTime();
    firebaseUpload(DataType::hum);
  }
  if(triggerTempInterupt(getTemp(), tempInteruptAt)) {
    updateTime();
    firebaseUpload(DataType::temp);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);
}