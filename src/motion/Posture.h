#ifndef POSTURE_H
#define POSTURE_H

#include <ArduinoJson.h>

#include <memory>
#include <vector>

#include "TargetPosition.h"

class Posture {
  public:
   // Constructor with arguments
   Posture(std::string name, std::string id, float transitionDuration,
           float pauseDuration, std::vector<TargetPosition> targetPositions);

   // Copy constructor
   Posture(const Posture& other);

   // Constructor with JSON object
   Posture(JsonObject& json);

   // Move constructor
   Posture(Posture&& other) noexcept;

   // Move assignment operator
   Posture& operator=(Posture&& other) noexcept;

   // Convert to JSON object
   DynamicJsonDocument toJSON() const;

   // Public getter methods
   const std::string& getName() const { return *m_name; }
   const std::string& getId() const { return *m_id; }
   float getTransitionDuration() const { return *m_transitionDuration; }
   float getPauseDuration() const { return *m_pauseDuration; }
   const std::vector<TargetPosition>& getTargetPositions() const {
      return m_targetPositions;
   }

  private:
   // Member variables
   std::unique_ptr<std::string> m_name;
   std::unique_ptr<std::string> m_id;
   std::unique_ptr<float> m_transitionDuration;
   std::unique_ptr<float> m_pauseDuration;
   std::vector<TargetPosition> m_targetPositions;
};

#endif  // POSTURE_H
