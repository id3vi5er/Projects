/*
  Author: N. Rattensperger
  Titel: Waterflowmeter
  Version: 0.2
  Beschreibung:
    Arduino Mega mit 2 Durchflusssensoren Typ FCH-m-PP-LC (Art. 155374 Conrad) zur Berechnung des Durchflusses bis 48l/h, min. 0,015l/min
    Anzeige über ein 4x20 Zeichen LCD per I2C
    Pins 2/3 sind respektive inFlow und outFLow
    Button 18/19 sind Auf und Ab für das Menü
*/


#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

#define buttonUp 18
#define buttonDown 19
#define inFlowSensor 2
#define outFlowSensor 3

#define delayTimeMeasurements 10000 //if changed, look for calculate() and change respectively!

volatile int pulses1 = 0;
volatile int pulses2 = 0;
volatile boolean upButton = 0;
volatile boolean downButton = 0;

//Anzeige
LiquidCrystal_I2C lcd(0x27,20,4);     // set the LCD address to 0x27 for a 20 chars and 4 line display
boolean firstBoot = true;
boolean firstLoop = true;

//Measurement & Calculation
long timeLast = 0;
int rowVal = 0;
int devval = 0;

//Datapoints storing
//float inputArray[1000];
//int outputArray[1000];
float flowArray[1800];                //Bei 10Sekunden pro Wert ~2,77h bis Overflow
int ArrayIndex = 0;
int ArrayView = 0;


void buttonPressedUp()
{
  upButton = true;
}
void buttonPressedDown()
{
  downButton = true;
}
void inputTriggered1()
{
  pulses1++;
}
void inputTriggered2()
{
  pulses2++;
}

void startupAnzeige()
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

void calculate(int val1, int val2)
{
  float lp = 1/10500.00;
  int pulsesproMinuteIn = val1*6.00;    //Achtung, bei ändern der Zählintervalls zu ändern!
  float lmIn = pulsesproMinuteIn*lp;
  float lhIn = lmIn*60;                 //Achtung, bei ändern der Zählintervalls zu ändern!
  Serial.println(lhIn,4);               //Debug

  int pulsesproMinuteOut = val2*6.00;   //Achtung, bei ändern der Zählintervalls zu ändern!
  float lmOut = pulsesproMinuteOut*lp;
  float lhOut = lmOut*60;               //Achtung, bei ändern der Zählintervalls zu ändern!
  Serial.println(lhOut,4);              //Debug

  flowArray[ArrayIndex] = lhIn-lhOut;

  if(ArrayIndex < 1799){
    ArrayIndex++;
    if (ArrayView == ArrayIndex-1){
      ArrayView++;
    }
    devval = pulses1;
    pulses1 = 0;
    pulses2 = 0;
  } else {
    ArrayIndex = 0;
    ArrayView = -1;
    devval = pulses1;
    pulses1 = 0;
    pulses2 = 0;
  }
}

void anzeigen()
{
  lcd.clear();
  for (int i = 0; i != 4; ++i)
  {
    lcd.setCursor(0, i);
    int val = ArrayView-i;
    lcd.print((String)val+": ");
    lcd.print(flowArray[ArrayView-i]);
    lcd.print(" + ");
    lcd.print(devval);
  }
  downButton = false;
  upButton = false;
}

void debug()
{
  Serial.print((String)"FlowArray["+ArrayView+"]: ");
  Serial.println(flowArray[ArrayView]);
  Serial.print("ArrayView: ");
  Serial.println(ArrayView);
  Serial.println();
}

void setup()
{
lcd.init(); //Im Setup wird der LCD gestartet
lcd.backlight(); //Hintergrundbeleuchtung einschalten (0 schaltet die Beleuchtung aus).
Serial.begin(115200);
pinMode(buttonUp, INPUT_PULLUP);
pinMode(buttonDown, INPUT_PULLUP);
pinMode(inFlowSensor, INPUT_PULLUP);
pinMode(outFlowSensor, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(buttonUp), buttonPressedUp, FALLING);
attachInterrupt(digitalPinToInterrupt(buttonDown), buttonPressedDown, FALLING);
attachInterrupt(digitalPinToInterrupt(inFlowSensor), inputTriggered1, FALLING);
attachInterrupt(digitalPinToInterrupt(outFlowSensor), inputTriggered2, FALLING);
}

void loop(){
if (firstBoot){
  startupAnzeige();
}
if(ArrayIndex >= 3){
  if(upButton == true || downButton == true) {
    if (downButton == true && ArrayView < ArrayIndex){
      ArrayView++;
    }
    if (upButton == true && ArrayView >= 4){
      ArrayView--;

    }
    anzeigen();
    debug();
  }
}
if (millis() - timeLast >= delayTimeMeasurements){
  if(firstLoop){
    lcd.clear();
    lcd.noCursor();
    //lcd.noBlink();  //just for debugging disabled!
    firstLoop=false;
  }
    timeLast = millis();
    calculate(pulses1, pulses2);
    anzeigen();
    debug();
  }
}