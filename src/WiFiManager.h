#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>

struct WiFiCredentials {
   const char* ssid;
   const char* password;
};

class WiFiManager {
  public:
   WiFiManager();
   void setup(const WiFiCredentials* credentials, int numCredentials,
              int timeout, const WiFiCredentials* apCredentials);
   bool isConnected();
   IPAddress getLocalIP();
   bool isAccessPoint();

  private:
   const WiFiCredentials* _credentials;
   int _numCredentials;
   int _timeout;
   bool _accessPointMode;
   int _currentCredentialIndex;
   const WiFiCredentials* _apCredentials;
};

#endif  // WIFIMANAGER_H
