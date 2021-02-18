#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


char* ssid = "JL21";
const char* password = "Wangerooge";
const char* mqtt_server = "192.168.2.100";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {     //Verbindung zum Wifi Netzwerk herstellen:
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {  //Wird ausgeführt wenn eine Nachricht auf einem Topic ankommt
  //for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
}

void reconnect() {      // Wiederholen bis wir wd vebindung zum Broker haben
  while (!client.connected()) {
    // Serial.print("Attempting MQTT connection...");
    String clientId = "Wassersensor";           // Name des ESPs
    if (client.connect(clientId.c_str())) {     // Versuchen zu verbinden:
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() { //Einmaliges Setup beim Start
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println("Wassersensor -- MQQT -- Debug");
}

void loop() { //Dauerschleife
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    if (touchRead(T0) <= 5) {
   client.publish("/alarm/wassermelder1", "Wasser!");
  }
  delay(1000);
}