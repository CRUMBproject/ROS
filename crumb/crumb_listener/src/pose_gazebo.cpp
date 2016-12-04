/**
 * This node sends pose from gazebo to a topic /simulation/pose.
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



ros::Publisher pub;


void cb(ConstPosesStampedPtr &_msg)
{
	geometry_msgs::Vector3 pose;
	pose.x=_msg->pose(0).position().x();
	pose.y=_msg->pose(0).position().y();
	pose.z=_msg->pose(0).position().z();
    //ROS_INFO("x: %.2f y:%.2f  z:%.2f ", pose.x, pose.y, pose.y);
    pub.publish(pose);
}

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo
  gazebo::setupClient(_argc, _argv);
  ros::init(_argc, _argv, "pose_gazebo");
  ros::NodeHandle n;
  
  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();
  ros::Rate loop_rate(10);
  
  // Create ROS node and init
  pub = n.advertise<geometry_msgs::Vector3>("/simulation/pose", 1000);

  // Listen to Gazebo world_stats topic
  gazebo::transport::SubscriberPtr sub = node->Subscribe("~/pose/info", cb);

  while (ros::ok())
  {
	  gazebo::common::Time::MSleep(10);
	  ros::spinOnce();
  }
    
  // Make sure to shut everything down.
  gazebo::transport::fini();
}

