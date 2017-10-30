#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#define DEFAULT_MQTT_PORT 1883
#define SERVICES_SIZE 10
#define CHARACTERISTICS_SIZE 20

struct Characteristic {
  String name;
  int value;
};

struct Service {
  String name;
  String type;
  Characteristic characteristics[CHARACTERISTICS_SIZE];
};

struct Accessory {
  String name;
  Service services[SERVICES_SIZE];
};

class ArduinoHomebridgeMqtt {
private:
  Accessory& accessory;
  AsyncMqttClient mqttClient;
  IPAddress mqttHost;
  
public:
  ArduinoHomebridgeMqtt(Accessory& accessory, IPAddress mqttHost);
  void getAccessory();
  void connect();
};
