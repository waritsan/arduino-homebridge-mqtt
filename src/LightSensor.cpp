#include "LightSensor.h"

LightSensor::LightSensor(int inputPin) {
  this->inputPin = inputPin;
  pinMode(inputPin, INPUT);
}

float LightSensor::readAmbientLight() {
  int value = analogRead(inputPin);
  return exp(float(value)/80.0);
}
