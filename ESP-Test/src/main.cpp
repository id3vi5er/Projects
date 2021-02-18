#include <Arduino.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  }

void loop() {
  Serial.println("Hat funktioniert!");
  delay(1000);
}