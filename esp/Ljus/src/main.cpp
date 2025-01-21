#include "secrets.h"
#include <FirebaseESP8266.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <Wire.h>
#include <WiFiUdp.h>

const byte lightSpeedControl = D1;
const byte lightDimmerControl = D3;

void setup() {
  pinMode(lightSpeedControl, OUTPUT);
  digitalWrite(lightDimmerControl, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(lightSpeedControl, 1024);
}

void loop() {
  
}