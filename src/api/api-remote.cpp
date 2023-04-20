#include "api-remote.h"

#include <ESPAsyncWebServer.h>

#include <string>

#include "../motion/player.h"
#include "../motion/remote.h"

void setupApiRemote(AsyncWebServer *server_p,
                    RemoteControlTarget *remoteControlTarget_p,
                    MotionMode *motionMode) {
   server_p->on("/api/remote/position", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/remote/position",
       [remoteControlTarget_p](AsyncWebServerRequest *request,
                               JsonVariant &json) {
          Serial.print("post /api/remote/position");

          JsonObject const &jsonObj = json.as<JsonObject>();

          if (!jsonObj.containsKey("id")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing id\"}");
             Serial.println(" | Missing id");
             return;
          }
          if (!jsonObj.containsKey("position")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing position\"}");
             Serial.println(" | Missing position");
             return;
          }
          int id = jsonObj["id"];
          Serial.print((" | id: " + std::to_string(id)).c_str());
          float position = jsonObj["position"];
          Serial.println((" position: " + std::to_string(position)).c_str());
          bool hasServo =
              remoteControlTarget_p->setServoPositionById(id, position);
          if (hasServo) {
             request->send(204);
          } else {
             std::string errorStr = "{\"error\":\"Servo with ID " +
                                    std::to_string(id) + " not found\"}";
             request->send(400, "application/json", errorStr.c_str());
          }
       }));
   server_p->on("/api/remote/speed", HTTP_GET,
                [remoteControlTarget_p](AsyncWebServerRequest *request) {
                   Serial.println("GET /api/remote/speed");
                   char buffer[10];

                   request->send(
                       200, "application/json",
                       dtostrf(remoteControlTarget_p->speed, 7, 3, buffer));
                });
   server_p->on("/api/remote/speed", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/remote/speed",
       [remoteControlTarget_p](AsyncWebServerRequest *request,
                               JsonVariant &json) {
          Serial.print("/api/remote/speed");

          JsonObject const &jsonObj = json.as<JsonObject>();
          if (!jsonObj.containsKey("speed")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing speed\"}");
             Serial.println(" | Missing speed");
             return;
          }

          float speed = jsonObj["speed"];
          Serial.println((" | speed: " + std::to_string(speed)).c_str());
          remoteControlTarget_p->speed = speed;
          request->send(204);
       }));
   server_p->on("/api/remote/mode", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   request->send(200, "text/plain", "");
                });

   server_p->on("/api/remote/mode", HTTP_GET,
                [motionMode](AsyncWebServerRequest *request) {
                   Serial.print("GET /api/remote/mode | ");
                   std::string mode;
                   switch (*motionMode) {
                      case IDLE:
                         mode = "idle";
                         break;
                      case PLAYER:
                         mode = "player";
                         break;
                      case REMOTE_CONTROL:
                         mode = "remote_control";
                         break;
                      case STARTUP:
                         mode = "startup";
                         break;
                   }
                   Serial.println(mode.c_str());
                   request->send(200, "text/plain", mode.c_str());
                });

   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/remote/mode",
       [motionMode](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.print("POST /api/remote/mode");
          if (*motionMode == STARTUP) {
             request->send(400, "application/json",
                           "{\"error\":\"Systems Starts up try again later\"}");
             Serial.println(" | Systems Starts up try again later");
             return;
          }
          JsonObject const &jsonObj = json.as<JsonObject>();
          if (!jsonObj.containsKey("mode")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing mode\"}");
             Serial.println(" | Missing mode");
             return;
          }
          //   std::string mode = jsonObj["mode"];
          std::string mode = json["mode"].as<std::string>();
          Serial.println((" | mode: " + mode).c_str());
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
          request->send(204);
       }));
}