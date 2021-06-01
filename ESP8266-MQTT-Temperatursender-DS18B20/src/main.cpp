#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define sendezeit 30000 //30 Sekunden

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)

const int ONE_WIRE_BUS = 4;
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

const char* ssid = "JL21";
const char* password = "Wangerooge";
const char* mqtt_server = "192.168.2.100";
const char* name = "ESP3-Motorraum";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temp1 = 0;
float temp2 = 0;
float temp3 = 0;
float temp4 = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(name)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(115200);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  sensors.begin();
}


void loop(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > sendezeit) {
    lastMsg = now;
    //sensors.setResolution(10);
    sensors.requestTemperatures(); // Send the command to get temperatures
    temp1 = sensors.getTempCByIndex(0);
    delay(200);
    client.publish("motor/temp1", String(temp1).c_str());
    Serial.println(temp1);

    temp2 = sensors.getTempCByIndex(1);
    delay(200);
    client.publish("motor/temp2", String(temp2).c_str());
    Serial.println(temp2);

    temp3 = sensors.getTempCByIndex(2);
    delay(200);
    client.publish("motor/temp3", String(temp3).c_str());
    Serial.println(temp3);

    temp4 = sensors.getTempCByIndex(3);
    delay(200);
    client.publish("motor/temp4", String(temp4).c_str());
    Serial.println(temp4);
  }
}