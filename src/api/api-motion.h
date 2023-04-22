#ifndef API_MOTION_H
#define API_MOTION_H
#include <ESPAsyncWebServer.h>

#include "../motion/MotionManager.h"
#include "WiFiManager.h"
#include "outputs.h"
// setup function gets server as parameter and a pointer to the outputs struct
void setupApiMotion(AsyncWebServer *server_p, MotionManager *motionManager_p);

#endif  // API_MOTION