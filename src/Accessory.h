#include <Arduino.h>
#include "Service.h"

class Accessory {
private:
  String name = "ESP" + ESP.getChipId();;
  Service services[10];
  int count = 0;

public:
  String getName();
  void addService(Service);
  Service* getServiceByName(String);
  Service* getFirstService();
};
