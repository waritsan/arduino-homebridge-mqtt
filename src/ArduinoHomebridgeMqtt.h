#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#ifndef DEFAULT_MQTT_PORT
#define DEFAULT_MQTT_PORT 1883
#endif

#ifndef ARDUINO_HOMEBRIDGE_MQTT_H
#define ARDUINO_HOMEBRIDGE_MQTT_H

class ArduinoHomebridgeMqtt {
private:
  AsyncMqttClient mqttClient;
  std::function<void(const char* name, const char* serviceName, const char* characteristic, int value)> callback;
  void publish(const char* topic, const char* payload);
  void initMqtt(IPAddress server);
  void connect();
  
public:
  ArduinoHomebridgeMqtt();
  void onSetValueFromHomebridge(std::function<void(const char* name, const char* serviceName, const char* characteristic, int value)>);
  void connect(IPAddress server);
  void loop();
  void addAccessory(const char* name, const char* serviceName, const char* service);
  void addService(const char* name, const char* serviceName, const char* service);
  void removeAccessory(const char* name);
  void removeService(const char* name, const char* serviceName);
  void getAccessory(const char* name);
  void setValueToHomebridge(const char* name, const char* serviceName, const char* characteristic, int value);
};

#endif