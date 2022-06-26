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
uint8_t buf2[36];

void loop()
{ // Choose Serial1 or Serial2 as required
  // Serial.print("hey");
  if (Serial1.available() >= 36)
  {

    // Serial.print(Serial1.read());
    // Serial1.write(555);
    // Serial.print("\n");
    //  // Serial.println(Serial2.read(), BIN);
    //  // Serial.print(char(Serial2.read()));
    //  Serial.print(Serial1.available());
   int len = (Serial1.readBytes(buf, 36)); 
    
    for (int i = 0; i < 36; i++)
    {
     // Serial.print(buf[i], HEX);
    //  buf2 = {Serial.println((buf[4*i + 3] <<24)+(buf[4*i + 2] << 16) + (buf[4*i + 1] << 8) + (buf[4*i]))};
     Serial.println((buf[4*i + 3] <<24)+(buf[4*i + 2] << 16) + (buf[4*i + 1] << 8) + (buf[4*i]), HEX);
      Serial.println();
      // Serial.print(" ");
    }
    Serial.println();
  }
  // Serial1.flush();
  // Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);
}
//     if (Serial1.available() < 36)
// {
//   Serial1.read();
// }
