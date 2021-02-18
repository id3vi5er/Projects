/*
  Simulate two sensor and send information back over the iBUS to the receiver (and back to transmitter
  as telemetry).
  
  Requires Arduino board with multiple UARTs (such as ATMEGA 2560, Micro or ESP32)
  - serial0 - monitor output (debug output to PC, this is through the build-in USB)
  - serial1 - connected RX of the serial port to the ibus receiver pin
    Connect the (TX) pin also to the RX/ibus connection using an 1.2k Ohm reistor or 1N4148 diode
    (cathode=white ring of the diode at the side of TX2) 

  sensor types defined in IBusBM.h:
  
  #define IBUSS_INTV 0 // Internal voltage (in 0.01)
  #define IBUSS_TEMP 1 // Temperature (in 0.1 degrees, where 0=-40'C)
  #define IBUSS_RPM  2 // RPM
  #define IBUSS_EXTV 3 // External voltage (in 0.1V)

  #define IBUSS_INTV 0x00 // Internal voltage (in 0.01)
  #define IBUSS_TEMP 0x01 // Temperature (in 0.1 degrees, where 0=-40'C)
  #define IBUSS_RPM  0x02 // RPM
  #define IBUSS_EXTV 0x03 // External voltage (in 0.01)
  #define IBUS_PRESS 0x41 // Pressure (in Pa)
  #define IBUS_SERVO 0xfd // Servo value
*/

#include <Arduino.h>
#include <IBusBM.h>
#define TEMPBASE 400    // base value for 0'C

IBusBM IBus; 

// -- sensor values -- 
//uint32_t speed=0;
//uint16_t temp=TEMPBASE+200; // start at 20'C
uint32_t voltage;
uint32_t readValue;


void setup() {
  // initialize serial port for debug
  Serial.begin(115200);
  pinMode(A0, INPUT);

  // iBUS connected to serial1
  IBus.begin(Serial1);
  /* The default RX/TX pins for Serial1 on ESP32 boards are pins 9/10 and they are sometimes not
   exposed on the printed circuit board. You can use Serial2 (for which the pins are often available) or
   you can change the pin number by replacing the line above with:
   IBusServo.begin(Serial1, 1, 21, 22);

   <<<<< Adding 2 sensors, JEDER Sensor hat eine Adress 0 ist der Reciever, also mit 1 anfangen! >>>>>  */

  //IBus.addSensor(IBUSS_RPM);
  IBus.addSensor(IBUSS_EXTV);
  IBus.addSensor(IBUS_PRESS);

}

uint32_t readVoltage() {
  readValue = analogRead(A0);
  voltage = map(readValue,0,1023,0,500);
  return voltage;
}

void loop() {
  IBus.setSensorMeasurement(1, readVoltage()); 
  Serial.print(" EXT. Volt= "); // Serial.println((temp-TEMPBASE)/10.);
  Serial.println(readVoltage());

  IBus.setSensorMeasurement(2, 1150);

  /*
  IBus.setSensorMeasurement(2,speed);
  speed += 10;  
  Serial.print("Speed=");
  Serial.print(speed);
  */

  delay(500);
}
