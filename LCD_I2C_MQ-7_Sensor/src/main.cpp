#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

double aktuellerWert;
long addierterWert;
int zeile;
int durchschnittsWert;
int letzterWert;
int jetztWert;

void setup()
{
  Serial.begin(19200);
  lcd.init();                      // initialize the lcd 
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
 
  // Print a message to the LCD for testing it & Information of Users
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Nano CO-Messg."); //Arduino CO-Mess.
  // wait to check & read
  delay(2000);
}

void clearLCD(int zeile)
{
  lcd.setCursor(0, zeile);
  lcd.print("                ");
}

void heizen()
{
  clearLCD(1);
  digitalWrite(4, LOW);
  delay(250);
  digitalWrite(2, LOW);
  lcd.setCursor(0,1);
  lcd.print("Heizzyklus 90s");
  delay(90000);
}

void messen()
{
  addierterWert = 0;
  letzterWert = durchschnittsWert;
  int counter = 0;
  clearLCD(1);
  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(2, HIGH);
  lcd.setCursor(0,1);
  lcd.print("Messzyklus 60s");  
  lcd.setCursor(0,1);
  for (int i = 0; i <= 600; i++) {            //100 Messungen machen und addieren ---- 600*100ms=60000ms = 60s
    if (i >= 500)
    {
      jetztWert = analogRead(A0);
      Serial.println(jetztWert);
      addierterWert = addierterWert + jetztWert;
      if (counter >= 580) 
          {
            lcd.print(".");
          }
    }
    delay(100);
    counter = counter +1;

  }
  durchschnittsWert = addierterWert/101; //Durchschnitt ausrechnen
}

void loop()
{
  heizen();
  messen();
  clearLCD(0);
  lcd.setCursor(0,0);
  lcd.print("PPM ");
  lcd.print(durchschnittsWert);
  lcd.print(" last ");
  lcd.print(letzterWert);
}

