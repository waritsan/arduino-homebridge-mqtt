#include <Arduino.h>
#include "ArduinoHomebridgeMqtt.h"
#include "WiFiManager.h"

#define MQTT_HOST IPAddress(192, 168, 1, 40)
#define MQTT_PORT 1883
#define ACCESSORY_NAME "esp_switch"
#define OUTPUT_PIN D1

WiFiManager wifiManager;
ArduinoHomebridgeMqtt homebridgeMqtt(ACCESSORY_NAME, MQTT_HOST, MQTT_PORT);

void callback(bool isOn) {
  if (isOn) {
    Serial.println("ON");
    digitalWrite(OUTPUT_PIN, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(OUTPUT_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  homebridgeMqtt.setCallback(callback);
  homebridgeMqtt.connect();
}

void loop() {
    // put your main code here, to run repeatedly:
}
