#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <vector>

struct LedState {
   int id;
   bool isOn;
   int pin;
};

struct ServoState {
   int id;
   int pin;
   float position;
   int minPwm;
   int maxPwm;
   int minAngle;
   int maxAngle;
};

struct Outputs {
   std::vector<LedState> leds;
   std::vector<ServoState> servos;
   bool setLedStatusById(int id, bool isOn);
   int getLedPin(int id);
   bool setServoPositionById(int id, float position);
   void print();
   String outputToJson();
};

#endif  // OUTPUTS_H