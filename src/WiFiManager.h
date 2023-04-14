#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <FS.h>

#include <string>
#include <vector>

struct WiFiCredentials {
   String ssid;
   String password;
};

class WiFiManager {
  public:
   WiFiManager();
   void setup(int timeout, const WiFiCredentials* apCredentials);
   bool isConnected();
   IPAddress getLocalIP();
   bool isAccessPoint();
   void addCredential(String ssid, String password);
   void removeCredential(int index);
   void saveCredentials();
   void loadCredentials();
   const std::vector<WiFiCredentials>& getCredentials() const;
   void setCredentials(const std::vector<WiFiCredentials>* credentials_p);

   std::vector<WiFiCredentials> _credentials;
   int _timeout;
   bool _accessPointMode;
   const WiFiCredentials* _apCredentials;
};

#endif  // WIFI_MANAGER_H
