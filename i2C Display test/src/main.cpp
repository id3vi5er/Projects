/***************************************************************************************************/
/*
  This is an Arduino sketch for LiquidCrystal_I2C library.

  This chip uses I2C bus to communicate, specials pins are required to interface
  Board:                                    SDA                    SCL                    Level
  Uno, Mini, Pro, ATmega168, ATmega328..... A4                     A5                     5v
  Mega2560................................. 20                     21                     5v
  Due, SAM3X8E............................. 20                     21                     3.3v
  Leonardo, Micro, ATmega32U4.............. 2                      3                      5v
  Digistump, Trinket, ATtiny85............. 0/physical pin no.5    2/physical pin no.7    5v
  Blue Pill, STM32F103xxxx boards.......... PB7                    PB6                    3.3v/5v
  ESP8266 ESP-01........................... GPIO0/D5               GPIO2/D3               3.3v/5v
  NodeMCU 1.0, WeMos D1 Mini............... GPIO4/D2               GPIO5/D1               3.3v/5v
  ESP32.................................... GPIO21/D21             GPIO22/D22             3.3v

  Frameworks & Libraries:
  ATtiny Core           - https://github.com/SpenceKonde/ATTinyCore
  ESP32 Core            - https://github.com/espressif/arduino-esp32
  ESP8266 Core          - https://github.com/esp8266/Arduino
  ESP8266 I2C lib fixed - https://github.com/enjoyneering/ESP8266-I2C-Driver
  STM32 Core            - https://github.com/rogerclarkmelbourne/Arduino_STM32

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#include <Arduino.h>
#include <Wire.h>              //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_I2C.h>

// #define COLUMS 16
// #define ROWS   2

// #define PAGE   ((COLUMS) * (ROWS))

// LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

// void setup()
// {
//   Serial.begin(115200);

//   while (lcd.begin(COLUMS, ROWS) != 1) //colums - 20, rows - 4
//   {
//     Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
//     delay(5000);   
//   }

//   lcd.print(F("PCF8574 is OK..."));    //(F()) saves string to flash & keeps dynamic memory free
//   delay(2000);

//   lcd.clear();
// }

// void loop()
// {
//   for (uint8_t i = 0; i < 256; i++)
//   {
//     if ((i != 0) && (i % PAGE == 0))
//     {
//       delay(10000);
//       lcd.clear();
//     }

//     lcd.write(i);
//   }
// } **/
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
}
