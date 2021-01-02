#include <ESP8266WiFi.h>
#include <PubSubClient.h>
            
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

// Network parameters
const char* SSID = "Network";
const char* PSK = "Password";
const char* MQTT_BROKER = "BrokerIP";
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
 
void setup() {
    Serial.begin(9600);
    delay(100);

    bme.begin(0x76); 
      
    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
    
}
 
void setup_wifi() {
    delay(100);
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

    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F; 

    Serial.println(temperature);
    Serial.println(humidity);
    Serial.println(pressure);
    
    client.publish("/home/temperature", String(temperature).c_str());
    client.publish("/home/humidity", String(humidity).c_str());
    client.publish("/home/pressure", String(pressure).c_str());
  
    delay(500);
}
