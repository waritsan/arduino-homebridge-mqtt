#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include "Accessory.h"
#include "Service.h"
#include "Characteristic.h"

#ifndef DEFAULT_MQTT_PORT
#define DEFAULT_MQTT_PORT 1883
#endif

#ifndef ARDUINO_HOMEBRIDGE_MQTT_H
#define ARDUINO_HOMEBRIDGE_MQTT_H

class ArduinoHomebridgeMqtt {
private:
  AsyncMqttClient mqttClient;
  std::function<void(Accessory, Service, Characteristic)> callback;

public:
  void onSetValueFromHomebridge(std::function<void(Accessory, Service, Characteristic)>);
  void connect(IPAddress);
  void addAccessory(Accessory, Service);
  void addService(Accessory, Service);
  void removeAccessory(Accessory);
  void removeService(Accessory, Service);
  void getAccessory(Accessory);
  void setValueToHomebridge(Accessory, Service, Characteristic);
};

#endif
