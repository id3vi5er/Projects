#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>


#define SSID      "WLAN"
#define PASSWD    "1Qay2Wsx3Edc"
#define HOSTNAME  "ESP"

ESP8266WebServer server ( 80 );

void httpIndex() {
  server.send(200, "text/plain", "Hello World!");
}

void setup() {
  Serial.begin(115200);

  delay(500);

  Serial.print("Connecting.");
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(SSID, PASSWD);
  do{
    delay(500);
    Serial.print(".");
  } while (WiFi.status() != WL_CONNECTED);
  Serial.print("OK\nIP: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(HOSTNAME)) {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", httpIndex);
  
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  MDNS.update();
  server.handleClient();
}