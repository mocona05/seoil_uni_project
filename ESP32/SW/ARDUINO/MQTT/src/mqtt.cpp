#include "Arduino.h"
#include <WiFi.h>
#include "PubSubClient.h"
#include "mqtt.h"
#include "gpio.h"
#include "board.h"
#include "drv_sht21.h"
#include <ArduinoJson.h>

const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";
const char* mqtt_server = "192.168.1.101";
const char* topic = "topic";
const char* nodeName = "esp32";
const char* subScribe = "esp32/output";
WiFiClient espClient;
PubSubClient client(espClient);




long lastMsg = 0;
//char msg[50];
//int value = 0;
//const int capacity =40;


void mqtt_init(void){
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi(void) {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char scribe_str[500]="";
  StaticJsonDocument<500> scribe;
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  strncpy(scribe_str,(char*)payload,length);

  DeserializationError error = deserializeJson(scribe, scribe_str);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
  Serial.println(scribe_str);
  int LED[4]={0,};
  for(int i=0;i<4;i++){
    LED[i] = scribe["LED"][i];
    Serial.print(LED[i]);
  }
  digitalWrite(LED_RED_PIN,LED[1]);
  digitalWrite(LED_GREEN_PIN,LED[2]);
  digitalWrite(LED_BLUE_PIN,LED[3]);
  digitalWrite(LED_BUILTIN_PIN,LED[0]);
}



void reconnect(void) {
  // Loop until we're reconnected
  while (!client.connected()) {
//    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(nodeName)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(subScribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}




void mqtt_handler(float humi, float temp){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;
    
  StaticJsonDocument<500> doc;
  char json_str[500];

  JsonArray sensorValues = doc.createNestedArray("sensor");
//  sensorValues.add(temp);
//  sensorValues.add(humi);
  char value_str[8];
  dtostrf(temp, 1, 2, value_str);
  sensorValues.add(value_str);
  dtostrf(humi, 1, 2, value_str);
  sensorValues.add(value_str);

  JsonArray digitalValues = doc.createNestedArray("led");
  digitalValues.add(READ_BUILTIN_LED_PIN);
  digitalValues.add(READ_RED_LED_PIN);
  digitalValues.add(READ_GREEN_LED_PIN);
  digitalValues.add(READ_BLUE_LED_PIN);

  serializeJson(doc, json_str);
  client.publish(topic,json_str);

  }
}