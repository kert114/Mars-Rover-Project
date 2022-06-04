#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <stdint.h>
#include <string.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <stdio.h>

#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5

#define RST_PIN 4
#define SS_PIN 2

//IPAddress local_IP(192,168,1,184);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,0,0);
//IPAddress primaryDNS(8,8,8,8);
//IPAddress secondaryDNS(8,8,4,4);

MFRC522 mfrc522(SS_PIN,RST_PIN);

//const char *host = "http://your-ip-adress/";
//String GetAddress = "get_name.php";
//String LinkGet = host + GetAddress;
//String getData = "RFID" ; // + string(idcard)

const char* serverName = "http://192.168.43.247:80/post-esp-data.php"; // replace middle with ipv4 of laptop "http://172.20.10.2/post-esp-data.php"
//192.168.158.188
//192.168.43.247
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BME280";
String sensorLocation = "Office";


//const char* ssid ="NOWTVVYRAY_EXT";
//const char* password = "Mnng4sADfXCn";
//const char* ssid ="Yusuf";
//const char* password = "peepeepoopoo";
//const char* ssid ="Kert12345";
//const char* password = "1234567891";
const char* ssid ="AndroidAP_5909";
const char* password = "a65e70efab37";

/*String http_GET_Request(String serverName,  String getData) {
  WiFiClient client;
  HTTPClient http;
 
  http.begin(serverName);                                                 //--> Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
  int httpCodeGet = http.POST(getData);                                //--> Send the request
  String payload="";
  if (httpCodeGet>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpCodeGet);
    payload = http.getString();
  }
  else {
    Serial.print(httpCodeGet);
  }
  http.end();
  return payload;
} */


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
 // if(!WiFi.config(local_IP,gateway,subnet,primaryDNS,secondaryDNS)){
 //   Serial.println("STA Failed to configure");
 // }
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


//String name = "";
//name = http_GET_Request(LinkGet, getData);
//Serial.println("name : " +name);

 if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    if (client.available()>0){
  Serial.println("DEBUG CLIENT IS AVAILABLE");
}
else{
  Serial.println("DOOM");
}

    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&value1=" + String(1)
                          + "&value2=" + String(2) + "&value3=" + String(3) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
  
    

    

    



    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");


    //String a = String(httpResponseCode);
    //Serial.print("HTTP response code:");
    //Serial.print(a);

   if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    if (httpResponseCode<0) {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    } 
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000);

}