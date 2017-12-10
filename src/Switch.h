#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"

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
