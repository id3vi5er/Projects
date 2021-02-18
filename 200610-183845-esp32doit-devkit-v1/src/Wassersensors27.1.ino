#include <WiFi.h>
#include <PubSubClient.h>

#define Threshold 5 /* Greater the value, more the sensitivity */
#define alarm 1

RTC_DATA_ATTR int bootCount = 0;
touch_pad_t touchPin;

//const char* ssid = "JL21";
//const char* password = "Wangerooge";
//const char* mqtt_server = "192.168.2.100";

char* ssid = "WLAN";
const char* password = "1Qay2Wsx3Edc";
const char* mqtt_server = "192.168.0.10";


WiFiClient espClient;
PubSubClient client(espClient);

void callbackAlarm() {
  delay(750);
  ++bootCount;
}

void setup_wifi() {
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
    String clientId = "Wassersensor1-";
    clientId += String(random(0xffff), HEX);
    //Attempt to connect
    if (client.connect(clientId.c_str())) {
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

void setup() {
  delay(750);
  Serial.begin(115200);
  delay(500);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println("MQTT Verbunden!");
  client.loop();

  if (!client.connected()) {
    reconnect();
  }
  if (bootCount == 0) {
    client.publish("/alarm/Wassersensor1/", "OK");
    Serial.print("MQTT Gesendet: ");
    Serial.println("OK!");
    delay(950);
  }

    while (bootCount != 0) {
    client.publish("/alarm/Wassersensor1/", "Wasser");
    Serial.print("MQTT Gesendet: ");
    Serial.println("Wasser");
    Serial.print("Bootcount: ");
    Serial.println(bootCount);
    delay(950);
  }




  if (bootCount == 0) {
    ++bootCount;
    touchAttachInterrupt(T3, callbackAlarm, Threshold); //Setup interrupt on Touch Pad 3 (GPIO15)
    esp_sleep_enable_touchpad_wakeup();
    Serial.println("ESP goes to Deepsleep");
    esp_deep_sleep_start();
  } else {
    Serial.println("ESP bleibt wach...");
  }
}

void loop() {
  //This will never be reached
}



/*


  char bootString[50];
  dtostrf(bootCount, 50, 1, bootString);



  void print_wakeup_touchpad(){
  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin)
  {
    case 0  : Serial.println("Touch detected on GPIO 4"); break;
    case 1  : Serial.println("Touch detected on GPIO 0"); break;
    case 2  : Serial.println("Touch detected on GPIO 2"); break;
    case 3  : Serial.println("Touch detected on GPIO 15"); break;
    case 4  : Serial.println("Touch detected on GPIO 13"); break;
    case 5  : Serial.println("Touch detected on GPIO 12"); break;
    case 6  : Serial.println("Touch detected on GPIO 14"); break;
    case 7  : Serial.println("Touch detected on GPIO 27"); break;
    case 8  : Serial.println("Touch detected on GPIO 33"); break;
    case 9  : Serial.println("Touch detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
  }

  //Print the wakeup reason for ESP32 and touchpad too
  print_wakeup_reason();
  print_wakeup_touchpad();

*/
