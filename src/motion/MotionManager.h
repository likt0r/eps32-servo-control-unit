#ifndef MOTION_MANAGER_H
#define MOTION_MANAGER_H

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <vector>

#include "Motion.h"

class MotionManager {
  public:
   MotionManager();
   // Store all motions to file
   void storeMotions();

   // Load all motions from file
   void loadMotions();

   // Public getter methods
   const std::vector<Motion>& getAllMotions() const { return m_allMotions; }
   const Motion& getCurrentMotion() const { return m_currentMotion; }

  private:
   // Member variables
   std::vector<Motion> m_allMotions;
   Motion m_currentMotion;
};

#endif  // MOTION_MANAGER_H
