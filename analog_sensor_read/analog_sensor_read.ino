/* 
 Soil Moisture Sensor  
 modified on 21 Feb 2019 
 by Saeed Hosseini @ Electropeak 
 https://electropeak.com/learn/ 
*/
#define SensorPin A0 
float sensorValue = 0; 

void setup() { 
 Serial.begin(9600); 
} 
void loop() {
 // iterate over 1000 measurements to avoid blunders
 for (int i = 0; i <= 1000; i++) 
 { 
   sensorValue = sensorValue + analogRead(SensorPin); 
   delay(1); 
 } 
 sensorValue = sensorValue/1000.0; 
 Serial.println(sensorValue); 
 
 // wait 1000 milisec before starting another measurement cycle
 delay(1000); 
} 
