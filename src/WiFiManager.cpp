#include "WiFiManager.h"

#include "SPIFFS.h"
#include "WiFi.h"

WiFiManager::WiFiManager() {}

void WiFiManager::setup(int timeout, const WiFiCredentials* apCredential) {
   _timeout = timeout;
   _apCredentials = apCredential;
   WiFi.mode(WIFI_STA);
   loadCredentials();
   for (const auto& cred : _credentials) {
      // print wifi credentials
      Serial.print("Connecting to ");
      Serial.print(cred.ssid);
      Serial.print(" with password ");
      Serial.println(cred.password);

      WiFi.begin(cred.ssid.c_str(), cred.password.c_str());

      unsigned long startAttemptTime = millis();
      while (WiFi.status() != WL_CONNECTED &&
             millis() - startAttemptTime < _timeout) {
         delay(100);
      }
      if (WiFi.status() == WL_CONNECTED) {
         break;
      }
      delay(500);
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

IPAddress WiFiManager::getLocalIP() { return WiFi.localIP(); }

bool WiFiManager::isAccessPoint() { return _accessPointMode; }

void WiFiManager::setCredentials(
    const std::vector<WiFiCredentials>* credentials_p) {
   this->_credentials = *credentials_p;
   Serial.println("Credentials set.");

   // Save credentials to SPIFFS
   saveCredentials();
   // Print all credentials
   Serial.println("All credentials:");
   for (const auto& cred : _credentials) {
      Serial.printf("SSID: %s, Password: %s\n", cred.ssid, cred.password);
   }
}

void WiFiManager::removeCredential(int index) {
   if (index >= 0 && index < _credentials.size()) {
      this->_credentials.erase(_credentials.begin() + index);
   }
}

void WiFiManager::saveCredentials() {
   if (!SPIFFS.begin(true)) {
      Serial.println("Failed to mount file system");
      return;
   }

   File file = SPIFFS.open("/wifi_credentials.txt", "w");
   if (!file) {
      Serial.println("Failed to create credentials file");
      return;
   }

   for (const auto& cred : _credentials) {
      file.printf(("" + cred.ssid + ":" + cred.password).c_str());
   }

   file.close();
}

void WiFiManager::loadCredentials() {
   if (!SPIFFS.begin(true)) {
      Serial.println("Failed to mount file system");
      return;
   }

   File file = SPIFFS.open("/wifi_credentials.txt", "r");
   if (!file) {
      Serial.println("Credentials file not found");
      return;
   }

   _credentials.clear();

   while (file.available()) {
      String line = file.readStringUntil('\n');
      int separatorIndex = line.indexOf(':');
      if (separatorIndex == -1) continue;

      String ssid = line.substring(0, separatorIndex);
      String password = line.substring(separatorIndex + 1);

      _credentials.push_back({ssid, password});
   }

   file.close();
}

const std::vector<WiFiCredentials>& WiFiManager::getCredentials() const {
   return _credentials;
}
