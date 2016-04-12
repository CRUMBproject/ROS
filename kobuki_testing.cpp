
/**
 * Basic access to the Turtlebot 2 robot.
 * Copyright (C) 2015 Ana Cruz-Martín (anacruzmartin@gmail.com)
 *
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 *
 * Based on the information and examples from this webs:
 *
 *   · http://wiki.ros.org/Robots/TurtleBot
 *   · https://github.com/turtlebot/turtlebot/blob/indigo/turtlebot_teleop/src/turtlebot_joy.cpp
 *   · http://answers.ros.org/question/47500/turtlebot-ros-moving-using-twist/
 *   · http://learn.turtlebot.com/
 *   · http://wiki.ros.org/kobuki/Tutorials/Examine%20Kobuki
 *   · https://github.com/yujinrobot/kobuki_msgs/blob/indigo/msg/SensorState.msg
 *   · http://docs.ros.org/jade/api/sensor_msgs/html/msg/Imu.html
 *
 */

#include "ros/ros.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/UInt32.h"
#include "std_msgs/Header.h"

#include <geometry_msgs/Twist.h>
#include <kobuki_msgs/Led.h>
#include <kobuki_msgs/SensorState.h>
#include <sensor_msgs/Imu.h>

#include <iostream>	
#include <fstream>

using namespace std;


// Global variables where the sensory information will be temporary stored
std_msgs::UInt32 header_time_stamp_secs[10000];
std_msgs::UInt32 header_time_stamp_nsecs[10000];
std_msgs::UInt16 sensors_time_stamp[10000];
std_msgs::UInt16 sensors_left_encoder[10000];
std_msgs::UInt16 sensors_right_encoder[10000];
int sensors_index = 0;


//  Callbacks for topics related to sensors

void storeSensorsCallback(const kobuki_msgs::SensorState::ConstPtr& sensormsgs)
{
   header_time_stamp_secs[sensors_index].data = (sensormsgs -> header).stamp.sec;
   header_time_stamp_nsecs[sensors_index].data = (sensormsgs -> header).stamp.nsec;
   sensors_time_stamp[sensors_index].data = sensormsgs->time_stamp;
   sensors_left_encoder[sensors_index].data = sensormsgs->left_encoder;
   sensors_right_encoder[sensors_index].data = sensormsgs->right_encoder;
   sensors_index += 1;
}

void showSensorsCallback(const kobuki_msgs::SensorState::ConstPtr& sensorsmsg)
{
   printf("Kobuki sensors data\n");
   printf("\tCore Data\n");  
   printf("\t\tTime stamp: %d\n", sensorsmsg->time_stamp);
   printf("\t\tBump: %d\n", sensorsmsg->bumper);
   printf("\t\tWheel drop: %d\n", sensorsmsg->wheel_drop);
   printf("\t\tCliff: %d\n", sensorsmsg->cliff);
   printf("\t\tLeft encoder: %d\n", sensorsmsg->left_encoder);
   printf("\t\tRight encoder: %d\n", sensorsmsg->right_encoder);
   printf("\t\tLeft pwm: %d\n", sensorsmsg->left_pwm);
   printf("\t\tRight pwm: %d\n", sensorsmsg->right_pwm);
   printf("\t\tButtons: %d\n", sensorsmsg->buttons);
   printf("\t\tCharger: %d\n", sensorsmsg->charger);
   printf("\t\tBattery: %d\n", sensorsmsg->battery);
   printf("\tCliff Data\n");  
   //printf("\t\tBottom: %d\n", sensorsmsg->bottom);
   printf("\tCurrent Data\n");  
   //printf("\t\tCurrent: %d\n", sensorsmsg->current);
   printf("\t\tOver current: %d\n", sensorsmsg->over_current);
   printf("\tInput Data\n");  
   printf("\t\tDigital input: %d\n", sensorsmsg->digital_input);
   //printf("\t\tAnalog input: %d\n", sensorsmsg->analog_input);
}

void imuCallback(const sensor_msgs::Imu::ConstPtr& imusmsg)
{
   printf("Imu data\n");
   printf("\tOrientation Data\n");  
   printf("\t\tCovariance x: %e %e %e\n",imusmsg->orientation_covariance[0],imusmsg->orientation_covariance[1],imusmsg->orientation_covariance[2]);
   printf("\t\tCovariance y: %e %e %e\n",imusmsg->orientation_covariance[3],imusmsg->orientation_covariance[4],imusmsg->orientation_covariance[5]);
   printf("\t\tCovariance z: %e %e %e\n",imusmsg->orientation_covariance[6],imusmsg->orientation_covariance[7],imusmsg->orientation_covariance[8]);
   printf("\tAngular velocity Data\n");  
   printf("\t\tCovariance x: %e %e %e\n",imusmsg->angular_velocity_covariance[0],imusmsg->angular_velocity_covariance[1],imusmsg->angular_velocity_covariance[2]);
   printf("\t\tCovariance y: %e %e %e\n",imusmsg->angular_velocity_covariance[3],imusmsg->angular_velocity_covariance[4],imusmsg->angular_velocity_covariance[5]);
   printf("\t\tCovariance z: %e %e %e\n",imusmsg->angular_velocity_covariance[6],imusmsg->angular_velocity_covariance[7],imusmsg->angular_velocity_covariance[8]);
  printf("\tLinear Acceleration Data\n");  
   printf("\t\tCovariance x: %e %e %e\n",imusmsg->linear_acceleration_covariance[0],imusmsg->linear_acceleration_covariance[1],imusmsg->linear_acceleration_covariance[2]);
   printf("\t\tCovariance y: %e %e %e\n",imusmsg->linear_acceleration_covariance[3],imusmsg->linear_acceleration_covariance[4],imusmsg->linear_acceleration_covariance[5]);
   printf("\t\tCovariance z: %e %e %e\n",imusmsg->linear_acceleration_covariance[6],imusmsg->linear_acceleration_covariance[7],imusmsg->linear_acceleration_covariance[8]);
}



// Auxiliary functions

/**
 * Stores the sensory data in a file.
 * Data obtained from different runnings are appened to the end of the file.
**/

void storeSensoryData(void)
{
  // Files where data from subscription topics are stored
  ofstream dataFile;
  dataFile.open("/home/turtlebot/Documentos/datafile.txt",std::ios_base::app);

  if (dataFile.is_open())
  {
     printf("Sensory data file opened OK\n");
     for(int i = 0; i < sensors_index; i++)
     {
        dataFile << header_time_stamp_secs[i].data << " " << header_time_stamp_nsecs[i].data << " "; 
        dataFile << sensors_time_stamp[i].data << " " << sensors_left_encoder[i].data << " " << sensors_right_encoder[i].data << "\n";
     }
  }
  else printf("Error: sensory data file could not be opened\n");

  dataFile.close();
}



// Main function. Comment/uncomment elements according to the tests to be deployed

int main(int argc, char **argv)
{
  ros::init(argc, argv , "kobuki_testing");
  ros::NodeHandle n;
  
  // Publishers
  ros::Publisher mobile_base_led2= n.advertise<kobuki_msgs::Led>("/mobile_base/commands/led2", 1000);
  ros::Publisher mobile_base_vel = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);
  ros::Rate loop_rate(10);

  // Subscribers
  ros::Subscriber storeSensors = n.subscribe("/mobile_base/sensors/core", 1000, storeSensorsCallback);
  //ros::Subscriber showSensors = n.subscribe("/mobile_base/sensors/core", 1000, showSensorsCallback);
  //ros::Subscriber imu = n.subscribe("/mobile_base/sensors/imu_data", 1000, imuCallback);

  // Messages for testing different mobile base elements
  geometry_msgs::Twist newTwist; 
  kobuki_msgs::Led led;

  newTwist.linear.x = 0.1;
  newTwist.linear.y = 0.0;
  newTwist.linear.z = 0.0;
  newTwist.angular.x = 0.0;
  newTwist.angular.y = 0.0;
  newTwist.angular.z = 0.0;
  int count = 0;

  while (ros::ok() && count<=300)
  {
    // Test: led values
    //led.value = count % 4;
    //mobile_base_led2.publish(led);

    // Test: mobile base movement
    mobile_base_vel.publish(newTwist);
  
    if (count == 299)
    {
       newTwist.linear.x = 0.0;
    }

    count++;
    printf("Count: %d\n",count);
    ros::spinOnce();
    loop_rate.sleep();
  }
  storeSensoryData();
  return 0;
}
