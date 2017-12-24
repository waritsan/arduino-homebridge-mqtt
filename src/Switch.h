#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"

#ifndef SWITCH_H
#define SWITCH_H

class Switch {
private:
  int outputPin;

public:
  Switch(int);
  void on();
  void off();
};

#endif
