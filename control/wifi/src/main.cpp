#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <WiFi.h>


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

const char* ssid ="Yusuf";
const char* password = "peepeepoopoo";

void initWiFi(){
  WiFi.mode(WIFI_STA);  WiFi.begin(ssid,password);
  Serial.print("connecting to WiFi ..");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print('.');
    delay(1000);
  
  }
  Serial.println(ssid);
  Serial.println();
  Serial.println("local ip is:");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  if(!WiFi.config(local_IP,gateway,subnet,primaryDNS,secondaryDNS)){
    Serial.println("STA Failed to configure");
  }
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {


unsigned long previousMillis=0;
unsigned long interval = 30000;
unsigned long currentMillis = millis();
if((WiFi.status() !=WL_CONNECTED)&& (currentMillis - previousMillis >=interval)){
  Serial.print(millis());
  Serial.println("Reconnecting to Wifi....");
  WiFi.disconnect();
  initWiFi();
  previousMillis=currentMillis;
}

}