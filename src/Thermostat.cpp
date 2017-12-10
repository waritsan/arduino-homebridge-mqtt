#include "Thermostat.h"

Thermostat::Thermostat(int inputPin, int heatPin, int coolPin) {
  this->inputPin = inputPin;
  this->heatPin = heatPin;
  this->coolPin = coolPin;
  accessory.name = String(ESP.getChipId());
  service.name = "Thermostat";
  service.type = "Thermostat";
  currentTemperature.name = "CurrentTemperature";
  currentHeatingCoolingState.name = "CurrentHeatingCoolingState";
  targetTemperature.name = "TargetTemperature";
  targetHeatingCoolingState.name = "TargetHeatingCoolingState";

  pinMode(inputPin, INPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(coolPin, OUTPUT);
}

void Thermostat::begin(IPAddress mqttServer) {
  homebridgeMqtt.onSetValueFromHomebridge([this](Accessory accessory, Service service, Characteristic characteristic) -> void {
    if (this->accessory.name != accessory.name || this->service.name != service.name) {
      return;
    }
    if (targetHeatingCoolingState.name == characteristic.name) {
      targetHeatingCoolingState = characteristic;
    } else if (targetTemperature.name == characteristic.name) {
      targetTemperature = characteristic;
    }
    operate();
  });
  homebridgeMqtt.connect(mqttServer);
  homebridgeMqtt.addAccessory(accessory, service);
  homebridgeMqtt.addService(accessory, service);
  homebridgeMqtt.getAccessory(accessory);
}

int Thermostat::readTemperature() {
  OneWire oneWire(inputPin);
  DallasTemperature dallasTemperature(&oneWire);
  dallasTemperature.requestTemperatures();
  int newTemperatureValue = dallasTemperature.getTempCByIndex(0);
  if (currentTemperature.value != newTemperatureValue) {
    currentTemperature.value = newTemperatureValue;
    homebridgeMqtt.setValueToHomebridge(accessory, service, currentTemperature);
    operate();
  }
  return currentTemperature.value;
}

// Private
void Thermostat::operate() {
  switch (targetHeatingCoolingState.value) {
    case 0: off();
            break;
    case 1: (currentTemperature.value < targetTemperature.value) ? heat() : off();
            break;
    case 2: (currentTemperature.value > targetTemperature.value) ? cool() : off();
            break;
    case 3: (currentTemperature.value < targetTemperature.value) ? heat() : cool();
            break;
  }
}

void Thermostat::setCurrentHeatingCoolingState(int state) {
  if (currentHeatingCoolingState.value != state) {
    currentHeatingCoolingState.value = state;
    homebridgeMqtt.setValueToHomebridge(accessory, service, currentHeatingCoolingState);
  }
}

void Thermostat::off() {
  digitalWrite(coolPin, LOW);
  digitalWrite(heatPin, LOW);
  setCurrentHeatingCoolingState(0);
}

void Thermostat::cool() {
  digitalWrite(coolPin, HIGH);
  digitalWrite(heatPin, LOW);
  setCurrentHeatingCoolingState(2);
}

void Thermostat::heat() {
  digitalWrite(coolPin, LOW);
  digitalWrite(heatPin, HIGH);
  setCurrentHeatingCoolingState(1);
}
