/*
  Signal.h - Library for using a Relay to output signals on ships with for example a horn.
  Created by Nils R., 18 August, 2021.
  Released for public use.
*/

#ifndef Signal_h
#define Signal_h

#include "Arduino.h"

class Signal
{
  public:
    Signal(int pin);
    void shortSignal();
    void longSignal();
  private:
    int _pin;
};

#endif