#include "ArduinoHomebridgeMqtt.h"

#define MQTT_SERVER IPAddress(192, 168, 1, 40)
#define SERVICE_NAME "Light"
#define OUTPUT_PIN D1

void callback(bool on);
ArduinoHomebridgeMqtt homebridgeMqtt;

void callback(bool on) {
  if (on) {
    Serial.println("ON");
    digitalWrite(OUTPUT_PIN, HIGH);
  } else {
    Serial.println("OFF");
    digitalWrite(OUTPUT_PIN, LOW);
  }
}

void setup() {
  homebridgeMqtt.setMqttServer(MQTT_SERVER);
  homebridgeMqtt.setServiceName(SERVICE_NAME);
  homebridgeMqtt.setCallback(callback);
  homebridgeMqtt.connect();
}

void loop() {

}
