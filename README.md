# Arduino Homebridge MQTT

[![Build Status](https://travis-ci.org/waritsan/arduino-homebridge-mqtt.svg?branch=master)](https://travis-ci.org/waritsan/arduino-homebridge-mqtt)

Arduino library for connecting to Homebridge.

### Dependancies:
* [Mosquitto](https://mosquitto.org)
* [Homebridge](https://github.com/nfarina/homebridge)
* [Homebridge-MQTT](https://github.com/cflurin/homebridge-mqtt)

### Installation Guide
* [PlatformIO](https://platformio.org/lib/show/1907/ArduinoHomebridgeMqtt)
* [Arduino IDE](https://www.arduino.cc/en/Guide/Libraries#toc4)

### Usage
```cpp
// Switch example
#include <Arduino.h>
#include <WiFiManager.h> // You can you anything else to connect to WiFi.
#include <ArduinoHomebridgeMqtt.h>        

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48); // Put your MQTT server IP address here.
const int OUTPUT_PIN = D1;
const char* NAME = "flex_lamp"; // Accessory name must be unique
const char* SERVICE_NAME = "Light"; // Service name. 
const char* SERVICE = "Switch"; // Service as defined in Homebridge

WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

// This function gets called when there is a message from homebridge eg. when value(s) is set via Home app.
void callback(const char* name, const char* serviceName, const char* characteristic, int value) {
  if (strcmp(name, NAME) == 0) { // Check to see if the message is for this accessory.
    if (strcmp(serviceName, SERVICE_NAME) == 0) { // Check if the message is to set the value for service "Light".
      digitalWrite(OUTPUT_PIN, value);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(NAME, SERVICE_NAME, SERVICE); // If running the first time, you need to add the accessory first.
  arduinoHomebridgeMqtt.getAccessory(NAME); // Get accessory states
}

void loop() {
  arduinoHomebridgeMqtt.loop();
}
```
