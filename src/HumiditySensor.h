#include <Arduino.h>

#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

class HumiditySensor {
private:
  int inputPin;

public:
  HumiditySensor(int inputPin);
  int readHumidity();
};
#endif
