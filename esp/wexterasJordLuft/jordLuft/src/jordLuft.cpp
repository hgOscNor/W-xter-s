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
const int earthInteruptAt = 2;
const int humInteruptAt = 5;
const int tempInteruptAt = 2;

int earthArray[numberOfValuesInAvrege];
int humArray[numberOfValuesInAvrege];
int tempArray[numberOfValuesInAvrege];

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
  return 0;
}

void timeSetup() {
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}

void updateTime() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
}

int calculateAverage(const int numberOfValues, const int Values[]) {
  int sum = 0;
  for (int i = 0; i < numberOfValues; i++){
    sum += Values[i];
  }
  return sum / numberOfValues;
}

void shiftArrayRight(int array[], const int size) {
  for (int i = size - 1; i > 0; i--) {
    array[i] = array[i - 1];
  }
}

int readEarthSensor() {
  int rawSensorValue = analogRead(earthSensorPin);
  int value = map(rawSensorValue, 0, 1023, 100, -1);
  Serial.print("earthSensor: ");
  Serial.println(value);
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
    return fb.setInt("sensor/earth/" + timeClient.getFormattedTime() + "/hum", earthInterupt(readEarthSensor()));
  break;
  case DataType::hum:
    Serial.println("has uploaded humData");
    return fb.setInt("sensor/air/hum/" + timeClient.getFormattedTime() + "/%", humInterupt(getHum()));
  break;
  case DataType::temp:
    Serial.println("has uploaded tempData");
    return fb.setInt("sensor/air/temp/" + timeClient.getEpochTime(), tempInterupt(getTemp()));
  }
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
}

void softwereSetup() {
  // wifiSetup();
  // timeSetup();
}

void setup() {
  hardwereSetup();
  softwereSetup();
}

void loop() {
  if(triggerEarthInterupt(earthArray[0], earthInteruptAt)) {
    firebaseUpload(DataType::earth);
  }
  if(triggerHumInterupt(humArray[0], humInteruptAt)) {
    firebaseUpload(DataType::hum);
  }
  if(triggerTempInterupt(tempArray[0], tempInteruptAt)) {
    firebaseUpload(DataType::temp);
  }
  delay(10);
}