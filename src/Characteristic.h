#include <Arduino.h>

class Characteristic {
private:
  String name;
  int value;
public:
  void setName(String);
  void setValue(int);
  String getName();
  int getValue();
};
