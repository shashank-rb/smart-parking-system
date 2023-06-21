#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#include<SPI.h>
#include<MFRC522.h>
#include<WiFi.h>
#include<ThingSpeak.h>

MFRC522 x(21,22);  //(SDA,RESET)
char s[] = "SSID";
char p[] = "psswd";
WiFiClient client;
unsigned long cn = 123456; //channel number
const char*API = "ThingSpeak API key here";

Servo myservo;  
int pos = 0;    
int servoPin = 4;

void setup() 
{
  pinMode(2,OUTPUT);
  SPI.begin();
  x.PCD_Init();
  
  WiFi.begin(s,p);
  WiFi.mode(WIFI_STA);
  
  Serial.begin(9600);
  
  ThingSpeak.begin(client);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000); 
  myservo.write(45);
}
void loop() 
{
    if(WiFi.status() == WL_CONNECTED)
    {
      digitalWrite(2,HIGH);
    }
    else
    {
      digitalWrite(2,LOW);
    }
    
    if(!x.PICC_IsNewCardPresent())
    {
      return;
    }
    if(!x.PICC_ReadCardSerial())
    {
      return;
    }
  String content = "";
  for(byte i = 0;i<x.uid.size;i++)
  {
    //Serial.print(x.uid.uidByte[i] < 10 ? "0" : " ");
    //Serial.print(x.uid.uidByte[i],HEX);
    content.concat(String(x.uid.uidByte[i] < 10 ? "0" : " "));
    content.concat(String(x.uid.uidByte[i],HEX));  
  }
  content.toUpperCase();
    if (content.substring(1) == "5A A7 5B 19" || content.substring(1)== "AA 83 20 86")
  {
     Serial.println("\nAuthorized access");
     if (content.substring(1) == "5A A7 5B 19")
     {
      Serial.println("\nAlex Entered UID:5A A7 5B 19 \n");
      senddata("Somil", content.substring(1));
      
     }
     
     if (content.substring(1) == "AA 83 20 86")
     {
      Serial.println("\nBob Entered UID:AA 83 20 86\n");
      senddata("Saurabh", content.substring(1));
     }
    
    
    moveServo();
 
  }
  else{
    Serial.println("Invalid RFID card!!");
    //delay(1000);
  }
  
  
}

void senddata(String input, String uid)
{
  ThingSpeak.setField(1,input);
  ThingSpeak.setField(2, uid);
  ThingSpeak.writeFields(cn, API);
}

void moveServo()
{
    myservo.write(180);
    delay(3000);
    myservo.write(45);
  }
  
 
