#include "ArduinoHomebridgeMqtt.h"

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt(Accessory& accessory, IPAddress mqttHost) : accessory(accessory) {
  this->mqttHost = mqttHost;
}

void ArduinoHomebridgeMqtt::getAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.name;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/get", 0, true, payload);
}

void ArduinoHomebridgeMqtt::addAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.name;
  root["service_name"] = accessory.services[0].name;
  root["service"] = accessory.services[0].type;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/add", 0, true, payload);
  Serial.println(payload);
}

void ArduinoHomebridgeMqtt::connect() {
  mqttClient.onConnect([this](bool sessionPresent) -> void {
    Serial.println("Connected to MQTT host");
    mqttClient.subscribe("homebridge/from/set", 0);
    mqttClient.subscribe("homebridge/from/response", 0);
    addAccessory();
    getAccessory();
  });
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      return;
    }
    if (strcmp(topic, "homebridge/from/set") == 0 && this->accessory.name == root["name"]) {
      for (size_t i = 0; i < SERVICES_SIZE; i++) {
        if (this->accessory.services[i].name == root["service_name"]) {
          for (size_t j = 0; j < CHARACTERISTICS_SIZE; j++) {
            if (this->accessory.services[i].characteristics[j].name == root["characteristic"]) {
              this->accessory.services[i].characteristics[j].value = root["value"];
            }
          }
        }
      }
    }
    if (strcmp(topic, "homebridge/from/response") == 0 && root[this->accessory.name]) {
      for (size_t i = 0; i < SERVICES_SIZE; i++) {
        if (root[this->accessory.name]["characteristics"][this->accessory.services[i].name]) {
          for (size_t j = 0; j < CHARACTERISTICS_SIZE; j++) {
            if (int value = root[this->accessory.name]["characteristics"][this->accessory.services[i].name][this->accessory.services[i].characteristics[j].name]) {
              this->accessory.services[i].characteristics[j].value = value;
            }
          }
        }
      }
    }
  });
  mqttClient.setServer(mqttHost, DEFAULT_MQTT_PORT);
  mqttClient.connect();
}
