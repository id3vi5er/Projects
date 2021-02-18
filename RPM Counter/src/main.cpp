#include <Arduino.h>
#include <math.h>
#include <time.h>

/*
 Impulszaehler
 Gibt die Impulse pro Sekunde des Spannungsignals 
 an Pin X aus wenn Signal auf LOW geht mit INTERNAL_PULLUP
*/

//int pin = 2;                          //Anschluss des Impulsgebers (max 5VDC/3,3VDC nach Arduino Modell)
//double ILiter = 1;                 //Angabe des Herstellers für ausgegebene Impulse pro Liter
unsigned long messZeit = 2;          //Messzeit in Sekunden

unsigned long N;                      //Anzahl der Impulse
unsigned long zeitNow;                //Startzeit
unsigned long T = messZeit*1000000;   //Messzeit in µS umwandeln
double v; //rechenvariable
double RPM = 0;
double RPS = 0;

//
float value = 0;
float rev = 0;
int rpm;
int oldtime = 0;
int rtime;
//
void isr() {
  rev++;
}
//

void setup() 
{ Serial.begin(9600);
  //pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt (2), isr, FALLING); //interrupt pin
}
void berechneRPM(){
    RPM = N*120;
    return;
}

void berechneRPS(){
    RPS = N*2;
    return;
}

void loop(){
  delay(500);
  pinMode(A0, INPUT);
  detachInterrupt(digitalPinToInterrupt(2));      
  rtime = millis() - oldtime;    //rozdiel casov
  rpm = (rev / rtime) * 60000;   //vyrataj otacky/min
  oldtime = millis();           //uloz aktualny cas
  rev = 0;
  Serial.print(rpm);
  Serial.println(" Umdrehungen pro Minute");
  attachInterrupt(digitalPinToInterrupt (2), isr, FALLING);
  Serial.println(analogRead(A0));


  /*N = 0; 



  zeitNow = micros();
  do
    {
      if (pulseIn(pin, HIGH)>0) {
        N++; //wenn pulseIn() 
      }
    }
    while( micros() < (zeitNow+T) ); 
  
  
  berechneRPM();
  berechneRPS();  
  Serial.print("Anzahl der Impulse: ");  
  Serial.print(N);
  Serial.print(" innerhalb ");
  Serial.print(messZeit);
  Serial.println("Sekunde(n)!");
  Serial.print("RPS: ");
  Serial.println(RPS);
  Serial.print("RPM: ");
  Serial.println(RPM); 
  */

}