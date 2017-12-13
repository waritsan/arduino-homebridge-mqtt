#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"

#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class LightSensor {
private:
  int inputPin;
  Accessory accessory;
  Service service;
  Characteristic currentAmbientLightLevel;
  ArduinoHomebridgeMqtt homebridgeMqtt;

public:
  LightSensor(int);
  void begin(IPAddress);
  int getCurrentAmbientLightLevel();
};

#endif
