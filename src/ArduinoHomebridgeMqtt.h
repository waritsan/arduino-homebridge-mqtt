#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

#ifndef DEFAULT_MQTT_PORT
#define DEFAULT_MQTT_PORT 1883
#endif

#ifndef ARDUINO_HOMEBRIDGE_MQTT_H
#define ARDUINO_HOMEBRIDGE_MQTT_H

class ArduinoHomebridgeMqtt {
private:
  AsyncMqttClient mqttClient;
  String accessoryName;
  std::function<void(String serviceName, String characteristic, float value)> callback;

public:
  ArduinoHomebridgeMqtt();
  void onSetValueFromHomebridge(std::function<void(String serviceName, String characteristic, float value)>);
  void connect(IPAddress);
  void addAccessory(String serviceName, String service);
  void addService(String serviceName, String service);
  void removeAccessory();
  void removeService(String serviceName);
  void getAccessory();
  void setValueToHomebridge(String serviceName, String characteristic, float value);
};

#endif
