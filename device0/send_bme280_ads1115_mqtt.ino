#include <ESP8266WiFi.h>
#include <PubSubClient.h>
            
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1015.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

Adafruit_ADS1115 ads;
const float multiplier = 0.1875F;



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

    ads.begin();
    
    
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

    int16_t adc0, adc1, adc2, adc3;

    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);
    adc2 = ads.readADC_SingleEnded(2);
    adc3 = ads.readADC_SingleEnded(3);

    int sensorValue = analogRead(A0);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float light = sensorValue * (5.0 / 1023.0);
    
    
    Serial.print("AIN0: "); Serial.println(adc0 * multiplier);
    Serial.print("AIN1: "); Serial.println(adc1 * multiplier);
    Serial.print("AIN2: "); Serial.println(adc2 * multiplier);
    Serial.print("AIN3: "); Serial.println(adc3 * multiplier);
    Serial.print("Light: ");Serial.println(light);
    Serial.print("Temperature: ");Serial.println(temperature);
    Serial.print("Humidity: ");Serial.println(humidity);
    Serial.print("Pressure: ");Serial.println(pressure);
    
    client.publish("/device0/temperature", String(temperature).c_str());
    client.publish("/device0/humidity", String(humidity).c_str());
    client.publish("/device0/pressure", String(pressure).c_str());
    client.publish("/device0/light", String(light).c_str());
    client.publish("/device0/adc0", String(adc0).c_str());
    client.publish("/device0/adc1", String(adc1).c_str());
    client.publish("/device0/adc2", String(adc2).c_str());
    client.publish("/device0/adc3", String(adc3).c_str());
  
    delay(500);
    
    // Deep Sleep for 60 seconds
    ESP.deepSleep(60e6); 
}
