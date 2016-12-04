#include "ros/ros.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <stdio.h>



/**
 * Application 2 for CRUMB
 * With this application CRUMB can grasp objects with its gripper.
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
  
  ros::init(argc, argv , "crumb_app2_teleop");
  ros::NodeHandle n;
    
  ros::Publisher joint1 = n.advertise<std_msgs::Float64>("/arm_1_joint/command", 1000);
  ros::Publisher joint2 = n.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);
  ros::Publisher joint3 = n.advertise<std_msgs::Float64>("/arm_3_joint/command", 1000);
  ros::Publisher joint4 = n.advertise<std_msgs::Float64>("/arm_4_joint/command", 1000);
  ros::Publisher joint5 = n.advertise<std_msgs::Float64>("/arm_5_joint/command", 1000);
  ros::Publisher joint7 = n.advertise<std_msgs::Float64>("/gripper_1_joint/command", 1000);
  ros::Rate loop_rate(10);

  /**
   * Main code.
   */

  std_msgs::Float64 degrees1;
  std_msgs::Float64 degrees2;
  std_msgs::Float64 degrees3;
  std_msgs::Float64 degrees4;
  std_msgs::Float64 degrees5;
  std_msgs::Float64 gripper;
	int count=0;
  //ROS_INFO("Grasping objects:\n");  
    gripper.data = 1; //initially, the gripper is open.
		degrees1.data = 0;
    degrees2.data = 0;
    degrees3.data = 0;
    degrees4.data = 0;
    degrees5.data = 0;
  
  while (ros::ok() && count<=2000) 
  {


		ROS_INFO("Crumb goes to home position and gripper opens\n");
    ros::Duration(3).sleep();
		joint1.publish(degrees1);
		joint2.publish(degrees2);
		joint3.publish(degrees3);
		joint4.publish(degrees4);
		joint5.publish(degrees5);
		joint7.publish(gripper);
		ros::Duration(3).sleep();	
		
		ROS_INFO("Crumb goes to object 1\n");
		degrees1.data = 1.57;
		joint1.publish(degrees1);
		ros::Duration(3).sleep();
		degrees2.data=0.4;
		degrees3.data=0.4;
		joint2.publish(degrees2);		
		joint3.publish(degrees3);
		ros::Duration(3).sleep();
		degrees4.data=-1.57;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
		gripper.data=0;
		joint7.publish(gripper);
		ros::Duration(9).sleep();
		
		ROS_INFO("Object 1 grasped\n");
		ROS_INFO("Moving object 1 to position 3\n");
		

		degrees2.data=0;
		degrees3.data=0;
		joint2.publish(degrees2);		
		joint3.publish(degrees3);
		ros::Duration(3).sleep();
		degrees4.data=0;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
		degrees1.data = 0;
		joint1.publish(degrees1);
		ros::Duration(3).sleep();
		degrees2.data=0.4;
		degrees3.data=0.4;
		joint2.publish(degrees2);		
		joint3.publish(degrees3);
		ros::Duration(3).sleep();	
		degrees4.data=-1.57;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
			
		gripper.data=1;
		joint7.publish(gripper);
		ros::Duration(8).sleep();
		ROS_INFO("Object 1 in position 3\n");
		
		gripper.data = 1; //initially, the gripper is open.
		degrees1.data = 0;
    degrees2.data = 0;
    degrees3.data = 0;
    degrees4.data = 0;
    degrees5.data = 0;
		
		ROS_INFO("Crumb goes to home position again\n");
    ros::Duration(3).sleep();
		joint1.publish(degrees1);
		joint2.publish(degrees2);
		joint3.publish(degrees3);
		joint4.publish(degrees4);
		joint5.publish(degrees5);
		joint7.publish(gripper);
		ros::Duration(3).sleep();	
		
		ROS_INFO("Crumb goes to object 2\n");
		degrees1.data = 0.8;
		joint1.publish(degrees1);
		ros::Duration(3).sleep();
		degrees2.data=0.4;
		degrees3.data=0.4;
		joint2.publish(degrees2);		
		joint3.publish(degrees3);
		ros::Duration(3).sleep();
		degrees4.data=-1.57;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
		gripper.data=0;
		joint7.publish(gripper);
		ros::Duration(9).sleep();
		
		ROS_INFO("Object 2 grasped\n");
		ROS_INFO("Moving object 2 to position 1\n");
		

		degrees2.data=0;
		degrees3.data=0;
		joint2.publish(degrees2);		
		joint3.publish(degrees3);
		ros::Duration(3).sleep();
		degrees4.data=0;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
		degrees1.data = 1.57;
		joint1.publish(degrees1);
		ros::Duration(3).sleep();
		degrees2.data=0.4;
		degrees3.data=0.4;
		joint2.publish(degrees2);		
		joint3.publish(degrees3);
		ros::Duration(3).sleep();	
		degrees4.data=-1.57;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
			
		gripper.data=1;
		joint7.publish(gripper);
		ros::Duration(8).sleep();
		ROS_INFO("Object 2 in position 1\n");
		
		
		degrees4.data=0;
		joint4.publish(degrees4);
		ros::Duration(3).sleep();
		
		ROS_INFO("CRUMB in home\n");
		gripper.data = 1; //initially, the gripper is open.
		degrees1.data = 0;
    degrees2.data = 0;
    degrees3.data = 0;
    degrees5.data = 0;
		joint1.publish(degrees1);
		joint2.publish(degrees2);
		joint3.publish(degrees3);
		joint5.publish(degrees5);
		joint7.publish(gripper);
		ros::Duration(6).sleep();	
		
		
		
		count++;	
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}

















