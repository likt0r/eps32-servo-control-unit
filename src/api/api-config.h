#ifndef API_CONFIG_H
#define API_CONFIG_H
#include <ESPAsyncWebServer.h>

#include "WiFiManager.h"
#include "outputs.h"
// setup function gets server as parameter and a pointer to the outputs struct
void setupApiConfig(AsyncWebServer *server_p, Outputs *outputs_p,
                    WiFiManager *wifiManager);

#endif  // API_CONFIG