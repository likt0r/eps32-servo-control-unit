#ifndef API_REMOTE_H
#define API_REMOTE_H
#include <ESPAsyncWebServer.h>

#include "../WiFiManager.h"
#include "../motion/player.h"
#include "../motion/remote.h"
// setup function gets server as parameter and a pointer to the outputs struct
void setupApiRemote(AsyncWebServer *server_p,
                    RemoteControlTarget *remoteControlTarget_p,
                    MotionMode *motionModes);

#endif  // API_REMOTE