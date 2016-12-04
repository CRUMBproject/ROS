/**
 * This node sends sim_time from gazebo to a topic /simulation/sim_time.
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
#include "std_msgs/Float64.h"



ros::Publisher pub;


void cb(ConstWorldStatisticsPtr &_msg)
{
	std_msgs::Float64 simtime;
	simtime.data=_msg->sim_time().sec();
    //ROS_INFO("%.2f", simtime.data);
    pub.publish(simtime);
}

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo
  gazebo::setupClient(_argc, _argv);
  ros::init(_argc, _argv, "sim_time_gazebo");
  ros::NodeHandle n;
  
  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();
  ros::Rate loop_rate(10);
  
  // Create ROS node and init
  pub = n.advertise<std_msgs::Float64>("/simulation/sim_time", 1000);

  // Listen to Gazebo world_stats topic
  gazebo::transport::SubscriberPtr sub = node->Subscribe("~/world_stats", cb);

  while (ros::ok())
  {
	  gazebo::common::Time::MSleep(10);
	  ros::spinOnce();
  }
    
  // Make sure to shut everything down.
  gazebo::transport::fini();
}

