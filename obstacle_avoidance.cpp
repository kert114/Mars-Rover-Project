#include <iostream>
#include <math.h>
#include <stdio.h>
//T90
//F12.6
//x:y:A:@TD:@TA
void turn_to_object (float angle_from_drive , float angle_from_vision){
  float angle_to_drive;
    if((angle_from_vision <0) &&(angle_from_drive >=0 && angle_from_drive <=180)){
      angle_to_drive = angle_from_drive + angle_from_vision;  // angle from vision is -ve turn left
    }
    else if((angle_from_vision <0) &&(angle_from_drive <=0 && angle_from_drive >=-180)){
      angle_to_drive = angle_from_drive + angle_from_vision;
     if(angle_to_drive <-180){
      angle_to_drive = angle_to_drive + 360;
      }
    }
    else if((angle_from_vision >0) &&(angle_from_drive >=0 && angle_from_drive <=180)){
      angle_to_drive = angle_from_drive + angle_from_vision; // turn right 
      if(angle_to_drive >180){
        angle_to_drive  = angle_to_drive - 360;
      }
    }
    else if((angle_from_vision >0) &&(angle_from_drive <=0 && angle_from_drive >=-180)){
      angle_to_drive = angle_from_drive + angle_from_vision; // angle from drive is -ve 
    }
  turn_gyro_angle(angle_to_drive); // this needs to return the function so that it turns. Then in the main script, call another function that doesn't do this. 
}
// float return_to_initial_angle (float angle_from_drive){
//   return angle_from_drive;
// }
std::string obstacle_avoidance (float distance_to_alien,float angle_from_vision, float angle_from_drive,  bool alien_detected, int xto, int yto, float xd, float yd , bool dest_angle, bool dest_distance, bool has_vision_sent){
  float forward_by;
  std::string command_to_drive;
  float at_angle;
  // continue driving to node 
  if(!alien_detected && !(dest_angle && dest_distance)){
    forward_by = sqrt(pow((xto - xd),2) + pow((yto - yd),2));
    at_angle = 90 - atan(abs(((yto - yd)/(xto - xd)))); // if no alien detected, drive forward. 
    turn_gyro_angle(at_angle);
        if(dest_angle){
          go_forwards(forward_by);
        }
  }  
  if(alien_detected && !(dest_angle && dest_distance)){
    // if not at dest and alien in path 
    if(distance_to_alien<30){
      rover_break();
      float current_angle_saved = angle_from_drive; // the angle at which it sees the object initially.
      delay(100);
      command_to_drive = turn_to_object( angle_from_drive,angle_from_vision);
        // to get in centre of line of sight.t
      delay(100);
      turn_gyro_angle(current_angle_saved);
    // return back to original angle
      if(dest_angle){
        // if you're at the original position then go to destination
        // from current position. 
        forward_by = sqrt(pow((xto - xd),2) + pow((yto - yd),2));
        at_angle = 90 - atan(abs(((yto - yd)/(xto - xd)))); 
        turn_gyro_angle(at_angle);
        if(dest_angle){
          go_forwards(forward_by);
        }
      }
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
  float angle_to_drive;
}
// you set dest coordinate. 
// - but you send distance and angle to that coordinate.