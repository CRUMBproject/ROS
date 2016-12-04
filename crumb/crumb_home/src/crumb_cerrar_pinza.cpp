/**
 * This node makes CRUMB close its gripper.
 * 
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while gazebo is running.
 * 

 */


#include "ros/ros.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"

 

int main(int argc, char **argv)
{
  ros::init(argc, argv , "crumb_close");
  ros::NodeHandle n;

  // Topics of the joints.
  ros::Publisher joint7 = n.advertise<std_msgs::Float64>("/gripper_1_joint/command", 1000);
  ros::Rate loop_rate(10);

  /**
   * Main code.
   */

  std_msgs::Float64 pinza;
  pinza.data = 0;
  int count = 0;
  ROS_INFO("Cargando nueva posicion: cerrar pinza");
  while (ros::ok() && count<=2000)
  {

    joint7.publish(pinza);
    //ROS_INFO("Count: %d\n",count);
    count++;
 
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
