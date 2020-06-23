/*
	ThermostatExample.ino
	This is a thermostat accessory example. It reads a temperature with Dallas temperature sensor attached to 
  pin D1 and sends the value to Homebridge MQTT server. You can control the thermostat from the Home app.

	The circuit:
	* Inputs
        D1 - Dallas temperature sensor (DS18B20)
	* Outputs
        D2 - output for heating
        D3 - output for cooling
*/

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include <ArduinoHomebridgeMqtt.h>

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48);
const int INPUT_PIN = D1;
const int HEAT_PIN = D2;
const int COOL_PIN = D3;
const char* NAME = "esp_thermostat";
const char* SERVICE_NAME = "Thermostat";
const char* SERVICE = "Thermostat";
const char* CURRENT_HEATING_COOLING_STATE = "CurrentHeatingCoolingState";
const char* TARGET_HEATNG_COOLING_STATE = "TargetHeatingCoolingState";
const char* CURRENT_TEMPERATURE = "CurrentTemperature";
const char* TARGET_TEMPERATURE = "TargetTemperature";
const int OFF = 0;
const int HEAT = 1;
const int COOL = 2;
const int AUTO = 3;

long lastMsg = 0;
WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;
int targetTemperature;
int currentTemperature;
int targetHeatingCoolingState;
int currentHeatingCoolingState;

// This gets called when a value is set from the Home app.
void callback(const char* name, const char* serviceName, const char* characteristic, int value) {
  if (strcmp(name, NAME) == 0) {
    if (strcmp(serviceName, SERVICE_NAME) == 0) {
      if (strcmp(characteristic, TARGET_TEMPERATURE) == 0) { 
        targetTemperature = value;
      } else if (strcmp(characteristic, TARGET_HEATNG_COOLING_STATE) == 0) {
        targetHeatingCoolingState = value;
      }
    }
  }
}

int readTemperature(int inputPin) {
  OneWire oneWire(inputPin);
  DallasTemperature dallasTemperature(&oneWire);
  dallasTemperature.requestTemperatures();
  float tempC = dallasTemperature.getTempCByIndex(0);
  return (int) tempC;
}

int off() {
  digitalWrite(COOL_PIN, LOW);
  digitalWrite(HEAT_PIN, LOW);
  return OFF;
}

int heat() {
  digitalWrite(COOL_PIN, LOW);
  digitalWrite(HEAT_PIN, HIGH);
  return HEAT;
}

int cool() {
  digitalWrite(COOL_PIN, HIGH);
  digitalWrite(HEAT_PIN, LOW);
  return COOL;
}

void setup() {
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);
  pinMode(HEAT_PIN, OUTPUT);
  pinMode(COOL_PIN, OUTPUT);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.onSetValueFromHomebridge(callback);
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(NAME, SERVICE_NAME, SERVICE);
  arduinoHomebridgeMqtt.getAccessory(NAME);
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    int newTemperature = readTemperature(INPUT_PIN);
    if (newTemperature != currentTemperature) {
      currentTemperature = newTemperature;
      Serial.print("Curent temperature: ");
      Serial.println(currentTemperature);
      arduinoHomebridgeMqtt.setValueToHomebridge(NAME, SERVICE_NAME, CURRENT_TEMPERATURE, currentTemperature);
    }
    int newHeatingCoolingState;
    switch (targetHeatingCoolingState) {
      case HEAT: (currentTemperature < targetTemperature) ? newHeatingCoolingState = heat() : newHeatingCoolingState = off();
                 break;
      case COOL: (currentTemperature < targetTemperature) ? newHeatingCoolingState = off() : newHeatingCoolingState = cool();
                 break;
      case AUTO: (currentTemperature < targetTemperature) ? newHeatingCoolingState = heat() : newHeatingCoolingState = cool();
                 break;
      default: newHeatingCoolingState = off();
    }
    if (currentHeatingCoolingState != newHeatingCoolingState) {
      currentHeatingCoolingState = newHeatingCoolingState;
      arduinoHomebridgeMqtt.setValueToHomebridge(NAME, SERVICE_NAME, CURRENT_HEATING_COOLING_STATE, currentHeatingCoolingState);
    }
  }
  arduinoHomebridgeMqtt.loop();
}
