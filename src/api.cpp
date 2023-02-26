#include "api.h"

#include <ESPAsyncWebServer.h>

#include <string>

#include "motion/motion.h"
#include "motion/remote.h"
#include "outputs.h"

void setupApi(AsyncWebServer *server_p, Outputs *outputs_p,
              RemoteControlTarget *remoteControlTarget_p,
              MotionMode *motionMode) {
   server_p->on(
       "/api/status", HTTP_GET, [outputs_p](AsyncWebServerRequest *request) {
          Serial.println("/api/status");
          request->send(200, "application/json", outputs_p->outputToJson());
       });

   //    server.on("/api/led", HTTP_GET, [](AsyncWebServerRequest *request) {
   //       request->send(200, "application/json", outputToJson());
   //    });
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
             request->send(200, "application/json", "{\"success\":true}");

          } else {
             std::string errorStr = "{\"error\":\"Led with ID " +
                                    std::to_string(id) + " not found\"}";
             request->send(400, "application/json", errorStr.c_str());
          }

          // ...
       }));
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/motion/position",
       [remoteControlTarget_p](AsyncWebServerRequest *request,
                               JsonVariant &json) {
          Serial.println("/api/motion/position");

          JsonObject const &jsonObj = json.as<JsonObject>();
          if (!jsonObj.containsKey("id")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing id\"}");
             return;
          }
          if (!jsonObj.containsKey("position")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing id\"}");
             return;
          }
          int id = jsonObj["id"];
          float position = jsonObj["position"];
          bool hasServo =
              remoteControlTarget_p->setServoPositionById(id, position);
          if (hasServo) {
             request->send(200, "application/json", "{\"success\":true}");
          } else {
             std::string errorStr = "{\"error\":\"Servo with ID " +
                                    std::to_string(id) + " not found\"}";
             request->send(400, "application/json", errorStr.c_str());
          }
       }));

   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/motion/mode",
       [motionMode](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.println("/api/motion/mode");
          if (*motionMode == STARTUP) {
             request->send(500, "application/json",
                           "{\"error\":\"Systems Starts up try again later\"}");
             return;
          }
          JsonObject const &jsonObj = json.as<JsonObject>();
          if (!jsonObj.containsKey("mode")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing mode\"}");
             return;
          }
          //   std::string mode = jsonObj["mode"];
          std::string mode = json["mode"].as<std::string>();
          if (mode == "player") {
             *motionMode = PLAYER;
          } else if (mode == "remote_control") {
             *motionMode = REMOTE_CONTROL;
          } else if (mode == "idle") {
             *motionMode = IDLE;
          } else {
             std::string errorStr =
                 "{\"error\":\"Invalid mode " + mode + " \"}";
             request->send(400, "application/json", errorStr.c_str());
             return;
          }
          request->send(200, "application/json", "{\"success\":true}");
       }));
};
