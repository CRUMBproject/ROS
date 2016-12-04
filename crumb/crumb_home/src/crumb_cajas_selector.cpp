#include "ros/ros.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <geometry_msgs/Vector3.h>
#include <stdio.h>
#include <math.h>
#define pi 3.1415


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

 

int main(int argc, char **argv)
{
  
  ros::init(argc, argv , "crumb_app3");
  ros::NodeHandle node;
  
  ros::Publisher joint[5];
	ros::Publisher joint_g;
    
  joint[0] = node.advertise<std_msgs::Float64>("/arm_1_joint/command", 1000);
  joint[1] = node.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);
  joint[2] = node.advertise<std_msgs::Float64>("/arm_3_joint/command", 1000);
  joint[3] = node.advertise<std_msgs::Float64>("/arm_4_joint/command", 1000);
  joint[4] = node.advertise<std_msgs::Float64>("/arm_5_joint/command", 1000);
  joint_g = node.advertise<std_msgs::Float64>("/gripper_1_joint/command", 1000);
  ros::Rate loop_rate(10);

  /**
   * Main code.
   */
 
	geometry_msgs::Vector3 poserobot[3],a,n[3];
		poserobot[0].x= 0;
		poserobot[0].y= 0.2432;
		poserobot[0].z=-0.0073;
		a.x= 0;
		a.y= 0;
		a.z=-1;
		n[0].x= 0;
		n[0].y= 1;
		n[0].z= 0;
		
		poserobot[1].x= 0.1720;
		poserobot[1].y= 0.1720;
		poserobot[1].z=-0.0073;
		n[1].x= 0.7071;
		n[1].y= 0.7071;
		n[1].z= 0;		
		
		poserobot[2].x= 0.2432;
	  poserobot[2].y= 0;
		poserobot[2].z=-0.0073;
		n[2].x= 1;
		n[2].y= 0;
		n[2].z= 0;
		
	int count=0;
	char box='0';
	int gap=3, red=1, orange=2, i=0; //initial conditions
  
  while (ros::ok() && count<=10) 
  {

		
		ROS_INFO("Which box do you want to move? Red:1 Orange:2 \n");
    std::cin >> box;
    //scanf("%c",&box);
    /*car=getchar();*/
    while(box!='1' && box!='2')
    {
    		ROS_INFO("This box is not available. Choose again.");
    		ROS_INFO("What object do you want to move? Red:1 Orange:2 \n");
    		box='0';
    		std::cin >> box;
    		
    }
    
    
		switch (box)
    {
    		case '1': //move red box
				{
						ROS_INFO("You have chosen the red box \n");
						i=red;
						break;
				}
				
				case '2': //move orange box
				{
						ROS_INFO("You have chosen the orange box \n");
						i=orange;
						break;
				}

		}

		ROS_INFO("Initial state:red box=%i orange box=%i gap=%i",red,orange,gap);
		
  	home(joint);
   	open_gripper(joint_g);
		ROS_INFO("Crumb goes to home position and opens the gripper\n");
    ros::Duration(3).sleep();	
		// Home position and open gripper
		
		ros::Duration(1).sleep();
	  move(joint,poserobot[i-1],a,n[i-1]);	 
		ros::Duration(1).sleep();
		
		ROS_INFO("Crumb in object, closing gripper\n");
		close_gripper(joint_g);
		ros::Duration(9).sleep();
		
		ROS_INFO("Object grasped\n");
		ROS_INFO("Moving object to position free\n");
		ROS_INFO("Lifting the wrist to avoid collisions with objects");
		
		avoid_objects(joint);
		ros::Duration(1).sleep();
		ROS_INFO("Going to position free");
		
		ros::Duration(1).sleep();
    place(joint,poserobot[gap-1],a,n[gap-1]);
		ros::Duration(1).sleep();
		
		ROS_INFO("Leaving the object in position free");
		open_gripper(joint_g);
		ros::Duration(8).sleep();
		ROS_INFO("Object in position \n");
		avoid_objects(joint);
		
		
		//Actualizar variables
		switch (box)
    {
    		case '1': //move red box
				{
						ROS_INFO("You have moved the red box \n");
						red=gap;
						gap=i;
						break;
				}
				
				case '2': //move orange box
				{
						ROS_INFO("You have moved the orange box \n");
						orange=gap;
						gap=i;
						break;
				}

		}
		ROS_INFO("Final state:red box=%i orange box=%i gap=%i",red,orange,gap);

		/*Final pose: Home position*/
		home(joint);
	  ros::Duration(1).sleep();
		
		ros::Duration(6).sleep();	
		count++;	
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
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
  ROS_INFO("t1:%f, t2=%f, t3=%f, t4=%f, t5=%f", t[0],t[1],t[2],t[3],t[4]);
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



