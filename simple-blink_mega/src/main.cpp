#include <Arduino.h>

/*
 Impulszaehler
 Gibt die Impulse pro Sekunde des Spannungsignals 
 an Pin X aus wenn Signal auf LOW geht mit INTERNAL_PULLUP
*/
const byte interruptPin = 2;
const int pin = 7;                          //Anschluss des Impulsgebers (max 5VDC/3,3VDC nach Arduino Modell)
const double Liter = 1800;                 //Angabe des Herstellers für ausgegebene Impulse pro Liter
const unsigned long messZeit = 10;          //Messzeit in Sekunden

int pulse;                   //Impuls pro Interrupt
unsigned long N = 0;                      //Anzahl der Impulse
unsigned long zeitNow;                //Startzeit
unsigned long T = messZeit*1000;   //Messzeit in miliS umwandeln
double durchfluss;                    //Berechneter Durchfluss pro MessZeit
unsigned long durchfluss_gesamt = 0;


void counter_add()
{
  pulse++;
}

void setup() 
{ Serial.begin(9600);
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), counter_add, FALLING);
  zeitNow = millis();  
}

void durchflussRechnung(int impulse)
{
  double var = 1 / Liter;
  durchfluss = var * impulse;
}

void debug(){
  Serial.print("Anzahl der Impulse: ");  
  Serial.println(pulse);
  Serial.print("Zeit der Messung in Sekunden: ");
  Serial.println(messZeit);
  Serial.print("Durchfluss insgesamt: ");
  Serial.print(durchfluss_gesamt,4);
  Serial.print(" Liter");
  if (durchfluss_gesamt < 0.1){
    double f = durchfluss_gesamt * 1000;
    Serial.print(" bzw. ");
    Serial.print(f, 4);
    Serial.println("ml");
    Serial.println();
  } else {
    Serial.println();
  }
}


void loop() 
{ 
  if (millis() > (zeitNow+T)){    //Wenn Zeit von letztes mal bis jetzt länger als Zeit T ist, führe Rechnung aus!
    durchflussRechnung(pulse);
    durchfluss_gesamt = durchfluss_gesamt + durchfluss;
    debug();
    zeitNow = millis(); //Resette Uhr
    pulse = 0; //Resette Puls-Counter
    durchfluss = 0; //resette Durchfluss pro Phase
  }
}