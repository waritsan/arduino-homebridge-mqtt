/*
	SwitchExample.ino

	This is an example of a switch accessory. You can control switch on/off from iOS Home app.

	The circuit:
	* Outputs
        D1 - LED

	Created Jul 6, 2017
	By Warit Santaputra
	Modified Jun 8, 2019
	By Warit Santaputra

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WifiManager.h>
#include "ArduinoHomebridgeMqtt.h"

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48);
const int PORT = 1883;
const int OUTPUT_PIN = D1;
const char* SERVICE_NAME = "Light";
const char* SERVICE = "Switch";

WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

void operateSwitch(const char* serviceName, const char* characteristic, int value) {
  if (strcmp(serviceName, SERVICE_NAME) == 0) {
    digitalWrite(OUTPUT_PIN, value);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.setServer(MQTT_SERVER, PORT);
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(operateSwitch);
  arduinoHomebridgeMqtt.connect();
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
  arduinoHomebridgeMqtt.loop();
}