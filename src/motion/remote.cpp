#include "remote.h"
bool RemoteControlTarget::setServoPositionById(int id, float position) {
   for (auto &servo : positions) {
      if (servo.id == id) {
         servo.position = position;
         return true;
      }
   }
   return false;
};