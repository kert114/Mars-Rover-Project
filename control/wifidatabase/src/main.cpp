
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h> 
#include <Arduino.h> 
#include <string.h>
#include <stdint.h> 
#include <WiFi.h>
#include <SPI.h> 
#include <Wire.h>
#include <MFRC522.h> 
//#include <ArduinoHttpClient.h>


#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5

#define RST_PIN 4
#define SS_PIN 2

IPAddress local_IP(192,168,1,184);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,0,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);

MFRC522 mfrc522(SS_PIN,RST_PIN);


void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  if(!WiFi.config(local_IP,gateway,subnet,primaryDNS,secondaryDNS)){
    Serial.println("STA Failed to configure");
  }
}


void initWiFi(){
  WiFi.mode(WIFI_STA);  WiFi.begin("Yusuf","peeepeepoopoo");
  Serial.print("connecting to WiFi ..");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print('.');
    delay(1000);
  
  }
  Serial.println(WiFi.localIP());
}


void loop() {


if(mfrc522.PICC_IsNewCardPresent())
{
  if(mfrc522.PICC_ReadCardSerial())
  {
     idcard = "";
    for(byte i=0; i<mfrc522.uid.size;i++){
        idcard +=(mfrc522.uid.uidByte[i]<0x10? "0":"")
        + String(mfrc522.uid.uidByte[i],HEX);
    }
  Serial.println("tag rfid :" + idcard);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  }
} 

unsigned long previousMillis=0;
unsigned long interval = 30000;
unsigned long currentMillis = millis();
if((WiFi.status() !=WL_CONNECTED)&& (currentMillis - previousMillis >=interval)){
  Serial.print(millis());
  Serial.println("Reconnecting to Wifi....");
  WiFi.disconnect();
  WiFi.reconnect();
  previousMillis=currentMillis;
}
const char *host = "http://your-ip-adress/" ;  
String GetAddress, LinkGet, getData;
GetAddress = "get_name.php";
LinkGet = "RFID=" + String(idcard);
name = "";
name = http_GET_Request(LinkGet, getData);
Serial.println("name : " +name);
}