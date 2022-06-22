#include <iostream>
#include <math.h>
#include <stdio.h>
//T90
//F12.6
//x:y:A:@TD:@TA

float distance_to_alien = 0;
float angle_from_vision = 0;
float current_angle = 0;
bool alien_detected = 0;
int target_x = 0;
int target_y = 0;
float total_x_overall = 0;
float total_y_overall = 0;
bool facing_target = 0;
bool dest = 0;

void turn_gyro_angle(float theta){}
void go_forwards(float y){}
void rover_break(){}
void delay(int milliseconds){}

void turn_to_object(){
  float angle_to_drive_to;
  angle_to_drive_to = current_angle + angle_from_vision; // angle from drive is -ve 
  if(angle_to_drive_to <-180){
    angle_to_drive_to += 360;
  }
  if(angle_to_drive_to >180){
    angle_to_drive_to -= 360;
  }
  turn_gyro_angle(angle_to_drive_to); // this needs to return the function so that it turns. Then in the main script, call another function that doesn't do this. 
}

std::string obstacle_avoidance(){
  float move_forward_by;
  std::string command_to_drive;
  float at_angle;
  // continue driving to node 
  if(!alien_detected){ // && !dest){ // don't think we need the second condition as when dest is reached new target should be used to move to
    move_forward_by = sqrt(pow((target_x - total_x_overall),2) + pow((target_y - total_y_overall),2));
    at_angle = 90 - atan(abs(((target_y - total_y_overall)/(target_x - total_x_overall)))); // if no alien detected, drive forward. 
    turn_gyro_angle(at_angle);
    delay(50);
    go_forwards(move_forward_by);
  }  
  if(alien_detected){ // && !dest){ // don't think we need the second condition as when dest is reached new target should be used to move to
    // if not at dest and alien in path 
    if(distance_to_alien<30){
      rover_break();
      float current_angle_saved = current_angle; // the angle at which it sees the object initially.
      delay(100);
      turn_to_object();
      // to get in centre of LOS
      delay(100);
      turn_gyro_angle(current_angle_saved);
      // return back to original angle then continue on to the original destination
      move_forward_by = sqrt(pow((target_x - total_x_overall),2) + pow((target_y - total_y_overall),2));
      at_angle = 90 - atan(abs(((target_y - total_y_overall)/(target_x - total_x_overall)))); 
      turn_gyro_angle(at_angle);
      delay(50);
      go_forwards(move_forward_by);
    }
  }
}
  // change this to recurrsion to go for more than once? 
// or put this in the main while loop - until 6 objects are detected. 
int main(){
  float distance_to_alien;
  std::cin>> distance_to_alien; 
  float angle_from_vision;
  std::cin>> angle_from_vision;
  float  perp_distance = tan(angle_from_vision)*distance_to_alien;
  float angle_to_drive_to;
}
// you set dest coordinate. 
// - but you send distance and angle to that coordinate.