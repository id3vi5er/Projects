#include <Arduino.h>
#include "DigiKeyboard.h"

int buttonPin0 = 0; // Hier ist der Taster angeschlossen
int buttonPin1 = 2;
// int buttonPin2 = 2;
// int buttonPin3 = 3;
// int buttonPin4 = 4;
// int buttonPin5 = 5;

void setup() {
  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  // pinMode(buttonPin2, INPUT);
  // pinMode(buttonPin3, INPUT);
  // pinMode(buttonPin4, INPUT);
  // pinMode(buttonPin5, INPUT);
  digitalWrite(buttonPin0, HIGH);
  digitalWrite(buttonPin1, HIGH);
  // digitalWrite(buttonPin2, HIGH);
  // digitalWrite(buttonPin3, HIGH);
  // digitalWrite(buttonPin4, HIGH);
  // digitalWrite(buttonPin5, HIGH);
}

void loop() {
  DigiKeyboard.sendKeyStroke(0);
  if (digitalRead(buttonPin0) == LOW){
    DigiKeyboard.sendKeyStroke(MOD_SHIFT_RIGHT, KEY_F9); // 
    DigiKeyboard.delay(50); // Kurz warten
    delay(50);
  }  
  if (digitalRead(buttonPin1) == LOW){
    DigiKeyboard.sendKeyStroke(MOD_SHIFT_RIGHT, KEY_F10); // Win+R Tatenkombination
    DigiKeyboard.delay(50); // Kurz warten
    delay(50);
  }
  //   if (digitalRead(buttonPin2) == LOW){
  //   DigiKeyboard.sendKeyStroke(MOD_CONTROL_RIGHT, KEY_F11); // Win+R Tatenkombination
  //   DigiKeyboard.delay(50); // Kurz warten
  // }
  //   if (digitalRead(buttonPin3) == LOW){
  //   DigiKeyboard.sendKeyStroke(MOD_CONTROL_RIGHT, KEY_F12); // Win+R Tatenkombination
  //   DigiKeyboard.delay(50); // Kurz warten
  // }
  //   if (digitalRead(buttonPin4) == LOW){
  //   DigiKeyboard.sendKeyStroke(); // Win+R Tatenkombination
  //   DigiKeyboard.delay(50); // Kurz warten
  // }
  //   if (digitalRead(buttonPin5) == LOW){
  //   DigiKeyboard.sendKeyStroke(); // Win+R Tatenkombination
  //   DigiKeyboard.delay(50); // Kurz warten
  // }
}