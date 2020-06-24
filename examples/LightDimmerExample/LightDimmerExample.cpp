/*
	This scetch uses RobotDyn light dimmer module.
	ESP32
*/


#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <RBDdimmer.h> // https://github.com/RobotDynOfficial/RBDDimmer.git
#include <ArduinoHomebridgeMqtt.h>

#define OUTPUT_PIN 4
#define ZERO_CROSS 2

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48);
const char* NAME = "night_lamp";
const char* SERVICE_NAME = "Night Lamp";
const char* SERVICE = "Lightbulb";
const char* C_ON = "On";
const char* C_BRIGHTNESS = "Brightness";

WebServer Server;
AutoConnect Portal(Server);
dimmerLamp dimmer(OUTPUT_PIN, ZERO_CROSS);
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;
int currentBrighness = 0;

void callback(const char* name, const char* serviceName, const char* characteristic, int value) {
  if (strcmp(name, NAME) == 0) {
    if (strcmp(serviceName, SERVICE_NAME) == 0) {
			if (strcmp(characteristic, C_ON) == 0) {
				if (value == 0) {
					dimmer.setPower(0);
					Serial.println("Off");
				} else {
					dimmer.setPower(currentBrighness);
					Serial.println(currentBrighness);
				}
			} else if (strcmp(characteristic, C_BRIGHTNESS) == 0) {
				currentBrighness = value;
				dimmer.setPower(currentBrighness);
				Serial.println(currentBrighness);
			}
    }
  }
}

void setup() {
  Serial.begin(9600);
	dimmer.begin(NORMAL_MODE, ON);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
	Portal.handleClient();
	arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(NAME, SERVICE_NAME, SERVICE);
  arduinoHomebridgeMqtt.getAccessory(NAME);
}

void loop() {
	arduinoHomebridgeMqtt.loop();
}