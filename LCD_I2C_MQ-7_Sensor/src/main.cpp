#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define transistor 2
#define lowVoltage 4
#define greenLED 5
#define redLED 6
#define Sensor1 A0
#define Sensor2 A1


const float spannungsVariable = 0.00488758553; // = 5/1023
const int Schwellwert1 = 50;    //Schwellwert für grünen Alarm
const int Schwellwert2 = 200;   //Schwellwert für roten Alarm
const float sensor_reading_clean_air1 = 246;                        //RAW Wert in sauberer Luft Sensor 1
const float sensor_reading_clean_air2 = 273;                        //RAW Wert in sauberer Luft Sensor 2
const long interval = 3600000; //Intervall für den Zähler zum zurücksetzen des Max-Wertes
const float spannungVlow = 1.55; //Eingestellte Spannung beim Messen in V
unsigned long previousMillis = 0;   //Letzte Zeit beim prüfen der vergangenen Zeit

float myPPM = 0;    //Wert anhand meiner Formel berechnet
float PPMdurchschnitt;  //Durchschnitt beider Sensoren
boolean error50 = false;   //Error bei PPM Unterschieden >=50ppm
boolean first_run = true;   //zum anzeigen der Zyklen vor den ersten Messwerten
int maxDurchschnitt = 0;

// Variablen 1. Sensor
long addierterWert1;
int aktuellerWert1;
int durchschnittsAnalogWert1;
int letzterWert1;
int jetztWert1;
int letzteWerte1[5];
float messSpannung1;
float RloadkOhm1 = 10; //Messwiderstand in kOhm (Referenz)  //WICHTIG 
float RloadOhm1 = 9810;
float sensor_reading_100_ppm_CO1 = 297;                        //Wenn Messbar eingeben, wenn nicht, dann so für Standartwerte - relativ genau
float sensor_100ppm_resistance1;
float sensor_100ppm_CO_resistance_kOhm1;
int R1_100ppm_CO = 230;
float sensor_base_resistance_kOhm1;
float Rsense1;
float R_relation1;  // Verhätnis aus Rs/Ro wobvei Rs der Widerstand in Gas und Ro der Widerstand in 100ppm CO ist
float PPM1;
float maxPPM1 = 0;    //Maximalwert des letzten "const long intervall"
int y = -1;

//Variablen für 2ten Sensor
long addierterWert2;
int aktuellerWert2;
int durchschnittsAnalogWert2;
int letzterWert2;
int jetztWert2;
int letzteWerte2[5];
float messSpannung2;
float RloadkOhm2 = 10; //Messwiderstand in kOhm (Referenz)  //WICHTIG 
float RloadOhm2 = 9920;
float sensor_reading_100_ppm_CO2 = 302;                        //Wenn Messbar eingeben, wenn nicht -> Standartwerte - relativ genau
float sensor_100ppm_resistance2;
float sensor_100ppm_CO_resistance_kOhm2;
int R2_100ppm_CO = 230;
float sensor_base_resistance_kOhm2;
float Rsense2;
float R_relation2;    // Verhätnis aus Rs/Ro wobvei Rs der Widerstand in Gas und Ro der Widerstand in 100ppm CO ist
float PPM2; 
float maxPPM2 = 0;  //Maximalwert des letzten "const long intervall"
int x = -1;


float sensor1_raw_value_to_CO_ppm(float value)
{
  if(value < 1) return -1;   //wrong input value
    Rsense1 = ((spannungVlow*RloadOhm1)/(spannungsVariable*value))-RloadOhm1;
    R_relation1 = Rsense1/R1_100ppm_CO;
    float zwischensumme = R_relation1/22.3;
    double CO_ppm = pow(zwischensumme, 1/-0.662);

  //  sensor_base_resistance_kOhm1 = RloadkOhm1 * 307 / sensor_reading_clean_air1 - RloadkOhm1;
  // // sensor_100ppm_CO_resistance_kOhm1 = sensor_base_resistance_kOhm1 * 0.25;
  // sensor_100ppm_CO_resistance_kOhm1 = RloadkOhm1 * 307 / sensor_reading_100_ppm_CO1 - RloadkOhm1;
  // float sensor_R_kOhm = RloadkOhm1 * 307 / value - RloadkOhm1;
  // R_relation1 = sensor_R_kOhm / sensor_100ppm_CO_resistance_kOhm1;
  // float CO_ppm = pow(R_relation1/22.3,-0.662);
  if(CO_ppm < 0) CO_ppm = 0;
  return CO_ppm;
}

float sensor2_raw_value_to_CO_ppm(float value)
{
  if(value < 1) return -1; //wrong input value
    Rsense2 = ((spannungVlow*RloadOhm2)/(spannungsVariable*value))-RloadOhm2;
    R_relation2 = Rsense2/R2_100ppm_CO;
    float zwischensumme = R_relation2/22.3;
    double CO_ppm = pow(zwischensumme,1/-0.662);

  // sensor_base_resistance_kOhm2 = RloadkOhm2 * 307 / sensor_reading_clean_air2 - RloadkOhm2;
  // sensor_100ppm_CO_resistance_kOhm2 = RloadkOhm2 * 307 / sensor_reading_100_ppm_CO2 - RloadkOhm2;
  // float sensor_R_kOhm = RloadkOhm2 * 307 / value - RloadkOhm2;
  // R_relation2 = sensor_R_kOhm/ sensor_100ppm_CO_resistance_kOhm2;
  // float CO_ppm = pow(R_relation2/22.3,-0.662);
  if(CO_ppm < 0) CO_ppm = 0;
  return CO_ppm;
}

void clearLCD(int zeile)  //LCD Zeile komplett mit Leerzeichen überschreiben -----> (Zeile 0 und 1 können übergeben werden)
{
  lcd.setCursor(0, zeile);
  lcd.print("                ");
}

void showLast5()         //bei 4 gesammelten Werten einmal alle anzeigen
{
  clearLCD(0);
  lcd.setCursor(0, 0);
  for (byte i = 0; i < 5; i = i + 1) 
    {
      lcd.print(letzteWerte1[i]);
      lcd.print("-");
    }
}

void berechnen()          //Berechnungen aller Werte durchführen
{
  durchschnittsAnalogWert1 = addierterWert1 / 101;      //Durchschnitt Sensor 1 ausrechnen
  durchschnittsAnalogWert2 = addierterWert2 / 101;      //Durchschnitt Sensor 2 ausrechnen

  messSpannung1 = spannungsVariable * durchschnittsAnalogWert1;      //Spannungs aus Analogwert Sensor 1 berechnen
  messSpannung2 = spannungsVariable * durchschnittsAnalogWert2;      //Spannungs aus Analogwert Sensor 2 berechnen

  PPM1 = sensor1_raw_value_to_CO_ppm(durchschnittsAnalogWert1);
  PPM2 = sensor2_raw_value_to_CO_ppm(durchschnittsAnalogWert2);

  letzteWerte1[x] = PPM1;                         //PPM1 Wert in Array schreiben
  letzteWerte2[x] = PPM2;                         //PPM2 Wert in Array schreiben
  PPMdurchschnitt = (PPM1 + PPM2) / 2;
  if (PPM1 >= maxPPM1){
    maxPPM1 = PPM1;
  }
  if (PPM2 >= maxPPM2){
    maxPPM2 = PPM2;
  }
  maxDurchschnitt = (maxPPM1 + maxPPM2) / 2;
}

void alarmtest()          //Testen ob ein Alarmzustand vorliegt (PPM1 > 100)
{
  if (PPM1 >= Schwellwert1)         //Grüne LED anschalten
    {
      digitalWrite(greenLED, LOW);
    }
  else if (PPM1 >= Schwellwert2)         //Rote LED anschalten
    {
     digitalWrite(redLED, LOW);
    }
  else                    //LEDs ausschalten
    {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);
    }
}

void anzeigen()           //Anzeige nach jedem kompletten Zyklus 
{
  first_run = false;
  if (PPM1 - PPM2 >= 50 || PPM1 -PPM2 <= -50)
    {
      clearLCD(1);
      lcd.setCursor(0, 1);
      lcd.print("Differenz >50ppm");
      clearLCD(0);
      lcd.setCursor(0, 0);
      lcd.print("S1: ");
      lcd.print(PPM1, 0);
      lcd.print(" S2: ");
      lcd.print(PPM2, 0);
      error50 = true;
    }
    else {
      error50 = false;
      clearLCD(0);
      lcd.setCursor(0, 0);
      lcd.print("PPM1 ");
      lcd.print(PPM1, 0);
      lcd.print(" PPM2 ");
      lcd.print(PPM2, 0);
      if(maxPPM1 >= 0)
      {
        clearLCD(1);
        lcd.setCursor(0,1);
        lcd.print("Max/h: ");
        lcd.print(maxDurchschnitt);
      }
    } 

  if (x == 4) //0-4 = 5er Array
    {
      showLast5();
    }
  //Aktuelle Zeit speichern und schauen ob das Intervall von 60 Minuten vergangen ist
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;
      maxPPM1 = 0;
    }
}

void heizen()             //LCD Zeile 2 löschen, neu beschriften und Transitor ON und 1,4V OFF Switchen, 60 Sekunden warten
{
  if (first_run == true)
  {
    clearLCD(1);
    lcd.setCursor(0, 1);
    lcd.print("Heizzyklus 60s");
  }
  Serial.println("Heizzyklus gestartet, 60 Sekunden warten...");
  digitalWrite(lowVoltage, LOW);
  delay(250);
  digitalWrite(transistor, LOW);
  delay(60000);
}

void messen()             // Spannung wird auf 1,4V gesenkt, 90 Sekunden lang gehalten und auf den letzten 10s werden 100Messungen gemacht und der Durchschnitt als Ergebis bezogen
{
  if (x >= 4)
    {
      x = -1;
    }
  x = x + 1;

  if (y >= 4)
    {
      y = -1;
    }
  y = y + 1;
  
  addierterWert1 = 0;
  letzterWert1 = PPM1;
  addierterWert2 = 0;
  letzterWert2 = PPM2;

  digitalWrite(lowVoltage, HIGH);
  delay(200);
  digitalWrite(transistor, HIGH);
  if(first_run == true)
  {
    clearLCD(1);
    lcd.setCursor(0, 1);
    lcd.print("Messzyklus 90s");
    lcd.setCursor(0, 1);
  }
  lcd.setCursor(0,1);
  Serial.println("Messzyklus gestartet, 90 Sekunden warten...");

  for (int i = 0; i <= 900; i++)
  { //100 Messungen machen und addieren ---- 900*100ms=90000ms = 90s
    if (i >= 800)
      {
        jetztWert1 = analogRead(Sensor1);
        Serial.println(jetztWert1);
        addierterWert1 = addierterWert1 + jetztWert1;

        jetztWert2 = analogRead(Sensor2);
        Serial.println(jetztWert2);
        addierterWert2 = addierterWert2 + jetztWert2;
        if (i >= 880)
          {
            lcd.print(".");
          }
      }
    delay(100);
  }
}

void debugging()          //Serial Debugging
{
  Serial.println();
  Serial.println("Allgemeine Debugging-Daten:");
  Serial.println();
  Serial.print("Error50 aktiv: ");
  Serial.println(error50);
  Serial.print("Durchschnittswert PPM: ");
  Serial.println(PPMdurchschnitt);
  Serial.println();
  Serial.println(Rsense1);
  Serial.println();
  Serial.println();

  //Daten von Sensor 1:
  Serial.println("Sensordaten S1:");
  Serial.println();
  Serial.print("Spannung1: ");
  Serial.print(messSpannung1, 4);
  Serial.print(" RAW: ");
  Serial.println(durchschnittsAnalogWert1);
  Serial.print("R Sensor: ");
  Serial.println(Rsense1);
  Serial.print("Rs/Rl: ");
  Serial.println(R_relation1, 5);
  Serial.print("PPM1: ");
  Serial.print(PPM1, 2);
  Serial.print(" Max der letzten Stunde: ");
  Serial.println(maxPPM1);
  Serial.println("Letzte Messungen:");
  for (byte i = 0; i < 5; i = i + 1) 
  {
    Serial.println(letzteWerte1[i]);
  }
  Serial.println();
  //Daten von Sensor 2:
  Serial.println("Sensordaten S2:");
  Serial.println();
  Serial.print("Spannung2: ");
  Serial.print(messSpannung2, 4);
  Serial.print(" RAW: ");
  Serial.println(durchschnittsAnalogWert2);
  Serial.print("R Sensor: ");
  Serial.println(Rsense2);
  Serial.print("Rs/Rl: ");
  Serial.println(R_relation2, 5);
  Serial.print("PPM2: ");
  Serial.print(PPM2, 2);
  Serial.print(" Max der letzten Stunde: ");
  Serial.println(maxPPM2);
  Serial.println("Letzte Messungen:");
  for (byte i = 0; i < 5; i = i + 1) 
  {
    Serial.println(letzteWerte2[i]);
  }
  Serial.println();
  // Serial.print("MyPPM: ");
  // Serial.println(myPPM, 2);
  // Serial.println();
  

}

void setup()              //Startup - initialize SerialUSB + all OUTPUTS + print first Message on LCD
{
  Serial.begin(19200);
  Serial.println("CO Messgeraet - Author: N. Rattensperger");

  // Print a message to the LCD for testing it & Information of Users
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.println("CO-Messer N.R.  "); //Startup-Anzeige
  
  pinMode(transistor, OUTPUT);
  pinMode(lowVoltage, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, HIGH);


  delay(2000);
}

void loop()               //Hauptschleife die alle anderen aufruft!
{
  heizen();
  messen();
  berechnen();
  alarmtest();
  anzeigen();
  debugging();
}