#ifndef MOTION_H
#define MOTION_H

#include <ArduinoJson.h>

#include <memory>
#include <vector>

#include "Posture.h"

class Motion {
  public:

   // Constructor with arguments
   Motion(std::string name, std::string id, std::vector<Posture> postures,
          bool loop, float speedMultiplier);

   // Constructor with JSON object
   Motion(JsonObject& json);

   // Move constructor
   Motion(Motion&& other) noexcept;

   // Move assignment operator
   Motion& operator=(Motion&& other) noexcept;

   // Convert to JSON object
   DynamicJsonDocument toJSON() const;

   // Public getter methods
   const std::string& getName() const { return *m_name; }
   const std::string& getId() const { return *m_id; }
   const std::vector<Posture>& getPostures() const { return m_postures; }
   bool getLoop() const { return m_loop; }
   float getSpeedMultiplier() const { return *m_speedMultiplier; }

  private:
   // Member variables
   std::unique_ptr<std::string> m_name;
   std::unique_ptr<std::string> m_id;
   std::vector<Posture> m_postures;
   bool m_loop;
   std::unique_ptr<float> m_speedMultiplier;
};

#endif  // MOTION_H
