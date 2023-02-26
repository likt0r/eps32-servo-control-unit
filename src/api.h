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

#include "SPIFFS.h"
#include "outputs.h"
// setup function gets server as parameter and a pointer to the outputs struct
void setup(AsyncWebServer *server, Outputs *outputs);
