#include <Arduino.h>

String serialInput;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {

  if(Serial.available()){
        serialInput = Serial.readStringUntil('\n');
        Serial.print("You typed: " );
        Serial.println(serialInput);
    }
  analogWrite(A3, serialInput.toInt());
}