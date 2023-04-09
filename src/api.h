#ifndef API_H
#define API_H
#include <ESPAsyncWebServer.h>

#include "WiFiManager.h"
#include "motion/motion.h"
#include "motion/remote.h"
#include "outputs.h"
// setup function gets server as parameter and a pointer to the outputs struct
void setupApi(AsyncWebServer *server_p, Outputs *outputs_p,
              RemoteControlTarget *remoteControlTarget, MotionMode *motionMode,
              WiFiManager *wifiManager);

#endif  // API_H