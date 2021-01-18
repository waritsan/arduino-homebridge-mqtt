#include "ArduinoHomebridgeMqtt.h"

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt() {}

void ArduinoHomebridgeMqtt::onSetValueFromHomebridge(std::function<void(const char* name, const char* serviceName, const char* characteristic, JsonVariantConst value)> callback) {
  this->callback = callback;
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    // Serial.printf("Message arrived [%s] ", topic);
    // Serial.println(payload);
    if (strcmp(topic, "homebridge/from/response") == 0) {
      StaticJsonDocument<512> doc;
      deserializeJson(doc, payload);
      if (doc.containsKey("ack")) return;
      int index = 0;
      JsonObject::iterator it = doc.as<JsonObject>().begin();
      it += index;
      const char* name = it->key().c_str();
      JsonObject serviceNames = doc[name]["characteristics"].as<JsonObject>();
      for (JsonPair serviceNameKeyValue: serviceNames) {
        const char* serviceName = serviceNameKeyValue.key().c_str();
        JsonObject characteristics = serviceNameKeyValue.value().as<JsonObject>();
        for (JsonPair characteristicKeyValue: characteristics) {
          const char* characteristic = characteristicKeyValue.key().c_str();
          JsonVariantConst value = characteristicKeyValue.value().as<JsonVariantConst>();
          this->callback(name, serviceName, characteristic, value);
        }
      }
    } else if (strcmp(topic, "homebridge/from/set") == 0) {
      StaticJsonDocument<256> doc;
      deserializeJson(doc, payload);
      const char* name = doc["name"];
      const char* serviceName = doc["service_name"];
      const char* characteristic = doc["characteristic"];
      JsonVariantConst value = doc["value"];
      this->callback(name, serviceName, characteristic, value);
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

void ArduinoHomebridgeMqtt::addAccessory(const char* name, const char* serviceName, const char* service) {
  StaticJsonDocument<128> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  doc["service"] = service;
  char payload[128];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/add";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::addAccessory(const char* name, const char* serviceName, const char* service, const char* characteristics[]) {
  StaticJsonDocument<1024> chs;
  for (int i = 0;;i++) {
    const char* ch = characteristics[i];
    if (ch) {
      chs[ch] = "default";
    } else {
      break;
    }
  }
  addAccessory(name, serviceName, service, chs.as<JsonObjectConst>());
}

void ArduinoHomebridgeMqtt::addAccessory(const char* name, const char* serviceName, const char* service, JsonObjectConst characteristics) {
  StaticJsonDocument<1024> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  doc["service"] = service;
  for (auto kvp : characteristics) {
    doc[kvp.key()] = kvp.value();
  }
  char payload[1024];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/add";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::addService(const char* name, const char* serviceName, const char* service) {
  StaticJsonDocument<128> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  doc["service"] = service;
  char payload[128];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/add/service";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::addService(const char* name, const char* serviceName, const char* service, const char* characteristics[]) {
  StaticJsonDocument<1024> chs;
  for (int i = 0;;i++) {
    const char* ch = characteristics[i];
    if (ch) {
      chs[ch] = "default";
    } else {
      break;
    }
  }
  addService(name, serviceName, service, chs.as<JsonObjectConst>());
}

void ArduinoHomebridgeMqtt::addService(const char* name, const char* serviceName, const char* service, JsonObjectConst characteristics) {
  StaticJsonDocument<1024> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  doc["service"] = service;
  for (auto kvp : characteristics) {
    doc[kvp.key()] = kvp.value();
  }
  char payload[1024];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/add/service";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::removeAccessory(const char* name) {
  StaticJsonDocument<64> doc;
  doc["name"] = name;
  char payload[64];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/remove";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::removeService(const char* name, const char* serviceName) {
  StaticJsonDocument<128> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  char payload[128];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/remove/service";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::getAccessory(const char* name) {
  StaticJsonDocument<64> doc;
  doc["name"] = name;
  char payload[64];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/get";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::setValueToHomebridge(const char* name, const char* serviceName, const char* characteristic, int value) {
  StaticJsonDocument<256> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  doc["characteristic"] = characteristic;
  doc["value"] = value;
  char payload[256];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/set";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::setValueToHomebridge(const char* name, const char* serviceName, const char* characteristic, float value) {
  StaticJsonDocument<256> doc;
  doc["name"] = name;
  doc["service_name"] = serviceName;
  doc["characteristic"] = characteristic;
  doc["value"] = value;
  char payload[256];
  serializeJson(doc, payload);
  const char* topic = "homebridge/to/set";
  publish(topic, payload);
}

void ArduinoHomebridgeMqtt::setValueToHomebridge(const char* name, const char* serviceName, const char* characteristic, JsonVariantConst value) {
  StaticJsonDocument<256> doc;
  doc["name"] = name;
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
  // Serial.printf("Message sent [%s] %s\n", topic, payload);
}
