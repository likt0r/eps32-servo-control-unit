#include "api.h"

#include <ESPAsyncWebServer.h>

#include <string>

#include "motion/motion.h"
#include "motion/remote.h"
#include "outputs.h"

void handleCredentialsSetRequest(AsyncWebServerRequest *request,
                                 JsonVariant &json, WiFiManager *wifiManager) {
   if (!json.is<JsonArray>()) {
      request->send(400);  // Bad request
      return;
   }

   std::vector<WiFiCredentials> credentials;
   for (const auto &cred : json.as<JsonArray>()) {
      if (!cred.containsKey("ssid") || !cred.containsKey("password")) {
         request->send(400);  // Bad request
         return;
      }
      credentials.push_back(
          {cred["ssid"].as<char *>(), cred["password"].as<char *>()});
   }

   wifiManager->setCredentials(credentials);
   wifiManager->saveCredentials();
   request->send(204);  // No content
}

void setupApi(AsyncWebServer *server_p, Outputs *outputs_p,
              RemoteControlTarget *remoteControlTarget_p,
              MotionMode *motionMode, WiFiManager *wifiManager) {
   server_p->on("/api/wifi/credentials", HTTP_GET,
                [wifiManager](AsyncWebServerRequest *request) {
                   Serial.println("GET /api/wifi-credentials");
                   String credentialsJson = "[";
                   for (const auto &cred : wifiManager->getCredentials()) {
                      credentialsJson += "{\"ssid\":\"" + String(cred.ssid) +
                                         "\",\"password\":\"" +
                                         String(cred.password) + "\"},";
                   }
                   if (credentialsJson.length() > 1) {
                      credentialsJson.remove(credentialsJson.length() -
                                             1);  // Remove the last comma
                   }
                   credentialsJson += "]";
                   request->send(200, "application/json", credentialsJson);
                });
   server_p->on(
       "api/wifi/credentials", HTTP_PUT,
       [](AsyncWebServerRequest *request) { request->send(204); },
       [wifiManager](AsyncWebServerRequest *request, const String &filename,
                     size_t index, uint8_t *data, size_t len, bool final) {
          if (!final) return;
          StaticJsonDocument<1024> doc;
          DeserializationError error = deserializeJson(doc, data, len);
          if (error) {
             request->send(400);  // Bad request
             return;
          }
          handleCredentialsSetRequest(request, doc.as<JsonVariant>(),
                                      wifiManager);
       });
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
             request->send(204);

          } else {
             std::string errorStr = "{\"error\":\"Led with ID " +
                                    std::to_string(id) + " not found\"}";
             request->send(400, "application/json", errorStr.c_str());
          }

          // ...
       }));
   server_p->on("/api/motion/position", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/motion/position",
       [remoteControlTarget_p](AsyncWebServerRequest *request,
                               JsonVariant &json) {
          Serial.println("post /api/motion/position");

          JsonObject const &jsonObj = json.as<JsonObject>();
          if (!jsonObj.containsKey("id")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing id\"}");
             return;
          }
          if (!jsonObj.containsKey("position")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing position\"}");
             return;
          }
          int id = jsonObj["id"];
          float position = jsonObj["position"];
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
   server_p->on("/api/motion/speed", HTTP_GET,
                [remoteControlTarget_p](AsyncWebServerRequest *request) {
                   Serial.println("GET /api/motion/speed");
                   char buffer[10];

                   request->send(
                       200, "application/json",
                       dtostrf(remoteControlTarget_p->speed, 7, 3, buffer));
                });
   server_p->on("/api/motion/speed", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/motion/speed",
       [remoteControlTarget_p](AsyncWebServerRequest *request,
                               JsonVariant &json) {
          Serial.println("/api/motion/speed");

          JsonObject const &jsonObj = json.as<JsonObject>();
          if (!jsonObj.containsKey("speed")) {
             request->send(400, "application/json",
                           "{\"error\":\"Missing speed\"}");
             return;
          }

          float speed = jsonObj["speed"];

          remoteControlTarget_p->speed = speed;
          request->send(204);
       }));
   server_p->on("/api/motion/mode", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/motion/mode",
       [motionMode](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.println("/api/motion/mode");
          if (*motionMode == STARTUP) {
             request->send(400, "application/json",
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
          request->send(204);
       }));
};
