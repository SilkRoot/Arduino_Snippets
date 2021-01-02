#include <ESP8266WiFi.h>
#include <PubSubClient.h>               
#include "DHT.h"

#define DHT_TYPE DHT22
 
const int DHT_PIN = 5;
DHT dht(DHT_PIN, DHT_TYPE);

// Network parameters
const char* SSID = "Network";
const char* PSK = "Password";
const char* MQTT_BROKER = "BrokerIP";
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



float temperature;
float humidity;
 
void setup() {
    Serial.begin(9600);
    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
    dht.begin();  
     
}
 
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);
 
    WiFi.begin(SSID, PSK);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
 
void reconnect() {
    while (!client.connected()) {
        Serial.print("Reconnecting...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}
void loop() {
 
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
 
    snprintf (msg, 50, "Alive since %ld milliseconds", millis());
    Serial.print("Publish message: ");
    Serial.println(msg);
  
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
   
    Serial.print("Temperatur: ");
    Serial.print(temperature);
    Serial.print("°C, Luftfeuchtigkeit: ");
    Serial.print(humidity);
    Serial.println("%");

    client.publish("/device2/temperature", String(temperature).c_str());
    client.publish("/device2/humidity", String(humidity).c_str());
    delay(500);
    
    // Deep Sleep for 60 seconds
    ESP.deepSleep(60e6); 
}
