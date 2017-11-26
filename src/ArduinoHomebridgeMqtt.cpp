#include "ArduinoHomebridgeMqtt.h"

void ArduinoHomebridgeMqtt::onSetValueFromHomebridge(std::function<void(Accessory, Service, Characteristic)> callback) {
  this->callback = callback;
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      return;
    }
    Accessory accessory;
    Service service;
    Characteristic characteristic;
    if (strcmp(topic, "homebridge/from/set") == 0) {
      String accessoryName = root["name"];
      String serviceName = root["service_name"];
      String characteristicName = root["characteristic"];
      int characteristicValue = root["value"];
      accessory.name = accessoryName;
      service.name = serviceName;
      characteristic.name = characteristicName;
      characteristic.value = characteristicValue;
      this->callback(accessory, service, characteristic);
    }
    if (strcmp(topic, "homebridge/from/response") == 0) {
      if (root["message"]) {
        String message = root["message"];
        Serial.println(message);
        return;
      }
      for (auto accessoryIterator = root.begin(); accessoryIterator != root.end(); accessoryIterator.operator++()) {
        accessory.name = accessoryIterator->key;
        JsonObject& servicesObject = root[accessory.name]["services"];
        for (auto serviceIterator = servicesObject.begin(); serviceIterator != servicesObject.end(); serviceIterator.operator++()) {
          service.name = serviceIterator->key;
          String serviceType = serviceIterator->value;
          service.type = serviceType;
          JsonObject& characteristicsObject = root[accessory.name]["characteristics"][service.name];
          for (auto characteristicIterator = characteristicsObject.begin(); characteristicIterator != characteristicsObject.end(); characteristicIterator.operator++()) {
            characteristic.name = characteristicIterator->key;
            characteristic.value = characteristicIterator->value;
            this->callback(accessory, service, characteristic);
          }
        }
      }
    }
  });
}

void ArduinoHomebridgeMqtt::connect(IPAddress server) {
  mqttClient.onConnect([this](bool sessionPresent) -> void {
    Serial.println(" Connected");
    mqttClient.subscribe("homebridge/from/set", 0);
    mqttClient.subscribe("homebridge/from/response", 0);
  });
  mqttClient.setServer(server, DEFAULT_MQTT_PORT);
  mqttClient.connect();
  Serial.print("Connecting to MQTT host:");
  Serial.print(server.toString());
  while (!mqttClient.connected()) {
    Serial.print(".");
    delay(1000);
  }
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
