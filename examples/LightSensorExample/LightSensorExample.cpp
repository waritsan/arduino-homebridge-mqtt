#include <Arduino.h>
#include <WiFiManager.h>
#include "LightSensor.h"
#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define INPUT_PIN A0
#define SERVICE_NAME "LightSensor"
#define SERVICE "LightSensor"

long lastMsg = 0;
WiFiManager wifiManager;
LightSensor lightSensor(INPUT_PIN);
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addService(SERVICE_NAME, SERVICE);
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
    arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_AMBIENT_LIGHT_LEVEL, ambientLight);
  }
}
