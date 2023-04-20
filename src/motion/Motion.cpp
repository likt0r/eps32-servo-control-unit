#include "Motion.h"

#include <ArduinoJson.h>

#include "Posture.h"

// Constructor with arguments
Motion::Motion(std::string name, std::string id, std::vector<Posture> postures,
               bool loop, float speedMultiplier)
    : m_name(new std::string(name)),
      m_id(new std::string(id)),
      m_postures(postures),
      m_loop(loop),
      m_speedMultiplier(new float(speedMultiplier)) {}

// Constructor with JSON object
Motion::Motion(JsonObject& json) {
   if (json.containsKey("name") && json.containsKey("id") &&
       json.containsKey("postures") && json.containsKey("loop") &&
       json.containsKey("speedMultiplier")) {
      m_name.reset(new std::string(json["name"].as<std::string>()));
      m_id.reset(new std::string(json["id"].as<std::string>()));
      m_loop = json["loop"];
      m_speedMultiplier.reset(new float(json["speedMultiplier"]));

      JsonArray posturesJson = json["postures"].as<JsonArray>();
      for (JsonObject postureJson : posturesJson) {
         // Posture posture(postureJson);
         m_postures.emplace_back(postureJson);
      }
   } else {
      // Handle the case when required keys are missing in the JSON object
      // You can throw an exception or set default values, depending on your
      // requirements
      m_name.reset(new std::string(""));
      m_id.reset(new std::string(""));
      m_loop = false;
      m_speedMultiplier.reset(new float(1.0f));
   }
}

// Move constructor
Motion::Motion(Motion&& other) noexcept
    : m_name(std::move(other.m_name)),
      m_id(std::move(other.m_id)),
      m_postures(std::move(other.m_postures)),
      m_loop(other.m_loop),
      m_speedMultiplier(std::move(other.m_speedMultiplier)) {}

// Move assignment operator
Motion& Motion::operator=(Motion&& other) noexcept {
   if (this != &other) {
      m_name = std::move(other.m_name);
      m_id = std::move(other.m_id);
      m_postures = std::move(other.m_postures);
      m_loop = other.m_loop;
      m_speedMultiplier = std::move(other.m_speedMultiplier);
   }
   return *this;
}

// Convert to JSON object
DynamicJsonDocument Motion::toJSON() const {
   DynamicJsonDocument jsonDoc(2048);
   JsonObject jsonObj = jsonDoc.to<JsonObject>();
   jsonObj["name"] = *m_name;
   jsonObj["id"] = *m_id;
   jsonObj["loop"] = m_loop;
   jsonObj["speedMultiplier"] = *m_speedMultiplier;

   JsonArray posturesJson = jsonObj.createNestedArray("postures");
   for (const Posture& posture : m_postures) {
      DynamicJsonDocument postureJson = posture.toJSON();
      posturesJson.add(postureJson.as<JsonObject>());
   }

   return jsonDoc;
}
