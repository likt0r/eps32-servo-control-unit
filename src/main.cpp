#include <Arduino.h>
#include <esp32-hal-gpio.h>
#include <esp32-hal-timer.h>
// Importing necessary libraries
#include <Adafruit_PWMServoDriver.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Wire.h>

#include <string>

#include "FrontendFiles.h"
#include "SPIFFS.h"
#include "motion/motion.h"
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

// Global State  Variables

#define SERVO_PWM_MIN \
   80  // this is the 'minimum' pulse length count (out of 4096)
#define SERVO_PWM_MAX \
   620  // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_FREQ 60  // Analog servos run at ~60 Hz updates

std::vector<LedState> leds = {{1, false, 16}, {2, false, 19}, {3, false, 15}};
std::vector<ServoState> servos = {{1, 32, 0.0, SERVO_PWM_MIN, SERVO_PWM_MAX,
                                   SERVO_MIN_ANGLE, SERVO_MAX_ANGLE},
                                  {2, 13, 0.0, SERVO_PWM_MIN, SERVO_PWM_MAX,
                                   SERVO_MIN_ANGLE, SERVO_MAX_ANGLE}};

Outputs outputs = {leds, servos};

// Creating a AsyncWebServer object
AsyncWebServer server(80);

String outputState(int output) {
   if (digitalRead(output)) {
      return "checked";
   } else {
      return "";
   }
}

/** Motion State Variables **/
MotionMode motionMode = STARTUP;
float remoteControlledMotionSpeed = 0.2;  // degrees per 10 ms
/** Hardware timer for 100 Hz servo update frequency*/
hw_timer_t *timer = NULL;

void IRAM_ATTR motorLoopISR() {
   switch (motionMode) {
      // Move servos to start position
      case STARTUP:
         for (auto &servo : outputs.servos) {
            int pwm = map(servo.position, servo.minAngle, servo.maxAngle,
                          servo.minPwm, servo.maxPwm);
            ledcWrite(servo.id, pwm);
         }
         delay(2000);
         motionMode = IDLE;
         break;
      case IDLE:
         // do nothing
         break;
      case REMOTE_CONTROL:
         break;
   }

   // Calculate new servo positions

   for (auto &servo : outputs.servos) {
      int pwm = map(servo.position, servo.minAngle, servo.maxAngle,
                    servo.minPwm, servo.maxPwm);
      ledcWrite(servo.id, pwm);
   }
}

void setup() {
   // Serial port for debugging purposes
   Serial.begin(115200);
   for (const auto &led : outputs.leds) {
      pinMode(led.pin, OUTPUT);
      digitalWrite(led.pin, led.isOn ? HIGH : LOW);
   }

   // setup servos
   for (auto &servo : outputs.servos) {
      pinMode(servo.pin, OUTPUT);
      ledcSetup(servo.id, 50, 12);  // configure PWM frequency and resolution
      ledcAttachPin(servo.pin, servo.id);
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
      request->send(200, "application/json", outputs.outputToJson());
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

   // Start Hardware Timer loop
   pinMode(14, OUTPUT);              // Set loop debug pin as output
   timer = timerBegin(0, 80, true);  // Timer 0, Prescaler 80, Count up
   timerAttachInterrupt(timer, &motorLoopISR, true);  // Attach the ISR function
   timerAlarmWrite(timer, 10000, true);               // 100 Hz, Repeat
   timerAlarmEnable(timer);
}

// Enable the timer

void loop() {
   //    for (int angle = 0; angle < 300; angle += 1) {
   //       for (int i = 0; i < 16; i++) {
   //          board1.setPWM(i, 0, angleToPulse(angle));
   //       }
   //       delay(10);
   //    }

   //    // robojax PCA9865 16 channel Servo control
   delay(500);
}
