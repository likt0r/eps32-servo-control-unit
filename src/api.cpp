#include "api.h"

#include <ESPAsyncWebServer.h>

#include <string>

#include "motion/motion.h"
#include "motion/remote.h"
#include "outputs.h"

void setupApi(AsyncWebServer *server_p, Outputs *outputs_p,
              RemoteControlTarget *remoteControlTarget_p,
              MotionMode *motionMode, WiFiManager *wifiManager_p) {
   server_p->on("/api/wifi/credentials", HTTP_GET,
                [wifiManager_p](AsyncWebServerRequest *request) {
                   Serial.println("GET /api/wifi-credentials");
                   String credentialsJson = "[";
                   for (const auto &cred : wifiManager_p->getCredentials()) {
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
   server_p->on("/api/wifi/credentials", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   Serial.println("OPTIONS /api/wifi/credentials");
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/wifi/credentials",
       [wifiManager_p](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.println("POST /api/wifi/credentials");
          JsonArray const &payload = json.as<JsonArray>();

          std::vector<WiFiCredentials> credentials;
          for (const auto &cred : payload) {
             if (!cred.is<JsonObject>()) {
                request->send(400);  // Bad request
                return;
             }
             JsonObject const &obj = cred.as<JsonObject>();
             if (!obj.containsKey("ssid") || !obj.containsKey("password")) {
                request->send(400);  // Bad request
                return;
             }
             // Create new char arrays for the SSID and password

             // Copy the values from the String objects to the char arrays
             const String ssid = obj["ssid"].as<String>();
             const String password = obj["password"].as<String>();
             credentials.push_back({ssid, password});
          }

          wifiManager_p->setCredentials(&credentials);
          request->send(204);  // No content
       }));
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
