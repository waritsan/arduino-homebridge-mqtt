#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#define MQTT_HOST IPAddress(192, 168, 1, 40)
#define MQTT_PORT 1883

class ArduinoHomebridgeMqtt {
private:
  static const String CURRENT_TEMPERATURE;
  static const String TARGET_TEMPERATURE;
  static const String CURRENT_HEATING_COOLING_STATE;
  static const String TARGET_HEATING_COOLING_STATE;
  static const String ON;

  AsyncMqttClient mqttClient;
  String name;
  std::function<void(int)> callback;
  std::function<void(int)> callback1;
  void set(String characteristic, int value);
public:
  void setName(String name);
  void setCallback(std::function<void(int)>);
  void setCallback(std::function<void(int)>, std::function<void(int)>);
  void begin();
  void setCurrentTemperature(int);
  void setCurrentHeatingCoolingState(int);
  void getAccessory();
};
