#include "Characteristic.h"

void Characteristic::setName(String name) {
  this->name = name;
}

void Characteristic::setValue(int value) {
  this->value = value;
}

String Characteristic::getName() {
  return name;
}

int Characteristic::getValue() {
  return value;
}
