/*
  Author: N. Rattensperger
  Titel: Waterflowmeter
  Version: 0.3
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
#define delayTimeMeasurements 60000 //if changed, look for calculate() and change respectively!
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

//Datapoints storing
float consumptionArray[1800];                //Verbrauchswerte, einmal alle 10s. ~5h bis Overflow
float usedsincestart = 0.00;
int ArrayIndex = 0;
int ArrayView = 0;


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
    delay(1000);
    lcd.print("Durchflussmessgeraet");
    firstBoot=false;
    ArrayView++;
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

  long vl1 = overflows*1800;
  long vl2 = vl1+ArrayIndex;

  if(ArrayIndex < 1799){
    ArrayIndex++;
    if (ArrayView == vl2-1){
        ArrayView++;
    }
      pulses1 = 0;
      pulses2 = 0;
    } else {
      ArrayIndex = 0;
      ArrayView++;
      overflows++;
      pulses1 = 0;
      pulses2 = 0;
    }

  // if(ArrayIndex < 1799 && ArrayView <1798){
  //   ArrayIndex++;
  //   if (ArrayView == ArrayIndex-1){
  //     ArrayView++;
  //   }
  //   pulses1, pulses2 = 0;
  //   } else if (ArrayIndex < 1799 && ArrayView >=1798){
  //   ArrayIndex++;
  //   ArrayView++;
  //   pulses1, pulses2 = 0;
  //   } else if (ArrayIndex == 1799){
  //   ArrayIndex = 0;
  //   ArrayView++;
  //   overflows++;
  //   pulses1, pulses2 = 0;
  //   }

}

void anzeigen()                                                 //Aktualisierung der LCD-Anzeige, getrigger durch Button oder Timer
{
  lcd.clear();
  for (int i = 0; i != 4; ++i)
  {
    int val = ArrayView-i;
    if (i == 0){
      lcd.setCursor(0, i);
      lcd.print((String)"Gesamt in l: "+usedsincestart);
    } else{
      lcd.setCursor(0, i);
      lcd.print((String)val+".Minute: ");
      lcd.print((String)consumptionArray[ArrayView-i]+"l/m");
    }
  }
  downButton = false;
  upButton = false;
}

void debug()                                                    //Nur zu debugzwecken, kann komplett deaktiviert werden!
{
  Serial.print((String)"consumptionArray["+ArrayView+"]: ");
  Serial.println(consumptionArray[ArrayView]);
  Serial.println((String)"ArrayView: ["+ArrayView+"]");
  Serial.println();
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
if(ArrayView >= 3 && ArrayView <=1798){                                      //Wenn mehr als 4 Werte vorhanden sind, und kein Overflow stattgefunden hat
  if(upButton == true || downButton == true) {
    if (downButton == true && ArrayView < ArrayIndex ){
      ArrayView++;
    }
    if (upButton == true && ArrayView >= 4){
      ArrayView--;

    }
    anzeigen();
    //debug();
  }
}
if(ArrayIndex >1798){
  if(upButton == true || downButton == true) {
    long vl1 = overflows*1800;
    long vl2 = vl1+ArrayIndex;
    if (downButton == true && ArrayView < vl2){
      ArrayView++;
    }
    if (upButton == true && ArrayView >= vl2){
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
  //debug();
  }
}
