#include "ArduinoHomebridgeMqtt.h"

void ArduinoHomebridgeMqtt::onSetValueFromHomebridge(std::function<void(Accessory, Service, Characteristic)> callback) {
  this->callback = callback;
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      return;
    }
    if (strcmp(topic, "homebridge/from/set") == 0) {
      String accessoryName = root["name"];
      String serviceName = root["service_name"];
      String characteristicName = root["characteristic"];
      int characteristicValue = root["value"];
      Accessory accessory;
      accessory.name = accessoryName;
      Service service;
      service.name = serviceName;
      Characteristic characteristic;
      characteristic.name = characteristicName;
      characteristic.value = characteristicValue;
      this->callback(accessory, service, characteristic);
    }
    if (strcmp(topic, "homebridge/from/response") == 0) {
      Accessory accessory;
      accessory.name = root.begin()->key;
      JsonObject& servicesObject = root["services"];
      Service service;
      service.name = servicesObject.begin()->key;
      String serviceType = servicesObject.begin()->value;
      service.type = serviceType;
      JsonObject& characteristicsObject = root[accessory.name]["characteristics"];
    }
  });
}

void ArduinoHomebridgeMqtt::connect(IPAddress server) {
  mqttClient.onConnect([this](bool sessionPresent) -> void {
    Serial.println("Connected to MQTT host");
    mqttClient.subscribe("homebridge/from/set", 0);
    mqttClient.subscribe("homebridge/from/response", 0);
  });
  mqttClient.setServer(server, DEFAULT_MQTT_PORT);
  mqttClient.connect();
}

void ArduinoHomebridgeMqtt::addAccessory(Accessory accessory, Service service) {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.name;
  root["service_name"] = service.name;
  root["service"] = service.type;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/set", 0, true, payload);
}

void ArduinoHomebridgeMqtt::getAccessory(Accessory accessory) {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.name;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/get", 0, true, payload);
}

void ArduinoHomebridgeMqtt::setValueToHomebridge(Accessory accessory, Service service, Characteristic characteristic) {
  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.name;
  root["service_name"] = service.name;
  root["characteristic"] = characteristic.name;
  root["value"] = characteristic.value;
  char payload[256];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/set", 0, true, payload);
}
