#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN        3
#define NUMPIXELS  4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 60 // Time (in milliseconds) to pause between pixels

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  int r = 100;
  int g = 0;
  int b = 0;
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.setPixelColor(1, pixels.Color(r, g, b));
  pixels.setPixelColor(2, pixels.Color(r, g, b));
  pixels.setPixelColor(3, pixels.Color(r, g, b));
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
}