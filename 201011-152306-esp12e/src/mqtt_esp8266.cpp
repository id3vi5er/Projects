// include libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define MSG_BUFFER_SIZE	(50)

// Konstanten und 

const char* ssid = "WLAN";
const char* password = "1Qay2Wsx3Edc";
const char* mqtt_server = "192.168.0.10";
const int analogInPin = PIN_A0;

float valueTemp = 0;
int valueLight = 0;

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
char msg1[MSG_BUFFER_SIZE];

//Initialisieren des ESP/PUB_SUB
WiFiClient espClient;
PubSubClient client(espClient);
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 't') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("LED1");
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
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(LED_BUILTIN, HIGH);
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

  unsigned long now = millis();
  if (now - lastMsg > 4000) {
    lastMsg = now;
    valueTemp = sensors.getTempCByIndex(0);
    valueLight = analogRead(analogInPin);
    
    snprintf (msg, MSG_BUFFER_SIZE, "%f", valueTemp);
    snprintf (msg1, MSG_BUFFER_SIZE, "%i", valueLight);

    Serial.print("Publish Temp: ");
    Serial.println(msg);
    Serial.print("Publish Light: ");
    Serial.println(msg1);
    client.publish("Temperatur", msg);
    delayMicroseconds(250);
    client.publish("Licht", msg1);
  }
  
}