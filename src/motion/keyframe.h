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
   Keyframe(int numMotors_, int transitionTime_, int stayTime_,
            Motor* motors_) {
      numMotors = numMotors_;
      transitionTime = transitionTime_;
      stayTime = stayTime_;
      motors = new Motor[numMotors_];
      for (int i = 0; i < numMotors_; i++) {
         motors[i] = motors_[i];
      }
   }

   ~Keyframe() { delete[] motors; }
   std::string toJsonString() const {
      std::stringstream ss;
      ss << "{";
      ss << "\"numMotors\": " << numMotors << ", ";
      ss << "\"transitionTime\": " << transitionTime << ", ";
      ss << "\"stayTime\": " << stayTime << ", ";
      ss << "\"motors\": [";
      for (int i = 0; i < numMotors; i++) {
         ss << "{";
         ss << "\"id\": " << motors[i].id << ", ";
         ss << "\"position\": " << motors[i].position;
         ss << "}";
         if (i < numMotors - 1) {
            ss << ", ";
         }
      }
      ss << "]";
      ss << "}";
      return ss.str();
   }
};