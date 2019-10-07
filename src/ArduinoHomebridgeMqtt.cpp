#include "ArduinoHomebridgeMqtt.h"

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt() {
  mName = new char[16];
  itoa(ESP.getChipId(), mName, 10);
}

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt(const char* name, IPAddress server) {
  mName = new char[16];
  strcpy(mName, name);
  initMqtt(server);
}

ArduinoHomebridgeMqtt::~ArduinoHomebridgeMqtt() {
  free(mName);
}

void ArduinoHomebridgeMqtt::onSetValueFromHomebridge(std::function<void(const char* serviceName, const char* characteristic, int value)> callback) {
  this->callback = callback;
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    Serial.printf("Message arrived [%s] ", topic);
    Serial.println(payload);
    if (strcmp(topic, "homebridge/from/response") == 0) {
      StaticJsonDocument<512> doc;
      deserializeJson(doc, payload);
      if (doc.containsKey("ack")) return;
      if (doc.containsKey(mName)) {
        JsonObject serviceNames = doc[mName]["characteristics"].as<JsonObject>();
        for (JsonPair serviceNameKeyValue: serviceNames) {
          const char* serviceName = serviceNameKeyValue.key().c_str();
          JsonObject characteristics = serviceNameKeyValue.value().as<JsonObject>();
          for (JsonPair characteristicKeyValue: characteristics) {
            const char* characteristic = characteristicKeyValue.key().c_str();
            const int value = characteristicKeyValue.value().as<int>();
            this->callback(serviceName, characteristic, value);
          }
        }
      }
    } else if (strcmp(topic, "homebridge/from/set") == 0) {
      StaticJsonDocument<256> doc;
      deserializeJson(doc, payload);
      const char* name = doc["name"];
      if (strcmp(name, mName) == 0) {
        const char* serviceName = doc["service_name"];
        const char* characteristic = doc["characteristic"];
        const int value = doc["value"];
        this->callback(serviceName, characteristic, value);
      }
    }
  });
}

void ArduinoHomebridgeMqtt::initMqtt(IPAddress server) {
  mqttClient.onConnect([this](bool sessionPresent) -> void {
    Serial.println("connected");
    mqttClient.subscribe("homebridge/from/set", 0);
    mqttClient.subscribe("homebridge/from/response", 0);
  });
  mqttClient.setServer(server, DEFAULT_MQTT_PORT);
}

void ArduinoHomebridgeMqtt::connect(IPAddress server) {
  initMqtt(server);
  connect();
}

void ArduinoHomebridgeMqtt::connect() {
  Serial.print("Connecting to MQTT host...");
  while (!mqttClient.connected()) {
    mqttClient.connect();
    Serial.print(".");
    delay(1000);
  }
}

void ArduinoHomebridgeMqtt::loop() {
  if (!mqttClient.connected()) {
    Serial.println("MQTT host disconnected.");
    connect();
  }
}

void ArduinoHomebridgeMqtt::addAccessory(const char* serviceName, const char* service) {
  StaticJsonDocument<128> doc;
  doc["name"] = mName;
  doc["service_name"] = serviceName;
  doc["service"] = service;
  char payload[128];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/add";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::addService(const char* serviceName, const char* service) {
  StaticJsonDocument<128> doc;
  doc["name"] = mName;
  doc["service_name"] = serviceName;
  doc["service"] = service;
  char payload[128];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/add/service";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::removeAccessory() {
  StaticJsonDocument<64> doc;
  doc["name"] = mName;
  char payload[64];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/remove";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::removeService(const char* serviceName) {
  StaticJsonDocument<128> doc;
  doc["name"] = mName;
  doc["service_name"] = serviceName;
  char payload[128];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/remove/service";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::getAccessory() {
  StaticJsonDocument<64> doc;
  doc["name"] = mName;
  char payload[64];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/get";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::setValueToHomebridge(const char* serviceName, const char* characteristic, int value) {
  StaticJsonDocument<256> doc;
  doc["name"] = mName;
  doc["service_name"] = serviceName;
  doc["characteristic"] = characteristic;
  doc["value"] = value;
  char payload[256];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/set";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::publish(const char* topic, const char* payload) {
  mqttClient.publish(topic, 0, true, payload);
  Serial.printf("Message sent [%s] %s\n", topic, payload);
}
