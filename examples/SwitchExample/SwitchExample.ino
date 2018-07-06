/*
	SwitchExample.ino

	This is an example of a switch accessory. You can control switch on/off from iOS Home app.

	The circuit:
	* Outputs
        D1 - LED

	Created Jul 6, 2017
	By Warit Santaputra
	Modified Jul 6, 2018
	By Warit Santaputra

*/

#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 40);
const int OUTPUT_PIN = D1;
const String SERVICE_NAME = "Switch";
const String SERVICE = "Switch";

WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

void callback(String serviceName, String characteristic, bool value) {
  if (serviceName == SERVICE_NAME) {
    if (value) {
      digitalWrite(OUTPUT_PIN, HIGH);
      Serial.println("On");
    } else {
      digitalWrite(OUTPUT_PIN, LOW);
      Serial.println("Off");
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(SERVICE_NAME, SERVICE);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {}