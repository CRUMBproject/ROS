#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include <stdio.h>


/**
 * This node sends 1 or 0 if noise is activated to a topic /Noise.
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while the widowx arm software by Robotnik is running.
 * 

 */
 
 
 

int main(int argc, char **argv)
{
  
  ros::init(argc, argv , "reset_odom");
  ros::NodeHandle node;
  ros::Publisher rodom = node.advertise<std_msgs::Empty>("/mobile_base/commands/reset_odometry", 1000);

  ros::Rate loop_rate(10);
  
  /**
   * Main code.
   */

  std_msgs::Empty n;
  
  rodom.publish(n);
  ROS_INFO("Reset Odometry! ");

  ros::spinOnce();
  loop_rate.sleep();
  return 0;
}
