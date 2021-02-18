/*
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Update these with values suitable for your network.
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char* ssid = "JL21";
const char* password = "Wangerooge";
const char* mqtt_server = "192.168.2.100";

//const char* ssid = "WLAN";
//const char* password = "1Qay2Wsx3Edc";
//const char* mqtt_server = "192.168.0.10";

float temp1 = 0;
float temp2 = 0;
float temp3 = 0;

WiFiClient espClient;
PubSubClient client(espClient);
//long lastMsg = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Bugstrahlruder";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  //pinMode(BUILTIn, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  sensors.begin();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
  sensors.requestTemperatures();
  temp1 = sensors.getTempCByIndex(0);
  temp2 = sensors.getTempCByIndex(1);
  temp3 = sensors.getTempCByIndex(2);

  
  char temp1String[8];
  dtostrf(temp1, 1, 2, temp1String);
  
  char temp2String[8];
  dtostrf(temp2, 1, 2, temp2String);
  
  char temp3String[8];
  dtostrf(temp3, 1, 2, temp3String);

  
  
  Serial.println(temp1String);
  Serial.println(temp2String);
  client.publish("/temp/bugstrahl_gehaeuse", temp1String);
  client.publish("/temp/bugstrahl_leitung", temp2String);
  //client.publish("/temp/bugstrahl_3", temp3String);
  
  delay(2000);
}