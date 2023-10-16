#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "NTPClient.h"
#include "WiFiUdp.h"

WiFiClient wifiClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
PubSubClient mqttClient(wifiClient);

// Wifi
const char* ssid     = "wifiname";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "password";     // The password of the Wi-Fi network

// GPIO
const int status_led = 14;
const int strobe_gpio = 5;
const int speaker_gpio = 4;

// MQTT
const char* mqtt_server = "192.168.1.53";
const uint16_t mqtt_server_port = 1883;
const char* mqttTopic = "fox";

void wifiConnect() {
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print(++i); Serial.print(' ');
  }

  timeClient.begin();

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void strobe() {
  digitalWrite(speaker_gpio, HIGH);
  for (int i = 0; i <= 220; i++) {
    // put your main code here, to run repeatedly:
    digitalWrite(strobe_gpio, HIGH);
    delay(50);
    digitalWrite(strobe_gpio, LOW);
    delay(50);
  }
  digitalWrite(speaker_gpio, LOW);
}

void mqttConnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String mqttClientId = "";
    if (mqttClient.connect(mqttClientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe(mqttTopic);
      String myCurrentTime = timeClient.getFormattedTime();
      mqttClient.publish(mqttTopic,("Fox strober connected - Time: " + myCurrentTime).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" will try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: '");
  Serial.print(topic);
  Serial.print("' with payload: ");
  String payload_text = "";
  for (unsigned int i = 0; i < length; i++) {
    payload_text += (char)payload[i];
    if (payload_text.length() == length) {
      Serial.println(payload_text);
        if (payload_text.indexOf("**strobe") > 0) {
          Serial.println();
          String myCurrentTime = timeClient.getFormattedTime();
          mqttClient.publish(mqttTopic,("Strobe request recieved - Time: " + myCurrentTime).c_str());
          strobe();
      }
    }
  }
}

void setup() {

  // Setup
  Serial.begin(115200);
  pinMode(status_led, OUTPUT);
  pinMode(strobe_gpio, OUTPUT);
  pinMode(speaker_gpio, OUTPUT);
  digitalWrite(status_led, HIGH);
  digitalWrite(strobe_gpio, LOW); // HIGH IS OFF
  digitalWrite(speaker_gpio, LOW);

  wifiConnect();

  mqttClient.setServer(mqtt_server, mqtt_server_port);
  mqttClient.setCallback(mqttCallback);

}

void loop() {

  timeClient.update();

  if (!mqttClient.connected()) {
     mqttConnect();
  }
  mqttClient.loop();
  
}