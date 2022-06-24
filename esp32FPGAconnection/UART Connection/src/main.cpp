#include <Arduino.h>
#include <SPI.h>

#define RXD2 17
#define TXD2 16

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

void loop()
{ // Choose Serial1 or Serial2 as required
  while (Serial1.available() >= 36)
  {
    // Serial.println(Serial2.read(), BIN);
    // Serial.print(char(Serial2.read()));
    int len = (Serial1.readBytes(buf, 28));
    for (int i = 0; i < 36; i++)
    {
      Serial.print(buf[i]);
    }
  }
  //     if (Serial1.available() < 36)
  // {
  //   Serial1.read();
  // }
}