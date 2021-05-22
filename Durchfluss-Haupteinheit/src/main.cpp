/*
  Author: N. Rattensperger
  Titel: Waterflowmeter
  Version: 0.4
  Beschreibung:
    Arduino Mega mit 2 Durchflusssensoren Typ FCH-m-PP-LC (Art. 155374 Conrad) zur Berechnung des Durchflusses (0,8l-0,015l/min)
    Sensor gibt 10500 Impulse pro Liter aus
    1800*Intervall wird gespeichert, dann überschrieben!


    Anzeige über ein 4x20 Zeichen LCD per I2C; Pins 2/3 sind respektive inFlow und outFLow; Button 18/19 sind Auf und Ab für das Menü
*/
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

#define buttonUp 18
#define buttonDown 19
#define inFlowSensor 2
#define outFlowSensor 3
#define delayTimeMeasurements 2000 //if changed, look for calculate() and change respectively! //ACHTUNG; ZURÜCKÄNDERN; WAR 60000!!!
float imppliter = 10500.00;

volatile int pulses1 = 0;
volatile int pulses2 = 0;
volatile boolean upButton = false;
volatile boolean downButton = false;

//Anzeige
LiquidCrystal_I2C lcd(0x27,20,4);     // set the LCD address to 0x27 for a 20 chars and 4 line display
boolean firstBoot = true;
boolean firstLoop = true;

//Measurement & Calculation
long lastTime = 0;
int rowVal = 0;
int overflows = 0;
long vl1, vl2;

//Datapoints storing
float consumptionArray[1500];                //Verbrauchswerte, einmal alle 10s. ~30h bis Overflow
float usedsincestart = 0.00;
unsigned int ArrayIndex = 0;
unsigned int ArrayView = 0;

void buttonPressedUp()          //Interrupt Handler
{
  upButton = true;
}
void buttonPressedDown()        //Interrupt Handler
{
  downButton = true;
}
void inputTriggered1()          //Interrupt Handler
{
  pulses1++;
}
void inputTriggered2()          //Interrupt Handler
{
  pulses2++;
}

void startupAnzeige()                                           //Einmalig beim Start für Author und Programmtitel
{
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0, 0);
    lcd.print("        N.R.");
    lcd.setCursor(0, 1);
    lcd.print("Durchflussmessgeraet");
    firstBoot=false;

}

void calculate(int val1, int val2)                              //Berechnung der Messwerte, getriggert durch Timer
{
  int iinsec = delayTimeMeasurements/1000;

  //Rechnung für InFlow:
  float LiterIn = val1/imppliter;                    //Liter Input
  float LiterIntervallIn = LiterIn*iinsec;    //Liter innerhalb Intervall
  //float lmIn = LiterIntervallIn*60;           //Input-Liter pro Minute

  //Rechnung für OutFlow:
  float LiterOut = val2/imppliter;                     //Liter Output
  float LiterIntervallOut = LiterOut*iinsec;  //
  //float lmOut = LiterIntervallOut*60;         //Output-Liter pro Minute

  //Storing Values
  consumptionArray[ArrayIndex] = LiterIntervallIn-LiterIntervallOut;
  
  //Rechnung für Gesamtverbrauch:
  float usedthisIntervall = LiterIntervallIn-LiterIntervallOut;
  usedsincestart = usedsincestart+usedthisIntervall;     //Speichern in Litern!

  vl1 = overflows*1500;
  vl2 = vl1+ArrayIndex;

  if(ArrayIndex < 1499){
    ArrayIndex++;
    if (ArrayView == vl2-1){
        ArrayView++;
    }
      pulses1 = 0;
      pulses2 = 0;
    } else {
      ArrayIndex = 0;
      ArrayView++;
      Serial.println((String)"overflows="+overflows);
      overflows = overflows+1;
      Serial.println("Overflow!");
      pulses1 = 0;
      pulses2 = 0;
    }

}

void anzeigen()                                                 //Aktualisierung der LCD-Anzeige, getrigger durch Button oder Timer
{
  lcd.clear();
  Serial.println((String)"ArrayView: "+ArrayView);
  Serial.println((String)"ArrayIndex: "+ArrayIndex);
  Serial.println((String)"overflows: "+overflows);
  Serial.print((String)"ConsumptionArray["+ArrayIndex+"]: ");
  Serial.println(consumptionArray[ArrayIndex], 7);
  Serial.println();
  
  for (int i = 0; i != 4; ++i)
  {
    int x = i-1;
    unsigned long val = ArrayView-x;

    if (i == 0){
      lcd.setCursor(0, i);
      lcd.print((String)"Gesamt in l: "+usedsincestart);
    } else{ 
      
      long ViewV = (ArrayView-vl1)-x;
      //Serial.println((String)"ViewV: "+ViewV);
      lcd.setCursor(0, i);
      lcd.print((String)val+".Min.: "+consumptionArray[ViewV]);
    }
  }
  downButton = false;
  upButton = false;
}

void setup()                                                    //Initialisieren (nur beim Boot)
{
lcd.init();                                                     //Im Setup wird der LCD gestartet
lcd.backlight();                                                //Hintergrundbeleuchtung einschalten (0 schaltet die Beleuchtung aus).
Serial.begin(115200);                                           //Serieller Monitor mit Baudrate 115200
pinMode(buttonUp, INPUT_PULLUP);                                //Input Pullups für Sensor und Button
pinMode(buttonDown, INPUT_PULLUP);                              //Input Pullups für Sensor und Button
pinMode(inFlowSensor, INPUT_PULLUP);                            //Input Pullups für Sensor und Button
pinMode(outFlowSensor, INPUT_PULLUP);                           //Input Pullups für Sensor und Button
attachInterrupt(digitalPinToInterrupt(buttonUp), buttonPressedUp, FALLING);       //Interrupts für Sensor und Button an Handler binden
attachInterrupt(digitalPinToInterrupt(buttonDown), buttonPressedDown, FALLING);   //Interrupts für Sensor und Button an Handler binden
attachInterrupt(digitalPinToInterrupt(inFlowSensor), inputTriggered1, FALLING);   //Interrupts für Sensor und Button an Handler binden
attachInterrupt(digitalPinToInterrupt(outFlowSensor), inputTriggered2, FALLING);  //Interrupts für Sensor und Button an Handler binden
}

void loop(){                                                    //Schleife, dauerhaft ausgeführt
if (firstBoot){                                           //nur beim ersten Boot für die Startup-Anzeige
  startupAnzeige();
}

if(ArrayView >= 3){                                       //Wenn mehr als 4 Werte vorhanden sind, und kein Overflow stattgefunden hat
  if(upButton == true || downButton == true) {
    if (downButton == true && ArrayView < ArrayIndex+vl1 ){
      ArrayView++;
    }
    if (upButton == true && ArrayView >= 4){
      ArrayView--;

    }
    anzeigen();
  }
}


if (millis() - lastTime >= delayTimeMeasurements){        //Schleife nach delayTimeMeasurements (60Sek) um zu rechnen und Anzeige zu aktualisieren
  if(firstLoop){
    lcd.noCursor();
    lcd.noBlink();
    firstLoop=false;
  }
  lastTime = millis();
  calculate(pulses1, pulses2);
  anzeigen();
  }
}
