#include <iostream>
#include <math.h>
#include <stdio.h>

float distance_to_alien = 0;
float angle_from_vision = 0;
float current_angle = 0;
bool alien_detected = false;
int target_x = 0;
int target_y = 0;
float total_x_overall = 0;
float total_y_overall = 0;
float total_y = 0;
bool facing_target = false;
bool dest = false;
bool object_noted = false;
float initial_angle = 0;
float width_of_rover = 10.5;
bool interacting = false;

void turn_gyro_angle(float theta){}
void go_forwards(float y){}
void brake_rover(){}
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

void object_avoid(int iteration, float temp_angle){
  if(iteration == 1){
    initial_angle = current_angle;
  }
  temp_angle += 10;
  if((initial_angle+temp_angle)>180){
    temp_angle-=360;
  }
  turn_gyro_angle(initial_angle+temp_angle);
  if(alien_detected && object_noted){
    if(abs(angle_from_vision)>-18 && distance_to_alien<=20){
      iteration+=1;
      object_avoid(iteration, temp_angle);
    }
  }
  float hypotenuse = 20/tan(temp_angle*(M_PI/180));
  go_forwards(hypotenuse);
  turn_gyro_angle(initial_angle-temp_angle);
  go_forwards(hypotenuse);
  turn_gyro_angle(initial_angle);
  total_y -= (2*hypotenuse-40); // this would correct the y value so the rover keeps driving to the correct point?
}

void object_interaction(){ // need to call this within the go_forwards function within an if(interacting = false);
  interacting = true;
  float move_forward_by;
  float at_angle;
  // continue driving to node 
  // if(!alien_detected){ // && !dest){ // don't think we need the second condition as when dest is reached new target should be used to move to
  //   move_forward_by = sqrt(pow((target_x - total_x_overall),2) + pow((target_y - total_y_overall),2));
  //   at_angle = 90 - atan(abs(((target_y - total_y_overall)/(target_x - total_x_overall)))); // if no alien detected, drive forward. 
  //   turn_gyro_angle(at_angle);
  //   delay(50);
  //   go_forwards(move_forward_by);
  // }  
  if(alien_detected && !object_noted){ // && !dest){ // don't think we need the second condition as when dest is reached new target should be used to move to
    // if not at dest and alien in path 
    if(distance_to_alien<30){
      brake_rover();
      float current_angle_saved = current_angle; // the angle at which it sees the object initially.
      delay(100);
      // to get in centre of LOS
      turn_to_object();
      delay(100);
      // return back to original angle then continue on to the original destination
      turn_gyro_angle(current_angle_saved);
      move_forward_by = sqrt(pow((target_x - total_x_overall),2) + pow((target_y - total_y_overall),2));
      at_angle = 90 - atan(abs(((target_y - total_y_overall)/(target_x - total_x_overall)))); 
      turn_gyro_angle(at_angle);
      delay(50);
      go_forwards(move_forward_by);
      object_noted = true;
    }
  }else if(alien_detected && object_noted){
    if(angle_from_vision>-18 && distance_to_alien<=20){
      brake_rover();
      int iteration = 1;
      float temp_angle = 0;
      object_avoid(iteration, temp_angle);
    }
  }else if(!alien_detected && object_noted){
    object_noted = false;
  }
  interacting = false;
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