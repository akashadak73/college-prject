#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
                          
#define FIREBASE_HOST "water-level-ae1c8-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "3KNTTqbbQ2iSPeLs3bmfLpeY0CzxKERBusiot9Hl" //Your Firebase Database Secret goes here

#define WIFI_SSID "iB-----"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "97----"                                      //Password of your wifi network 
 

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
int val=0;
int pump=0;
int TRIGGER = D3;
int ECHO   = D2;

void setup() {
pinMode(TRIGGER, OUTPUT); 
 pinMode(ECHO, INPUT); 
 pinMode(D4, OUTPUT);  
  delay(1000);
  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor

  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}

void loop() { 

// Firebase Error Handling And Writing Data At Specifed Path************************************************


if (Firebase.setInt(firebaseData, "water_level_monitoring_app/data", val)) {    // On successful Write operation, function returns 1  
      
                digitalWrite(TRIGGER, LOW);  
                delayMicroseconds(2); 
                digitalWrite(TRIGGER, HIGH);
                delayMicroseconds(10); 
                digitalWrite(TRIGGER, LOW);
                long duration = pulseIn(ECHO, HIGH);
                val = (duration/2) / 29.09;
                val=(40-val)*2.5;
               Serial.println("Value Uploaded Successfully");
               Serial.print("water level= ");
               Serial.println(val);
               Serial.println("\n");
               delay(1000);

     }

else {        
    Serial.println(firebaseData.errorReason());
  }
  

  if (Firebase.getInt(firebaseData, "water_level_monitoring_app/pump")) {                           // On successful Read operation, function returns 1 
     

    if (firebaseData.dataType() == "string") {   
                                                       // print read data if it is integer
       pump = firebaseData.stringData().toInt();
        if(pump==1)                                                             // If, the Status is 1, turn on the Relay1
     {
      digitalWrite(D4,HIGH);
      Serial.println("pump ON");
    }
    else if(pump==0)                                                      // If, the Status is 0, turn Off the Relay1
    {                                      
      digitalWrite(D4,LOW);
      Serial.println("pump OFF");
    } 
     
      Serial.println(pump);
      Serial.println("\n Change value at firebase console to see changes here."); 
      delay(1000);
      
    }

  }
  else {
    Serial.println(firebaseData.errorReason());
  }

if(val>=90){
  digitalWrite(D4,LOW);
  Serial.println("pump OFF");
  Firebase.setString(firebaseData, "water_level_monitoring_app/pump", "0");
}
else if(val>=0 && val<=30){
  digitalWrite(D4,HIGH);
  Serial.println("pump On");
  Firebase.setString(firebaseData, "water_level_monitoring_app/pump", "1");
}
}




