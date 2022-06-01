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

IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
WiFiClient client;
MFRC522 mfrc522(SS_PIN, RST_PIN);

const char *ssid = "Kert12345";
const char *password = "1234567891";
const uint16_t port = 12000;
const char *host = "18.234.238.43";

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup()
{

  Serial.begin(115200);

  SPI.begin();

  mfrc522.PCD_Init();

  // if(!WiFi.config(local_IP,gateway,subnet,primaryDNS,secondaryDNS)){

  //  Serial.println("STA Failed to configure");

  // }

  initWiFi();

  Serial.print("RRSI: ");

  Serial.println(WiFi.RSSI());
}

void loop()
{

  unsigned long previousMillis = 0;

  unsigned long interval = 30000;

  unsigned long currentMillis = millis();

  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
  {

    Serial.print(millis());

    Serial.println("Reconnecting to Wifi....");

    WiFi.disconnect();

    initWiFi();

    previousMillis = currentMillis;
  }

  Serial.println("debug1)");

  // Serial.println(client.connect(host, port));

  bool connected;

  connected = client.connect(host, port);

  while (true)
  {

    if (!connected)
    {

      Serial.println("debug2)");

      Serial.println("connection to host server failed");

      delay(1000);

      return;
    }

    Serial.println("Connected to server successful!");

    client.print("Hello from ESP32!");

    connected = true;

    while (client.available())
    {

      char c = client.read();

      Serial.println(c);
    }

    // client.stop();

    delay(1000);

    Serial.println("debug4)");
  }
}