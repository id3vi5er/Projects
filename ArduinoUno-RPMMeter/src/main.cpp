#include <Arduino.h>

const int irPin = A0;
int triggers = 0;
unsigned long elapsedTime;
int laststate = 0;
const unsigned long sampleTime = 3000;
int rpmMax = 0;

int rpmGet()
{
  unsigned long currentTime = 0;
  unsigned long startTime = millis();
  while (currentTime <= sampleTime)
  {
    int val = digitalRead(irPin);
    if (!val)
    {
      if (laststate)
      {
        triggers++;
        laststate = 0;
      }
    }
    else
    {
      laststate = 1;
    }
    currentTime = millis() - startTime;
  }
  int countRpm = int(60000 / float(sampleTime)) * (triggers / 2);

  triggers = 0;
  return countRpm;
}

void setup()
{
  pinMode(irPin, INPUT);
  Serial.begin(9600);
}

void loop()
{

  int rpm = rpmGet();
  if (rpm > rpmMax)
  {
    rpmMax = rpm;
    Serial.print("MaxRpm:");
    Serial.println(rpm);
  }
}

