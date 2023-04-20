#include "TargetPosition.h"

// Constructor with arguments
TargetPosition::TargetPosition(const int id, float position)
    : m_id(new int(id)), m_position(new float(position)) {}

// Constructor with JSON object
TargetPosition::TargetPosition(JsonObject& json) {
   if (json.containsKey("id") && json.containsKey("position")) {
      int id = json["id"];
      m_id.reset(new int(id));
      float position = json["position"];
      m_position.reset(new float(position));
   } else {
      throw std::runtime_error(
          "Required keys are missing in the JSON TargetPosition object");
   }
}

// Copy constructor
TargetPosition::TargetPosition(const TargetPosition& other)
    : m_id(new int(*other.m_id)), m_position(new float(*other.m_position)) {}

// Move constructor
TargetPosition::TargetPosition(TargetPosition&& other) noexcept
    : m_id(std::move(other.m_id)), m_position(std::move(other.m_position)) {}

// Move assignment operator
TargetPosition& TargetPosition::operator=(TargetPosition&& other) noexcept {
   if (this != &other) {
      m_id = std::move(other.m_id);
      m_position = std::move(other.m_position);
   }
   return *this;
}

// Convert to JSON object
JsonObject TargetPosition::toJSON() const {
   StaticJsonDocument<64> jsonDoc;
   JsonObject jsonObj = jsonDoc.to<JsonObject>();
   jsonObj["id"] = *m_id;
   jsonObj["position"] = *m_position;
   return jsonObj;
}

// Setter implementation
void TargetPosition::setPosition(float position) { *m_position = position; }
