#include "secrets.h"
#include <FirebaseESP8266.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <Wire.h>
#include <WiFiUdp.h>

FirebaseData firebaseData;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const byte LightDimmerControl = D1;
const byte LightDir = D3;
const int LightSensorPin = A0;

bool lightIsOn = false;
bool lightManualOverride = false;
bool manualOn = false;
int lightDimmer = 0;
int lightTurnOnAt = 0;
int lightInteruptAt = 5;
int lightValue = 0;

int latestLightValue = 0;

String isoDate = "";

int i = 0;

void firebaseSetup()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void lightSetup()
{
  pinMode(LightDimmerControl, OUTPUT);
  digitalWrite(LightDir, HIGH);
}

void wifiSetup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void firebaseFetch()
{
  Firebase.getBool(firebaseData, "control/lights/manualOverride");
  lightManualOverride = firebaseData.boolData();
  Firebase.getBool(firebaseData, "control/lights/manualOn");
  manualOn = firebaseData.boolData();
  Firebase.getInt(firebaseData, "control/lights/turnOnAt");
  lightTurnOnAt = firebaseData.intData();
  Firebase.getInt(firebaseData, "control/lights/dimmer");
  lightDimmer = map(firebaseData.intData(), 0, 100, 1, 1024);
}



void timeSetup()
{
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}

void setup() {
  Serial.begin(9600);
  lightSetup();
  firebaseSetup();
  timeSetup();
  wifiSetup();
}

int getLightValue()
{
  int lightValue = map(constrain(analogRead(LightSensorPin), 0, 470), 0, 470, 1, 100);
  Serial.print("Light value: ");
  Serial.println(lightValue);
  return lightValue;
}

void turnLightOn(const int lightValue)
{
  if(manualOn && lightManualOverride)
  {
    analogWrite(LightDimmerControl, lightDimmer);
  }
  else
  {
    analogWrite(LightDimmerControl, map(constrain(lightValue, 1, 40), 40, 1, 1, 1024));
  }
  lightIsOn = true;
  Firebase.setBool(firebaseData, "control/lights/isOn", lightIsOn);
}

void turnLightOff()
{
  analogWrite(LightDimmerControl, 0);
  lightIsOn = false;
  Firebase.setBool(firebaseData, "control/lights/isOn", lightIsOn);
}

bool triggerLightInterupt(const int newLightValue, const int latestLightValue , const int lightInteruptAt)
{
  if(abs(newLightValue - latestLightValue) >= lightInteruptAt)
  {
    return true;
  }
  return false;
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

int firebaseUpload()
{
  Serial.println(lightValue);
  return Firebase.setInt(firebaseData, "sensor/light/" + isoDate + "/light", lightValue);
}

void light()
{
  if (lightManualOverride) {
    if (manualOn) {
      turnLightOn(lightValue);
      return;
    }
    else {
      turnLightOff();
      return;
    }
  }
  else if (lightValue <= lightTurnOnAt) {
    turnLightOn(lightValue);
    return;
  }
  else {
    turnLightOff();
    return;
  }
}

void lightInterupt()
{
  if(triggerLightInterupt(lightValue, latestLightValue , lightInteruptAt))
  {
    updateTime();
    firebaseUpload();
    latestLightValue = lightValue;
  }
}

void loop() {
  lightValue = getLightValue();
  if(i == 10 || i == 0)
  {
    firebaseFetch();
    i = 1;
  }

  lightInterupt();
  light();
  i++;
  delay(100);
}