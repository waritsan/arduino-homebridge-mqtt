#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef DEFAULT_MQTT_PORT
#define DEFAULT_MQTT_PORT 1883
#endif

#ifndef ARDUINO_HOMEBRIDGE_MQTT_H
#define ARDUINO_HOMEBRIDGE_MQTT_H

class ArduinoHomebridgeMqtt {
private:
  PubSubClient *client;
  char mName[16];
  std::function<void(const char* serviceName, const char* characteristic, int value)> callback;
  void publish(const char* topic, const char* payload);

public:
  ArduinoHomebridgeMqtt();
  ~ArduinoHomebridgeMqtt();
  void setServer(IPAddress server, int port);
  void onSetValueFromHomebridge(std::function<void(const char* serviceName, const char* characteristic, int value)>);
  void connect();
  void addAccessory(const char* serviceName, const char* service);
  void addService(const char* serviceName, const char* service);
  void removeAccessory();
  void removeService(const char* serviceName);
  void getAccessory();
  void setValueToHomebridge(const char* serviceName, const char* characteristic, int value);
  void loop();
};

#endif
