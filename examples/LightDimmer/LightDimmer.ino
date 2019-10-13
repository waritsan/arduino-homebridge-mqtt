#include <Arduino.h>
#include <WiFiManager.h>
#include <ArduinoHomebridgeMqtt.h>

const IPAddress mqttServer = IPAddress(192, 168, 1, 42);
const int led = D1;
const char* name = "lamp";
const char* service = "Lightbulb";
WiFiManager wifiManager;
ArduinoHomebridgeMqtt accessory;
int brightness;

void callback(const char* serviceName, const char* characteristic, int value) {
  if (strcmp(serviceName, name) == 0) {
    if (strcmp(characteristic, "On") == 0) {
      if (value == true) {
        analogWrite(led, brightness);
      } else {
        digitalWrite(led, value);
      }
    }
    if (strcmp(characteristic, "Brightness") == 0) {
      brightness = value * 10.23;
      analogWrite(led, brightness);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  wifiManager.autoConnect();
  accessory.onSetValueFromHomebridge(callback);
  accessory.connect(mqttServer);
  accessory.addAccessory(name, service);
  accessory.getAccessory();
}

void loop() {
  accessory.loop();
}