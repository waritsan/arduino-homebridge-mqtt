#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#define DEFAULT_MQTT_HOST IPAddress(192, 168, 1, 40)
#define DEFAULT_MQTT_PORT 1883

class ArduinoHomebridgeMqtt {
private:
  static const String CURRENT_TEMPERATURE;
  static const String TARGET_TEMPERATURE;
  static const String CURRENT_HEATING_COOLING_STATE;
  static const String TARGET_HEATING_COOLING_STATE;
  static const String ON;

  AsyncMqttClient mqttClient;
  IPAddress ipAddress;
  int port;
  String name;
  std::function<void(int)> callback;
  std::function<void(int)> callback1;
  void set(String characteristic, int value);
  void onMessage();
public:
  ArduinoHomebridgeMqtt();
  ArduinoHomebridgeMqtt(String name, IPAddress ipAddress, int port);
  void setName(String name);
  void setMqttServer(IPAddress ipAddress, int port);
  void setCallback(std::function<void(int)>);
  void setCallback(std::function<void(int)>, std::function<void(int)>);
  void connect();
  void setCurrentTemperature(int);
  void setCurrentHeatingCoolingState(int);
  void getAccessory();
};
