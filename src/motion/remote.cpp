#include "remote.h"

#include "outputs.h"

bool RemoteControlTarget::setServoPositionById(int id, float position) {
   for (auto& servo : positions) {
      if (servo.id == id) {
         servo.position = position;
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
      TargetPosition position;
      position.id = servo.id;
      position.position = servo.position;
      target.positions.push_back(position);
   }

   return target;
}