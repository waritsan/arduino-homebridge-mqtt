#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ArduinoHomebridgeMqtt.h"
#include "WiFiManager.h"

#define MQTT_HOST IPAddress(192, 168, 1, 40)
#define SWITCH_OUTPUT_PIN D1

WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridge(MQTT_HOST);

void onSetOn(bool on) {
  if (on) {
    digitalWrite(SWITCH_OUTPUT_PIN, HIGH);
  } else {
    digitalWrite(SWITCH_OUTPUT_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  homebridge.connect();
  homebridge.addSwitch(onSetOn);
}

void loop() {
}
