#include <Arduino.h>
#include <WiFiManager.h>
#include "Thermostat.h"
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define INPUT_PIN D1
#define HEAT_PIN D2
#define COOL_PIN D3
#define SERVICE_NAME "Thermostat"
#define SERVICE "Thermostat";

long lastMsg = 0;
WiFiManager wifiManager;
Thermostat thermostat(INPUT_PIN, HEAT_PIN, COOL_PIN);
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;
float targetTemperature;
int targetHeatingCoolingState;

void callback(String serviceName, String characteristic, float value) {
  if (serviceName == SERVICE_NAME) {
    if (characteristic == TARGET_TEMPERATURE) {
      targetTemperature = value;
    } else if (characteristic == TARGET_HEATING_COOLING_STATE) {
      targetHeatingCoolingState = value;
    }
  }
}

void off() {
  thermostat.off();
  arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_HEATING_COOLING_STATE, OFF);
}

void heat() {
  thermostat.heat();
  arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_HEATING_COOLING_STATE, HEAT);
}

void cool() {
  thermostat.cool();
  arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_HEATING_COOLING_STATE, COOL);
}

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float currentTemperature = thermostat.readTemperature();
    Serial.print("Current temperature: ");
    Serial.println(currentTemperature);
    arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_TEMPERATURE, currentTemperature);
    switch (targetHeatingCoolingState) {
      case HEAT: (currentTemperature < targetTemperature) ? heat() : off();
                 break;
      case COOL: (currentTemperature < targetTemperature) ? off() : cool();
                 break;
      case AUTO: (currentTemperature < targetTemperature) ? heat() : cool();
                 break;
      default: off();
    }
  }
}
