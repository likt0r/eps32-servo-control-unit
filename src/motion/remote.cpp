#include "remote.h"

#include "outputs.h"

bool RemoteControlTarget::setServoPositionById(int id, float position) {
   for (auto& servo : positions) {
      if (servo.getId() == id) {
         servo.setPosition(position);
         return true;
      }
   }
   return false;
};

RemoteControlTarget createRemoteControlTarget(
    const std::vector<ServoState>& servos, float speed) {
   RemoteControlTarget target;
   target.speed = speed;

   for (const auto& servo : servos) {
      target.positions.emplace_back(servo.id, servo.position);
   }

   return target;
}