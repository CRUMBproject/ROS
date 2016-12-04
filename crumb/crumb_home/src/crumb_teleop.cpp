#include "ros/ros.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <stdio.h>
#include "geometry_msgs/Twist.h"


using namespace std;

/**
 * Basic teleoperation for WidowX arm.
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while the widowx arm software by Robotnik is running.
 * https://github.com/RobotnikAutomation/widowx_arm
 * 
 * For videos and further information
 * http://jafma.net/ana/theweekendarchaeologist/wp-admin/post.php?post=564
 *
 * TO DO:
 * - Clean and organize code
 * - Use services for relaxing joints
 * - Open/close gripper
 * - Read joints values
 * - Create a proper launch file
 * - rviz integration
 */
 
 
 

int main(int argc, char **argv)
{
  
  ros::init(argc, argv , "crumb_teleops");
  ros::NodeHandle n;
  const double FORWARD_SPEED_MPS=0.1;
    
  ros::Publisher joint1 = n.advertise<std_msgs::Float64>("/arm_1_joint/command", 1000);
  ros::Publisher joint2 = n.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);
  ros::Publisher joint3 = n.advertise<std_msgs::Float64>("/arm_3_joint/command", 1000);
  ros::Publisher joint4 = n.advertise<std_msgs::Float64>("/arm_4_joint/command", 1000);
  ros::Publisher joint5 = n.advertise<std_msgs::Float64>("/arm_5_joint/command", 1000);
  //ros::Publisher joint7 = n.advertise<std_msgs::Float64>("/gripper_1_joint/command", 1000);
  //ros::Publisher pub =n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",100);
  ros::Rate loop_rate(5);

  /**
   * Main code.
   */

  std_msgs::Float64 degrees1;
  std_msgs::Float64 degrees2;
  std_msgs::Float64 degrees3;
  std_msgs::Float64 degrees4;
  std_msgs::Float64 degrees5;
  std_msgs::Float64 pinza;
  pinza.data = 0;
  degrees1.data = 0;
  degrees2.data = 0;
  degrees3.data = 0;
  degrees4.data = 0;
  degrees5.data = 0;
  geometry_msgs::Twist msg;
  //msg.linear.x = FORWARD_SPEED_MPS;
  int count = 0;
  char car='0';

  ROS_INFO("Cargando teleoperacion para Widow-X arm.\nJoint 1: 1 +0.2rad, q -0.2rad\n");
  while (ros::ok() && count<=2000) 
  {

    scanf("%c",&car);
    /*car=getchar();*/
    
    switch (car)
    {
		case 's':
		{
			msg.angular.z = 0.5;
			//ros::Duration(4).sleep();
			break;
		}
		       
		
		/*case 's':
		{
			msg.linear.x = 0;
			break;
		}
		case 'f':
		{
			msg.linear.x = FORWARD_SPEED_MPS;
			break;
		}*/
	    case '1':
		{
			degrees1.data = degrees1.data + 0.2;
			if (degrees1.data >= 1.57)
			{
				ROS_INFO("Articulacion 1 esta al tope");
				degrees1.data =1.57;
			}
			break;
		}
		case 'q':
		{
			degrees1.data = degrees1.data - 0.2;
			if (degrees1.data <= -1.57)
			{
				ROS_INFO("Articulacion 1 esta al tope");
				degrees1.data =-1.57;
			}
			break;
		}
		case '2':
		{
			degrees2.data = degrees2.data + 0.2;
			if (degrees2.data >= 1.57)
			{
				ROS_INFO("Articulacion 2 esta al tope");
				degrees2.data =1.57;
			}
			break;
		}
		case 'w':
		{
			degrees2.data = degrees2.data - 0.2;
			if (degrees2.data <= -1.57)
			{
				ROS_INFO("Articulacion 2 esta al tope");
				degrees2.data =-1.57; 
			}
			break;
		}
		case '3':
		{
			degrees3.data = degrees3.data + 0.2;
			if (degrees3.data >= 1.57)
			{
				ROS_INFO("Articulacion 3 esta al tope");
				degrees3.data =1.57;
			}
			break;
		}
		case 'e':
		{
			degrees3.data = degrees3.data - 0.2;
			if (degrees3.data <= -1.57)
			{
				ROS_INFO("Articulacion 3 esta al tope");
				degrees3.data =-1.57;
			}
			break;
		}
		case '4':
		{
			degrees4.data = degrees4.data + 0.2;
			if (degrees4.data >= 1.57)
			{
				ROS_INFO("Articulacion 4 esta al tope");
				degrees4.data =1.57;
			}
			break;
		}
		case 'r':
		{
			degrees4.data = degrees4.data - 0.2;
			if (degrees4.data <= -1.57)
			{
				ROS_INFO("Articulacion 4 esta al tope");
				degrees4.data =-1.57;

			}
			break;
		}
		case '5':
		{
			degrees5.data = degrees5.data + 0.2;
			if (degrees5.data >= 1.57)
			{
				ROS_INFO("Articulacion 5 esta al tope");
				degrees5.data =1.57;
			}
			break;
		}
		case 't':
		{
			degrees5.data = degrees5.data - 0.2;
			if (degrees5.data <= -1.57)
			{
				ROS_INFO("Articulacion 5 esta al tope");
				degrees5.data =-1.57;
			}
			break;
		}
		//case 'a':
		//{
			//pinza.data =   pinza.data + 0.005;
			//if (pinza.data >= 0.03)
			//{
				//ROS_INFO("Pinza abierta al tope");
				//pinza.data =0.03;
			//}
			//break;
		//}
		//case 'c':
		//{
			//pinza.data =   pinza.data - 0.005;
			//if (pinza.data <= 0)
			//{
				//ROS_INFO("Pinza cerrada al tope");
				//pinza.data =0; 
			//}
			//break;
		//}								
	}
	//pub.publish(msg);		
	joint1.publish(degrees1);
	joint2.publish(degrees2);
	joint3.publish(degrees3);
	joint4.publish(degrees4);
	joint5.publish(degrees5);
	//joint7.publish(pinza);
	count++;	
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
