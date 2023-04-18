
#include <Arduino.h>

// Importing necessary libraries
#include <Adafruit_PWMServoDriver.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Wire.h>

#include <algorithm>
#include <string>

#include "FrontendFiles.h"
#include "SPIFFS.h"
#include "WiFiManager.h"
#include "api.h"
#include "api/api-config.h"
#include "motion/motion.h"
#include "motion/remote.h"
#include "outputs.h"

// Setting network credentials
// const char *ssid = "Sauf-Lan";
// const char *password = "gpun94$_/W";
const char *ssid = "ALM4OG_RPT";
const char *password = "domn8udomn8u2domn8u3";

WiFiCredentials apCredentials = {"MotionController", "password"};

WiFiManager wifiManager = WiFiManager();

// Servorboard
// called this way, it uses the default address 0x40

// Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Watch video V1 to understand the two lines below: http://youtu.be/y8X9X10Tn1k

// Global State  Variables
#define CYCLE_PIN 18
#define SERVO_PWM_MIN \
   80  // this is the 'minimum' pulse length count (out of 4096)
#define SERVO_PWM_MAX \
   620  // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_FREQ 60  // Analog servos run at ~60 Hz updates
std::vector<LedState> leds = {{1, false, 16}, {2, false, 19}, {3, false, 15}};
std::vector<ServoState> servos = {{0, 32, 0.0, SERVO_PWM_MIN, SERVO_PWM_MAX,
                                   SERVO_MIN_ANGLE, SERVO_MAX_ANGLE},
                                  {1, 25, 0.0, SERVO_PWM_MIN, SERVO_PWM_MAX,
                                   SERVO_MIN_ANGLE, SERVO_MAX_ANGLE}};

// SemaphoreHandle_t outputsSemaphore;
Outputs outputs = {leds, servos};
// initialise the remote control target from the outputs
RemoteControlTarget remoteControlTarget =
    createRemoteControlTarget(outputs.servos, 0.5);  // 0.5 degrees per cycle

// loading data from file system
outputs.loadLeds();
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
int startupCounter = 0;
/** Hardware timer for 100 Hz servo update frequency*/
hw_timer_t *timer = NULL;
volatile bool timerIsRunning = false;

void motorLoopISR() {
   digitalWrite(CYCLE_PIN, HIGH);

   switch (motionMode) {
      // Move servos to start position
      case STARTUP:
         startupCounter++;  // wait for 2 seconds before starting
         if (startupCounter > 200) motionMode = IDLE;
         break;
      case IDLE:
         // do nothing
         break;
      case REMOTE_CONTROL:
         float speed = remoteControlTarget.speed;
         for (const auto &pos : remoteControlTarget.positions) {
            int id = pos.id;
            auto servo =
                std::find_if(outputs.servos.begin(), outputs.servos.end(),
                             [&](const ServoState &s) { return s.id == id; });
            if (servo != outputs.servos.end()) {
               float currentPos = servo->position;
               float targetPos = pos.position;
               float gradient = targetPos - currentPos;
               gradient = std::max(-speed, std::min(speed, gradient));  // limit
               currentPos += gradient;
               servo->position = std::max(servo->minAngle,
                                          std::min(servo->maxAngle,
                                                   currentPos));  // limit servo
            }
         }
         break;
   }

   for (auto &servo : outputs.servos) {
      int position = servo.position;
      int pwm = map(round(position), servo.minAngle, servo.maxAngle,
                    servo.minPwm, servo.maxPwm);
      ledcWrite(servo.id, pwm);
   }
   digitalWrite(CYCLE_PIN, LOW);
}

void setup() {
   // Serial port for debugging purposes
   Serial.begin(115200);

   // intialize semaphores
   //  Initialize the semaphore with a count of 1
   // outputsSemaphore = xSemaphoreCreateBinary();

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

   //    // Connect to Wi-Fi
   //    WiFi.begin(ssid, password);
   //    while (WiFi.status() != WL_CONNECTED) {
   //       delay(1000);
   //       Serial.println("Connecting to WiFi");
   //    }

   wifiManager.loadCredentials();
   wifiManager.setup(10000, &apCredentials);

   // Print ESP Local IP Address

   Serial.print("IP-Address ");
   Serial.println(wifiManager.getLocalIP());

   // Initialize SPIFFS
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
   }

   // Set the server to run on the secondary core
   // Set the server to run on the secondary core

   // iterate of frontendFiles
   for (int i = 0; i < frontendFilesCount; i++) {
      const FrontendFileInfo fileInfo = frontendFileInfos[i];
      server.on(fileInfo.url.c_str(), HTTP_GET,
                [fileInfo](AsyncWebServerRequest *request) {
                   Serial.println((std::string("get ") + fileInfo.url +
                                   std::string(" file: ") + fileInfo.filepath)
                                      .c_str());
                   if (fileInfo.isGzipped) {
                      AsyncWebServerResponse *response = request->beginResponse(
                          SPIFFS, fileInfo.filepath.c_str(),
                          fileInfo.mimeType.c_str(), false);
                      response->addHeader("Content-Encoding", "gzip");
                      request->send(response);
                   } else {
                      request->send(SPIFFS, fileInfo.filepath.c_str(),
                                    fileInfo.mimeType.c_str(), false);
                   }
                });
   }

   setupApi(&server, &outputs, &remoteControlTarget, &motionMode);
   setupApiConfig(&server, &outputs, &wifiManager);

   server.onNotFound([](AsyncWebServerRequest *request) {
      request->send(404, "text/html", "Not found");
   });
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
   // board1.begin();
   // board1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

   // Start Hardware Timer loop
   pinMode(CYCLE_PIN, OUTPUT);  // Set loop debug pin as output
   //    timer = timerBegin(0, 80, true);  // Timer 0, Prescaler 80, Count up
   //    timerAttachInterrupt(timer, &motorLoopISR, true);  // Attach the ISR
   //    timerAlarmWrite(timer, 10000, true);               // 100 Hz,
   //    timerAlarmEnable(timer);
}

// Enable the timer
#define TIMER_DURATION 10
unsigned long lastTimerStart = 0;
bool timerExpired12 = false;

void loop() {
   if (millis() - lastTimerStart >= TIMER_DURATION && timerExpired12 == false) {
      timerExpired12 = true;
      // Do something when the timer expires
      motorLoopISR();
   }

   // Reset the timer
   if (timerExpired12 == true) {
      lastTimerStart = millis();
      timerExpired12 = false;
   }
}
