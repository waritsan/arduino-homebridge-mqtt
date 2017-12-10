#include <Arduino.h>
#include <WiFiManager.h>
#include "Thermostat.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define INPUT_PIN D1
#define HEAT_PIN D2
#define COOL_PIN D3

long lastMsg = 0;
WiFiManager wifiManager;
Thermostat thermostat(INPUT_PIN, HEAT_PIN, COOL_PIN);

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  thermostat.begin(MQTT_SERVER);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    Serial.print("Current temperature: ");
    Serial.println(thermostat.readTemperature());
  }
}
