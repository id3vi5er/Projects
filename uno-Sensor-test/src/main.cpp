#include <Arduino.h>

#define sensorPin 2
#define potPin A5

long counter = 0;
long previousMillis = 0;
long interval = 1000;


void tick() {
  counter++;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), tick, FALLING);
}

void loop() {
  int val = analogRead(sensorPin);

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
  delay(100);
}