#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define transistor 2
#define lowVoltage 4
#define greenLED 5
#define redLED 6


long addierterWert;
int aktuellerWert;
int durchschnittsWert;
int letzterWert;
int jetztWert;
int letzteWerte[4];



int x = -1;
float messSpannung;
int Rload = 11770; //Messwiderstand
double Rsense; //Widerstand des MQ Sensors
float RSenseRLoad;
float PPM;
float spannungsVariable = 0.0048828125;
int maxPPM = 0;
int minPPM = 0;

void setup()              //Startup - initialize SerialUSB + all OUTPUTS + print first Message on LCD
{
  Serial.begin(19200);
  lcd.init(); // initialize the lcd
  pinMode(transistor, OUTPUT);
  pinMode(lowVoltage, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, HIGH);

  // Print a message to the LCD for testing it & Information of Users
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Nano CO-Messg."); //Arduino CO-Mess.
  // wait to check & read
  delay(2000);
}

void clearLCD(int zeile)  //LCD Zeile komplett mit Leerzeichen überschreiben -----> (Zeile 0 und 1 können übergeben werden)
{
  lcd.setCursor(0, zeile);
  lcd.print("                ");
}

void ledReset()           //Indikator-LEDs ausschalten, da GND geschaltet, Pins auf HIGH setzen
{
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, HIGH);
}

void showLast10()         //bei 4 gesammelten Werten einmal alle anzeigen
{
  clearLCD(0);
  lcd.setCursor(0, 0);
  for (byte i = 0; i < 4; i = i + 1) 
    {
      lcd.print(letzteWerte[i]);
      lcd.print(" ");
    }
  Serial.println();
  Serial.println("Array Werte:");
  for (byte i = 0; i < 4; i = i + 1) 
    {
      Serial.println(letzteWerte[i]);
    }
}

void berechnen()
{
  durchschnittsWert = addierterWert / 101;      //Durchschnitt ausrechnen
  messSpannung = spannungsVariable * durchschnittsWert;      //Spannungs aus Analogwert berechnen
  Rsense = ((1.32*Rload)/messSpannung)-Rload;   //Rsense berechnen
  RSenseRLoad = Rsense/Rload;                   //Rs/Rl berechnen  
  PPM = pow(RSenseRLoad/22.3, 1/(-0.662));      //PPM Wert berechnen
  letzteWerte[x] = PPM;                         //PPM Wert in Array schreiben
  if (PPM >= maxPPM){
    maxPPM = PPM;
  }
}

void alarmtest()
{
  if (PPM >= 100)         //Grüne LED anschalten
    {
      digitalWrite(greenLED, LOW);
    }
  else if (PPM >= 200)         //Rote LED anschalten
    {
     digitalWrite(redLED, LOW);
    }
  else                    //LEDs ausschalten
    {
    ledReset();
    }
}

void anzeigen()
{
  clearLCD(0);
  lcd.setCursor(0, 0);
  if(PPM >=20)
  {
    lcd.print("PPM ");
    lcd.print(PPM, 0);
  }
  else 
  {
    lcd.print("PPM ");
    lcd.print("/");
  }
  if(letzterWert != 0)
    {
      lcd.print(" Max ");
      lcd.print(maxPPM);
    }
  if (x == 3)
    {
      showLast10();
    }
}

void heizen()             //LCD Zeile 2 löschen, neu beschriften und Transitor ON und 1,4V OFF Switchen, 90 Sekunden warten
{
  clearLCD(1);
  lcd.setCursor(0, 1);
  lcd.print("Heizzyklus 90s");
  digitalWrite(lowVoltage, LOW);
  delay(250);
  digitalWrite(transistor, LOW);
  delay(90000);
}

void messen()             // Spannung wird auf 1,4V gesenkt, 60 Sekunden lang gehalten und auf den letzten 10s werden 100Messungen gemacht und der Durchschnitt als Ergebis bezogen
{
  if (x >= 3)
    {
      x = -1;
    }
  addierterWert = 0;
  letzterWert = PPM;
  x = x + 1;
  clearLCD(1);
  digitalWrite(lowVoltage, HIGH);
  delay(200);
  digitalWrite(transistor, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("Messzyklus 60s");
  lcd.setCursor(0, 1);
  for (int i = 0; i <= 600; i++)
  { //100 Messungen machen und addieren ---- 600*100ms=60000ms = 60s
    if (i >= 500)
      {
        jetztWert = analogRead(A0);
        Serial.println(jetztWert);
        addierterWert = addierterWert + jetztWert;
        if (i >= 580)
          {
            lcd.print(".");
          }
      }
    delay(100);
  }
}

void loop()               //Hauptschleife die alle anderen aufruft!
{
  heizen();
  messen();
  berechnen();
  alarmtest();
  anzeigen();
  
  //Serial Debugging
  Serial.println();
  Serial.print("Spannung: ");
  Serial.println(messSpannung, 4);
  Serial.print("Rs/Rl ");
  Serial.println(RSenseRLoad, 5);
  Serial.print("PPM: ");
  Serial.println(PPM, 2);
  Serial.println();
  Serial.println("Array Werte:");
  for (byte i = 0; i < 3; i = i + 1) 
  {
    Serial.println(letzteWerte[i]);
  }

}