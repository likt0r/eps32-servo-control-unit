#ifndef REMOTE_H
#define REMOTE_H
#include <vector>

#include "TargetPosition.h"
#include "outputs.h"

struct RemoteControlTarget {
   float speed;
   std::vector<TargetPosition> positions;
   bool setServoPositionById(int id, float position);
};

RemoteControlTarget createRemoteControlTarget(
    const std::vector<ServoState>& servos, float speed);

#endif  // REMOTE_H