#ifndef REMOTE_H
#define REMOTE_H
#include <vector>

struct TargetPosition {
   int id;
   float position;
};

struct RemoteControlTarget {
   float speed;
   std::vector<TargetPosition> positions;
   bool setServoPositionById(int id, float position);
};

#endif  // REMOTE_H