#include <ESP8266WiFi.h>
#include <WiFiClient.h>
int TRIGGER = D3;
int ECHO   = D2;
// Replace with your network credentials
const char* ssid = "......";
const char* password = "......";

int data; 
void setup(void){
 pinMode(TRIGGER, OUTPUT); 
 pinMode(ECHO, INPUT); 
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println(".");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.print("Connected to ");
  Serial.println(ssid);
 
}
 void loop(void){
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  data = (duration/2) / 29.09;
  Serial.print (”Distance:“);
  Serial.print(data);
  Serial.println ("cm");

  delay(2000);
}
