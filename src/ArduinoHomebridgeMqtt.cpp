#include "ArduinoHomebridgeMqtt.h"

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt() {
  mName = new char[16];
  itoa(ESP.getChipId(), mName, 10);
}

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt(const char* name) {
  mName = new char[16];
  strcpy(mName, name);
}

ArduinoHomebridgeMqtt::~ArduinoHomebridgeMqtt() {
  free(mName);
}

void ArduinoHomebridgeMqtt::onSetValueFromHomebridge(std::function<void(const char* serviceName, const char* characteristic, int value)> callback) {
  this->callback = callback;
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      return;
    }
    if (strcmp(topic, "homebridge/from/set") == 0) {
      const char* mName = root["name"];
      this->callback(root["service_name"], root["characteristic"], root["value"]);
    }
    if (strcmp(topic, "homebridge/from/response") == 0) {
      if (root.containsKey("message")) {
        const char* message = root["message"];
        Serial.println(message);
        return;
      }
      for (auto accessoryIterator = root.begin(); accessoryIterator != root.end(); accessoryIterator.operator++()) {
        if (mName == accessoryIterator->key) {
          JsonObject& servicesObject = root[mName]["services"];
          for (auto serviceIterator = servicesObject.begin(); serviceIterator != servicesObject.end(); serviceIterator.operator++()) {
            const char* serviceName = serviceIterator->key;
            JsonObject& characteristicsObject = root[mName]["characteristics"][serviceName];
            for (auto characteristicIterator = characteristicsObject.begin(); characteristicIterator != characteristicsObject.end(); characteristicIterator.operator++()) {
              this->callback(serviceName, characteristicIterator->key, characteristicIterator->value);
            }
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
  Serial.print(server.toconst char*());
  while (!mqttClient.connected()) {
    Serial.print(".");
    delay(1000);
  }
}

void ArduinoHomebridgeMqtt::addAccessory(const char* serviceName, const char* service) {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = mName;
  root["service_name"] = serviceName;
  root["service"] = service;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/add", 0, true, payload);
  delay(1000);
}

void ArduinoHomebridgeMqtt::addService(const char* serviceName, const char* service) {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = mName;
  root["service_name"] = serviceName;
  root["service"] = service;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/add/service", 0, true, payload);
  delay(1000);
}

void ArduinoHomebridgeMqtt::removeAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = mName;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/remove", 0, true, payload);
  delay(1000);
}

void ArduinoHomebridgeMqtt::removeService(const char* serviceName) {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = mName;
  root["service_name"] = serviceName;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/remove/service", 0, true, payload);
  delay(1000);
}

void ArduinoHomebridgeMqtt::getAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = mName;
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/get", 0, true, payload);
  delay(1000);
}

void ArduinoHomebridgeMqtt::setValueToHomebridge(const char* serviceName, const char* characteristic, int value) {
  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = mName;
  root["service_name"] = serviceName;
  root["characteristic"] = characteristic;
  root["value"] = value;
  char payload[256];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/set", 0, true, payload);
  delay(1000);
}
