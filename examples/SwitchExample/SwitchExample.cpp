/*
	SwitchExample.ino

	This is an example of a switch accessory. You can control switch on/off from iOS Home app.

	The circuit:
	* Outputs
        D1 - LED
*/

#include <Arduino.h>
#include <WiFiManager.h>  
#include <ArduinoHomebridgeMqtt.h>        

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48);
const int OUTPUT_PIN = D1;
const char* NAME = "flex_lamp";
const char* SERVICE_NAME = "Light";
const char* SERVICE = "Switch";

WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

void callback(const char* name, const char* serviceName, const char* characteristic, int value) {
  if (strcmp(name, NAME) == 0) {
    if (strcmp(serviceName, SERVICE_NAME) == 0) {
      digitalWrite(OUTPUT_PIN, value);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(NAME, SERVICE_NAME, SERVICE);
  arduinoHomebridgeMqtt.getAccessory(NAME);
}

void loop() {
  arduinoHomebridgeMqtt.loop();
}
