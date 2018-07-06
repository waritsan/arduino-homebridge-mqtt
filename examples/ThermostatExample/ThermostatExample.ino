/*
	ThermostatExample.ino

	This is a thermostat accessory example. It reads a temperature with Dallas temperature sensor attached to 
  pin D1 and sends the value to Homebridge MQTT server. You can control the thermostat operation from the Home app.

	The circuit:
	* Inputs
        D1 - Dallas temperature sensor (DS18B20)
	* Outputs
        D2 - output for heating
        D3 - output for cooling

	Created Oct 19, 2017
	By Warit Santaputra
	Modified Jul 5, 2018
	By Warit Santaputra

*/

#include <Arduino.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 40);
const int INPUT_PIN = D1;
const int HEAT_PIN = D2;
const int COOL_PIN = D3;
const String SERVICE_NAME = "Thermostat";
const String SERVICE = "Thermostat";
const String CURRENT_HEATING_COOLING_STATE = "CurrentHeatingCoolingState";
const String TARGET_HEATNG_COOLING_STATE = "TargetHeatingCoolingState";
const String CURRENT_TEMPERATURE = "CurrentTemperature";
const String TARGET_TEMPERATURE = "TargetTemperature";
const int OFF = 0;
const int HEAT = 1;
const int COOL = 2;
const int AUTO = 3;

long lastMsg = 0;
WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;
float targetTemperature;
float currentTemperature;
int targetHeatingCoolingState;
int currentHeatingCoolingState;

// This gets called when a value is set from the Home app.
void callback(String serviceName, String characteristic, float value) {
  if (serviceName == SERVICE_NAME) {
    if (characteristic == TARGET_TEMPERATURE) { 
      targetTemperature = value;
    } else if (characteristic == TARGET_HEATNG_COOLING_STATE) {
      targetHeatingCoolingState = value;
    }
  }
}

float readTemperature(int inputPin) {
  OneWire oneWire(inputPin);
  DallasTemperature dallasTemperature(&oneWire);
  dallasTemperature.requestTemperatures();
  return dallasTemperature.getTempCByIndex(0);
}

void off() {
  digitalWrite(COOL_PIN, LOW);
  digitalWrite(HEAT_PIN, LOW);
  currentHeatingCoolingState = OFF;
}

void heat() {
  digitalWrite(COOL_PIN, LOW);
  digitalWrite(HEAT_PIN, HIGH);
  currentHeatingCoolingState = HEAT;
}

void cool() {
  digitalWrite(COOL_PIN, HIGH);
  digitalWrite(HEAT_PIN, LOW);
  currentHeatingCoolingState = COOL;
}

void setup() {
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);
  pinMode(HEAT_PIN, OUTPUT);
  pinMode(COOL_PIN, OUTPUT);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(SERVICE_NAME, SERVICE);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    currentTemperature = readTemperature(INPUT_PIN);
    switch (targetHeatingCoolingState) {
      case HEAT: (currentTemperature < targetTemperature) ? heat() : off();
                 break;
      case COOL: (currentTemperature < targetTemperature) ? off() : cool();
                 break;
      case AUTO: (currentTemperature < targetTemperature) ? heat() : cool();
                 break;
      default: off();
    }
    Serial.print("Current temperature: ");
    Serial.print(currentTemperature);
    Serial.print(", Current heating cooling state: ");
    Serial.println(currentHeatingCoolingState);
    arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_TEMPERATURE, currentTemperature);
    arduinoHomebridgeMqtt.setValueToHomebridge(SERVICE_NAME, CURRENT_HEATING_COOLING_STATE, currentHeatingCoolingState);
  }
}
