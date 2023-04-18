#include <ESPAsyncWebServer.h>

#include <string>

#include "api.h"
#include "motion/motion.h"
#include "motion/remote.h"
#include "outputs.h"

void setupApiConfig(AsyncWebServer *server_p, PinOutputs *outputs_p,
                    WiFiManager *wifiManager_p) {
   // routes for wifi credentials
   server_p->on("/api/config/wifi/credentials", HTTP_GET,
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
   server_p->on("/api/config/wifi/credentials", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   Serial.println("OPTIONS /api/wifi/credentials");
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/config/config/wifi/credentials",
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
   // routes for servos
   server_p->on("/api/config/servos", HTTP_GET,
                [outputs_p](AsyncWebServerRequest *request) {
                   Serial.println("GET /api/config/servos");
                   request->send(200, "application/json",
                                 outputs_p->servosToJson());
                });
   server_p->on("/api/config/servos", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   Serial.println("OPTIONS /api/config/servos");
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/config/servos",
       [outputs_p](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.println("POST /api/config/servos");
          const bool result = outputs_p->setServosByJSON(json);
          if (!result) {
             request->send(400);  // Bad request
             return;
          }
          const bool storeResult = outputs_p->storeServos();
          if (!storeResult) {
             request->send(500);  // Internal server error
             return;
          }
          request->send(204);  // No content
       }));
   // routes for leds
   server_p->on("/api/config/leds", HTTP_GET,
                [outputs_p](AsyncWebServerRequest *request) {
                   Serial.println("GET /api/config/leds");
                   request->send(200, "application/json",
                                 outputs_p->ledsToJson());
                });
   server_p->on("/api/config/leds", HTTP_OPTIONS,
                [](AsyncWebServerRequest *request) {
                   Serial.println("OPTIONS /api/config/leds");
                   request->send(200, "text/plain", "");
                });
   server_p->addHandler(new AsyncCallbackJsonWebHandler(
       "/api/config/leds",
       [outputs_p](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.println("POST /api/config/leds");
          const bool result = outputs_p->setLedsByJSON(json);
          if (!result) {
             Serial.println("Bad request");
             request->send(400);  // Bad request
             return;
          }
          const bool storeResult = outputs_p->storeLeds();
          if (!storeResult) {
             Serial.println("Server error result from request");
             request->send(500);  // Internal server error
             return;
          }
          request->send(204);  // No content
       }));
};
