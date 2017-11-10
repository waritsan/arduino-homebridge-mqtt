#include "ArduinoHomebridgeMqtt.h"

ArduinoHomebridgeMqtt::ArduinoHomebridgeMqtt(IPAddress mqttHost) {
  this->mqttHost = mqttHost;
}

void ArduinoHomebridgeMqtt::addSwitch(std::function<void (int)> onSetOn) {
  this->onSetOn = onSetOn;
  Service switchService;
  switchService.setName("Switch");
  switchService.setType("Switch");
  Characteristic onCharacteristic;
  onCharacteristic.setName("On");
  switchService.addCharacteristic(onCharacteristic);
  addService(switchService);
}

void ArduinoHomebridgeMqtt::connect() {
  mqttClient.onConnect([this](bool sessionPresent) -> void {
    Serial.println("Connected to MQTT host");
    mqttClient.subscribe("homebridge/from/set", 0);
    mqttClient.subscribe("homebridge/from/response", 0);
    getAccessory();
  });
  mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void {
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.print("Error: ");
      Serial.println(topic);
      Serial.println(payload);
      return;
    }
    if (root["ack"] == false) {
      String message = root["message"];
      if (message.indexOf(accessory.getName()) > 0) {
        addAccessory();
      }
      return;
    }
    if (strcmp(topic, "homebridge/from/set") == 0 && accessory.getName() == root["name"] && accessory.getServiceByName(root["service_name"])) {
      accessory.getServiceByName(root["service_name"])->getCharacteristicByName(root["characteristic"])->setValue(root["value"]);
      if (root["characteristic"] == "On") {
        onSetOn(root["value"]);
      }
    }
    if (strcmp(topic, "homebridge/from/response") == 0 && root[accessory.getName()]) {
      if (int value = root[accessory.getName()]["characteristics"]["Switch"]["On"]) {
        accessory.getServiceByName("Switch")->getCharacteristicByName("On")->setValue(value);
        onSetOn(value);
      }
    }
  });
  mqttClient.setServer(mqttHost, DEFAULT_MQTT_PORT);
  mqttClient.connect();
}

// Private
void ArduinoHomebridgeMqtt::addService(Service service) {
  accessory.addService(service);
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.getName();
  root["service_name"] = service.getName();
  root["service"] = service.getType();
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/add/service", 0, true, payload);
  Serial.println(payload);
}

void ArduinoHomebridgeMqtt::addAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.getName();
  root["service_name"] = accessory.getFirstService()->getName();
  root["service"] = accessory.getFirstService()->getType();
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/add", 0, true, payload);
  Serial.println(payload);
}

void ArduinoHomebridgeMqtt::getAccessory() {
  StaticJsonBuffer<128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = accessory.getName();
  char payload[128];
  root.printTo(payload);
  mqttClient.publish("homebridge/to/get", 0, true, payload);
}
