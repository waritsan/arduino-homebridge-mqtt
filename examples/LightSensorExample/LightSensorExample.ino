/*
	LightSensorExample.ino

	This example shows how to read light intensity (lux) with a photoresistor and sends the value to Homeberidge.

	The circuit:
	* Input
        A0 - Photoresistor
*/

#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48);
const int INPUT_PIN = A0;
const char* LIGHT_SENSOR_NAME = "LightSensor";
const char* LIGHT_SENSOR_SERVICE = "LightSensor";
const char* CURRENT_AMBIENT_LIGHT_LEVEL = "CurrentAmbientLightLevel";

long lastMsg = 0;
WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

float readAmbientLight(int inputPin) {
  int value = analogRead(inputPin);
  return exp(float(value)/80.0);
}

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float ambientLight = readAmbientLight(INPUT_PIN);
    Serial.print("Ambient light: ");
    Serial.print(ambientLight);
    Serial.println(" lux");
    arduinoHomebridgeMqtt.setValueToHomebridge(LIGHT_SENSOR_NAME, CURRENT_AMBIENT_LIGHT_LEVEL, ambientLight);
  }
  arduinoHomebridgeMqtt.loop();
}
