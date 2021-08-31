#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


//Ein und Ausgänge werden definiert
#define relais 2

#define Taster1 A0    // Kurs Steuerbord
#define Taster2 A1    // Kurs Backbord
#define Taster3 A2    // Rückwärts
#define Taster4 A3    // Hafeneinfahrt
#define Taster5 A4    // Warnsignal Allgemein (dauerhaft wiederholt)
#define Taster6 A5    // Bleib-Weg (dauerhaft wiederholt)
#define Taster7 A6    // Fahrt bei verminderter Sicht (min. alle 2 Minuten wiederholt)
#define Taster8 A7    // SOS (dauerhaft wiederholt mit 5s Pause nach beenden)

int pausetime = 2000; //Muss nach jeden Signalton 2 Sekunden warten!
int state = 0;


static void shortSignal() {    //ca. 1ne Sekunde lang
  digitalWrite(relais, HIGH);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  digitalWrite(relais, LOW);
  vTaskDelay(pausetime/portTICK_PERIOD_MS);
}

static void longSignal() {     //ca. 4-6 Sekunden
  digitalWrite(relais, HIGH);
  vTaskDelay(4000/portTICK_PERIOD_MS);
  digitalWrite(relais, LOW);
  vTaskDelay(pausetime/portTICK_PERIOD_MS);
}

static void kursSteuerbord(void* pvParameters) { // 1x Kurz (rechts)
  shortSignal();
}

static void kursBackbord(void* pvParameters) {  // 2x Kurz (links)
  shortSignal();
  shortSignal();
}

static void rueckwaerts(void* pvParameters) {  // 3x kurz
  shortSignal();
  shortSignal();
  shortSignal();
}

static void hafenEinfahrt(void* pvParameters) { //1x lang
  longSignal();
}

static void warnSignal(void* pvParameters) { //1x lang, 4x kurz dauerhaft wiederholt
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
  shortSignal();

}

static void bleibWeg(void* pvParameters) {  // kurz, lang in Dauerschleife
  shortSignal();
  longSignal();
}

static void schlechteSicht(void* pvParameters) { //1x lang, min. alle 2 Minuten
  longSignal();
}

static void sos(void* pvParameters) {
  shortSignal();
  shortSignal();
  shortSignal();
  longSignal();
  longSignal();
  longSignal();
  shortSignal();
  shortSignal();
  shortSignal();
}


//            Hauptcode             //




static void executer(void* pvParameters){
  while(1){
    if (state == 1){
      state = 0;
      xTaskCreate(kursSteuerbord, "Steuerbord", 100, NULL, 1, NULL);
    }

    if (state == 2){
      state = 0;
      xTaskCreate(kursBackbord, "Backbord", 100, NULL, 1, NULL);
    }

    if (state == 3){
      state = 0;
      xTaskCreate(rueckwaerts, "Rueckwaerts", 100, NULL, 1, NULL);
    }

    if (state == 4){
      state = 0;
      xTaskCreate(hafenEinfahrt, "Hafen", 100, NULL, 1, NULL);
    }

    if (state == 5){
      xTaskCreate(warnSignal, "Warnung", 100, NULL, 1, NULL);
    }

    if (state == 6){
      xTaskCreate(bleibWeg, "bleibWeg", 100, NULL, 1, NULL);
    }

    if (state == 7){      //alle 2 min!
      xTaskCreate(schlechteSicht, "verminderteSicht", 100, NULL, 1, NULL);
    }

    if (state == 8){
      //sos();
      xTaskCreate(sos, "SOS", 100, NULL, 1, NULL);
    }
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
}


static void checker(void *pvParameters)
{
  while (1)
  {
    if (digitalRead(Taster1) == LOW)
    {
      state = 1;
    }
    else if (digitalRead(Taster2) == LOW)
    {
      state = 2;
    }
    else if (digitalRead(Taster3) == LOW)
    {
      state = 3;
    }
    if (digitalRead(Taster4) == LOW)
    {
      state = 4;
    }


    if (digitalRead(Taster5) == LOW)
    {
      if (state == 5)
      {
        state = 0;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
      else
      {
        state = 5;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
    }
    if (digitalRead(Taster6) == LOW)
    {
      if (state == 6)
      {
        state = 0;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
      else
      {
        state = 6;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
    }
    if (digitalRead(Taster7) == LOW)
    {
      if (state == 7)
      {
        state = 0;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
      else
      {
        state = 7;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
    }
    if (digitalRead(Taster8) == LOW)
    {
      if (state == 8)
      {
        state = 0;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
      else
      {
        state = 8;
        vTaskDelay(200/portTICK_PERIOD_MS);
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(relais, OUTPUT);
  pinMode(Taster1, INPUT_PULLUP);
  pinMode(Taster2, INPUT_PULLUP);
  pinMode(Taster3, INPUT_PULLUP);
  pinMode(Taster4, INPUT_PULLUP);
  pinMode(Taster5, INPUT_PULLUP);
  pinMode(Taster6, INPUT_PULLUP);
  pinMode(Taster7, INPUT_PULLUP);
  pinMode(Taster8, INPUT_PULLUP);

  state = 0;

  xTaskCreate(executer, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(checker, "Task2", 100, NULL, 1, NULL);
  vTaskStartScheduler();
}

void loop() {
}