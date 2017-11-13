#include <Arduino.h>
#include "WiFiManager.h"
#include "HomebridgeSwitch.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define OUTPUT_PIN D1
#define SWITCH_NAME "Light"

WiFiManager wifiManager;
HomebridgeSwitch homebridgeSwitch(SWITCH_NAME, OUTPUT_PIN, MQTT_SERVER);

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  homebridgeSwitch.connect();
}

void loop() {
    // put your main code here, to run repeatedly:
}
