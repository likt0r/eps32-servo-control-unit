#include <Arduino.h>
// Importing necessary libraries
#include <Adafruit_PWMServoDriver.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Wire.h>

#include <string>

#include "FrontendFiles.h"
#include "SPIFFS.h"
#include "outputs.h"

// Setting network credentials
// const char *ssid = "Sauf-Lan";
// const char *password = "gpun94$_/W";
const char *ssid = "Karl-Fritz";
const char *password = "24333800157909807591";

// Servorboard
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Watch video V1 to understand the two lines below: http://youtu.be/y8X9X10Tn1k
#define SERVOMIN 125  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600  // this is the 'maximum' pulse length count (out of 4096)

// Webinterface

const char *input_parameter1 = "output";
const char *input_parameter2 = "state";

const int LED1 = 4;
const int LED2 = 16;
const int LED3 = 17;
// Creating a AsyncWebServer object
AsyncWebServer server(80);

String outputState(int output) {
   if (digitalRead(output)) {
      return "checked";
   } else {
      return "";
   }
}

// Replaces placeholder with button section in your web page

void setup() {
   // Serial port for debugging purposes
   Serial.begin(115200);
   for (const auto &led : outputs.leds) {
      // Serial.print("Led id: ");
      // Serial.println(led.id);
      // Serial.print("Led status: ");
      // Serial.println(led.isOn ? "On" : "Off");
      // Serial.print("Led pin: ");
      // Serial.println(led.pin);
      pinMode(led.pin, OUTPUT);
      digitalWrite(led.pin, led.isOn ? HIGH : LOW);
   }

   // Connect to Wi-Fi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi");
   }

   // Print ESP Local IP Address
   Serial.println(WiFi.localIP());

   // Initialize SPIFFS
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
   }
   // iterate of frontendFiles
   for (int i = 0; i < frontendFilesCount; i++) {
      // open file for writing
      // Serial.println("set route on ");
      // Serial.println(frontendFileInfos[i].url.c_str());
      // Serial.println(frontendFileInfos[i].filepath.c_str());
      // Serial.println(frontendFileInfos[i].mimeType.c_str());
      const FrontendFileInfo fileInfo = frontendFileInfos[i];
      server.on(frontendFileInfos[i].url.c_str(), HTTP_GET,
                [fileInfo](AsyncWebServerRequest *request) {
                   Serial.println("get /");
                   request->send(SPIFFS, fileInfo.filepath.c_str(),
                                 fileInfo.mimeType.c_str(), false);
                });
   }

   // Route for root / web page
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      Serial.println("get /");
      request->send(SPIFFS, "/index.html", "text/html", false);
   });
   server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
      Serial.println("get /favicon.ico");
      request->send(SPIFFS, "/favicon.ico", "image/vnd.microsoft.icon", false);
   });
   server.on("/assets/logo-da9b9095.svg", HTTP_GET,
             [](AsyncWebServerRequest *request) {
                Serial.println("get /favicon.ico");
                request->send(SPIFFS, "/assets_logo-da9b9095.svg",
                              "image/svg+xml", false);
             });

   server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
      Serial.println("/api/status");
      request->send(200, "application/json", outputToJson());
   });

   //    server.on("/api/led", HTTP_GET, [](AsyncWebServerRequest *request) {
   //       request->send(200, "application/json", outputToJson());
   //    });
   server.on("/api/led", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "");
   });

   server.addHandler(new AsyncCallbackJsonWebHandler(
       "/api/led", [](AsyncWebServerRequest *request, JsonVariant &json) {
          Serial.print("/api/led");
          JsonObject const &jsonObj = json.as<JsonObject>();
          int id = jsonObj["id"];
          bool isOn = jsonObj["isOn"];
          bool hasLed = outputs.setLedStatusById(id, isOn);

          if (hasLed) {
             digitalWrite(outputs.getLedPin(id), isOn ? HIGH : LOW);
             request->send(200, "application/json", "{\"success\":true}");

          } else {
             request->send(400, "application/json",
                           "{\"error\":\"Led not found\"}");
          }

          // ...
       }));

   // Set Headers
   DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
   DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods",
                                        "GET,PUT,POST,DELETE,OPTIONS");
   DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers",
                                        "Content-Type");

   // Start server
   server.begin();

   // Servo Stuff
   Serial.println("32 channel Servo test!");
   outputs.print();
   board1.begin();
   board1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

/*
 * angleToPulse(int ang)
 * gets angle in degree and returns the pulse width
 * also prints the value on seial monitor
 * written by Ahmad Nejrabi for Robojax, Robojax.com
 */
int angleToPulse(int ang) {
   int pulse =
       map(ang, 0, 180, SERVOMIN,
           SERVOMAX);  // map angle of 0 to 180 to Servo min and Servo max
   // Serial.print("Angle: ");
   // Serial.print(ang);
   // Serial.print(" pulse: ");
   // Serial.println(pulse);
   return pulse;
}

void loop() {
   for (int angle = 0; angle < 300; angle += 1) {
      for (int i = 0; i < 16; i++) {
         board1.setPWM(i, 0, angleToPulse(angle));
      }
      delay(10);
   }

   // robojax PCA9865 16 channel Servo control
   delay(500);
}