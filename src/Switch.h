#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"

#ifndef SWITCH_H
#define SWITCH_H

class Switch {
private:
  int outputPin;
  Accessory accessory;
  Service service;
  Characteristic on;
  ArduinoHomebridgeMqtt homebridgeMqtt;

public:
  Switch(int);
  void begin(IPAddress);
  int getOnValue();
};

#endif
