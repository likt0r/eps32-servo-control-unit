#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <Arduino.h>

#include <sstream>

struct Motor {
   int id;
   int position;
   Motor() : id(0), position(0) {}
};

class Keyframe {
  public:
   int numMotors;
   int transitionTime;
   int stayTime;
   Motor* motors;
   Keyframe() : numMotors(0), transitionTime(0), stayTime(0), motors(nullptr) {}
   Keyframe(int numMotors_, int transitionTime_, int stayTime_, Motor* motors_);

   ~Keyframe();
   std::string toJsonString() const;
};

#endif  // KEYFRAME_H