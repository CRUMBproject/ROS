#include "ros/ros.h"
#include "kobuki_msgs/WheelDropEvent.h"
#include <geometry_msgs/Vector3.h>
#include <stdio.h>


/**
 * This node receives the pose and sends 1 to 
 * /mobile_base/events/wheel_drop if z is not 0.
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
 
void cb(const geometry_msgs::Vector3::ConstPtr& _msg)
{
	geometry_msgs::Vector3 pose;
	kobuki_msgs::WheelDropEvent m;
	
	pose.z=_msg->z;
	if(pose.z >1)
	{
		m.state=1;
	}
	else
	{
		m.state=0;
	}
	
    pub.publish(m);
}
 

int main(int argc, char **argv)
{
  
  ros::init(argc, argv , "wheel_drop");
  ros::NodeHandle node;
  
  
  ros::Subscriber sub =node.subscribe("/simulation/pose",10, cb);
  pub = node.advertise<kobuki_msgs::WheelDropEvent>("/mobile_base/events/wheel_drop", 1000);
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
