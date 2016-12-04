#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <stdio.h>


/**
 * This node sends 1 or 0 if noise is activated to a topic /simulation/noise.
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
  
  ros::init(argc, argv , "noise");
  int m;
  ros::NodeHandle node;
  ros::NodeHandle nh("~");
  nh.getParam("exist_noise", m );
  ros::Publisher noise = node.advertise<std_msgs::Float64>("/simulation/noise", 1000);

  ros::Rate loop_rate(10);

  /**
   * Main code.
   */

  std_msgs::Float64 n;
  n.data=0;
  int count=0;

  ROS_INFO("Noise: 1, Without noise:0 ");
  while (ros::ok()) 
  {
	  n.data= m;
	  noise.publish(n);
	  count++;
	  ros::spinOnce();
	  loop_rate.sleep();
  }
  return 0;
}
