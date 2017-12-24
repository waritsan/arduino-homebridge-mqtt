#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define OUTPUT_PIN D1
#define INPUT_PIN A0

long lastMsg = 0;
WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridgeMqtt;
Accessory accessory;
Service lightSensor;
Service light;
Characteristic on;
Characteristic currentAmbientLightLevel;

void callback(Accessory a, Service s, Characteristic c) {
  if (accessory.name != a.name || light.name != s.name || on.name != c.name) {
    return;
  }
  if (light.name == s.name) {
    if (c.value) {
      Serial.println("ON");
      digitalWrite(OUTPUT_PIN, HIGH);
    } else {
      Serial.println("OFF");
      digitalWrite(OUTPUT_PIN, LOW);
    }
    on = c;
  }
}

float readAmbientLight() {
  int value = analogRead(INPUT_PIN);
  currentAmbientLightLevel.value = exp(float(value)/80.0);
  homebridgeMqtt.setValueToHomebridge(accessory, lightSensor, currentAmbientLightLevel);
  return currentAmbientLightLevel.value;
}

void turnOn() {
  digitalWrite(OUTPUT_PIN, HIGH);
  on.value = true;
  homebridgeMqtt.setValueToHomebridge(accessory, light, on);
}

void turnOff() {
  digitalWrite(OUTPUT_PIN, LOW);
  on.value = false;
  homebridgeMqtt.setValueToHomebridge(accessory, light, on);
}

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT);
  wifiManager.autoConnect();

  accessory.name = String(ESP.getChipId());

  light.name = "Light";
  light.type = "Switch";
  on.name = "On";

  lightSensor.name = "LightSensor";
  lightSensor.type = "LightSensor";
  currentAmbientLightLevel.name = "CurrentAmbientLightLevel";

  homebridgeMqtt.onSetValueFromHomebridge(callback);
  homebridgeMqtt.connect(MQTT_SERVER);
  homebridgeMqtt.addService(accessory, light);
  homebridgeMqtt.addService(accessory, lightSensor);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float lux = readAmbientLight();
    Serial.println(lux);
    if (lux < 1.10) {
      turnOn();
    } else {
      turnOff();
    }
  }
}
