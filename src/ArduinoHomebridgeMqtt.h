#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

class ArduinoHomebridgeMqtt {
private:
  static const int DEFAULT_MQTT_PORT;
  static const String CURRENT_TEMPERATURE;
  static const String TARGET_TEMPERATURE;
  static const String CURRENT_HEATING_COOLING_STATE;
  static const String TARGET_HEATING_COOLING_STATE;
  static const String ON;

  AsyncMqttClient mqttClient;
  String accessoryName;
  String serviceName;
  std::function<void(int)> callback;
  std::function<void(int)> callback1;
  void onMessage();
  void getAccessory();
  void addAccessory();
  void addService();

public:
  ArduinoHomebridgeMqtt();
  ArduinoHomebridgeMqtt(String serviceName, IPAddress server);
  void setServiceName(String serviceName);
  void setMqttServer(IPAddress server);
  void setCallback(std::function<void(int)> callback);
  void setCallback(std::function<void(int)> callback, std::function<void(int)> callback1);
  void connect();
  void set(String characteristic, int value);
};
