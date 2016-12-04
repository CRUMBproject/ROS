


#include "ros/ros.h"
#include <iostream>
#include <cmath>
#include "std_msgs/Float64.h"
#include "kobuki_msgs/SensorState.h"
#include "kobuki_msgs/WheelDropEvent.h"
#include "kobuki_msgs/CliffEvent.h"
#include "kobuki_msgs/BumperEvent.h"
#include "sensor_msgs/JointState.h"
#include "nav_msgs/Odometry.h"
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <string>
#define pi 3.1415


/**
 * Application 3 for CRUMB
 * With this application CRUMB can move objects from one table to another.
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while Gazebo and CRUMB are running.
 * 
 */

float x=0,y=0,w=0;

void callbackodom(const nav_msgs::Odometry::ConstPtr& _msg)
{
		//ROS_INFO("HOLA");
		x=_msg->pose.pose.position.x;
		y=_msg->pose.pose.position.y;
		w=_msg->pose.pose.orientation.w;
		ROS_INFO("x=%f y=%f w=%f", x,y,w);
		
		
		
}

void open_gripper(ros::Publisher joint_g);
//this function open the gripper.
void close_gripper(ros::Publisher joint_g);
//this function closes the gripper.
geometry_msgs::Vector3 p(geometry_msgs::Vector3 pr, geometry_msgs::Vector3 a);
//this function obtain, with final efector position, pose before the wirst.
void mci(float* t, geometry_msgs::Vector3 poserobot, geometry_msgs::Vector3 a, geometry_msgs::Vector3 n);
//this function calculates robots joint with final efector position.
void move(ros::Publisher* joint, geometry_msgs::Vector3 poserobot, geometry_msgs::Vector3 a, geometry_msgs::Vector3 n);
//with a pose given, this function moves the robot to this position.
void place(ros::Publisher* joint, geometry_msgs::Vector3 poserobot, geometry_msgs::Vector3 a, geometry_msgs::Vector3 n);
//this function place the object over the table.
void home(ros::Publisher* joint);
//this function makes the robot move to its home pose.
void avoid_objects(ros::Publisher* joint);
//this function raises the arm to avoid collisions with other objects.


/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  
  ros::init(_argc, _argv, "odom_crumb");
  ros::NodeHandle node;
  
  ros::Rate loop_rate(10);
  ros::Publisher joint[5];
	ros::Publisher joint_g, wheel;
	
  // Create ROS node and init
  joint[0] = node.advertise<std_msgs::Float64>("/arm_1_joint/command", 1000);
  joint[1] = node.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);
  joint[2] = node.advertise<std_msgs::Float64>("/arm_3_joint/command", 1000);
  joint[3] = node.advertise<std_msgs::Float64>("/arm_4_joint/command", 1000);
  joint[4] = node.advertise<std_msgs::Float64>("/arm_5_joint/command", 1000);
  joint_g  = node.advertise<std_msgs::Float64>("/gripper_1_joint/command", 1000);
	wheel    = node.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);
	
	
  ros::Subscriber sub_odom  =node.subscribe("/odom",10, callbackodom);
 	geometry_msgs::Twist mov;
	geometry_msgs::Vector3 poserobot,a,n;
 	int count=0;
 			
	poserobot.x= 0.2432;
	poserobot.y= 0;
	poserobot.z=-0.0073;
	n.x= 1;
	n.y= 0;
	n.z= 0;
 	a.x= 0;
	a.y= 0;
	a.z=-1;
  
  while (ros::ok() && count<1)
  {

		ROS_INFO("HOME POSE");
		home(joint);
   	open_gripper(joint_g);		
		ros::Duration(2).sleep();
		
		ROS_INFO("CRUMB GOES TO OBJECT");
		mov.linear.x=0.2;
		mov.linear.y=0;
		mov.linear.z=0;
		mov.angular.x=0;
		mov.angular.y=0;
		mov.angular.z=0;
		
		while ( ros::ok() & x<=2 )
		{
			wheel.publish(mov);
			ROS_INFO("X=%f Y=%f",x,y);
			ros::spinOnce();
		}
		
		ROS_INFO("CRUMB PICKS OBJECT");
		ros::Duration(1).sleep();
	  move(joint,poserobot,a,n);
		ros::Duration(2).sleep();

		close_gripper(joint_g);
		ros::Duration(9).sleep();
		avoid_objects(joint);
		
		ROS_INFO("CRUMB SPINS");
		mov.linear.x=0;
		mov.linear.y=0;
		mov.linear.z=0;
		mov.angular.x=0;
		mov.angular.y=0;
		mov.angular.z=0.2;
		ros::spinOnce();
		while ( ros::ok() & w>=0.7 ) //0.70+time to stop
		{
			wheel.publish(mov);
			ROS_INFO("w=%f",w);
			ros::spinOnce();
		}
		
		
		ROS_INFO("CRUMB MOVES");
		mov.linear.x=0.2;
		mov.linear.y=0;
		mov.linear.z=0;
		mov.angular.x=0;
		mov.angular.y=0;
		mov.angular.z=0;
		
		ROS_INFO("CRUMB PLACES OBJECT");
		while ( ros::ok() & y<=2.6 )
		{
			wheel.publish(mov);
			ROS_INFO("X=%f Y=%f",x,y);
			ros::spinOnce();
		}
		
		ros::Duration(1).sleep();
    place(joint,poserobot,a,n);
		ros::Duration(1).sleep();
		open_gripper(joint_g);
		ros::Duration(8).sleep();
		avoid_objects(joint);
		
		ROS_INFO("OBJECT IN TARJET");
		home(joint);		
		ros::Duration(2).sleep();
		
		ROS_INFO("CRUMB GOES BACK TO INITIAL POSITION");
		
		count++;
	  ros::spinOnce();
	  loop_rate.sleep();
  }
    

}










void open_gripper(ros::Publisher joint_g)
{
  std_msgs::Float64 gripper;
  gripper.data=1;
  joint_g.publish(gripper);
}

void close_gripper(ros::Publisher joint_g)
{
  std_msgs::Float64 gripper;
  gripper.data=0;
  joint_g.publish(gripper);
}
  
geometry_msgs::Vector3 p(geometry_msgs::Vector3 pr, geometry_msgs::Vector3 a)
{
	float l45=0.1221;
	geometry_msgs::Vector3 pose;
	pose.x=pr.x-l45*a.x;
  pose.y=pr.y-l45*a.y;
  pose.z=pr.z-l45*a.z;
  return pose;
} 

void mci(float* t, geometry_msgs::Vector3 poserobot, geometry_msgs::Vector3 a, geometry_msgs::Vector3 n)
{
	
	float k, k1, k2, beta, L;
  float l3=0.1423, d2=0.1528;
  int codo=-1;
  geometry_msgs::Vector3 pose;
  
  pose = p(poserobot,a);
	t[0]= atan2(pose.y,pose.x);
	
	k=pose.z*pose.z+d2*d2+(pose.x*cos(t[0])+pose.y*sin(t[0]))*(pose.x*cos(t[0])+pose.y*sin(t[0]))-l3*l3;
	k1=2*pose.x*cos(t[0])*d2+2*pose.y*sin(t[0])*d2;
	k2=2*pose.z*d2;
	beta=atan2(k1,k2)-atan2(k,codo*sqrt(k1*k1+k2*k2-k*k));
  t[1]=beta + 1.2496;
	      
  t[2]=t[1]-asin((-pose.z-d2*sin(beta))/l3);
	  
  L=a.z*cos(t[1]-t[2])+a.x*sin(t[1]-t[2])*cos(t[0])+a.y*sin(t[1]-t[2])*sin(t[0]);
	t[3]=acos(-L)-(pi/2);
	  
  t[4]=asin(n.x*sin(t[0]) - n.y*cos(t[0]));
  
}



void move(ros::Publisher* joint, geometry_msgs::Vector3 poserobot, geometry_msgs::Vector3 a, geometry_msgs::Vector3 n)
{
  float t[5]={0,0,0,0,0};
	std_msgs::Float64 degrees1;
  std_msgs::Float64 degrees2;
  std_msgs::Float64 degrees3;
  std_msgs::Float64 degrees4;
  std_msgs::Float64 degrees5;
  ros::Rate loop_rate(10);
	mci(t,poserobot,a,n);	 //obtain the theta for this pose
		
	ros::Duration(1).sleep();
	degrees1.data=t[0];
  degrees2.data=t[1];
  degrees3.data=t[2];
  degrees4.data=t[3];
  degrees5.data=t[4];
  //ROS_INFO("t1:%f, t2=%f, t3=%f, t4=%f, t5=%f", t[0],t[1],t[2],t[3],t[4]);
	joint[0].publish(degrees1);
	joint[1].publish(degrees2);
	joint[2].publish(degrees3);
	ros::Duration(2).sleep();
	joint[3].publish(degrees4);
	joint[4].publish(degrees5);	
		
} 


void place(ros::Publisher* joint, geometry_msgs::Vector3 poserobot, geometry_msgs::Vector3 a, geometry_msgs::Vector3 n)
{
  float t[5]={0,0,0,0,0};
	std_msgs::Float64 degrees1;
  std_msgs::Float64 degrees2;
  std_msgs::Float64 degrees3;
  std_msgs::Float64 degrees4;
  std_msgs::Float64 degrees5;
  ros::Rate loop_rate(10);
	mci(t,poserobot,a,n);	 //obtain the theta for this pose
		
	ros::Duration(1).sleep();
	degrees1.data=t[0];
  degrees2.data=t[1];
  degrees3.data=t[2]+0.2;
  degrees4.data=t[3];
  degrees5.data=t[4];
  ROS_INFO("t1:%f, t2=%f, t3=%f, t4=%f, t5=%f", t[0],t[1],t[2],t[3],t[4]);
	joint[0].publish(degrees1);
	joint[1].publish(degrees2);
	joint[2].publish(degrees3);

	ros::Duration(2).sleep();
	joint[3].publish(degrees4);
	joint[4].publish(degrees5);

	ros::Duration(2).sleep();
  degrees3.data=t[2];
 	joint[2].publish(degrees3);
  
		
} 


void home(ros::Publisher* joint)
{
  float t[5]={0,0,0,0,0};
	geometry_msgs::Vector3 poserobot,a,n;
	std_msgs::Float64 degrees1;
  std_msgs::Float64 degrees2;
  std_msgs::Float64 degrees3;
  std_msgs::Float64 degrees4;
  std_msgs::Float64 degrees5;
	poserobot.x= 0.3125;
	poserobot.y= 0;
	poserobot.z= 0.1450;
	a.x= 1;
	a.y= 0;
	a.z= 0;
	n.x= 0;
	n.y= 0;
	n.z= 1;
	
	mci(t,poserobot,a,n);	 //obtain the theta for this pose
		
	ros::Duration(1).sleep();
	degrees1.data=t[0];
  degrees2.data=t[1];
  degrees3.data=t[2];
  degrees4.data=t[3];
  degrees5.data=t[4];
  ROS_INFO("t1:%f, t2=%f, t3=%f, t4=%f, t5=%f", t[0],t[1],t[2],t[3],t[4]);
	joint[0].publish(degrees1);
	joint[1].publish(degrees2);
	joint[2].publish(degrees3);
	joint[3].publish(degrees4);
	joint[4].publish(degrees5);	
		
}

void avoid_objects(ros::Publisher* joint)
{
  std_msgs::Float64 degrees3;
  std_msgs::Float64 degrees4;
  ros::Rate loop_rate(10);
	
  degrees3.data=0.6;
  degrees4.data=0;
  
	joint[2].publish(degrees3);
	ros::Duration(1).sleep();
	joint[3].publish(degrees4);
}



