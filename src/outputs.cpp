#include "outputs.h"

#include "SPIFFS.h"

bool pinIsValid(int pin) {
   if (pin < 0 || pin >= NUM_DIGITAL_PINS) {
      return false;  // pin number out of range
   }
   if (pin <= 32) {
      return true;  // valid pin number
   }
   return false;  // pin number not valid for ESP32
}
// PinOutputs implementation
bool PinOutputs::setLedStatusById(int id, bool isOn) {
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
};

bool PinOutputs::setServoPositionById(int id, float position) {
   for (auto &servo : servos) {
      if (servo.id == id) {
         servo.position = position;
         return true;
      }
   }
   return false;
};

int PinOutputs::getLedPin(int id) {
   for (auto &led : leds) {
      if (led.id == id) {
         return led.pin;
      }
   }
   return -1;
};
void PinOutputs::print() {
   Serial.println("Leds: ");
   for (auto &led : leds) {
      Serial.print("\t id: ");
      Serial.print(led.id);
      Serial.print(" isOn: ");
      Serial.print(led.isOn);
      Serial.print(" pin: ");
      Serial.println(led.pin);
   }
   Serial.println("Servos: ");
   for (auto &servo : servos) {
      Serial.print("\t id: ");
      Serial.print(servo.id);
      Serial.print(" pin: ");
      Serial.println(servo.pin);
   }
}

String PinOutputs::outputToJson() {
   DynamicJsonDocument doc(1024);
   JsonArray jsonLeds = doc.createNestedArray("leds");
   for (const auto &led : leds) {
      bool isOn = led.isOn;
      JsonObject jsonLed = jsonLeds.createNestedObject();
      jsonLed["id"] = led.id;
      jsonLed["isOn"] = isOn;
      jsonLed["pin"] = led.pin;
   }
   JsonArray jsonServos = doc.createNestedArray("servos");
   for (const auto &servo : servos) {
      float position = servo.position;
      JsonObject jsonServo = jsonServos.createNestedObject();
      jsonServo["id"] = servo.id;
      jsonServo["pin"] = servo.pin;
      jsonServo["position"] = position;
      jsonServo["minPwm"] = servo.minPwm;
      jsonServo["maxPwm"] = servo.maxPwm;
      jsonServo["minAngle"] = servo.minAngle;
      jsonServo["maxAngle"] = servo.maxAngle;
   }

   String json;
   serializeJson(doc, json);
   return json;
}
String PinOutputs::servosToJson() {
   DynamicJsonDocument doc(1024);
   JsonArray jsonServos = doc.to<JsonArray>();
   for (const auto &servo : servos) {
      float position = servo.position;
      JsonObject jsonServo = jsonServos.createNestedObject();
      jsonServo["id"] = servo.id;
      jsonServo["pin"] = servo.pin;
      jsonServo["position"] = position;
      jsonServo["minPwm"] = servo.minPwm;
      jsonServo["maxPwm"] = servo.maxPwm;
      jsonServo["minAngle"] = servo.minAngle;
      jsonServo["maxAngle"] = servo.maxAngle;
   }
   String json;
   serializeJson(jsonServos, json);
   return json;
}

String PinOutputs::ledsToJson() {
   DynamicJsonDocument doc(1024);
   JsonArray jsonLeds = doc.createNestedArray("leds");
   for (const auto &led : leds) {
      bool isOn = led.isOn;
      JsonObject jsonLed = jsonLeds.createNestedObject();
      jsonLed["id"] = led.id;
      jsonLed["isOn"] = isOn;
      jsonLed["pin"] = led.pin;
   }

   String json;
   serializeJson(jsonLeds, json);
   return json;
}

const bool PinOutputs::setServosByJSON(const JsonVariant &json) {
   JsonArray jsonServos = json.as<JsonArray>();

   // Check all conditions and create temporary ServoState objects
   std::vector<ServoState> newServos;
   newServos.reserve(jsonServos.size());
   for (const auto &jsonServo : jsonServos) {
      int id = jsonServo["id"];
      int pin = jsonServo["pin"];
      float position = jsonServo["position"];
      int minPwm = jsonServo["minPwm"];
      int maxPwm = jsonServo["maxPwm"];
      float minAngle = jsonServo["minAngle"];
      float maxAngle = jsonServo["maxAngle"];
      if (minPwm < 0 || maxPwm < 0 || minAngle < 0 || maxAngle < 0 ||
          maxAngle > 360)
         return false;
      if (minPwm >= maxPwm || minAngle >= maxAngle) return false;
      if (position < minAngle || position > maxAngle) return false;
      if (!pinIsValid(pin)) return false;  // check if pin is valid
      newServos.push_back(
          {id, pin, position, minPwm, maxPwm, minAngle, maxAngle});
   }

   // Replace existing servos with the new ones
   servos = std::move(newServos);
   return true;
}

const bool PinOutputs::setLedsByJSON(const JsonVariant &json) {
   JsonArray jsonLeds = json.as<JsonArray>();

   // Check all conditions and create temporary ServoState objects
   std::vector<LedState> newLeds;
   newLeds.reserve(jsonLeds.size());
   for (const auto &jsonLed : jsonLeds) {
      int id = jsonLed["id"];
      bool isOn = jsonLed["isOn"];
      int pin = jsonLed["pin"];
      Serial.print("Leds id: ");
      Serial.println(pin);
      if (!pinIsValid(pin)) {
         return false;
      }  // check if pin is valid
      newLeds.push_back({id, isOn, pin});
   }
   leds = std::move(newLeds);
   return true;
}

const bool PinOutputs::loadLeds() {
   File file = SPIFFS.open("/leds.bin",
                           FILE_READ);  // open file for reading in binary mode
   if (!file) {
      Serial.println("Failed to open file for reading");
      return false;
   }

   leds.clear();  // clear the existing LED data

   while (file.available()) {  // loop over file data and read LED data
      LedState led;
      file.read((uint8_t *)&led, sizeof(LedState));
      leds.push_back(led);
   }

   file.close();  // close the file
   return true;
}

const bool PinOutputs::storeLeds() {
   File file = SPIFFS.open("/leds.bin",
                           FILE_WRITE);  // open file for writing in binary mode
   if (!file) {
      Serial.println("Failed to open file for writing");
      return false;
   }

   for (const auto &led : leds) {  // loop over all LEDs and write data to file
      file.write((const uint8_t *)&led, sizeof(LedState));
   }
   file.close();  // close the file
   return true;
}

const bool PinOutputs::storeServos() {
   File file = SPIFFS.open("/servos.bin", "w");
   if (!file) {
      Serial.println("Failed to open file for writing");
      return false;
   }

   for (const auto &servo : servos) {
      file.write((uint8_t *)&servo, sizeof(ServoState));
   }

   file.close();
   return true;
}

const bool PinOutputs::loadServos() {
   File file = SPIFFS.open("/servos.bin", "r");
   if (!file) {
      Serial.println("Failed to open file for reading");
      return false;
   }

   servos.clear();
   while (file.available()) {
      ServoState servo;
      file.read((uint8_t *)&servo, sizeof(ServoState));
      servos.push_back(servo);
   }

   file.close();
   return true;
}