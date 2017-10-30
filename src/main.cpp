#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"
#include "WiFiManager.h"

#define MQTT_HOST IPAddress(192, 168, 1, 40)
#define OUTPUT_PIN D1

WiFiManager wifiManager;
Accessory accessory;
ArduinoHomebridgeMqtt homebridge(accessory, MQTT_HOST);

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  // accessory.name = "ESP" + String(ESP.getChipId());
  accessory.name = "esp_switch";
  // accessory.services[0].name = accessory.name + "esp_switch";
  accessory.services[0].name = "esp_switch";
  accessory.services[0].type = "Switch";
  accessory.services[0].characteristics[0].name = "On";
  homebridge.connect();
  homebridge.getAccessory();
}

void loop() {
  digitalWrite(OUTPUT_PIN, accessory.services[0].characteristics[0].value);
}
