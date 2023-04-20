#include "MotionManager.h"

#include <FS.h>
#include <SPIFFS.h>

#include "MotionManager.h"

MotionManager::MotionManager() {}
// Store all motions to file
void MotionManager::storeMotions() {
   File file = SPIFFS.open("motions.bin", "w");
   if (!file) {
      Serial.println("Failed to open file for writing");
      return;
   }

   DynamicJsonDocument doc(2048);
   JsonArray motionsArray = doc.createNestedArray("motions");

   for (const Motion& motion : m_allMotions) {
      DynamicJsonDocument motionDoc = motion.toJSON();
      motionsArray.add(motionDoc.as<JsonObject>());
   }

   serializeJson(doc, file);
   file.close();
}

// Load all motions from file
void MotionManager::loadMotions() {
   File file = SPIFFS.open("motions.bin", "r");
   if (!file) {
      Serial.println("Failed to open file for reading");
      return;
   }

   DynamicJsonDocument doc(2048);
   DeserializationError error = deserializeJson(doc, file);
   if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      file.close();
      return;
   }

   file.close();
   JsonArray motionsArray = doc["motions"];
   m_allMotions.clear();

   for (JsonObject motionJson : motionsArray) {
      Motion motion(motionJson);
      m_allMotions.emplace_back(motion);
   }
}
