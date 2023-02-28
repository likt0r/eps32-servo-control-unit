#include "outputs.h"

// Outputs implementation
bool Outputs::setLedStatusById(int id, bool isOn) {
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

bool Outputs::setServoPositionById(int id, float position) {
   for (auto &servo : servos) {
      if (servo.id == id) {
         servo.position = position;
         return true;
      }
   }
   return false;
};

int Outputs::getLedPin(int id) {
   for (auto &led : leds) {
      if (led.id == id) {
         return led.pin;
      }
   }
   return -1;
};
void Outputs::print() {
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

String Outputs::outputToJson() {
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