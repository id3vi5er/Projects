/*
/ CC Nils Rattensperger
/ 14.10.2022
/ Axis and Allies Point Counter
*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define up 2
#define down 3
#define p1 4
#define p2 5
#define p3 6
#define p4 7
#define p5 8
#define p6 9

//displayed Player Names
String nameP1 = "Player 1";
String nameP2 = "Player 2";
String nameP3 = "Player 3";
String nameP4 = "Player 4";
String nameP5 = "Player 5";
String nameP6 = "Player 6";

int startdelaytime = 500;
int buffering = 200;

//Point Values
int pointsP1 = 0;
int pointsP2 = 0;
int pointsP3 = 0;
int pointsP4 = 0;
int pointsP5 = 0;
int pointsP6 = 0;



LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);
LiquidCrystal_I2C lcd3(0x25, 16, 2);
LiquidCrystal_I2C lcd4(0x24, 16, 2); //Adresse prüfen!!!
LiquidCrystal_I2C lcd5(0x23, 16, 2);
LiquidCrystal_I2C lcd6(0x22, 16, 2);

byte Character[8] =
{ 0b01110,
  0b01110,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010,
  0b10001
};

void refreshLCD() {
  lcd1.clear();
  lcd1.setCursor(4,0);
  lcd1.print(nameP1);
  lcd1.setCursor(7, 1);
  lcd1.print(pointsP1);
  
  lcd2.clear();
  lcd2.setCursor(4,0);
  lcd2.print(nameP2);
  lcd2.setCursor(7, 1);
  lcd2.print(pointsP2);

  lcd3.clear();
  lcd3.setCursor(4,0);
  lcd3.print(nameP3);
  lcd3.setCursor(7, 1);
  lcd3.print(pointsP3);

  lcd4.clear();
  lcd4.setCursor(4,0);
  lcd4.print(nameP4);
  lcd4.setCursor(7, 1);
  lcd4.print(pointsP4);

  lcd5.clear();
  lcd5.setCursor(4,0);
  lcd5.print(nameP5);
  lcd5.setCursor(7, 1);
  lcd5.print(pointsP5);

  lcd6.clear();
  lcd6.setCursor(4,0);
  lcd6.print(nameP6);
  lcd6.setCursor(7, 1);
  lcd6.print(pointsP6);
}

void initLCD() {
  lcd1.init();
  lcd1.backlight();
  lcd1.setCursor(0,0);
  lcd1.printstr("Axis & Allies");
  lcd1.setCursor(0, 1);
  lcd1.printstr("Punktezaehler");
  delay(startdelaytime);

  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(5,0);
  lcd2.printstr("Software by");
  lcd2.setCursor(0, 1);
  lcd2.printstr("N. Rattensperger");
  delay(startdelaytime);

  lcd3.init();
  lcd3.backlight();
  lcd3.setCursor(0,0);
  lcd3.printstr("Befehle werden");
  lcd3.setCursor(0, 1);
  lcd3.printstr("erteilt...");
  delay(startdelaytime);

  lcd4.init();
  lcd4.backlight();
  lcd4.setCursor(3, 0);
  lcd4.printstr("Strategien");
  lcd4.setCursor(0, 1);
  lcd4.printstr("werden erstellt");
  delay(startdelaytime);

  lcd5.init();
  lcd5.backlight();
  lcd5.setCursor(4, 0);
  lcd5.printstr("Motoren");
  lcd5.setCursor(0, 1);
  lcd5.printstr("werden gestartet");
  delay(startdelaytime);

  lcd6.init();
  lcd6.backlight();
  lcd6.setCursor(0, 0);
  lcd6.printstr("Auf in die");
  lcd6.setCursor(0, 1);
  lcd6.printstr("Schlacht!");
  delay(startdelaytime);
  
  refreshLCD();
}

void initPins() {
pinMode(up, INPUT_PULLUP);
pinMode(down, INPUT_PULLUP);
pinMode(p1, INPUT_PULLUP);
pinMode(p2, INPUT_PULLUP);
pinMode(p3, INPUT_PULLUP);
pinMode(p4, INPUT_PULLUP);
pinMode(p5, INPUT_PULLUP);
pinMode(p6, INPUT_PULLUP);
}

void debug() {
  Serial.print("P1: ");
  Serial.print(pointsP1);

  Serial.print(" P2:");
  Serial.print(pointsP2);

  Serial.print(" P3:");
  Serial.print(pointsP3);

  Serial.print(" P4:");
  Serial.print(pointsP4);

  Serial.print(" P5:");
  Serial.print(pointsP5);

  Serial.print(" P6:");
  Serial.println(pointsP6);
}

void setup() {
  Serial.begin(9600);
  initLCD();
  initPins();
}

void loop() {
        // Taste Up
        // 0 = GND, Taster mit Pullup!
  if(digitalRead(up) == 0){     //Up is been tested
    if(digitalRead(p1) == 0){   //if player 1 is selected (rotary switch)
      pointsP1 = pointsP1+1;    //add 1 point
      delay(buffering);         //Bounce is a problem
    }
    if(digitalRead(p2) == 0){   //rinse and repeat
      pointsP2 =  pointsP2+1;
      delay(buffering);
    }
    if(digitalRead(p3) == 0){
      pointsP3 = pointsP3+1;
      delay(buffering);
    }
    if(digitalRead(p4) == 0){
      pointsP4 = pointsP4+1;
      delay(buffering);
    }
    if(digitalRead(p5) == 0){
      pointsP5 = pointsP5+1;
      delay(buffering);
    }
    if(digitalRead(p6) == 0){
      pointsP6 = pointsP6+1;
      delay(buffering);
    }
    refreshLCD();
  }
        // Taste Down

  if(digitalRead(down) == 0){   //down is been tested
    if(digitalRead(p1) == 0){
      pointsP1 = pointsP1-1;
      delay(buffering);
    }
    if(digitalRead(p2) == 0){
      pointsP2 = pointsP2-1;
      delay(buffering);
    }
    if(digitalRead(p3) == 0){
      pointsP3 = pointsP3-1;
      delay(buffering);
    }
    if(digitalRead(p4) == 0){
      pointsP4 = pointsP4-1;
      delay(buffering);
    }
    if(digitalRead(p5) == 0){
      pointsP5 = pointsP5-1;
      delay(buffering);
    }
    if(digitalRead(p6) == 0){
      pointsP6 = pointsP6-1;
      delay(buffering);
    }
    refreshLCD();
  }

  //debug();
}
