/*
@author: Nils Rattensperger
@title: RC Racecar with arduino Nano
@Version 0.2 -- Test
Pin 9 ist Motorausgang
Pin 11 ist Servo für Lenkung 0-180°
*/
#include <Arduino.h>
#include <Servo.h>
#include <RemoteXY.h>

boolean DEBUG = false;

Servo Motor;
Servo Lenkung; 

int motorPin = 9;     //Anschluss des MotorPWM
int lenkPin = 11;    //Anschluss des ServoPWM
int motorpwm = 0;     //0-180
int lenkpwm = 90;  //0-180
int slider_1 = 0;

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6666


// RemoteXY configurate  
#pragma pack(push, 1)
int RemoteXY_CONF[] {255,1,0,1,0,19,0,10,13,0,4,0,15,22,8,24,2,26,66,130,54,24,26,20,2,26};
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t slider_1; // =0..100 slider position 

    // output variables
  int8_t level_1; // =0..100 level position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


void setup() {
  RemoteXY_Init();
  //Serial.begin(115200);         //Serial for Debugging:
  delay(50);                    //Intizialisiere PWM-Driver
  pinMode(INPUT, A0);           //Deklariere Pins:
  pinMode(OUTPUT, motorPin);
  pinMode(OUTPUT, lenkPin);
  Motor.attach(motorPin);       //PWM Ausgänge an Pin binden:
  Lenkung.attach(lenkPin);
}

int getMotorPWM() {
  int speedwert = 0;
  //speedwert = analogRead(A0);             // Wert zwischen 0 und 1023 bei 5V
  // motorpwm = map(speedwert,0,1023,0,180); //map from 0°=0kmh bis 180°=FTL
  motorpwm = map(slider_1);
  return motorpwm;
}

int getLenkPWM() {
  int winkel = 90;
  winkel = analogRead(A0);               // ÄNDERN!
  if (winkel < 0 || winkel > 180) {
    Serial.println("+++Winkel kleiner 0° oder groeßer 180° => auf 90° gesetzt+++");
    return 90; //ERROR
    } else {
      lenkpwm = map(winkel,0,1023,0,180);  //map from 90° links bnis -90° rechts
      return lenkpwm; 
    }
}

void loop() {       // put your main code here, to run repeatedly:
  RemoteXY_Handler();
  Motor.write(getMotorPWM());   
  Lenkung.write(getLenkPWM());
  
  
  
  //if (DEBUG = true) {
  //Serial.print(getMotorPWM());
  //Serial.print(getMotorPWM());
  //}
}
