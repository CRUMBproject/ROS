#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include <stdio.h>


/**
 * This node sends /mobile_base/sensors/imu_data to a topic 
 * /mobile_base/sensors/imu_data_raw.
 * 
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while gazebo is running.
 * 

 */
 
ros::Publisher pub;
 
void callback(const sensor_msgs::Imu::ConstPtr& msg)
{
    sensor_msgs::Imu data;
    data.header.seq= msg->header.seq;
    
    if( !pub ) 
    {
		ROS_WARN("Publisher invalid!");
		ROS_INFO("%u",data.header.seq);
	}
	else
	{
		
		pub.publish(msg);
	}
}
 

int main(int argc, char **argv)
{
  
  ros::init(argc, argv , "imu_data_raw");
  ros::NodeHandle node;
  
  
  ros::Subscriber sub =node.subscribe("/mobile_base/sensors/imu_data",10, callback);
  pub = node.advertise<sensor_msgs::Imu>("/mobile_base/sensors/imu_data_raw", 1000);
  ros::Rate loop_rate(1000);

  /**
   * Main code.
   */


  while (ros::ok()) 
  {
	  ros::spinOnce();
	  loop_rate.sleep();
  }
  return 0;
}
