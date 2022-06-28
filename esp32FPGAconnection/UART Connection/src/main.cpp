#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string>
#include <iostream>


#define RXD2 17
#define TXD2 16
String temp[7] = {"a", "b", "c", "d", "e", "f", "1"};
WiFiClient client;
const char *serverName = "http://172.20.10.10:80/post-map-data.php"; // replace middle with ipv4 of laptop "http:///post-esp-data.php"
String apiKeyValue = "tPmAT5Ab3j7F9";
// String object = "Rover";
// const char *ssid = "Kert12345";
// const char *password = "1234567891";
// const char *ssid = "AnroidAP_ 5909";
// const char *password = "a65e70efab37";
const char *ssid = "AngusiPhone";
const char *password = "AngusJames";
// const uint16_t port = 12000;
// const char *host = "192.168.158.30";

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
  }
  Serial.println(ssid);
  Serial.println();
  Serial.println("local ip is:");
  Serial.println(WiFi.localIP());
}
void setup()
{
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  // Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: " + String(TX));
  Serial.println("Serial Rxd is on pin: " + String(RX));
}

uint8_t buf[36];
uint8_t buf2[36];
// int tostore;

void loop()
{ // Choose Serial1 or Serial2 as required

  if (Serial1.available() >= 36)
  {

    // Serial.print(Serial1.read());
    // Serial1.write(555);
    // Serial.print("\n");
    //  // Serial.println(Serial2.read(), BIN);
    //  // Serial.print(char(Serial2.read()));
    //  Serial.print(Serial1.available());
    int len = (Serial1.readBytes(buf, 36));
    char identifier;
    String distance;
    String angle;
    String tostore;
    for (int i = 0; i < 8; i++)
    {
      // Serial.print(buf[i], HEX);
    //  String tostore = String((buf[((4 * i) + 3)] << 24) + (buf[((4 * i) + 2)] << 16) + (buf[((4 * i) + 1)] << 8) + (buf[(4 * i)]), HEX);
      tostore = "a019a004";
      identifier = tostore[0];
      distance = tostore.substring(1, 4);
      angle = tostore.substring(5, 8);
      Serial.println("string");
      Serial.println(tostore);
      Serial.println();
      Serial.println("identifier");
      Serial.println(identifier);
      Serial.println("distance");
      Serial.println(distance);
      Serial.println("angle");
      Serial.println(angle);

      Serial.println();
      // Serial.print(" ");
    }
    // const char* distance2;
    // distance2 = distance;
    const char* angle2;
    Serial.println();
  // String temp[7] = {"a", "b", "c", "d", "e", "f", "1"};
    String object = "";
  String xvalue = "";
    String yvalue = "";
   // Serial.print("distance2: "), Serial.println(distance2);
   Serial.println(temp[0]);
   Serial.println("hello");
   Serial.println(strtol(distance.c_str(), NULL, 16));
   float temp2 = strtol(distance.c_str(), NULL, 16);
    if (String(identifier) == String("a")  && (temp[0] != "sent") && distance != "001" && temp2 <= 30 )
    //  || angle != "000"   && strtoul( angle2 ,NULL, 16) <= 30)
    {
      Serial.print("red");
      object = "red";
      temp[0] = "sent";
      xvalue = "1";
      yvalue = "300";
      Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      
      //xvalue = currentx + distance*sin(temp_gyro_angle);
      //yvalue = currenty + distance*cos(temp_gyro_angle);
    }
    else if (String(identifier) == String("b") && temp[1] != "sent" && distance != "001" && angle != "000"  && strtoul( angle2 ,NULL, 16) <= 30)
    {
      object = "teal";
      temp[1] = "sent";
      xvalue = "322";
      yvalue = "75";
    }
    else if (String(identifier) == String("c") && temp[2] != "sent" && distance != "001" && angle != "000"  && strtoul( angle2 ,NULL, 16) <= 30)
    {
      object = "fuschia";
      temp[2] = "sent";
      xvalue = "345";
      yvalue = "100";
    }
    else if (String(identifier) == String("d") && temp[3] != "sent" && distance != "001" && angle != "000"  && strtoul( angle2 ,NULL, 16) <= 30)
    {
      object = "blue";
      temp[3] = "sent";
      xvalue = "200";
      yvalue = "50";
    }
    else if (String(identifier) == String("e") && temp[4] != "sent" && distance != "001" && angle != "000"  && strtoul( angle2 ,NULL, 16) <= 30)
    {
      object = "green";
      temp[4] = "sent";
      xvalue = "70";
      yvalue = "150";
    }
    else if (String(identifier) == String("f") && temp[5] != "sent" && distance != "001" && angle != "000"  && strtoul( angle2 ,NULL, 16) <= 30)
    {
      object = "yellow";
      temp[5] = "sent";
      xvalue = "250";
      yvalue = "180";
    }
    else if (String(identifier) == String("1") && temp[6] != "sent" && distance != "001" && angle != "000"  && strtoul( angle2 ,NULL, 16) <= 30)
    {
      object = "building";
      temp[6] = "sent";
      xvalue = "123";
      yvalue = "24";
    }

    unsigned long previousMillis = 0;
    unsigned long interval = 30000;
    unsigned long currentMillis = millis();
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
    {
      Serial.print(millis());
      // Serial.println("Reconnecting to Wifi....");
      WiFi.disconnect();
      initWiFi();
      previousMillis = currentMillis;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;
      if (client.available() > 0)
      {
        //  Serial.println("DEBUG CLIENT IS AVAILABLE");
      }
      else
      {
        // Serial.println("DOOM");
      }

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Prepare your HTTP POST request data
      
      String httpRequestData = "api_key=" + apiKeyValue + "&object=" + object + "&xvalue=" + (xvalue) + "&yvalue=" + (yvalue);
      //   Serial.print("httpRequestData: ");
      // Serial.println(httpRequestData);
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode > 0)
      {
        //     Serial.print("HTTP Response code: ");
        //    Serial.println(httpResponseCode);
      }
      if (httpResponseCode < 0)
      {
        //  Serial.print("Error code: ");
        // Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else
    {
      //  Serial.println("WiFi Disconnected");
    }
    delay(300);
  }
}
// Serial1.flush();
// Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);

//     if (Serial1.available() < 36)
// {
//   Serial1.read();
// }
