#include <Arduino.h>
#include <WiFiManager.h>
#include "LightSensor.h"
#include "Switch.h"
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define LIGHT_SENSOR_INPUT_PIN A0
#define LIGHT_SWITCH_OUTPUT_PIN D1
#define LIGHT_SENSOR_NAME "LightSensor"
#define LIGHT_SENSOR_SERVICE "LightSensor"
#define LIGHT_SWITCH_NAME "Light"
#define LIGHT_SWITCH_SERVICE "Switch"
#define AMBIENT_LIGHT_LIMIT 2

long lastMsg = 0;
WiFiManager wifiManager;
LightSensor lightSensor(LIGHT_SENSOR_INPUT_PIN);
Switch lightSwitch(LIGHT_SWITCH_OUTPUT_PIN);
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

void callback(String serviceName, String characteristic, bool value) {
  if (serviceName == LIGHT_SWITCH_NAME) {
    if (value) {
      lightSwitch.on();
    } else {
      lightSwitch.off();
    }
  }
}

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addService(LIGHT_SENSOR_NAME, LIGHT_SENSOR_SERVICE);
  arduinoHomebridgeMqtt.addService(LIGHT_SWITCH_NAME, LIGHT_SWITCH_SERVICE);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float ambientLight = lightSensor.readAmbientLight();
    Serial.print("Ambient light: ");
    Serial.print(ambientLight);
    Serial.println(" lux");
    arduinoHomebridgeMqtt.setValueToHomebridge(LIGHT_SENSOR_NAME, CURRENT_AMBIENT_LIGHT_LEVEL, ambientLight);
    if (ambientLight < AMBIENT_LIGHT_LIMIT) {
      lightSwitch.on();
      arduinoHomebridgeMqtt.setValueToHomebridge(LIGHT_SWITCH_NAME, ON, true);
    } else {
      lightSwitch.off();
      arduinoHomebridgeMqtt.setValueToHomebridge(LIGHT_SWITCH_NAME, ON, false);
    }
  }
}
