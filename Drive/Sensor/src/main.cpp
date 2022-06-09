#include <Arduino.h>
#include "SPI.h"
#include <Robojax_L298N_DC_motor.h>
#include <Wire.h>
#include <wifi.h>
#include <MFRC522.h>

// these pins may be different on different boards

#define PIN_SS 5
#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_SCK 18

#define PIN_MOUSECAM_RESET 35
#define PIN_MOUSECAM_CS 5

#define ADNS3080_PIXELS_X 30
#define ADNS3080_PIXELS_Y 30

#define CHA 0
#define ENA 21 // 19 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 22 // 18
#define IN2 14 // 5
// motor 2 settings
#define IN3 17
#define IN4 16
#define ENB 4 // this pin must be PWM enabled pin if Arduino board is used
#define CHB 1
const int CCW = 2; // do not change
const int CW = 1;  // do not change
#define motor1 1   // do not change
#define motor2 2   // do not change
// for two motors without debug information // Watch video instruciton for this line: https://youtu.be/2JTMqURJTwg
Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB);
// for two motors with debug information
// Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB, true);

#define ADNS3080_PRODUCT_ID 0x00
#define ADNS3080_REVISION_ID 0x01
#define ADNS3080_MOTION 0x02
#define ADNS3080_DELTA_X 0x03
#define ADNS3080_DELTA_Y 0x04
#define ADNS3080_SQUAL 0x05
#define ADNS3080_PIXEL_SUM 0x06
#define ADNS3080_MAXIMUM_PIXEL 0x07
#define ADNS3080_CONFIGURATION_BITS 0x0a
#define ADNS3080_EXTENDED_CONFIG 0x0b
#define ADNS3080_DATA_OUT_LOWER 0x0c
#define ADNS3080_DATA_OUT_UPPER 0x0d
#define ADNS3080_SHUTTER_LOWER 0x0e
#define ADNS3080_SHUTTER_UPPER 0x0f
#define ADNS3080_FRAME_PERIOD_LOWER 0x10
#define ADNS3080_FRAME_PERIOD_UPPER 0x11
#define ADNS3080_MOTION_CLEAR 0x12
#define ADNS3080_FRAME_CAPTURE 0x13
#define ADNS3080_SROM_ENABLE 0x14
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_LOWER 0x19
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_UPPER 0x1a
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_LOWER 0x1b
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_UPPER 0x1c
#define ADNS3080_SHUTTER_MAX_BOUND_LOWER 0x1e
#define ADNS3080_SHUTTER_MAX_BOUND_UPPER 0x1e
#define ADNS3080_SROM_ID 0x1f
#define ADNS3080_OBSERVATION 0x3d
#define ADNS3080_INVERSE_PRODUCT_ID 0x3f
#define ADNS3080_PIXEL_BURST 0x40
#define ADNS3080_MOTION_BURST 0x50
#define ADNS3080_SROM_LOAD 0x60

#define ADNS3080_PRODUCT_ID_VAL 0x17

//  WIFI STUFF
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
WiFiClient client;
MFRC522 mfrc522(IN2, IN3); //// RANDOM IN2 AND IN3 FOR TESTING

const char *ssid = "Kert12345";
const char *password = "1234567891";
const uint16_t port = 12000;
const char *host = "192.168.158.30";

///

int total_x = 0;
int total_y = 0;

int total_x1 = 0;
int total_y1 = 0;

int x = 0;
int y = 0;

int a = 0;
int b = 0;

int distance_x = 0;
int distance_y = 0;

int temp_x = 0;
int temp_y = 0;

float angle = 0;

volatile byte movementflag = 0;
volatile int xydat[2];

//////////////////////// MORE WIFI STUFFF//////////////////////
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

///

int convTwosComp(int b)
{
  // Convert from 2's complement
  if (b & 0x80)
  {
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}

int tdistance = 0;

void mousecam_reset()
{
  digitalWrite(PIN_MOUSECAM_RESET, HIGH);
  delay(1); // reset pulse >10us
  digitalWrite(PIN_MOUSECAM_RESET, LOW);
  delay(35); // 35ms from reset to functional
}

int mousecam_init()
{
  pinMode(PIN_MOUSECAM_RESET, OUTPUT);
  pinMode(PIN_MOUSECAM_CS, OUTPUT);

  digitalWrite(PIN_MOUSECAM_CS, HIGH);

  mousecam_reset();

  return 1;
}

void mousecam_write_reg(int reg, int val)
{
  digitalWrite(PIN_MOUSECAM_CS, LOW);
  SPI.transfer(reg | 0x80);
  SPI.transfer(val);
  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(50);
}

int mousecam_read_reg(int reg)
{
  digitalWrite(PIN_MOUSECAM_CS, LOW);
  SPI.transfer(reg);
  delayMicroseconds(75);
  int ret = SPI.transfer(0xff);
  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(1);
  return ret;
}

struct MD
{
  byte motion;
  char dx, dy;
  byte squal;
  word shutter;
  byte max_pix;
};

void mousecam_read_motion(struct MD *p)
{
  digitalWrite(PIN_MOUSECAM_CS, LOW);
  SPI.transfer(ADNS3080_MOTION_BURST);
  delayMicroseconds(75);
  p->motion = SPI.transfer(0xff);
  p->dx = SPI.transfer(0xff);
  p->dy = SPI.transfer(0xff);
  p->squal = SPI.transfer(0xff);
  p->shutter = SPI.transfer(0xff) << 8;
  p->shutter |= SPI.transfer(0xff);
  p->max_pix = SPI.transfer(0xff);
  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(5);
}

// pdata must point to an array of size ADNS3080_PIXELS_X x ADNS3080_PIXELS_Y
// you must call mousecam_reset() after this if you want to go back to normal operation
int mousecam_frame_capture(byte *pdata)
{
  mousecam_write_reg(ADNS3080_FRAME_CAPTURE, 0x83);

  digitalWrite(PIN_MOUSECAM_CS, LOW);

  SPI.transfer(ADNS3080_PIXEL_BURST);
  delayMicroseconds(50);

  int pix;
  byte started = 0;
  int count;
  int timeout = 0;
  int ret = 0;
  for (count = 0; count < ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y;)
  {
    pix = SPI.transfer(0xff);
    delayMicroseconds(10);
    if (started == 0)
    {
      if (pix & 0x40)
        started = 1;
      else
      {
        timeout++;
        if (timeout == 100)
        {
          ret = -1;
          break;
        }
      }
    }
    if (started == 1)
    {
      pdata[count++] = (pix & 0x3f) << 2; // scale to normal grayscale byte range
    }
  }

  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(14);

  return ret;
}

float find_angle(int x, int y)
{
  int r = 70;
  float angle;
  float d = x ^ 2 + y ^ 2;
  angle = abs(((2 * (r ^ 2)) - d) / (2 * (r ^ 2)));
  angle = acos(angle);
  return angle;
}
/////////////////////////////// ROVER MOVE FUNCTIONS///////////////////////////////
void move_F(int x = 0)
{
  robot.rotate(motor1, 40, CCW); // turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, 40, CW);  // turn motor2 with 25% speed in CW direction
  delay(x);
  robot.brake(1);
  robot.brake(2);
}
void move_B(int x = 0)
{
  robot.rotate(motor1, 40, CW);  // turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, 40, CCW); // turn motor2 with 25% speed in CW direction
  delay(x);
  robot.brake(1);
  robot.brake(2);
}
void move_R(int x = 90){
  robot.rotate(motor1, 29, CW);// turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, 29, CW);// turn motor2 with 25% speed in CW direction
  // convert distance required to move to time needed to turn
  x=9500*x/360;00
  delay(x);
  robot.brake(1);
  robot.brake(2);
}
void move_L(int x = 90){
  robot.rotate(motor1, 29, CCW);// turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, 29, CCW);// turn motor2 with 25% speed in CW direction
  // convert distance required to move to time needed to turn
  x=9500*x/360;
  delay(x);
}
void brake_rover(int x = 0)
{
  robot.brake(1);
  robot.brake(2);
  delay(x);
  robot.brake(1);
  robot.brake(2);

}

// Need to use the sensor working with an LED so we can callebrate the distances along x and y accurately
// If this gets working then we can accurately measure angle travelled as well as distance travelled so if
// driving commands are in the form of final distance to travel that can work but we can also figure out
// how far the rover travels if the motors are at a set speed for a set time allowing for accurately moving
// set distances

void setup()
{
  pinMode(PIN_SS, OUTPUT);
  pinMode(PIN_MISO, INPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_SCK, OUTPUT);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);

  Serial.begin(115200);
  robot.begin();

  if (mousecam_init() == -1)
  {
    Serial.println("Mouse cam failed to init");
    while (1)
      ;
  }
  /////////////WIFI STUFF IN SETUP///////////

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

char asciiart(int k)
{
  static char foo[] = "WX86*3I>!;~:,`. ";
  return foo[k >> 4];
}

byte frame[ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y];
////////////////////////////////////////////////////////////////////////VOID LOOP
void loop()
{

  /////////////////////////CONTROL THE ROVER USING 123456789
  int i;
  while (Serial.available() == 0)
  {
  } // if it breaks, do >= 0 in conditions as per Hepple
  i = Serial.parseInt();
  switch (i)
  {
  case 1:
    // move straight for 3 sec
    move_F(1000);
    brake_rover(10);
    break;

  case 2:
    // rotate left for 3 sec
    move_L(1000);
    brake_rover(10);
    break;

  case 3:
    // rotate right for 3 sec
    move_R(1000);
    brake_rover(10);
    break;

  case 4:
    // move back for 3 sec
    move_B(1000);
    brake_rover(10);
    break;

  case 5:
    while (i == 5)
    {
      // continuous case forward
      move_F(3);
      // Serial.println("We are a go");
      i = Serial.parseInt();
    }
    break;

  case 6:
    while (i == 6)
    {
      // continuous case left
      move_L(3);
      i = Serial.parseInt();
    }
    break;

  case 7:
    while (i == 7)
    {
      // continuous case right
      move_R(3);
      i = Serial.parseInt();
    }
    break;

  case 8:
    while (i == 8)
    {
      // continuous case back
      move_B(3);
      i = Serial.parseInt();
    }
    break;

  case 9:
    brake_rover(10);
    break;

  case 10:
    while (i == 10)
    {

      // adjust to the right
      robot.rotate(motor1, 80, CCW); // run motor1 at 60% speed in CCW direction
      robot.rotate(motor2, 75, CW);  // run motor1 at 60% speed in CW direction
      delay(10);
      i = Serial.parseInt();
    }
    break;

  default:
    // brake
    brake_rover;
    // Serial.println("We are a go0");
    break;
  }
  // acceleration test
  //  for(int i=0; i<=100; i++)
  //  {
  //    robot.rotate(motor1, i, CW);// turn motor1 with i% speed in CW direction (whatever is i)
  //    robot.rotate(motor2, i, CCW);// turn motor1 with i% speed in CW direction (whatever is i)
  //    delay(100);
  //  }
  //  delay(2000);
  //  robot.brake(1);
  //  robot.brake(2);
  //  delay(2000);

#if 0

    if(movementflag){

    tdistance = tdistance + convTwosComp(xydat[0]);
    Serial.println("Distance = " + String(tdistance));
    movementflag=0;
    delay(3);
    }

  
  // if enabled this section grabs frames and outputs them as ascii art

  if(mousecam_frame_capture(frame)==0)
  {
    int i,j,k;
    for(i=0, k=0; i<ADNS3080_PIXELS_Y; i++)
    {
      for(j=0; j<ADNS3080_PIXELS_X; j++, k++)
      {
        Serial.print(asciiart(frame[k]));
        Serial.print(' ');
      }
      Serial.println();
    }
  }
  Serial.println();
  delay(250);

#else

  // if enabled this section produces a bar graph of the surface quality that can be used to focus the camera
  // also drawn is the average pixel value 0-63 and the shutter speed and the motion dx,dy.

  int val = mousecam_read_reg(ADNS3080_PIXEL_SUM);
  MD md;
  mousecam_read_motion(&md);
  for (int i = 0; i < md.squal / 4; i++)
  {
    Serial.print('*');
  }
  Serial.print(md.squal);
  Serial.print(' ');
  Serial.print(md.squal / 4);
  Serial.print(' ');
  Serial.print((val * 100) / 351);
  Serial.print(' ');
  Serial.print(md.shutter);
  Serial.print(" (");
  Serial.print((int)md.dx);
  Serial.print(',');
  Serial.print((int)md.dy);
  Serial.println(')');

  // Serial.println(md.max_pix);
  delay(100);

  distance_x = md.dx; // convTwosComp(md.dx);
  distance_y = md.dy; // convTwosComp(md.dy);

  total_x1 = total_x1 + distance_x;
  total_y1 = total_y1 + distance_y;

  total_x = total_x1 / 50.8;
  total_y = total_y1 / 50.8;

  // angle = find_angle(total_x-temp_x, total_y-temp_y);
  // Serial.println("Angle might be: " + String(angle));

  Serial.print('\n');

  Serial.println(ADNS3080_PIXELS_X);
  Serial.println("Distance_x = " + String(total_x));

  Serial.println("Distance_y = " + String(total_y));
  Serial.print('\n');

  delay(250);
  temp_x = total_x;
  temp_y = total_y;
  // move_F();
  // delay(2000);
  // move_R();
  // delay(2000);
  // move_B();
  // delay(2000);
  move_L(360);
  move_R(360);
  // delay(2000);
  // for(int i=0; i<=100; i++)
  //   {
  //     robot.rotate(motor1, i, CCW);// turn motor1 with i% speed in CW direction (whatever is i)
  //     robot.rotate(motor2, i, CW);// turn motor2 with i% speed in CCW direction (whatever is i) 
  //     // delay(100);
  //   }
  // robot.rotate(motor1, 25, CCW);// turn motor1 with 25% speed in CCW direction
  // robot.rotate(motor2, 25, CW);// turn motor2 with 25% speed in CW direction
  // delay(2000);
  // robot.brake(1);
  // robot.brake(2);
  // delay(500);
  // for(int i=0; i<=100; i++)
  //   {
  //     robot.rotate(motor1, i, CCW);// turn motor1 with i% speed in CW direction (whatever is i)
  //     robot.rotate(motor2, i, CW);// turn motor2 with i% speed in CCW direction (whatever is i) 
  //     delay(100);
  //   }
  //   delay(2000);
  //   robot.brake(1);
  //   robot.brake(2);
  #endif
}
