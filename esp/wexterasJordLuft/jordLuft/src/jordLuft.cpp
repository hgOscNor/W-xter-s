
#include "secrets.h"
#include <FirebaseESP8266.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <Wire.h>
#include <AM2320.h>
#include <WiFiUdp.h>
#include <Servo.h>

FirebaseData firebaseData;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
AM2320 airSensor;
Servo servo;

const byte pumpSpeedControl = D1;
const byte pumpDirection = D3;
const byte fanSpeedControl = D2;
const byte fanDirection = D4;
const byte servoPin = D7;
const byte humSensorPin = D5;
const byte tempSensorPin = D6;

const int earthSensorPin = 0;
const int numberOfValuesInAvrege = 3;
const int earthInteruptAt = 5;
const int humInteruptAt = 5;
const int tempInteruptAt = 3;

int earthHist = 0;
int humHist = 0;
int tempHist = 0;

int earthValue;
int humValue;
int tempValue;

bool fanIsOn = false;
bool fanManualOverride = false;
bool fanManualOn = false;
int fanSpeed = 0;
int fanManualSpeed = 0;
int fanTurnOnAtHum = 0;
int fanTurnOnAtTemp = 0;

bool pumpIsOn = false;
bool pumpManualOverride = false;
bool pumpManualOn = false;
int pumpSpeed = 0;
int pumpManualSpeed = 0;
int pumpTurnOnAtSoil = 0;
int pumpTimeOn = 30 * 1000;

bool trapdoorIsOpen = false;
bool trapdoorManualOverride = false;
bool trapdoorManualOn = false;
int trapdoorOpenAtHum = 0;
int trapdoorOpenAtTemp = 0;

int servoPos = 0;

int maxSpeed = 1024;

int i = 0;

String isoDate = "";

enum class DataType
{
  earth,
  hum,
  temp,
};

void firebaseFetch()
{
  Firebase.getBool(firebaseData, "control/fan/manualOverride");
  fanManualOverride = firebaseData.boolData();
  Firebase.getInt(firebaseData, "control/fan/manualSpeed");
  fanManualSpeed = map(firebaseData.intData(), 0, 100, 200, 1024);
  Firebase.getInt(firebaseData, "control/fan/turnOnAtHum");
  fanTurnOnAtHum = firebaseData.intData();
  Firebase.getInt(firebaseData, "control/fan/turnOnAtTemp");
  fanTurnOnAtTemp = firebaseData.intData();
  Firebase.getBool(firebaseData, "control/fan/manualOn");
  fanManualOn = firebaseData.boolData();
  Firebase.getBool(firebaseData, "control/pump/manualOverride");
  pumpManualOverride = firebaseData.boolData();
  Firebase.getInt(firebaseData, "control/pump/speed");
  pumpManualSpeed = map(firebaseData.intData(), 0, 100, 450, 1024);
  Firebase.getInt(firebaseData, "control/pump/turnOnAtSoil");
  pumpTurnOnAtSoil = firebaseData.intData();
  Firebase.getBool(firebaseData, "control/pump/manualOn");
  pumpManualOn = firebaseData.boolData();
  Firebase.getBool(firebaseData, "control/trapdoor/manualOverride");
  trapdoorManualOverride = firebaseData.boolData();
  Firebase.getInt(firebaseData, "control/trapdoor/openAtHum");
  trapdoorOpenAtHum = firebaseData.intData();
  Firebase.getInt(firebaseData, "control/trapdoor/openAtTemp");
  trapdoorOpenAtTemp = firebaseData.intData();
  Firebase.getBool(firebaseData, "control/trapdoor/manualOn");
  trapdoorManualOn = firebaseData.boolData();
}

void motorSetup()
{
  pinMode(pumpSpeedControl, OUTPUT);
  digitalWrite(pumpDirection, HIGH);

  pinMode(fanSpeedControl, OUTPUT);
  digitalWrite(fanDirection, HIGH);
}

int getTemp()
{
  if (airSensor.measure())
  {
    return airSensor.getTemperature();
  }
  else
  {
    switch (airSensor.getErrorCode())
    {
    case 1:
      Serial.println("ERR: Sensor is offline");
      break;
    case 2:
      Serial.println("ERR: CRC validation failed");
      break;
    }
  }
  Serial.println("ERR: Unknown error getting temperature");
  return 200;
}

int getHum()
{
  if (airSensor.measure())
  {
    return airSensor.getHumidity();
  }
  else
  {
    switch (airSensor.getErrorCode())
    {
    case 1:
      Serial.println("ERR: Sensor is offline");
      break;
    case 2:
      Serial.println("ERR: CRC validaion failed");
      break;
    }
  }
  Serial.println("ERR: Unknown error getting humidity");
  return 200;
}

void timeSetup()
{
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}

void updateTime()
{
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

int calculateAverage(const int numberOfValuesInAverage, const int Values[])
{
  int sum = 0;
  for (int i = 0; i < numberOfValuesInAverage; i++)
  {
    sum += Values[i];
  }
  return sum / numberOfValuesInAverage;
}

int readEarthSensor()
{
  int rawSensorValue = analogRead(earthSensorPin);
  int value = map(rawSensorValue, 0, 1023, 100, -1);
  return value;
}

bool triggerInterrupt(const int latestValue, const int hist, const int interuptAt)
{
  if (abs(latestValue - hist) >= interuptAt && latestValue != 200)
  {
    return true;
  }
  return false;
}

int firebaseUpload(DataType dataType)
{
  switch (dataType)
  {
  case DataType::earth:
    return Firebase.setInt(firebaseData, ("sensor/earth/" + isoDate + "/soil").c_str(), earthValue);
  case DataType::hum:
    return Firebase.setInt(firebaseData, ("sensor/air/hum/" + isoDate + "/moist").c_str(), humValue);
  case DataType::temp:
    return Firebase.setInt(firebaseData, ("sensor/air/temp/" + isoDate + "/C").c_str(), tempValue);
  };
  return 0;
}

void wifiSetup()
{
  WiFi.disconnect();
  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
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

void hardwereSetup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(humSensorPin, tempSensorPin);
  motorSetup();
}

void softwereSetup()
{
  wifiSetup();
  timeSetup();
  servo.attach(servoPin, 544, 2400);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void setup()
{
  hardwereSetup();
  softwereSetup();
}

void turnPumpOn()
{
  analogWrite(pumpSpeedControl, pumpManualSpeed);  
  pumpIsOn = true;
  Firebase.setBool(firebaseData, "control/pump/isOn", pumpIsOn);
}

void turnPumpOff()
{
  analogWrite(pumpSpeedControl, 0);
  pumpIsOn = false;
  Firebase.setBool(firebaseData, "control/pump/isOn", pumpIsOn);
}

void pump()
{
  if (pumpManualOverride)
  {
    pumpManualOn ? turnPumpOn() : turnPumpOff();
    return;
  }
  else if (earthValue >= pumpTurnOnAtSoil)
  {
    turnPumpOn();
  }
  else
  {
    turnPumpOff();
  }
}

void turnFanOn()
{
  analogWrite(fanSpeedControl, fanManualSpeed);
  fanIsOn = true;
  Firebase.setBool(firebaseData, "control/fan/isOn", fanIsOn);
}

void turnFanOff()
{
  analogWrite(fanSpeedControl, 0);
  fanIsOn = false;
  Firebase.setBool(firebaseData, "control/fan/isOn", fanIsOn);
}

void openTrapdoor()
{
if (!trapdoorIsOpen)
  { 
    if (trapdoorManualOverride && !trapdoorManualOn)
    {
      return;
    }
    else
    {
    servo.write(90);
    trapdoorIsOpen = true;
    Firebase.setBool(firebaseData, "control/trapdoor/open", trapdoorIsOpen);
    }
  }
}

void closeTrapdoor()
{
  if (trapdoorIsOpen)
  { 
    if (fanIsOn == false)
    {
    servo.write(0);
    trapdoorIsOpen = false;
    Firebase.setBool(firebaseData, "control/trapdoor/open", trapdoorIsOpen);
    }
  }
}

void fan()
{
  if (fanManualOverride)
  {
    if (fanManualOn)
    {
    turnFanOn();
    openTrapdoor();
    }
    else
    {
      turnFanOff();
    }
    return;
  }
  else if (humValue >= fanTurnOnAtHum || tempValue >= fanTurnOnAtTemp)
  {
    turnFanOn();
    openTrapdoor();
  }
  else if (((humValue <= fanTurnOnAtHum) || (tempValue <= fanTurnOnAtTemp)) && humValue != 200 && tempValue != 200)
  {
    turnFanOff();
  }
}

void trapdoor()
{
  if (trapdoorManualOverride == true)
  {
    trapdoorManualOn ? openTrapdoor() : closeTrapdoor();
    return;
  }
  else if (humValue >= trapdoorOpenAtHum || tempValue >= trapdoorOpenAtTemp)
  {
    openTrapdoor();
  }
  else
  {
    closeTrapdoor();
  }
}

void earthLoop() {
  if (triggerInterrupt(earthValue, earthHist, earthInteruptAt))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    updateTime();
    earthHist = earthValue;
    firebaseUpload(DataType::earth);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void humLoop() {
  if (triggerInterrupt(humValue, humHist, humInteruptAt))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    updateTime();
    humHist = humValue;
    firebaseUpload(DataType::hum);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void tempLoop() {
  if (triggerInterrupt(tempValue, tempHist, tempInteruptAt))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    updateTime();
    tempHist = tempValue;
    firebaseUpload(DataType::temp);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop()
{
  earthValue = readEarthSensor();
  humValue = getHum();
  tempValue = getTemp();
  
  firebaseFetch();
  earthLoop();
  humLoop();
  tempLoop();
  fan();
  pump();
  trapdoor();

  i++;
  delay(100);
}