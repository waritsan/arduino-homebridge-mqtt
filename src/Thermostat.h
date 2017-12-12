#include <Arduino.h>
#include <DallasTemperature.h>
#include "ArduinoHomebridgeMqtt.h"

#ifndef THERMOSTAT_H
#define THERMOSTAT_H

class Thermostat {
private:
  int inputPin;
  int heatPin;
  int coolPin;
  Accessory accessory;
  Service service;
  Characteristic targetHeatingCoolingState;
  Characteristic currentHeatingCoolingState;
  Characteristic targetTemperature;
  Characteristic currentTemperature;
  ArduinoHomebridgeMqtt homebridgeMqtt;
  void operate();
  void setCurrentHeatingCoolingState(int);
  void off();
  void cool();
  void heat();

public:
  Thermostat(int, int, int);
  void begin(IPAddress);
  int readTemperature();
};

#endif
