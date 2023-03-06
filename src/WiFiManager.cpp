#include "WiFiManager.h"

#include "WiFi.h"

WiFiManager::WiFiManager() {
   _credentials = NULL;
   _numCredentials = 0;
   _timeout = 10000;
   _accessPointMode = false;
   _currentCredentialIndex = 0;
   _apCredentials = NULL;
}

void WiFiManager::setup(const WiFiCredentials* credentials, int numCredentials,
                        int timeout, const WiFiCredentials* apCredentials) {
   _credentials = credentials;
   _numCredentials = numCredentials;
   _timeout = timeout;
   _accessPointMode = false;
   _currentCredentialIndex = 0;
   _apCredentials = apCredentials;

   WiFi.mode(WIFI_STA);

   for (int i = 0; i < _numCredentials; i++) {
      // print wifi credentials
      Serial.print("Connecting to ");
      Serial.print(_credentials[i].ssid);
      Serial.print(" with password ");
      Serial.println(_credentials[i].password);

      WiFi.begin(_credentials[i].ssid, _credentials[i].password);
      unsigned long startAttemptTime = millis();
      while (WiFi.status() != WL_CONNECTED &&
             millis() - startAttemptTime < _timeout) {
         delay(100);
      }
      if (WiFi.status() == WL_CONNECTED) {
         break;
      }
   }

   if (WiFi.status() != WL_CONNECTED) {
      WiFi.mode(WIFI_AP);
      IPAddress ip(192, 168, 4, 1);  // Set the static IP address of the ESP32
      IPAddress gateway(192, 168, 4,
                        1);  // Set the gateway address of the ESP32
      IPAddress subnet(255, 255, 255, 0);  // Set the subnet mask of the ESP32
      _accessPointMode = true;
      // print AP mode and Credentials
      Serial.print("Starting Access Point with SSID: ");
      Serial.print(_apCredentials->ssid);
      Serial.print(" and password: ");
      Serial.println(_apCredentials->password);
      WiFi.softAPConfig(ip, gateway, subnet);
      WiFi.softAPsetHostname("myESP32");  // Set the hostname of the ESP32

      WiFi.softAP(_apCredentials->ssid, _apCredentials->password);
   }
}

bool WiFiManager::isConnected() { return WiFi.status() == WL_CONNECTED; }

IPAddress WiFiManager::getLocalIP() {
   if (_accessPointMode) {
      return IPAddress(192, 168, 4, 1);
   }
   return WiFi.localIP();
}

bool WiFiManager::isAccessPoint() { return _accessPointMode; }
