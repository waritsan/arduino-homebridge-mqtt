#include "LightSensor.h"

LightSensor::LightSensor(int inputPin) {
  pinMode(inputPin, INPUT);
  this->inputPin = inputPin;
  accessory.name = String(ESP.getChipId());
  service.name = "LightSensor";
  service.type = "LightSensor";
  currentAmbientLightLevel.name = "CurrentAmbientLightLevel";
}

void LightSensor::begin(IPAddress mqttServer) {
  homebridgeMqtt.connect(mqttServer);
  homebridgeMqtt.addAccessory(accessory, service);
  homebridgeMqtt.addService(accessory, service);
  homebridgeMqtt.getAccessory(accessory);
}

float LightSensor::readAmbientLight() {
  int value = analogRead(inputPin);
  float lux = exp(float(value)/80.0);
  if (currentAmbientLightLevel.value != lux) {
    currentAmbientLightLevel.value = lux;
    homebridgeMqtt.setValueToHomebridge(accessory, service, currentAmbientLightLevel);
  }
  return lux;
}
