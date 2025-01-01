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

Firebase fb(REFERENCE_URL, AUTH_TOKEN);

const int earthSensorPin = 0;

void firebase();
void wifiSetup();
void hardwereSetup();
int readEarthSensor();

void setup() {
  hardwereSetup();
  delay(3000);
  Serial.print("has started");
  wifiSetup();
  Serial.println("Test");
}

void loop() {
  readEarthSensor();
  delay(1000);
}

int readEarthSensor() {
  int rawSensorValue = analogRead(earthSensorPin);
  int procentSensorValue =  map(rawSensorValue, 0, 1023, 100, -1);
  Serial.print("earthSensor: ");
  Serial.println(procentSensorValue);
  return procentSensorValue;
}

void wifiSetup() {
  WiFi.disconnect();
  delay(1000);

  /* Connect to WiFi */
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

void firebase() {
  /*
    Set String, Int, Float, or Bool in Firebase
    
    Parameters:
      - path: The path in Firebase where the data will be stored.
      - data: The value to set, which can be of type String, Int, Float, or Bool.

    Returns:
      - HTTP response code as an integer.
        - 200 indicates success.
        - Other codes indicate failure.
  */
  Serial.println("Started firebase func");

  if (fb.setString("Example/myString", "Cool!") == 200){ 
    Serial.println("succes");
  }
  
  fb.setInt("Example/myInt", 123);
  fb.setFloat("Example/myFloat", 45.67);
  fb.setBool("Example/myBool", true);
  fb.pushString("Push", "Foo-Bar");
  fb.pushInt("Push", 890);
  fb.pushFloat("Push", 12.34);
  fb.pushBool("Push", false);

  /*
    Get String, Int, Float, or Bool from Firebase
    
    Parameters:
      - path: The path in Firebase from which the data will be retrieved.

    Returns:
      - The value retrieved from Firebase as a String, Int, Float, or Bool.
      - If the HTTP response code is not 200, returns NULL (for String) or 0 (for Int, Float, Bool).
  */
  String retrievedString = fb.getString("Example/myString");
  Serial.print("Retrieved String:\t");
  Serial.println(retrievedString);

  int retrievedInt = fb.getInt("Example/myInt");
  Serial.print("Retrieved Int:\t\t");
  Serial.println(retrievedInt);

  float retrievedFloat = fb.getFloat("Example/myFloat");
  Serial.print("Retrieved Float:\t");
  Serial.println(retrievedFloat);

  bool retrievedBool = fb.getBool("Example/myBool");
  Serial.print("Retrieved Bool:\t\t");
  Serial.println(retrievedBool);

  /*
    Remove Data from Firebase
    
    Parameters:
      - path: The path in Firebase from which the data will be removed.

    Returns:
      - HTTP response code as an integer.
        - 200 indicates success.
        - Other codes indicate failure.
  */
  fb.remove("Example");
  fb.remove("Push");
  }