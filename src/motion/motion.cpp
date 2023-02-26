#include "motion.h"

Motion::Motion(const Keyframe* keyframes_, int numKeyframes_) {
   maxNumKeyframes_ = numKeyframes_;
   keyframes = new Keyframe[maxNumKeyframes_];
   for (int i = 0; i < maxNumKeyframes_; i++) {
      keyframes[i] = keyframes_[i];
   }
}

Motion::~Motion() { delete[] keyframes; }
std::string Motion::toJsonString() const {
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