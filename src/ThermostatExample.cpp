#include <Arduino.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define TEMPERATURE_INPUT_PIN D1
#define COOL_PIN D2
#define HEAT_PIN D3
#define OFF_MODE 0
#define HEAT_MODE 1
#define COOL_MODE 2
#define AUTO_MODE 3

long lastMsg = 0;
WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridge;
Accessory thermostatAccessory;
Service thermostatService;
Characteristic targetHeatingCoolingStateCharacteristic;
Characteristic currentHeatingCoolingStateCharacteristic;
Characteristic targetTemperatureCharacteristic;
Characteristic currentTemperatureCharacteristic;

void setCurrentTemperature() {
  OneWire oneWire(TEMPERATURE_INPUT_PIN);
  DallasTemperature dallasTemperature(&oneWire);
  dallasTemperature.requestTemperatures();
  int temperatureValue = dallasTemperature.getTempCByIndex(0);
  if (currentTemperatureCharacteristic.value != temperatureValue) {
    currentTemperatureCharacteristic.value = temperatureValue;
    homebridge.setValueToHomebridge(thermostatAccessory, thermostatService, currentTemperatureCharacteristic);
    Serial.print("Current temperature: ");
    Serial.println(currentTemperatureCharacteristic.value);
  }
}

void setCurrentHeatingCoolingState(int state) {
  if (currentHeatingCoolingStateCharacteristic.value != state) {
    currentHeatingCoolingStateCharacteristic.value = state;
    homebridge.setValueToHomebridge(thermostatAccessory, thermostatService, currentHeatingCoolingStateCharacteristic);
    Serial.print("Current mode: ");
    Serial.println(currentHeatingCoolingStateCharacteristic.value);
  }
}

void off() {
  digitalWrite(COOL_PIN, LOW);
  digitalWrite(HEAT_PIN, LOW);
  setCurrentHeatingCoolingState(OFF_MODE);
}

void cool() {
  digitalWrite(COOL_PIN, HIGH);
  digitalWrite(HEAT_PIN, LOW);
  setCurrentHeatingCoolingState(COOL_MODE);
}

void heat() {
  digitalWrite(COOL_PIN, LOW);
  digitalWrite(HEAT_PIN, HIGH);
  setCurrentHeatingCoolingState(HEAT_MODE);
}

void callback(Accessory accessory, Service service, Characteristic characteristic) {
  if (thermostatAccessory.name != accessory.name || thermostatService.name != service.name) {
    return;
  }
  if (targetHeatingCoolingStateCharacteristic.name == characteristic.name) {
    targetHeatingCoolingStateCharacteristic = characteristic;
    Serial.print("Target mode: ");
    Serial.println(targetHeatingCoolingStateCharacteristic.value);
  } else if (targetTemperatureCharacteristic.name == characteristic.name) {
    targetTemperatureCharacteristic = characteristic;
    Serial.print("Target temperature: ");
    Serial.println(targetTemperatureCharacteristic.value);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TEMPERATURE_INPUT_PIN, INPUT);
  pinMode(COOL_PIN, OUTPUT);
  pinMode(HEAT_PIN, OUTPUT);
  wifiManager.autoConnect();
  thermostatAccessory.name = "esp_thermostat";
  thermostatService.name = "esp_thermostat";
  thermostatService.type = "Thermostat";
  targetHeatingCoolingStateCharacteristic.name = "TargetHeatingCoolingState";
  targetTemperatureCharacteristic.name = "TargetTemperature";
  currentHeatingCoolingStateCharacteristic.name = "CurrentHeatingCoolingState";
  currentTemperatureCharacteristic.name = "CurrentTemperature";
  homebridge.onSetValueFromHomebridge(callback);
  homebridge.connect(MQTT_SERVER);
  homebridge.addAccessory(thermostatAccessory, thermostatService);
  homebridge.getAccessory(thermostatAccessory);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    setCurrentTemperature();
    switch (targetHeatingCoolingStateCharacteristic.value) {
      case OFF_MODE: off();
              break;
      case HEAT_MODE: (currentTemperatureCharacteristic.value < targetTemperatureCharacteristic.value) ? heat() : off();
              break;
      case COOL_MODE: (currentTemperatureCharacteristic.value > targetTemperatureCharacteristic.value) ? cool() : off();
              break;
      case AUTO_MODE: (currentTemperatureCharacteristic.value < targetTemperatureCharacteristic.value) ? heat() : cool();
              break;
    }
  }
}
