#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "......";

const char SSID[]               =".....";    // Network SSID (name)
const char PASS[]               ="....";    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  ="......";    // Secret device password

void onMotorSwitchChange();

float water_level;
bool motor_switch;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(water_level, READ, 5 * SECONDS, NULL);
  ArduinoCloud.addProperty(motor_switch, READWRITE, ON_CHANGE, onMotorSwitchChange, 1);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

int TRIGGER = D3;
int ECHO   = D2;
// Replace with your network credentials
const char* ssid = "i.....";
const char* password = "......";
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


  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
    ArduinoCloud.update();
  // Your code here 
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  water_level= (duration/2) / 29.09;
  Serial.print ("Distance:");
  Serial.print(water_level);
  Serial.println ("cm");



  
  
}





/*
  Since MotorSwitch is READ_WRITE variable, onMotorSwitchChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onMotorSwitchChange()  {
  // Add your code here to act upon MotorSwitch change
}
