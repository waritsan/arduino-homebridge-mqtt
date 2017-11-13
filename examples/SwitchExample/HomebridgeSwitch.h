#include "ArduinoHomebridgeMqtt.h"

class HomebridgeSwitch {
private:
  String name;
  int outputPin;
  bool on;
  ArduinoHomebridgeMqtt homebridgeMqtt;

public:
  HomebridgeSwitch(String name, IPAddress server, int outputPin);
  void connect();
};
