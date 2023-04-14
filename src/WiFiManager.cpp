#include "WiFiManager.h"

#include "SPIFFS.h"
#include "WiFi.h"

WiFiManager::WiFiManager() {}

void WiFiManager::setup(int timeout, const WiFiCredentials *apCredential) {
   _timeout = timeout;
   _apCredentials = apCredential;
   WiFi.mode(WIFI_STA);
   loadCredentials();
   for (const auto &cred : _credentials) {
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
    const std::vector<WiFiCredentials> *credentials_p) {
   this->_credentials = *credentials_p;
   Serial.println("Credentials set.");

   // Save credentials to SPIFFS
   saveCredentials();
   // Print all credentials
   Serial.println("All credentials:");
   for (const auto &cred : _credentials) {
      Serial.printf("SSID: %s, Password: %s\n", cred.ssid, cred.password);
   }
}

void WiFiManager::removeCredential(int index) {
   if (index >= 0 && index < _credentials.size()) {
      this->_credentials.erase(_credentials.begin() + index);
   }
}

void WiFiManager::saveCredentials() {
   if (!SPIFFS.begin()) {
      Serial.println("Failed to mount file system");
      return;
   }

   File file = SPIFFS.open("/wifi_credentials.bin", FILE_WRITE);
   if (!file) {
      Serial.println("Failed to open file for writing");
      return;
   }

   // Write the number of credentials to the file
   size_t count = _credentials.size();
   file.write(reinterpret_cast<const uint8_t *>(&count), sizeof(count));

   // Write each credential to the file
   Serial.printf("Writing %d credentials to file", count);

   for (const auto &cred : _credentials) {
      size_t ssidLength = cred.ssid.length();
      size_t passwordLength = cred.password.length();

      // Write the length of the ssid and password to the file
      file.write(reinterpret_cast<const uint8_t *>(&ssidLength),
                 sizeof(ssidLength));
      file.write(reinterpret_cast<const uint8_t *>(&passwordLength),
                 sizeof(passwordLength));

      // Write the ssid and password to the file
      file.write(reinterpret_cast<const uint8_t *>(cred.ssid.c_str()),
                 ssidLength);
      file.write(reinterpret_cast<const uint8_t *>(cred.password.c_str()),
                 passwordLength);
   }

   file.close();
}

void WiFiManager::loadCredentials() {
   if (!SPIFFS.begin()) {
      Serial.println("Failed to mount file system");
      return;
   }

   File file = SPIFFS.open("/wifi_credentials.bin", FILE_READ);
   if (!file) {
      Serial.println("Failed to open file for reading");
      return;
   }

   if (file.size() == 0) {
      Serial.println("File is empty");

      return;
   }

   // Read the number of credentials from the file
   size_t count;
   file.read(reinterpret_cast<uint8_t *>(&count), sizeof(count));

   _credentials.clear();
   _credentials.reserve(count);

   // Read each credential from the file
   // Read each credential from the file
   for (size_t i = 0; i < count; ++i) {
      size_t ssidLength, passwordLength;

      // Read the length of the ssid and password from the file
      file.read(reinterpret_cast<uint8_t *>(&ssidLength), sizeof(ssidLength));
      file.read(reinterpret_cast<uint8_t *>(&passwordLength),
                sizeof(passwordLength));

      // Allocate memory for the ssid and password strings
      char *ssidBuf = new char[ssidLength + 1];
      char *passwordBuf = new char[passwordLength + 1];

      // Read the ssid and password from the file
      file.read(reinterpret_cast<uint8_t *>(ssidBuf), ssidLength);
      file.read(reinterpret_cast<uint8_t *>(passwordBuf), passwordLength);

      // Null-terminate the strings
      ssidBuf[ssidLength] = '\0';
      passwordBuf[passwordLength] = '\0';

      // Construct the WiFiCredentials struct with the ssid and password strings
      _credentials.emplace_back(WiFiCredentials{ssidBuf, passwordBuf});

      // Free the memory allocated for the ssid and password strings
      delete[] ssidBuf;
      delete[] passwordBuf;
   }

   file.close();
}

const std::vector<WiFiCredentials> &WiFiManager::getCredentials() const {
   return _credentials;
}
