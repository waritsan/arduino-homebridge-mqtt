#include "Thermostat.h"

Thermostat::Thermostat(int inputPin, int heatPin, int coolPin) {
  this->inputPin = inputPin;
  this->heatPin = heatPin;
  this->coolPin = coolPin;
  pinMode(inputPin, INPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(coolPin, OUTPUT);
}

float Thermostat::readTemperature() {
  OneWire oneWire(inputPin);
  DallasTemperature dallasTemperature(&oneWire);
  dallasTemperature.requestTemperatures();
  return dallasTemperature.getTempCByIndex(0);
}

void Thermostat::off() {
  digitalWrite(coolPin, LOW);
  digitalWrite(heatPin, LOW);
}

void Thermostat::cool() {
  digitalWrite(coolPin, HIGH);
  digitalWrite(heatPin, LOW);
}

void Thermostat::heat() {
  digitalWrite(coolPin, LOW);
  digitalWrite(heatPin, HIGH);
}
