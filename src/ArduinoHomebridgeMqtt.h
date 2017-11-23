#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#define DEFAULT_MQTT_PORT 1883

struct Accessory {
  String name;
};

struct Service {
  String name;
  String type;
};

struct Characteristic {
  String name;
  int value;
};

class ArduinoHomebridgeMqtt {
private:
  AsyncMqttClient mqttClient;
  std::function<void(Accessory, Service, Characteristic)> callback;

public:
  void onCallback(std::function<void(Accessory, Service, Characteristic)>);
  void connect(IPAddress);
  void addAccessory(Accessory, Service);
  void addService(Accessory, Service);
  void removeAccessory(Accessory);
  void removeService(Accessory, Service);
  void getCharacteristic(Accessory, Service, Characteristic);
  void setChracteristic(Accessory, Service, Characteristic);
};
