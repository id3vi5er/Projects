#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

#define OLED_ADDR   0x3C
#define MPU6050_ADRESS 0x68

const int ACCEL_OFFSET   = 200;
const int GYRO_OFFSET    = 151;  // 151
const int GYRO_SENSITITY = 131;  // 131 is sensivity of gyro from data sheet
const float GYRO_SCALE   = 0.2; //  0.02 by default - tweak as required
const float LOOP_TIME    = 0.15; // 0.1 = 100ms

int16_t accX, accY, accZ, gyrX, gyrY, gyrZ, tVal;
int accValue[3], accAngle[3], gyroValue[3], temperature, accCorr;
float gyroAngle[3], gyroCorr;


Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADRESS); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  delay(1000);
}

void loop(){
Wire.beginTransmission(MPU6050_ADRESS);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADRESS, 7*2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  for(byte i=0; i<3; i++) {
    accValue[i] = Wire.read()<<8 | Wire.read(); // reading registers: ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT
  }
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  for(byte i=0; i<3; i++) {
    gyroValue[i] = Wire.read()<<8 | Wire.read(); // reading registers: GYRO_XOUT, GYRO_>OUT, GYRO_ZOUT
  }
display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Accel: ");
  for(byte i=0; i<3; i++) {
    accCorr = accValue[i] - ACCEL_OFFSET;
    accCorr = map(accCorr, -16800, 16800, -90, 90);
    accAngle[i] = constrain(accCorr, -90, 90);
    display.print(accAngle[i]);
    display.print("\t");
  }
  display.println();
  display.print("Gyro: \t");

  for(byte i=0; i<3; i++) {
    gyroCorr = (float)((gyroValue[i]/GYRO_SENSITITY) - GYRO_OFFSET);
    gyroAngle[i] = (gyroCorr * GYRO_SCALE) * -LOOP_TIME;
    display.print(gyroAngle[i]);
    display.print("\t");
  }
display.display();
  delay(LOOP_TIME * 1000);
}