#include "Accessory.h"

String Accessory::getName() {
  return name;
}

void Accessory::addService(Service service) {
  services[count] = service;
  count++;
}

Service* Accessory::getServiceByName(String serviceName) {
  for (size_t i = 0; i < count; i++) {
    if (services[i].getName() == serviceName) {
      return &services[i];
    }
  }
  return NULL;
}

Service* Accessory::getFirstService() {
  return &services[0];
}
