#include "Posture.h"

// Constructor with arguments
Posture::Posture(std::string name, std::string id, float transitionDuration,
                 float pauseDuration,
                 std::vector<TargetPosition> targetPositions)
    : m_name(new std::string(name)),
      m_id(new std::string(id)),
      m_transitionDuration(new float(transitionDuration)),
      m_pauseDuration(new float(pauseDuration)),
      m_targetPositions(targetPositions) {}

// Constructor with JSON object
Posture::Posture(JsonObject& json) {
   if (json.containsKey("name") && json.containsKey("id") &&
       json.containsKey("transitionDuration") &&
       json.containsKey("pauseDuration") &&
       json.containsKey("targetPositions")) {
      m_name.reset(new std::string(json["name"].as<std::string>()));
      m_id.reset(new std::string(json["id"].as<std::string>()));
      m_transitionDuration.reset(new float(json["transitionDuration"]));
      m_pauseDuration.reset(new float(json["pauseDuration"]));

      JsonArray targetPositionsJson = json["targetPositions"].as<JsonArray>();
      for (JsonObject targetPositionJson : targetPositionsJson) {
         // TargetPosition targetPosition(targetPositionJson);
         m_targetPositions.emplace_back(targetPositionJson);
      }
   } else {
      // Handle the case when required keys are missing in the JSON object
      // You can throw an exception or set default values, depending on your
      // requirements
      m_name.reset(new std::string(""));
      m_id.reset(new std::string(""));
      m_transitionDuration.reset(new float(0.0f));
      m_pauseDuration.reset(new float(0.0f));
   }
}

// Copy constructor
Posture::Posture(const Posture& other)
    : m_name(new std::string(*other.m_name)),
      m_id(new std::string(*other.m_id)),
      m_transitionDuration(new float(*other.m_transitionDuration)),
      m_pauseDuration(new float(*other.m_pauseDuration)),
      m_targetPositions(other.m_targetPositions) {}

// Move constructor
Posture::Posture(Posture&& other) noexcept
    : m_name(std::move(other.m_name)),
      m_id(std::move(other.m_id)),
      m_transitionDuration(std::move(other.m_transitionDuration)),
      m_pauseDuration(std::move(other.m_pauseDuration)),
      m_targetPositions(std::move(other.m_targetPositions)) {}

// Move assignment operator
Posture& Posture::operator=(Posture&& other) noexcept {
   if (this != &other) {
      m_name = std::move(other.m_name);
      m_id = std::move(other.m_id);
      m_transitionDuration = std::move(other.m_transitionDuration);
      m_pauseDuration = std::move(other.m_pauseDuration);
      m_targetPositions = std::move(other.m_targetPositions);
   }
   return *this;
}

// Convert to JSON object
DynamicJsonDocument Posture::toJSON() const {
   DynamicJsonDocument jsonDoc(1024);
   JsonObject jsonObj = jsonDoc.to<JsonObject>();
   jsonObj["name"] = *m_name;
   jsonObj["id"] = *m_id;
   jsonObj["transitionDuration"] = *m_transitionDuration;
   jsonObj["pauseDuration"] = *m_pauseDuration;

   JsonArray targetPositionsJson = jsonObj.createNestedArray("targetPositions");
   for (const TargetPosition& targetPosition : m_targetPositions) {
      JsonObject targetPositionJson = targetPosition.toJSON();
      targetPositionsJson.add(targetPositionJson);
   }

   return jsonDoc;
}
