#include <Arduino.h>
#include <WiFiManager.h>
#include "Thermostat.h"
#include "Switch.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define THERMOSTAT_INPUT_PIN D1
#define THERMOSTAT_HEAT_PIN D2
#define THERMOSTAT_COOL_PIN D3
#define SWITCH_OUTPUT_PIN D4

long lastMsg = 0;
WiFiManager wifiManager;
Thermostat thermostat(THERMOSTAT_INPUT_PIN, THERMOSTAT_HEAT_PIN, THERMOSTAT_COOL_PIN);
Switch mySwitch(SWITCH_OUTPUT_PIN);

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  thermostat.begin(MQTT_SERVER);
  mySwitch.begin(MQTT_SERVER);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    Serial.print("Current temperature: ");
    Serial.println(thermostat.readTemperature());
  }
}
