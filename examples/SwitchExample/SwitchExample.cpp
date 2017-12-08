#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define OUTPUT_PIN D1

WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridgeMqtt;
Accessory switchAccessory;
Service switchService;
Characteristic onCharacteristic;

void callback(Accessory accessory, Service service, Characteristic characteristic) {
  if (switchAccessory.name != accessory.name || switchService.name != service.name || onCharacteristic.name != characteristic.name) {
    return;
  }
  if (characteristic.value) {
    Serial.println("ON");
    digitalWrite(OUTPUT_PIN, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(OUTPUT_PIN, LOW);
  }
  onCharacteristic = characteristic;
}

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();

  switchAccessory.name = "esp_switch";
  switchService.name = "esp_switch";
  switchService.type = "Switch";
  onCharacteristic.name = "On";

  homebridgeMqtt.onSetValueFromHomebridge(callback);
  homebridgeMqtt.connect(MQTT_SERVER);
  homebridgeMqtt.addAccessory(switchAccessory, switchService);
  homebridgeMqtt.getAccessory(switchAccessory);
}

void loop() {
}
