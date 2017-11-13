#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ArduinoHomebridgeMqtt.h"
#include "WiFiManager.h"

#define MQTT_HOST IPAddress(192, 168, 1, 40)
#define COOLING_PIN D3
#define HEATING_PIN D4
#define TEMPERATURE_INPUT_PIN D2

WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridge(MQTT_HOST);
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

}

void onSetTargetTemperature(int value) {

}

void off() {
  digitalWrite(COOLING_PIN, LOW);
  digitalWrite(HEATING_PIN, LOW);
  if (currentHeatingCoolingState != 0) {
    currentHeatingCoolingState = 0;
    homebridge.setCurrentHeatingCoolingState(currentHeatingCoolingState);
  }
}

void cool() {
  digitalWrite(COOLING_PIN, HIGH);
  digitalWrite(HEATING_PIN, LOW);
  if (currentHeatingCoolingState != 2) {
    currentHeatingCoolingState = 2;
    homebridge.setCurrentHeatingCoolingState(currentHeatingCoolingState);
  }
}

void heat() {
  digitalWrite(COOLING_PIN, LOW);
  digitalWrite(HEATING_PIN, HIGH);
  if (currentHeatingCoolingState != 1) {
    currentHeatingCoolingState = 1;
    homebridge.setCurrentHeatingCoolingState(currentHeatingCoolingState);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TEMPERATURE_INPUT_PIN, INPUT);
  pinMode(COOLING_PIN, OUTPUT);
  pinMode(HEATING_PIN, OUTPUT);
  wifiManager.autoConnect();
  homebridge.connect();
  homebridge.addThermostat(onSetTargetTemperature, onSetTargetHeatingCoolingState);
}

void loop() {
  currentTemperature = getCurrentTemperature();
  switch (targetHeatingCoolingState) {
    case 0: off();
            break;
    case 1: (currentTemperature < targetTemperature) ? cool() : off();
            break;
    case 2: (currentTemperature > targetTemperature) ? cool() : off();
            break;
    case 3: (currentTemperature < targetTemperature) ? heat() : cool();
            break;
  }
  homebridge.setCurrentTemperature(currentTemperature);
}
