#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"
#include "Switch.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 42)
#define OUTPUT_PIN D1
#define SERVICE_NAME "Sprinkler"
#define SERVICE "Switch"
#define CURRENT_RELATIVE_HUMIDITY "CurrentRelativeHumidity"

WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;
Switch sprinkler(OUTPUT_PIN);

void callback(String serviceName, String characteristic, bool value) {
  if (serviceName == SERVICE_NAME) {
    if (value) {
      sprinkler.on();
    } else {
      sprinkler.off();
    }
  }
}

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(SERVICE_NAME, SERVICE);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
}
