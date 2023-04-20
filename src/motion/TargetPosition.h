#ifndef TARGET_POSITION_H
#define TARGET_POSITION_H

#include <ArduinoJson.h>

#include <memory>
#include <string>

class TargetPosition {
  public:
   // Constructor with arguments
   TargetPosition(const int id, float position);

   // Constructor with JSON object
   TargetPosition(JsonObject& json);

   // Copy constructor
   TargetPosition(const TargetPosition& other);

   // Move constructor
   TargetPosition(TargetPosition&& other) noexcept;

   // Move assignment operator
   TargetPosition& operator=(TargetPosition&& other) noexcept;
   // Convert to JSON object
   JsonObject toJSON() const;

   // Public getter methods
   const int getId() const { return *m_id; }
   float getPosition() const { return *m_position; }
   // Setter method
   void setPosition(float position);

  private:
   // Member variables
   std::unique_ptr<int> m_id;
   std::unique_ptr<float> m_position;
};

#endif  // TARGET_POSITION_H
