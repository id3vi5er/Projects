#include <Arduino.h>

const byte interruptPin = 2;

int trigger = 0;  //Value for the Input-Pulses
long oldtime = 0;
long time = 0;

void triggered(){     //Runs if the Input Pin 2 (Interrupt) gets triggered
  trigger++;      //+1 auf den Zähler
}

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPin), triggered, FALLING);
}

void loop() {
  delay(5000); // Delay
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  time = millis()-oldtime; 
  unsigned val = trigger/5*3600; //*3600000   //Durchschnittsverbrauch pro Stunde -> durchschnitt in millisekunden*60.000 = /m * 60 = /h
  long durchschnitt = val*3600;
  oldtime = millis();
  Serial.println(trigger);  //Write Anzahl der Impulse
  delay(100);
  Serial.println(durchschnitt);
  trigger = 0;
  attachInterrupt(digitalPinToInterrupt(interruptPin), triggered, FALLING);
}