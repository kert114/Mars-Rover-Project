#include <arduino.h>

int IRSensor = 32; // connect ir sensor to arduino pin 2

void setup()
{

  pinMode(IRSensor, INPUT); // sensor pin INPUT
}

void loop()
{
  Serial.begin(115200);
  int statusSensor = digitalRead(IRSensor);
  // Serial.print("in the loop");
  if (statusSensor == 1)
  {
    Serial.println("Sensor = High");
  }
  else
  {
    Serial.println("Sensor = Low ");
  }
}
