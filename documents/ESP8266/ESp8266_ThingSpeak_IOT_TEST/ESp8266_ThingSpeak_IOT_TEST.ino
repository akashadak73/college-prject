#define DEBUG true
#define RX 2
#define TX 3
#include <SoftwareSerial.h>

SoftwareSerial esp8266(RX,TX); 

String AP = "Smart Switch"; //"AP NAME";
String PASS = "";//"subhajit"; //"AP PASSWORD";

String HOST = "api.thingspeak.com";
String PORT = "80";
String API = "6ZM027GB0FLT84IY"; //"S887XDVLL7BL43YG"; //"DR5NULIQIKTBGWBI"; // "CJZ1G46URZWEGRVR";
String field = "field1";

int countTrueCommand = 0;
int countTimeCommand = 0; 
boolean found = false; 

int valSensor = 0;
String valSensor_String = String(123123);

//String data = "";
//char d[] = "SEND";
//int c;


///////////////////////////////////// SETUP ///////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\"" +AP+ "\",\"" +PASS+ "\"", 15,"OK");
  countTrueCommand = 0;
}

///////////////////////////////////// LOOP ///////////////////////////////////////////////////
void loop() {
  
  String getData = "GET /update?api_key="+ API +"&"+ field +"="+valSensor_String+"\r\n\r\n";
  switch(countTrueCommand)
  {  
    case 0:
              valSensor = analogRead(A0); // random(100000); // random value, change with sensor value if using sensor
              valSensor_String = String(valSensor);            
            sendCommand("AT",5,"OK");
            break;
    case 1:
            sendCommand("AT+RST",10,"ready");
            break;
    case 2:
            sendCommand("AT+CIPMUX=1",5,"OK");
            break;
    case 3: 
            sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
            break;
    case 4:
            sendCommand("AT+CIPSEND=0," +String(getData.length()+0),10,">");
            break;
    case 5:
            esp8266.println(getData);
            delay(1500);
            countTrueCommand++;
            break;
    case 6:
            sendCommand("AT+CIPCLOSE=0",5,"OK");
            break;
    case 7:
            Serial.println(valSensor);
            Serial.print(getData);
            Serial.print(",");
            Serial.println(getData.length());
            
            //valSensor =  12332;// random(100000); // random value, change with sensor value if using sensor
            //valSensor_String = String(valSensor);
            countTrueCommand = 0;
            delay(10000);
            break;
  }
}

///////////////////////////////////// SEND_COMMAND ///////////////////////////////////////////////////
void sendCommand(String command, int maxTime, char readReplay[]) {
  
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  
  while(countTimeCommand < (maxTime*1))
  {
      esp8266.println(command);//at+cipsend
      if(esp8266.find(readReplay))//ok
      {
        found = true;
        break;      
      }
      countTimeCommand++;     
  }
  
  if(found == true)
  {  
    Serial.println("DONE");
    
    countTrueCommand++;
    countTimeCommand = 0;  
  }
  
  else if(found == false)
  {  
    Serial.println("Fail");
    
    countTrueCommand = 0;
    countTimeCommand = 0;  
  }
  
  found = false;

}
