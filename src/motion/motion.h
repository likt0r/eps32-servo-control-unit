
#include <sstream>

#include "keyframe.h"

enum MotionMode { PLAYER, REMOTE_CONTROL, STARTUP, IDLE };

class Motion {
  public:
   int maxNumKeyframes_;
   Keyframe* keyframes;
   Motion(const Keyframe* keyframes_, int numKeyframes_) {
      maxNumKeyframes_ = numKeyframes_;
      keyframes = new Keyframe[maxNumKeyframes_];
      for (int i = 0; i < maxNumKeyframes_; i++) {
         keyframes[i] = keyframes_[i];
      }
   }

   ~Motion() { delete[] keyframes; }
   std::string toJsonString() const {
      std::stringstream ss;
      ss << "{";
      ss << "\"maxNumKeyframes\": " << maxNumKeyframes_ << ", ";
      ss << "\"keyframes\": [";
      for (int i = 0; i < maxNumKeyframes_; i++) {
         ss << keyframes[i].toJsonString();
         if (i < maxNumKeyframes_ - 1) {
            ss << ", ";
         }
      }
      ss << "]";
      ss << "}";
      return ss.str();
   };
};

struct TargetAngles {
   int id;
   int position;
   TargetAngles() : id(0), position(0) {}
};
