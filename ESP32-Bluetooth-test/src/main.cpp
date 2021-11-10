#include <Arduino.h>
#include <BluetoothSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int ONE_WIRE_BUS = 4;
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);


BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);   //Just for Debugging
  SerialBT.begin("ESP32test"); //Name des ESP32
  sensors.begin();
  sensors.setResolution(9);
}

void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  SerialBT.println((String)"t:"+temp);
  // int touch = touchRead(13);
  // SerialBT.println((String)"Wasser (<0!): "+touch);
  delay(500);
}