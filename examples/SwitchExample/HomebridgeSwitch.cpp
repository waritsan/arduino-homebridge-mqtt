#include "HomebridgeSwitch.h"

HomebridgeSwitch::HomebridgeSwitch(String name, IPAddress server, int outputPin) {
  this->name = name;
  this->outputPin = outputPin;
  homebridgeMqtt.setServiceName(name);
  homebridgeMqtt.setMqttServer(server);
  homebridgeMqtt.setCallback([this](bool on) -> void {
    this->on = on;
    if (on) {
      Serial.println("ON");
      digitalWrite(this->outputPin, HIGH);
    } else {
      Serial.println("OFF");
      digitalWrite(this->outputPin, LOW);
    }
  });
}

void HomebridgeSwitch::connect() {
  homebridgeMqtt.connect();
}
