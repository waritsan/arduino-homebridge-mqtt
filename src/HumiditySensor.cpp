#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(int inputPin) {
  this->inputPin = inputPin;
  pinMode(inputPin, INPUT);
}

int HumiditySensor::readHumidity() {
    int value = analogRead(inputPin);
    value = constrain(value, 567, 959);
    return map(value, 567, 959, 100, 0);
  }
