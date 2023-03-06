#ifndef REMOTE_H
#define REMOTE_H
#include <vector>

#include "outputs.h"
struct TargetPosition {
   int id;
   volatile float position;
};

struct RemoteControlTarget {
   volatile float speed;
   std::vector<TargetPosition> positions;
   bool setServoPositionById(int id, float position);
};

RemoteControlTarget createRemoteControlTarget(
    const std::vector<ServoState>& servos, float speed);

#endif  // REMOTE_H