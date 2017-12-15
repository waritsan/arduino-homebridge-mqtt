#include <Arduino.h>
#include <WiFiManager.h>
#include "LightSensor.h"
#include "Switch.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define LIGHT_SENSOR_INPUT_PIN A0
#define SWITCH_OUTPUT_PIN D1

long lastMsg = 0;
WiFiManager wifiManager;
LightSensor lightSensor(LIGHT_SENSOR_INPUT_PIN);
Switch mySwitch(SWITCH_OUTPUT_PIN);

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  lightSensor.begin(MQTT_SERVER);
  mySwitch.begin(MQTT_SERVER);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    Serial.println(lightSensor.readAmbientLight());
  }
}
