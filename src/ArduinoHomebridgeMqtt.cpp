#include "ArduinoHomebridgeMqtt.h"

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt() {
  itoa(ESP.getChipId(), this->mName, 10);
  WiFiClient espClient;
  this->client = new PubSubClient(espClient);
}

ArduinoHomebridgeMqtt::~ArduinoHomebridgeMqtt() {
  free(this->client);
}

void ArduinoHomebridgeMqtt::setServer(IPAddress server, int port) {
  this->client->setServer(server, port);
}

void ArduinoHomebridgeMqtt::onSetValueFromHomebridge(std::function<void(const char* serviceName, const char* characteristic, int value)> callback) {
  this->callback = callback;
  this->client->setCallback([this](char* topic, byte* payload, unsigned int length) -> void {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
    }
    Serial.println();
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

void ArduinoHomebridgeMqtt::loop() {
  connect();
  client->loop();
}

void ArduinoHomebridgeMqtt::connect() {
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client->connect("arduinoClient")) {
      Serial.println("connected");
      client->subscribe("homebridge/from/response");
      client->subscribe("homebridge/from/set");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
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
  StaticJsonDocument<64> doc;
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
  client->publish(topic, payload);
  Serial.printf("Message sent [%s] %s\n", topic, payload);
}