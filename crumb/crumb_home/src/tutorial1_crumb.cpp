

#include "ros/ros.h"
#include "std_msgs/Float64.h"

 /**
 * Tutorial1 for CRUMB
 * With this application we can publish in a topic.
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
  ros::init(argc, argv , "tutorial1_crumb");
  ros::NodeHandle n;
  // Topics of the joints.
  ros::Publisher joint2 = n.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);

  ros::Rate loop_rate(10);

  /**
   * Main code.
   */

  std_msgs::Float64 degrees;
  degrees.data = 1;
  int count = 0;
  ROS_INFO("Joint 2");
  while (ros::ok() && count<=100)
  {

    joint2.publish(degrees); /*[-pi/2, pi/2]  */
    count++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
