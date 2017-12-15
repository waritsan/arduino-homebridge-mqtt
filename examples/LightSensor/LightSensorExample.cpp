#include <Arduino.h>
#include <WiFiManager.h>
#include "LightSensor.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define INPUT_PIN A0

WiFiManager wifiManager;
LightSensor lightSensor(INPUT_PIN);

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  lightSensor.begin(MQTT_SERVER);
}

void loop() {
  Serial.println(lightSensor.readAmbientLight());
  delay(3000);
}
