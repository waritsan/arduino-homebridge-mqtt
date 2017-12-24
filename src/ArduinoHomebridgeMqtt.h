#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#ifndef DEFAULT_MQTT_PORT
#define DEFAULT_MQTT_PORT 1883
#endif

#define ON "On"
#define CURRENT_AMBIENT_LIGHT_LEVEL "CurrentAmbientLightLevel"
#define CURRENT_TEMPERATURE "CurrentTemperature"
#define CURRENT_HEATING_COOLING_STATE "CurrentHeatingCoolingState"
#define TARGET_HEATING_COOLING_STATE "TargetHeatingCoolingState"
#define TARGET_TEMPERATURE "TargetTemperature"
#define OFF 0
#define HEAT 1
#define COOL 2
#define AUTO 3

#ifndef ARDUINO_HOMEBRIDGE_MQTT_H
#define ARDUINO_HOMEBRIDGE_MQTT_H

class ArduinoHomebridgeMqtt {
private:
  AsyncMqttClient mqttClient;
  String accessoryName;
  std::function<void(String serviceName, String characteristic, float value)> callback;

public:
  ArduinoHomebridgeMqtt();
  void onSetValueFromHomebridge(std::function<void(String serviceName, String characteristic, float value)>);
  void connect(IPAddress);
  void addAccessory(String serviceName, String service);
  void addService(String serviceName, String service);
  void removeAccessory();
  void removeService(String serviceName);
  void getAccessory();
  void setValueToHomebridge(String serviceName, String characteristic, float value);
};

#endif
