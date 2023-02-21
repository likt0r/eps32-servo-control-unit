#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <vector>

struct Led {
   int id;
   bool isOn;
   int pin;
};

struct Outputs {
   std::vector<Led> leds;
   bool setLedStatusById(int id, bool isOn) {
      Serial.print("setLedStatusById: ");
      Serial.print(id);
      Serial.print(" ");
      Serial.println(isOn);

      for (auto &led : leds) {
         if (led.id == id) {
            led.isOn = isOn;
            return true;
         }
      }
      return false;
   }
   int getLedPin(int id) {
      for (auto &led : leds) {
         if (led.id == id) {
            return led.pin;
         }
      }
      return -1;
   }
   void print() {
      Serial.println("Leds: ");
      for (auto &led : leds) {
         Serial.print("\t id: ");
         Serial.print(led.id);
         Serial.print(" isOn: ");
         Serial.print(led.isOn);
         Serial.print(" pin: ");
         Serial.println(led.pin);
      }
   }
};

Outputs outputs = {{{1, false, 16}, {2, false, 19}, {3, false, 15}}};

String outputToJson() {
   DynamicJsonDocument doc(1024);
   JsonArray jsonLeds = doc.createNestedArray("leds");
   for (const auto &led : outputs.leds) {
      JsonObject jsonLed = jsonLeds.createNestedObject();
      jsonLed["id"] = led.id;
      jsonLed["isOn"] = led.isOn;
      jsonLed["pin"] = led.pin;
   }
   String json;
   serializeJson(doc, json);
   return json;
}
