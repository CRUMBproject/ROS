

#include "ros/ros.h"
#include <geometry_msgs/Twist.h>

 /**
 * Tutorial1 for CRUMB
 * With this application we can move the Turtlebot platform.
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while Gazebo and CRUMB are running.
 * 
 */
 
 

int main(int argc, char **argv)
{
  ros::init(argc, argv , "tutorial3_crumb");
  ros::NodeHandle node;
  // Topics of the joints.
	ros::Publisher wheel    = node.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);

  ros::Rate loop_rate(10);

  /**
   * Main code.
   */
 	geometry_msgs::Twist mov;
	mov.linear.x=0.2;
	mov.linear.y=0;
	mov.linear.z=0;
	mov.angular.x=0;
	mov.angular.y=0;
	mov.angular.z=0;
  ROS_INFO("Turtlebot goes forward");
  while (ros::ok())
  {

    wheel.publish(mov);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
