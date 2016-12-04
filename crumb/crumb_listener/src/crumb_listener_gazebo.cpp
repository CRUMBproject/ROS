/**
 * This node sends data from gazebo to a topic in ROS.
 * 
 * Marina Aguilar-Moreno, 2016
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while gazebo is running.
 * 

 */

#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo.hh>
#include "ros/ros.h"
#include <iostream>
#include <geometry_msgs/Vector3.h>
#include "std_msgs/Float64.h"
#include "kobuki_msgs/SensorState.h"
#include "kobuki_msgs/WheelDropEvent.h"
#include "kobuki_msgs/CliffEvent.h"
#include "kobuki_msgs/BumperEvent.h"
#include <cmath>



ros::Publisher pub1;
ros::Publisher pub2;
ros::Publisher pub3;
//ros::Publisher pub4;



void cb1(ConstPosesStampedPtr &_msg)
{
	geometry_msgs::Vector3 pose;
	pose.x=_msg->pose(0).position().x();
	pose.y=_msg->pose(0).position().y();
	pose.z=_msg->pose(0).position().z();
    //ROS_INFO("x: %.2f y:%.2f  z:%.2f ", pose.x, pose.y, pose.y);
    
    pub1.publish(pose);
}


void cb2(ConstWorldStatisticsPtr &_msg)
{
	std_msgs::Float64 realtime, simtime,nanosec;
	
	realtime.data=_msg->real_time().sec();
	//nanosec.data=_msg->real_time().nsec();   	
	simtime.data=_msg->sim_time().sec();
	
    pub2.publish(realtime);
    pub3.publish(simtime);
    //pub4.publish(nanosec);
}


/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo
  gazebo::setupClient(_argc, _argv);
  ros::init(_argc, _argv, "crumb_listener_gazebo");
  ros::NodeHandle n;
  
  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();
  ros::Rate loop_rate(10);
  
  // Create ROS node and init
  pub1 = n.advertise<geometry_msgs::Vector3>("/simulation/pose", 1000);
  pub2 = n.advertise<std_msgs::Float64>("/simulation/real_time", 1000);
  pub3 = n.advertise<std_msgs::Float64>("/simulation/sim_time", 1000);
  //pub4 = n.advertise<std_msgs::Float64>("/simulation/real_time_nsec", 1000);
  
  // Listen to Gazebo world_stats topic
  gazebo::transport::SubscriberPtr sub1 = node->Subscribe("~/pose/info", cb1);
  gazebo::transport::SubscriberPtr sub2 = node->Subscribe("~/world_stats", cb2);


  while (ros::ok())
  {
	  gazebo::common::Time::MSleep(10);
	  ros::spinOnce();
  }
    
  // Make sure to shut everything down.
  gazebo::transport::fini();
}

