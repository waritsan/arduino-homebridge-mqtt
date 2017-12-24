#include <Arduino.h>
#include <DallasTemperature.h>

#ifndef THERMOSTAT_H
#define THERMOSTAT_H

class Thermostat {
private:
  int inputPin;
  int heatPin;
  int coolPin;

public:
  Thermostat(int, int, int);
  float readTemperature();
  void off();
  void cool();
  void heat();
};

#endif
