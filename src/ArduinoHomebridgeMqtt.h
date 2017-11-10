#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include "Accessory.h"

#define DEFAULT_MQTT_PORT 1883

class ArduinoHomebridgeMqtt {
private:
  Accessory accessory;
  AsyncMqttClient mqttClient;
  IPAddress mqttHost;
  std::function<void(int)> onSetOn;
  std::function<void(int)> onSetTargetTemperature;
  std::function<void(int)> onSetTargetHeatingCoolingState;
  void addAccessory();
  void addService(Service);
  void getAccessory();

public:
  ArduinoHomebridgeMqtt(IPAddress mqttHost);
  void connect();
  void addSwitch(std::function<void(int)> onSetOn);
  void addThermostat(std::function<void(int)> onSetTargetTemperature, std::function<void(int)> onSetTargetHeatingCoolingState);
  void setCurrentTemperature(int currentTemperature);
  void setCurrentHeatingCoolingState(int currentHeatingCoolingState);
};
