#include <Arduino.h>
#include <driver/adc.h>



// adc1_config_width(ADC_WIDTH_BIT_12);


void setup() {
  Serial.begin(115200);
  Serial.println("ADC READ:");
}

void loop() {
  // int val = adc1_get_raw(ADC1_CHANNEL_0);
  // Serial.println(val);
  analogRead(1);
}