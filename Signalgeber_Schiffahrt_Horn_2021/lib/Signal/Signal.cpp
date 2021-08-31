/*
  Signal.h - Library for using a Relay to output signals on ships with for example a horn.
  Created by Nils R., 18 August, 2021.
  Released for public use.
*/

#include "Arduino.h"
#include "Signal.h"
#include "Arduino_FreeRTOS.h"

Signal::Signal(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Signal::shortSignal()
{
  digitalWrite(_pin, HIGH);
  vTaskDelay(1000);
  digitalWrite(_pin, LOW);
  vTaskDelay(2000);
}

void Signal::longSignal()
{
  digitalWrite(_pin, HIGH);
  vTaskDelay(4000);
  digitalWrite(_pin, LOW);
  vTaskDelay(2000);
}