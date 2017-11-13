#include "ArduinoHomebridgeMqtt.h"

const int ArduinoHomebridgeMqtt::DEFAULT_MQTT_PORT = 1883;
const String ArduinoHomebridgeMqtt::CURRENT_TEMPERATURE = "CurrentTemperature";
const String ArduinoHomebridgeMqtt::CURRENT_HEATING_COOLING_STATE = "CurrentHeatingCoolingState";
const String ArduinoHomebridgeMqtt::TARGET_TEMPERATURE = "TargetTemperature";
const String ArduinoHomebridgeMqtt::TARGET_HEATING_COOLING_STATE = "TargetHeatingCoolingState";
const String ArduinoHomebridgeMqtt::ON = "On";

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt() {}

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt(String serviceName, IPAddress server) {
  setServiceName(serviceName);
  setMqttServer(server);
}

void ArduinoHomebridgeMqtt::setMqttServer(IPAddress server) {
  mqttClient.setServer(server, DEFAULT_MQTT_PORT);
}

void ArduinoHomebridgeMqtt::setServiceName(String serviceName) {
  this->serviceName = serviceName;
}

void ArduinoHomebridgeMqtt::setCallback(std::function<void (int)> callback) {
  this->callback = callback;
  onMessage();
}

void ArduinoHomebridgeMqtt::setCallback(std::function<void (int)> callback, std::function<void (int)> callback1) {
  this->callback1 = callback1;
  setCallback(callback);
}

void ArduinoHomebridgeMqtt::connect() {
  mqttClient.onConnect([this](bool sessionPresent) -> void {
    Serial.println("Connected to MQTT host");
    mqttClient.subscribe("homebridge/from/set", 0);
    mqttClient.subscribe("homebridge/from/response", 0);
    getAccessory();
  });
  mqttClient.connect();
}

void ArduinoHomebridgeMqtt::getAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessoryName;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/get", 0, true, payload);
}

void ArduinoHomebridgeMqtt::set(String characteristic, int value) {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessoryName;
  root["characteristic"] = characteristic;
  root["value"] = value;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/set", 0, true, payload);
}

// Private
void ArduinoHomebridgeMqtt::onMessage() {
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      return;
    }
    if (strcmp(topic, "homebridge/from/set") == 0 && accessoryName == root["name"]) {
      String characteristic = root["characteristic"];
      int value = root["value"];
      if (characteristic == TARGET_TEMPERATURE) {
        callback(value);
      } else if (characteristic == TARGET_HEATING_COOLING_STATE) {
        callback1(value);
      } else if (characteristic == ON) {
        callback(value);
      }
    }
    if (strcmp(topic, "homebridge/from/response") == 0 && root[accessoryName]) {
      JsonObject& characteristics = root[accessoryName]["characteristics"][serviceName];
      if (int value = characteristics[TARGET_TEMPERATURE]) {
        callback(value);
      }
      if (int value = characteristics[TARGET_HEATING_COOLING_STATE]) {
        callback1(value);
      }
      if (int value = characteristics[ON]) {
        callback(value);
      }
    }
  });
}
