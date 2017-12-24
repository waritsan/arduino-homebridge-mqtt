#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"

#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class LightSensor {
private:
  int inputPin;

public:
  LightSensor(int inputPin);
  float readAmbientLight();
};

#endif
