#include <Arduino.h>
#include "SPI.h"
#include <Robojax_L298N_DC_motor.h>
#include <Wire.h>
#include <wifi.h>
#include <MFRC522.h>
#include <math.h>
#include <iostream>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <stdint.h>
#include <string.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <stdio.h>

///////////////wifi pins//////////
#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5
// was 2
#define RST_PIN 4
#define SS_PIN 2

#define CHA 0
#define ENA 33 // 4  // 23 //21 // 19 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 26 // 22 // 18
#define IN2 25 // 14 // 5
// motor 2 settings
#define IN3 17
#define IN4 16
#define ENB 27 // 4 // this pin must be PWM enabled pin if Arduino board is used
#define CHB 1
const int CCW = 2; // do not change
const int CW = 1;  // do not change
#define motor1 1   // do not change
#define motor2 2   // 2   // do not change
// for two motors without debug information // Watch video instruciton for this line: https://youtu.be/2JTMqURJTwg
Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB);
// for two motors with debug information
// Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB, true);

#define PIN_SS 5
#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_SCK 18

#define PIN_MOUSECAM_RESET 35
#define PIN_MOUSECAM_CS 5

#define ADNS3080_PIXELS_X 30
#define ADNS3080_PIXELS_Y 30

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
MFRC522 mfrc522(SS_PIN, RST_PIN); //// RANDOM IN2 AND IN3 FOR TESTING

const char *serverName = "http://192.168.158.188:80/post-esp-data.php"; // replace middle with ipv4 of laptop "http:///post-esp-data.php"

String apiKeyValue = "tPmAT5Ab3j7F9a";
String object = "Rover";

Adafruit_MPU6050 mpu;

const char *ssid = "Kert12345";
const char *password = "1234567891";
// const char *ssid = "AngusiPhone";
// const char *password = "AngusJames";
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
    // delay(1000);
  }
  Serial.println(ssid);
  Serial.println();
  Serial.println("local ip is:");
  Serial.println(WiFi.localIP());
}

///
// float pi = 3.14159265359;
float angle = 0;
float current_angle = 0;
float initial_angle = 0;
float prev_angle = 0;
const float r = 12.6; // the distance from flow sensor to the centre of the axis the rover turns around

float dist, target_angle = 0;

float delta_y = 0;

float total_x = 0;
float total_y = 0;
float total_x_overall = 0;
float total_y_overall = 0;

float initial_total_y = 0;

bool dest = false;
bool new_dest = true;
bool facing_target = false;
bool turning = false;

int counter = 0;

float total_x1 = 0;
float total_y1 = 0;
float total_x1_overall = 0;
float total_y1_overall = 0;

float x = 0;
float y = 0;

int m1 = 36;
int m2 = 34;

float prev_dx;
float prev_dy;

float correction = 30;

float a = 0;
float b = 0;

float gyro_rotation = 0;
float angle_gyro = 0;
float temp_gyro_angle = 0;

float currenttimedelay = 0;
float previoustimedelay = 0;

float distance_x = 0;
float distance_y = 0;
float distance_x_overall = 0;
float distance_y_overall = 0;

float temp_x = 0;
float temp_y = 0;

volatile byte movementflag = 0;
volatile int xydat[2];
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

// float find_angle(int x, int y)
// {
//   int r = 70;
//   float angle;
//   float d = x ^ 2 + y ^ 2;
//   angle = abs(((2 * (r ^ 2)) - d) / (2 * (r ^ 2)));
//   angle = acos(angle);
//   return angle;
// }
/////////////////////////////// ROVER MOVE FUNCTIONS///////////////////////////////

// aim is to get moving forwards working in a straight line with an autocorrection function so we can move in a straight line fine.
// Once this works, moving backwards short distances in a straight line should be fine as well - will be more variance as the
// sensor is out front for it.
// Finally by using the find current angle changed function - it will be possible to turn to a certain angle allowing for all
// direction driving. There is no need to turn on the move as that adds an extra radius that will be unneccessarily hard to calculate.

void move_F(int x = 50, int m1 = 25, int m2 = 25)
{
  robot.rotate(motor1, m1, CCW); // turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, m2, CW);  // turn motor2 with 25% speed in CW direction
  delay(x);
}
void move_B(int x = 50, int m1 = 25, int m2 = 25)
{
  robot.rotate(motor1, m1, CW);  // turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, m2, CCW); // turn motor2 with 25% speed in CW direction
  delay(x);
}
void turn_L(int x = 10, int m1 = 25, int m2 = 25)
{
  robot.rotate(motor1, m1, CW); // turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, m2, CW); // turn motor2 with 25% speed in CW direction
  delay(x);
  // robot.brake(1);
  // robot.brake(2);
}
void turn_R(int x = 10, int m1 = 25, int m2 = 25)
{
  robot.rotate(motor1, m1, CCW); // turn motor1 with 25% speed in CCW direction
  robot.rotate(motor2, m2, CCW); // turn motor2 with 25% speed in CW direction
  delay(x);
  // robot.brake(1);
  // robot.brake(2);
}
void brake_rover()
{
  robot.brake(1);
  robot.brake(2);
  // delay(1000);
}

float angle_facing(float total_x)
{
  float delta_angle = (total_x / r);
  delta_angle = atan2(sin(delta_angle), cos(delta_angle)) * (180 / M_PI);
  Serial.print("Angle: ");
  Serial.println(delta_angle, 4);
  return delta_angle;
}

void turn_to(float target_angle_temp)
{
  turning = true;
  facing_target=false;
  float temp_delta_angle = 0;
  int delay = 10;
  int m1, m2 = 40;
  temp_delta_angle = current_angle - target_angle_temp;
  Serial.println('\n');
  Serial.print("temp_delta_angle = ");
  Serial.println(temp_delta_angle);
  Serial.println('\n');
  if (abs(temp_delta_angle) < 10)
  {
    // delay = 4;
    m1 = m2 = 30;
  }
  if(abs(temp_delta_angle) < 5){
    m1=m2=25;
  }
  m1 += 2;
  if (!(abs(temp_delta_angle) < 2)) // && facing_target))
  {
    if ((temp_delta_angle > 0 && temp_delta_angle < 180) || temp_delta_angle < -180)
    {
      turn_L(delay, m1, m2);
    }
    else if ((temp_delta_angle < 0 && temp_delta_angle > -180) || temp_delta_angle > 180)
    {
      turn_R(delay, m1, m2);
    }
  }
  else
  {
    brake_rover();
    facing_target = true;
    turning = false;
  }
}

void turn_gyro_angle(float target_angle)
{
  turning = true;
  facing_target=false;
  float temp_delta_angle = 0;
  int delay = 10;
  int m1, m2 = 40;
  temp_delta_angle = temp_gyro_angle - target_angle;
  if (abs(temp_delta_angle) < 10)
  {
    // delay = 4;
    m1 = m2 = 30;
  }
  if(abs(temp_delta_angle) < 5){
    m1=m2=25;
  }
  m1 += 2;
  if (!(abs(temp_delta_angle) < 2)) // && facing_target))
  {
    if ((temp_delta_angle > 0 && temp_delta_angle < 180) || temp_delta_angle < -180)
    {
      turn_L(delay, m1, m2);
    }
    else if ((temp_delta_angle < 0 && temp_delta_angle > -180) || temp_delta_angle > 180)
    {
      turn_R(delay, m1, m2);
    }
  }
  else
  {
    brake_rover();
    facing_target = true;
    turning = false;
  }
}
void go_forwards(float y)
{ 
  if (new_dest == true)
  {
    new_dest = false;
    dest = false;
    target_angle = current_angle;
    initial_total_y = total_y;
    dist = y + initial_total_y;
  }
  float delta_y = dist - total_y; // difference in y left to be moved
  // aim of this section is to read current position, then find target position's relative distance and angle, then move to that location
  // angle to move is arctan(delta_x/delta_y) and distance is sqrt(x^2 + y^2)

  Serial.print("Angle: ");
  Serial.println(angle, 3);
  Serial.print("Target angle: ");
  Serial.println(target_angle, 3);
  Serial.print("Distance: ");
  Serial.println(dist, 3);
  Serial.print("Delta_y: ");
  Serial.println(delta_y, 3);

  if (!(delta_y < 0.3 && delta_y > -0.3))
  {
    if (delta_y > 0.1)
    {
      if (abs(current_angle) < (initial_angle + 0.5) && delta_y < 5 && abs(prev_angle) < (initial_angle + 0.5)){
        m1 = 24;
        m2 = 24;
      }else if (abs(current_angle) < (initial_angle + 0.5) && delta_y > 5 && delta_y < 10 && abs(prev_angle) < (initial_angle + 0.5)){
        m1 = 30;
        m2 = 30;
      }else if (abs(current_angle) < (initial_angle + 0.5) && abs(prev_angle) < (initial_angle + 0.5) && delta_y > 10){
        m1 = 40;
        m2 = 40;
      }

      if (angle_gyro > (initial_angle + 0.5)){
        m1 -= 2;
        m2 += 2;
      }
      else if (angle_gyro < (initial_angle - 0.5)){
        m1 += 2;
        m2 -= 2;
      }
      move_F(50, m1, m2);
    }
    else if (delta_y < -0.1)
    {
      if (abs(current_angle) < (initial_angle + 0.5) && delta_y < 5 && abs(prev_angle) < (initial_angle + 0.5))
      {
        m1 = 24;
        m2 = 24;
      }
      else if (abs(current_angle) < (initial_angle + 0.5) && delta_y > 5 && delta_y < 10 && abs(prev_angle) < (initial_angle + 0.5))
      {
        m1 = 30;
        m2 = 30;
      }
      else if (abs(current_angle) < (initial_angle + 0.5) && abs(prev_angle) < (initial_angle + 0.5) && delta_y > 10)
      {
        m1 = 40;
        m2 = 40;
      }

      if (current_angle > (initial_angle + 0.5) && current_angle > prev_angle + 0.3)
      {
        m1 -= 3;
        m2 += 3;
      }
      else if (current_angle < (initial_angle - 0.5) && current_angle < prev_angle + 0.3)
      {
        m1 += 3;
        m2 -= 3;
      }

      move_B(50, m1, m2);
    }
    else
    {
      brake_rover();
    }
  }
  else{
    brake_rover();
  }
  // }
  if (delta_y < 0.3 && delta_y > -0.3)
  {
    counter += 1;
    if (counter > 4)
      dest = true;
  }
}
TaskHandle_t Task1;
TaskHandle_t Task2;

// these pins may be different on different boards

// void Task1code(void *pvParameters) {}
// void Task2code(void *pvParameters) {}

void Task1code(void *pvParameters)
{
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    dest = true;
    /////////////////////////CONTROL THE ROVER USING 123456789
    int i = 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    // while (Serial.available() == 0)
    // {
    // } // if it breaks, do >= 0 in conditions as per Hepple
    if (Serial.available())
    {
      i = Serial.parseInt();
    }
    switch (i)
    {
    case 1:
      // move straight for 3 sec
      move_F(1000);
      brake_rover();
      break;

    case 2:
      // rotate left for 3 sec
      turn_R(1000);
      brake_rover();
      break;

    case 3:
      // rotate right for 3 sec
      turn_L(1000);
      brake_rover();
      break;

    case 4:
      // move back for 3 sec
      move_B(1000);
      brake_rover();
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
        turn_R(3);
        i = Serial.parseInt();
      }
      break;

    case 7:
      while (i == 7)
      {
        // continuous case right
        turn_L(3);
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
      brake_rover();
      break;
    }
  }
}
//////////////////////////////////// core 2 for sensor readings
void Task2code(void *pvParameters)
{
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float val = mousecam_read_reg(ADNS3080_PIXEL_SUM);
    MD md;
    mousecam_read_motion(&md);
    if (md.squal <= 20)
    { // fixing random increases in x when the rover sees a low quality image (***** or lower)
      md.dx = 0;
      md.dy = 0;
    }
    if (md.dx == 255)
    {
      md.dx = 0;
    }
    // if (md.dy == 255)
    // {
    //   md.dy = 0;
    // }
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
    // delay(100);
    prev_angle = current_angle;
    current_angle = angle_facing(total_x); // still need to find the right conversion from md values to cm or mm
    current_angle = temp_gyro_angle;
    // normal values are relative to the rover, overall values are relative to the overall y axis
    distance_x = /*md.dx; //*/ convTwosComp(md.dx);
    distance_y = /*md.dy; //*/ convTwosComp(md.dy);
    if(!turning){
        distance_x_overall = convTwosComp(md.dy)*sin(temp_gyro_angle*(M_PI/180));
        distance_y_overall = convTwosComp(md.dy)*cos(temp_gyro_angle*(M_PI/180));
    } 
    // distance_x_overall = /*md.dx; //*/ convTwosComp(md.dy) * sin(current_angle * (M_PI / 180));
    // distance_y_overall = /*md.dy; //*/ convTwosComp(md.dy) * cos(current_angle * (M_PI / 180));

    total_x1 = total_x1 + distance_x;
    total_y1 = total_y1 + distance_y;
    total_x1_overall = total_x1_overall + distance_x_overall;
    total_y1_overall = total_y1_overall + distance_y_overall;

    total_x = total_x1 / correction;
    total_y = total_y1 / correction;
    total_x_overall = total_x1_overall / correction;
    total_y_overall = total_y1_overall / correction;

    currenttimedelay = millis();
    Serial.print("time for cycle: "), Serial.println((currenttimedelay - previoustimedelay) / 1000, 5);

    if (g.gyro.z * (180 / M_PI) > 2 || g.gyro.z * (180 / M_PI) < -2)
    {
      temp_gyro_angle += ((g.gyro.z * (180 / M_PI)) * ((currenttimedelay - previoustimedelay) / 1000));
    }
    if(temp_gyro_angle>180){
      temp_gyro_angle-=360;
    }else if(temp_gyro_angle<-180){
      temp_gyro_angle+=360;
    }
    Serial.print("gyroangle: "), Serial.println(temp_gyro_angle, 5);
    previoustimedelay = currenttimedelay;
    delay(100);
    temp_x = total_x;
    temp_y = total_y;
    prev_dx = md.dx / correction;
    prev_dy = md.dy / correction;
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  robot.begin();
  pinMode(PIN_SS, OUTPUT);
  // Serial.println("SS");
  pinMode(PIN_MISO, INPUT);
  // Serial.println("MISO");
  pinMode(PIN_MOSI, OUTPUT);
  // Serial.println("MOSI");
  pinMode(PIN_SCK, OUTPUT);
  // Serial.println("SCK");

  SPI.begin();
  // Serial.println("SPI");
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  // Serial.println("SPI CLOCK DIV");
  SPI.setDataMode(SPI_MODE3);
  // Serial.println("SPI MODE");
  SPI.setBitOrder(MSBFIRST);
  // Serial.println("MSB");

  if (mousecam_init() == -1)
  {
    Serial.println("Mouse cam failed to init");
    while (1)
      ;
  }

  Serial.println("INIT");
  /////////////WIFI STUFF IN SETUP///////////

  // Serial.begin(115200);

  SPI.begin();

  mfrc522.PCD_Init();

  // Try to initialize the mpu6050
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange())
  {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange())
  {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth())
  {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  ////wifi stufff
  mfrc522.PCD_Init();

  // initWiFi();
  // Serial.print("RRSI: ");
  // Serial.println(WiFi.RSSI());

  dest = false;

  /////////////////////////////// dual core stuff////////////////////////
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  delay(500);

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task2code, /* Task function. */
      "Task2",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task2,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  delay(500);
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
  /*
    dest = true;
    /////////////////////////CONTROL THE ROVER USING 123456789
    int i = 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    // while (Serial.available() == 0)
    // {
    // } // if it breaks, do >= 0 in conditions as per Hepple
    if (Serial.available())
    {
      i = Serial.parseInt();
    }
    switch (i)
    {
    case 1:
      // move straight for 3 sec
      move_F(1000);
      brake_rover();
      break;

    case 2:
      // rotate left for 3 sec
      turn_R(1000);
      brake_rover();
      break;

    case 3:
      // rotate right for 3 sec
      turn_L(1000);
      brake_rover();
      break;

    case 4:
      // move back for 3 sec
      move_B(1000);
      brake_rover();
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
        turn_R(3);
        i = Serial.parseInt();
      }
      break;

    case 7:
      while (i == 7)
      {
        // continuous case right
        turn_L(3);
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
      brake_rover();
      break;

      // case 0:
      //   while (i == 0)
      //   {

      //     // adjust to the right
      //     robot.rotate(motor1, 80, CCW); // run motor1 at 60% speed in CCW direction
      //     robot.rotate(motor2, 75, CW);  // run motor1 at 60% speed in CW direction
      //     delay(10);
      //     i = Serial.parseInt();
      //   }
      //   break;

    default:
      // brake
      // brake_rover();
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

    float val = mousecam_read_reg(ADNS3080_PIXEL_SUM);
    MD md;
    mousecam_read_motion(&md);
    if (md.squal <= 20)
    { // fixing random increases in x when the rover sees a low quality image (***** or lower)
      md.dx = 0;
      md.dy = 0;
    }
    if (md.dx == 255)
    {
      md.dx = 0;
    }
    // if (md.dy == 255)
    // {
    //   md.dy = 0;
    // }
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
    // delay(100);
    prev_angle = current_angle;
    current_angle = angle_facing(total_x); // still need to find the right conversion from md values to cm or mm
    // normal values are relative to the rover, overall values are relative to the overall y axis
    distance_x = /*md.dx; //*/
  /* convTwosComp(md.dx);
   distance_y = convTwosComp(md.dy);
   distance_x_overall = convTwosComp(md.dy) * sin(current_angle * (M_PI / 180));
   distance_y_overall = convTwosComp(md.dy) * cos(current_angle * (M_PI / 180));

   total_x1 = total_x1 + distance_x;
   total_y1 = total_y1 + distance_y;
   total_x1_overall = total_x1_overall + distance_x_overall;
   total_y1_overall = total_y1_overall + distance_y_overall;

   total_x = total_x1 / correction;
   total_y = total_y1 / correction;
   total_x_overall = total_x1_overall / correction;
   total_y_overall = total_y1_overall / correction;

   // Serial.print('\n');
   // Serial.print("Current angle: ");
   // Serial.println(current_angle, 5);
   // Serial.print('\n');
   currenttimedelay = millis();
   Serial.print("time for cycle: "), Serial.println((currenttimedelay - previoustimedelay) / 1000, 5);

   if (g.gyro.z * (180 / M_PI) > 2 || g.gyro.z * (180 / M_PI) < -2)
   {
     temp_gyro_angle += ((g.gyro.z * (180 / M_PI)) * ((currenttimedelay - previoustimedelay) / 1000));
   }
   Serial.print("gyroangle: "), Serial.println(temp_gyro_angle, 5);
   previoustimedelay = currenttimedelay;

   // turn_gyro_angle(90);

   turn_to(90);
   //  Serial.print("gyroangle: "), Serial.print(angle_gyro);
   //   rotation changed in rad/s
   //   if (g > -2 && gyro_rotation < 2)
   //  {
   //     gyro_rotation = 0;
   //   }

   // Serial.println(ADNS3080_PIXELS_X);
   // Serial.print("Relative distance_x = ");
   // Serial.print(total_x, 5);
   // Serial.print("    Total distance_x = ");
   // Serial.println(total_x_overall, 5);

   // Serial.print("Relative distance_y = ");
   // Serial.print(total_y, 5);
   // Serial.print("    Total distance_y = ");
   // Serial.println(total_y_overall, 5);
   // Serial.print('\n');
   // Serial.println(gyro_rotation, 5);
   // Serial.println("");
   // // if (!dest)
   // {
   //   go_forwards(30);
   //   Serial.println("Should be going forwards...");
   // }
   // else if (dest) // && stop)
   // {
   //   turn_to(90);
   //   Serial.println("Should be turning now...");
   // }

   delay(100);
   temp_x = total_x;
   temp_y = total_y;
   prev_dx = md.dx / correction;
   prev_dy = md.dy / correction;
 #endif

 #if 0
   //////////////////////////////////////wifi stuff loop//////////////////
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
   if (WiFi.status() == WL_CONNECTED)
   {
     WiFiClient client;
     HTTPClient http;
     if (client.available() > 0)
     {
       Serial.println("DEBUG CLIENT IS AVAILABLE");
     }
     else
     {
       Serial.println("DOOM");
     }
     // Your Domain name with URL path or IP address with path
     http.begin(client, serverName);
     // Specify content-type header
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
     // Prepare your HTTP POST request data
     String httpRequestData = "api_key=" + apiKeyValue + "&object=" + object + "&xvalue=" + String(11) + "&yvalue=" + String(2);
     Serial.print("httpRequestData: ");
     Serial.println(httpRequestData);
     // You can comment the httpRequestData variable above
     // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
     // String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";
     // Send HTTP POST request
     int httpResponseCode = http.POST(httpRequestData);
     if (httpResponseCode > 0)
     {
       Serial.print("HTTP Response code: ");
       Serial.println(httpResponseCode);
     }
     if (httpResponseCode < 0)
     {
       Serial.print("Error code: ");
       Serial.println(httpResponseCode);
     }
     // Free resources
     http.end();
   }
   else
   {
     Serial.println("WiFi Disconnected");
   }
 #endif
   // Send an HTTP POST request every 30 seconds
   // delay();
   */
}