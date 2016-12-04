#include "ros/ros.h"
#include "kobuki_msgs/Led.h"
#include <stdio.h>


/**
 * This node sends 1 (green) to the topics 
 * /mobile_base/commands/led1 and /mobile_base/commands/led2.
 * 
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while gazebo is running.
 * 

 */
 


int main(int argc, char **argv)
{
  
  ros::init(argc, argv , "led");
  ros::NodeHandle node;
  
  
  ros::Publisher pub1 = node.advertise<kobuki_msgs::Led>("/mobile_base/commands/led1", 1000);
  ros::Publisher pub2 = node.advertise<kobuki_msgs::Led>("/mobile_base/commands/led2", 1000);
  ros::Rate loop_rate(1);

  /**
   * Main code.
   */
  kobuki_msgs::Led color;
  color.value=1;
  ROS_INFO("LED1 and LED2 are GREEN.");
  while (ros::ok()) 
  {
	  pub1.publish(color);
	  pub2.publish(color);
	  ros::spinOnce();
	  loop_rate.sleep();
  }
  return 0;
}
