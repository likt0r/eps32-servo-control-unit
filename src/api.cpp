#include "api.h"

#include <ESPAsyncWebServer.h>

#include <string>

#include "motion/remote.h"
#include "outputs.h"

void setupApi(AsyncWebServer *server_p, PinOutputs *outputs_p) {
   server_p->on(
       "/api/status", HTTP_GET, [outputs_p](AsyncWebServerRequest *request) {
          Serial.println("/api/status");
          request->send(200, "application/json", outputs_p->outputToJson());
       });

   server_p->on("/api/led", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "");
   });

   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/led",
       [outputs_p](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.println("/api/led");
          JsonObject const &jsonObj = json.as<JsonObject>();
          int id = jsonObj["id"];
          bool isOn = jsonObj["isOn"];
          bool hasLed = outputs_p->setLedStatusById(id, isOn);

          if (hasLed) {
             digitalWrite(outputs_p->getLedPin(id), isOn ? HIGH : LOW);
             request->send(204);

          } else {
             std::string errorStr = "{\"error\":\"Led with ID " +
                                    std::to_string(id) + " not found\"}";
             request->send(400, "application/json", errorStr.c_str());
          }
       }));
};
