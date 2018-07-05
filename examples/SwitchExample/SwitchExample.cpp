#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"
#include "Switch.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define OUTPUT_PIN D1
#define SERVICE_NAME "Switch"
#define SERVICE "Switch";
#define ON "On"

WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;
Switch mySwitch(OUTPUT_PIN);

void callback(String serviceName, String characteristic, bool value) {
  if (serviceName == SERVICE_NAME) {
    if (value) {
      mySwitch.on();
    } else {
      mySwitch.off();
    }
  }
}

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
}
