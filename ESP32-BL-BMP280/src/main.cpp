#include <Arduino.h>
#include <BluetoothSerial.h>


BluetoothSerial SerialBT;

void setup() {
  SerialBT.begin("ESP32"); //Name des ESP32
}

void loop() {
    delay(2000);
}