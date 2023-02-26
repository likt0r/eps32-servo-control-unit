#ifndef MOTION_H
#define MOTION_H

#include <sstream>
#include <unordered_map>

#include "keyframe.h"
enum MotionMode { PLAYER, REMOTE_CONTROL, STARTUP, IDLE };

class Motion {
  public:
   int maxNumKeyframes_;
   Keyframe* keyframes;
   Motion(const Keyframe* keyframes_, int numKeyframes_);
   ~Motion();
   std::string toJsonString() const;
};

#endif  // MOTION_H