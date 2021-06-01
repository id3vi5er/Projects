    // listDir(SD_MMC, "/", 0);
    // createDir(SD_MMC, "/mydir");
    // listDir(SD_MMC, "/", 0);
    // removeDir(SD_MMC, "/mydir");
    // listDir(SD_MMC, "/", 2);
    // writeFile(SD_MMC, "/hello.txt", "Hello ");
    // appendFile(SD_MMC, "/hello.txt", "World!\n");
    // readFile(SD_MMC, "/hello.txt");
    // deleteFile(SD_MMC, "/foo.txt");
    // renameFile(SD_MMC, "/hello.txt", "/foo.txt");
    // readFile(SD_MMC, "/foo.txt");
    // testFileIO(SD_MMC, "/test.txt");

#include "Arduino.h"
#include "FS.h"
#include "SD_MMC.h"
#include <WiFi.h>
#include <NMEAGPS.h>
#include <GPSport.h>

#define path "/gps.txt"


NMEAGPS  gps; // This parses the GPS characters
gps_fix  fix; // This holds on to the latest values

// Storing-Values/Settings
String firstString = "point,latitude,longitude,elevation";
File myFile;
unsigned long previousMillis = 0;
const long interval = 10000;
long counter = 0;
char buffer[14];

//Struct for saving/get Data more efficent
struct Data{
    float latitude;
    float longitude;
};

void blink(){
    digitalWrite(33, LOW);
    delay(300);
    digitalWrite(33, HIGH);
    delay(300);
    digitalWrite(33, LOW);
    delay(300);
    digitalWrite(33, HIGH);
    delay(300);
}

void setup(){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    //Begin GPS
    gpsPort.begin(9600);
    //Enable Flashlight (also for more power cosumption to disengange Auto-Power-Off Battery Pack)
    pinMode(33, OUTPUT);
    digitalWrite(33, HIGH);

    //Error Handling:
    if(!SD_MMC.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        Serial.println("No SD_MMC card attached");
        return;
    }
}

void loop(){
    while (gps.available( gpsPort )) {
    fix = gps.read();
    if (millis() - previousMillis >= interval) {
        previousMillis = millis();
        if (fix.valid.location) {
        counter++;
        struct Data data = {fix.latitude(), fix.longitude()};
        struct Data *d = &data;
        myFile = SD_MMC.open(path, FILE_APPEND);
        myFile.print((String)counter+",");
        myFile.print(fix.latitude());
        myFile.print(",");
        myFile.println(fix.longitude());
        myFile.close();
        blink();
        }
    }
    }

}