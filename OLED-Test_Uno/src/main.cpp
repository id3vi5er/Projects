#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Wire.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_GFX.h>

// #define OLED_WIDTH 128
// #define OLED_HEIGHT 32

// #define OLED_ADDR   0x3C

int a = 0;

//Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);


static void counter(void* pvParameters){
  while(1){
  Serial.print("+");
  a++;
  vTaskDelay(30/portTICK_PERIOD_MS);
  }
}

static void displayer(void* pvParameters){
  while(1){
    Serial.println(a);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  xTaskCreate(counter, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(displayer, "Task2", 100, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop() {
//wird nicht ausgeführt!
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setCursor(0, 0);
  // display.setTextColor(WHITE);
  // display.println("Test");
  // display.display();
  // delay(200);
}