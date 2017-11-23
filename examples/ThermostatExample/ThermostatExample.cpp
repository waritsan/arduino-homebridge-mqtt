#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ArduinoHomebridgeMqtt.h"
#include "WiFiManager.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define NAME "esp_thernostat"
#define TYPE "Thermostat"
#define COOLING_PIN D3
#define HEATING_PIN D4
#define TEMPERATURE_INPUT_PIN D2
#define OFF 0
#define HEAT 1
#define COOL 2
#define AUTO 3

WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridge;
int currentTemperature;
int currentHeatingCoolingState;
int targetHeatingCoolingState;
int targetTemperature;

int getCurrentTemperature() {
  OneWire oneWire(TEMPERATURE_INPUT_PIN);
  DallasTemperature dallasTemperature(&oneWire);
  dallasTemperature.requestTemperatures();
  return dallasTemperature.getTempCByIndex(0);
}

void onSetTargetHeatingCoolingState(int value) {
  targetHeatingCoolingState = value;
}

void onSetTargetTemperature(int value) {
  targetTemperature = value;
}

void setCurrentHeatingCoolingState(int state) {
  if (currentHeatingCoolingState != state) {
    currentHeatingCoolingState = state;
    homebridge.setCurrentHeatingCoolingState(currentHeatingCoolingState);
  }
}

void off() {
  digitalWrite(COOLING_PIN, LOW);
  digitalWrite(HEATING_PIN, LOW);
  setCurrentHeatingCoolingState(OFF);
}

void cool() {
  digitalWrite(COOLING_PIN, HIGH);
  digitalWrite(HEATING_PIN, LOW);
  setCurrentHeatingCoolingState(COOL);
}

void heat() {
  digitalWrite(COOLING_PIN, LOW);
  digitalWrite(HEATING_PIN, HIGH);
  setCurrentHeatingCoolingState(HEAT);
}

void setup() {
  Serial.begin(115200);
  pinMode(TEMPERATURE_INPUT_PIN, INPUT);
  pinMode(COOLING_PIN, OUTPUT);
  pinMode(HEATING_PIN, OUTPUT);
  wifiManager.autoConnect();
  homebridge.connect(MQTT_SERVER);
  homebridge.setService(NAME, TYPE, onSetTargetTemperature, onSetTargetHeatingCoolingState);
}

void loop() {
  currentTemperature = getCurrentTemperature();
  switch (targetHeatingCoolingState) {
    case OFF: off();
            break;
    case HEAT: (currentTemperature < targetTemperature) ? cool() : off();
            break;
    case COOL: (currentTemperature > targetTemperature) ? cool() : off();
            break;
    case AUTO: (currentTemperature < targetTemperature) ? heat() : cool();
            break;
  }
  homebridge.setCurrentTemperature(currentTemperature);
}
