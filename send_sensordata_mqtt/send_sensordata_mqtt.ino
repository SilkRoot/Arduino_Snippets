#include <ESP8266WiFi.h>
#include <PubSubClient.h>               

float Temperature;
float Humidity;

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
    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
     
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
  
     /*for (int i = 0; i <= 1000; i++) 
     { 
       soilMoisture = soilMoisture + analogRead(SensorPin); 
       Temperature = Temperature + dht.readTemperature(); // Gets the values of the temperature
       Humidity = Humidity + dht.readHumidity(); // Gets the values of the humidity 
       delay(1); 
     } 
     soilMoisture = soilMoisture/1000.0;
     Temperature = Temperature/1000.0;
     Humidity = Humidity/1000.0; 
     Serial.println(soilMoisture); 
     Serial.println(Temperature); 
     Serial.println(Humidity); */

    
/*    client.publish("/home/soilMoisture", String(soilMoisture).c_str());
    client.publish("/home/temperature", String(Temperature).c_str());
    client.publish("/home/humidity", String(Humidity).c_str());*/
    delay(500);
}
