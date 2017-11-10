#include "Service.h"

void Service::setName(String name) {
  this->name = name;
}

void Service::setType(String type) {
  this->type = type;
}

String Service::getName() {
  return name;
}

String Service::getType() {
  return type;
}

void Service::addCharacteristic(Characteristic characteristic) {
  characteristics[count] = characteristic;
  count++;
}

Characteristic* Service::getCharacteristicByName(String characteristicName) {
  for (size_t i = 0; i < count; i++) {
    if (characteristics[i].getName() == characteristicName) {
      return &characteristics[i];
    }
  }
  return NULL;
}
