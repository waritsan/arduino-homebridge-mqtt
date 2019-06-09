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
  char* mName;
  std::function<void(const char* serviceName, const char* characteristic, int value)> callback;
  void publish(const char* topic, const char* payload);
  void initMqtt(IPAddress server);

public:
  ArduinoHomebridgeMqtt();
  ArduinoHomebridgeMqtt(const char* name, IPAddress server);
  ~ArduinoHomebridgeMqtt();
  void onSetValueFromHomebridge(std::function<void(const char* serviceName, const char* characteristic, int value)>);
  void connect();
  void connect(IPAddress server);
  void loop();
  void addAccessory(const char* serviceName, const char* service);
  void addService(const char* serviceName, const char* service);
  void removeAccessory();
  void removeService(const char* serviceName);
  void getAccessory();
  void setValueToHomebridge(const char* serviceName, const char* characteristic, int value);
};

#endif