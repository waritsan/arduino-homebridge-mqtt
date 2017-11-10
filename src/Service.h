#include <Arduino.h>
#include "Characteristic.h"

class Service {
private:
  String name;
  String type;
  Characteristic characteristics[10];
  int count = 0;

public:
  void setName(String);
  void setType(String);
  String getName();
  String getType();
  void addCharacteristic(Characteristic);
  Characteristic* getCharacteristicByName(String);
};
