#include <Arduino.h>
#include <WiFiManager.h>
#include "Switch.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define OUTPUT_PIN D1

WiFiManager wifiManager;
Switch mySwitch(OUTPUT_PIN);

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  mySwitch.begin(MQTT_SERVER);
}

void loop() {
}
