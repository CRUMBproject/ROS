/**
 * This node sends information about de robot from gazebo 
 * to the topic /mobile_base/sensors/core.
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package
 * while gazebo is running.
 * 

 */


#include "ros/ros.h"
#include <iostream>
#include <cmath>
#include "std_msgs/Float64.h"
#include "kobuki_msgs/SensorState.h"
#include "kobuki_msgs/WheelDropEvent.h"
#include "kobuki_msgs/CliffEvent.h"
#include "kobuki_msgs/BumperEvent.h"
#include "sensor_msgs/JointState.h"
#include <math.h>
#include <string>



ros::Publisher pub;
kobuki_msgs::SensorState m;
float encoder[2]={0,0}, encoder_init[2],ticks[2]={0,0};
float tiempo[2]={0,0};
std::string str1 ("wheel_left_joint");




void callback0(const sensor_msgs::JointState::ConstPtr& _msg)

{
	float s[2];
	if (str1.compare(_msg->name[0]) == 0)
	{
		encoder_init[0]=encoder[0];
		encoder_init[1]=encoder[1];
		encoder[0]=_msg->position[0];
		encoder[1]=_msg->position[1];
		s[0]=encoder[0]-encoder_init[0];
		s[1]=encoder[1]-encoder_init[1];	
		ticks[0]=410.35458*s[0]+ticks[0];
		ticks[1]=410.35458*s[1]+ticks[1];
		if (ticks[0]>= 65535)
		{
			ticks[0]=ticks[0]-65535;
		}
		if (ticks[1]>= 65535)
		{
			ticks[1]=ticks[1]-65535;
		}
			
		m.left_encoder=ticks[0];
		m.right_encoder=ticks[1];
		//ROS_INFO("%f %f %f %f",encoder[1],encoder_init[1],ticks[1],s[1]);
		pub.publish(m);
	}
}


void callback1(const kobuki_msgs::WheelDropEvent::ConstPtr& _msg)
{
	m.wheel_drop=_msg->state;
    pub.publish(m);
}

void callback2(const kobuki_msgs::CliffEvent::ConstPtr& _msg)
{
	unsigned int cliffs=0;
	cliffs= cliffs & ~ (1<<_msg->sensor);
	cliffs= cliffs | (_msg->state << _msg->sensor);

	m.cliff=cliffs;	
	pub.publish(m);
}

void callback3(const kobuki_msgs::BumperEvent::ConstPtr& _msg)
{
	unsigned int bumpers=0;
	bumpers=bumpers & ~ (1<< _msg->bumper);
	bumpers=bumpers | (_msg->state << _msg->bumper);
    
  m.bumper=bumpers;
  pub.publish(m);
}

void callback4(const std_msgs::Float64::ConstPtr& _msg)
{
	m.time_stamp=_msg->data*1000;
		if (m.time_stamp>= 65536)
		{
			m.time_stamp=m.time_stamp % 65536;
		}
  pub.publish(m);
}

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  
  ros::init(_argc, _argv, "core");
  ros::NodeHandle n;
  
  ros::Rate loop_rate(10);
  
  // Create ROS node and init
  pub = n.advertise<kobuki_msgs::SensorState>("/mobile_base/sensors/core", 1000);

  ros::Subscriber sub_e  =n.subscribe("/joint_states",10, callback0);
  ros::Subscriber sub_wd =n.subscribe("/mobile_base/events/wheel_drop",10, callback1);
  ros::Subscriber sub_ce =n.subscribe("/mobile_base/events/cliff",10, callback2);
  ros::Subscriber sub_b  =n.subscribe("/mobile_base/events/bumper",10, callback3);
  ros::Subscriber sub_t  =n.subscribe("/simulation/real_time",10, callback4);
  
  
  
  while (ros::ok())
  {

	  ros::spinOnce();
	  loop_rate.sleep();
  }
    

}

