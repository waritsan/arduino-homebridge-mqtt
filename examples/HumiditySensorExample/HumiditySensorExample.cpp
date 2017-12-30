#include <Arduino.h>
#include <WiFiManager.h>
#include "HumiditySensor.h"
#include "Switch.h"
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define INPUT_PIN A0
#define WATER_PIN D1
#define HUMIDITY_SENSOR_NAME "HumiditySensor"
#define HUMIDITY_SENSOR_SERVICE "HumiditySensor"
#define SWITCH_NAME "Water"
#define SWITCH_SERVICE "Switch"
#define HUMIDITY_LIMIT 30

long lastMsg = 0;
WiFiManager wifiManager;
HumiditySensor humiditySensor(INPUT_PIN);
Switch waterSwitch(WATER_PIN);
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

void callback(String serviceName, String characteristic, bool value) {
  if (serviceName == SWITCH_NAME) {
    if (value) {
      waterSwitch.on();
    } else {
      waterSwitch.off();
    }
  }
}

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(HUMIDITY_SENSOR_NAME, HUMIDITY_SENSOR_SERVICE);
  arduinoHomebridgeMqtt.addService(HUMIDITY_SENSOR_NAME, HUMIDITY_SENSOR_SERVICE);
  arduinoHomebridgeMqtt.addService(SWITCH_NAME, SWITCH_SERVICE);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    int currentHumidity = humiditySensor.readHumidity();
    Serial.print("Current humidity ");
    Serial.print(currentHumidity);
    Serial.println("%");
    arduinoHomebridgeMqtt.setValueToHomebridge(HUMIDITY_SENSOR_NAME, CURRENT_RELATIVE_HUMIDITY, currentHumidity);
    if (currentHumidity < HUMIDITY_LIMIT) {
      waterSwitch.on();
      arduinoHomebridgeMqtt.setValueToHomebridge(SWITCH_NAME, ON, true);
    } else {
      waterSwitch.off();
      arduinoHomebridgeMqtt.setValueToHomebridge(SWITCH_NAME, ON, false);
    }
  }
}
