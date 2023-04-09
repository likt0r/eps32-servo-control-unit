#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <FS.h>

#include <vector>

struct WiFiCredentials {
   const char* ssid;
   const char* password;
};

class WiFiManager {
  public:
   WiFiManager();
   void setup(int timeout, const WiFiCredentials* apCredentials);
   bool isConnected();
   IPAddress getLocalIP();
   bool isAccessPoint();
   void addCredential(const char* ssid, const char* password);
   void removeCredential(int index);
   void saveCredentials();
   void loadCredentials();
   const std::vector<WiFiCredentials>& getCredentials() const;
   void WiFiManager::setCredentials(
       const std::vector<WiFiCredentials>& credentials)

       private : std::vector<WiFiCredentials> _credentials;
   int _timeout;
   bool _accessPointMode;
   const WiFiCredentials* _apCredentials;
};

#endif  // WIFI_MANAGER_H
