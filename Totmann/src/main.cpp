#include <Arduino.h>

#define Lampe 6
#define Buzzer 7
#define Alarm 8

void setup() {
  pinMode(Lampe, OUTPUT);
  digitalWrite(Lampe, LOW);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  pinMode(Alarm, OUTPUT);
  digitalWrite(Alarm, LOW);

  //Lampe anschalten und 15 Minuten warten
  digitalWrite(Lampe, HIGH);
  delay(900000);

  //Buzzer anschalten und 5 Minuten warten
  digitalWrite(Buzzer, HIGH);
  delay(300000);
  
  //Alarm anschalten
  digitalWrite(Alarm, HIGH);
}

void loop() {
}