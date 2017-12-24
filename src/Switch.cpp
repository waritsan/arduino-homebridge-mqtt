#include "Switch.h"

Switch::Switch(int outputPin) {
  this->outputPin = outputPin;
  pinMode(outputPin, OUTPUT);
}

void Switch::on() {
  digitalWrite(outputPin, HIGH);
}

void Switch::off() {
  digitalWrite(outputPin, LOW);
}
