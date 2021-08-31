#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


//Ein und Ausgänge werden definiert
#define relais 2

#define Taster1 A0    // Kurs Steuerbord
#define Taster2 A1    // Kurs Backbord
#define Taster3 A2    // Rückwärts
#define Taster4 A3    // Hafeneinfahrt
#define Taster5 A4    // Warnsignal Allgemein (dauerhaft wiederholt)
#define Taster6 A5    // Bleib-Weg (dauerhaft wiederholt)
#define Taster7 A6    // Fahrt bei verminderter Sicht (min. alle 2 Minuten wiederholt)
#define Taster8 A7    // SOS (dauerhaft wiederholt mit 5s Pause nach beenden)

int pausetime = 2000; //Muss nach jeden Signalton 2 Sekunden warten!
boolean t5,t6,t7,t8 = false;


void shortSignal() {    //ca. 1ne Sekunde lang
  digitalWrite(relais, HIGH);
  delay(1000);
  digitalWrite(relais, LOW);
  delay(pausetime);
}

void longSignal() {     //ca. 4-6 Sekunden
  digitalWrite(relais, HIGH);
  delay(4500);
  digitalWrite(relais, LOW);
  delay(pausetime);
}

void kursSteuerbord() { // 1x Kurz (rechts)
  shortSignal();
}

void kursBackbord() {  // 2x Kurz (links)
  shortSignal();
  shortSignal();
}

void rueckwaerts() {  // 3x kurz
  shortSignal();
  shortSignal();
  shortSignal();
}

void hafenEinfahrt() { //1x lang
  longSignal();
}

void warnSignal() { //1x lang, 4x kurz dauerhaft wiederholt
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();

}

void bleibWeg() {  // kurz, lang in Dauerschleife
  shortSignal();
  longSignal();
}

void schlechteSicht() { //1x lang, min. alle 2 Minuten
  longSignal();
}

void sos() {
  shortSignal();
  shortSignal();
  shortSignal();
  longSignal();
  longSignal();
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}


//            Hauptcode             //

void setup() {
  // put your setup code here, to run once:
  pinMode(relais, OUTPUT);
  pinMode(Taster1, INPUT_PULLUP);
  pinMode(Taster2, INPUT_PULLUP);
  pinMode(Taster3, INPUT_PULLUP);
  pinMode(Taster4, INPUT_PULLUP);
  pinMode(Taster5, INPUT_PULLUP);
  pinMode(Taster6, INPUT_PULLUP);
  pinMode(Taster7, INPUT_PULLUP);
  pinMode(Taster8, INPUT_PULLUP);
}


void loop() {
  if (digitalRead(Taster1) == LOW){kursSteuerbord();}
  if (digitalRead(Taster2) == LOW){kursBackbord();}
  if (digitalRead(Taster3) == LOW){rueckwaerts();}
  if (digitalRead(Taster4) == LOW){hafenEinfahrt();}

  if (digitalRead(Taster5) == LOW){}
  if (digitalRead(Taster6) == LOW){}
  if (digitalRead(Taster7) == LOW){}
  if (digitalRead(Taster8) == LOW){}
}