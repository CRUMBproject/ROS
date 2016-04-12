#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/JointState.h"
#include "arbotix_msgs/Relax.h"

/**
 * Basic testing for the WidowX arm.
 * Copyright (C) 2015 Ana Cruz-Martín (anacruzmartin@gmail.com)
 *
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 *
 * This code must be integrated into a ros package, and then called with rosrun,
 * while the widowx arm software by Robotnik is running.
 * https://github.com/RobotnikAutomation/widowx_arm
 *
 * For videos and further information
 * http://jafma.net/ana/theweekendarchaeologist/?post=564
 *
 * TO DO:
 * - Clean and organize code
 * - Create a proper launch file
 * - rviz integration
 */


void jointsCallback(const sensor_msgs::JointState::ConstPtr& jointsmsg)
{
  ROS_INFO("JOINTS POSITIONS[1: %f] [2: %f] [3: %f] [4: %f] [5: %f] [6: %f]\n", jointsmsg->position[1], jointsmsg->position[2], jointsmsg->position[3], jointsmsg->position[4], jointsmsg->position[5], jointsmsg->position[6]);
  ROS_INFO("\nJOINTS VELOCITIES[1: %f] [2: %f] [3: %f] [4: %f] [5: %f] [6: %f]\n", jointsmsg->velocity[1], jointsmsg->velocity[2], jointsmsg->velocity[3], jointsmsg->velocity[4], jointsmsg->velocity[5], jointsmsg->velocity[6]);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv , "widowx_arm_testing");
  ros::NodeHandle n;

  // Topics of the joints.
  ros::Publisher joint1 = n.advertise<std_msgs::Float64>("/arm_1_joint/command", 1000);
  ros::Publisher joint2 = n.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);
  ros::Publisher joint3 = n.advertise<std_msgs::Float64>("/arm_3_joint/command", 1000);
  ros::Publisher joint4 = n.advertise<std_msgs::Float64>("/arm_4_joint/command", 1000);
  ros::Publisher joint5 = n.advertise<std_msgs::Float64>("/arm_5_joint/command", 1000);
  ros::Publisher gripper = n.advertise<std_msgs::Float64>("/gripper_1_joint/command", 1000);
  ros::Rate loop_rate(10);

  ros::Subscriber joints = n.subscribe("/joint_states", 1000, jointsCallback);

  // Relax services of the joints
  ros::ServiceClient client_service_joint1_relax = n.serviceClient<arbotix_msgs::Relax>("/arm_1_joint/relax");
  ros::ServiceClient client_service_joint2_relax = n.serviceClient<arbotix_msgs::Relax>("/arm_2_joint/relax");
  ros::ServiceClient client_service_joint3_relax = n.serviceClient<arbotix_msgs::Relax>("/arm_3_joint/relax");
  ros::ServiceClient client_service_joint4_relax = n.serviceClient<arbotix_msgs::Relax>("/arm_4_joint/relax");
  ros::ServiceClient client_service_joint5_relax = n.serviceClient<arbotix_msgs::Relax>("/arm_5_joint/relax");
  ros::ServiceClient client_service_gripper_relax = n.serviceClient<arbotix_msgs::Relax>("/gripper_1_joint/relax");

  arbotix_msgs::Relax srv_joint1_relax;
  arbotix_msgs::Relax srv_joint2_relax;
  arbotix_msgs::Relax srv_joint3_relax;
  arbotix_msgs::Relax srv_joint4_relax;
  arbotix_msgs::Relax srv_joint5_relax;
  arbotix_msgs::Relax srv_gripper_relax;

  /**
   * Main code.
   */

  std_msgs::Float64 radszero, radspos, radsneg, gripperzero, gripperpos;
  radszero.data = 0;
  radspos.data = 0.25;
  radsneg.data = -0.25;
  gripperzero.data = 0;
  gripperpos.data = 2.0;
  int count = 0;
  while (ros::ok() && count<=10)
  {
    if (count == 0)
    {
       // "Homing" (all joints to zero)
       ROS_INFO("3 SECONDS TO HOME\n");	
       ros::Duration(3).sleep();
       joint2.publish(radszero);
       joint3.publish(radszero);
       joint4.publish(radszero);
       joint5.publish(radszero);
       joint1.publish(radszero); 
       gripper.publish(gripperzero);
       ros::Duration(2).sleep(); 
       gripper.publish(gripperpos); 
       ros::Duration(2).sleep();
       gripper.publish(gripperzero); 
       ros::Duration(2).sleep(); 
       
       // Once the robot is "homed", we check that all joints move in both directions
       ROS_INFO("3 SECONDS TO CHECK DIRECTIONS\n");	
       ros::Duration(3).sleep();
       joint1.publish(radspos);	// Move joint 1 with positive and negative values
       ros::Duration(2).sleep();
       joint1.publish(radsneg);
       ros::Duration(2).sleep();
       joint1.publish(radszero);
       ros::Duration(3).sleep();
 
       joint2.publish(radspos);	// Move joint 2 with positive and negative values
       ros::Duration(2).sleep();
       joint2.publish(radsneg);
       ros::Duration(2).sleep();
       joint2.publish(radszero);
       ros::Duration(3).sleep();

       joint3.publish(radspos);	// Move joint 3 with positive and negative values
       ros::Duration(2).sleep();
       joint3.publish(radsneg);
       ros::Duration(2).sleep();
       joint3.publish(radszero);
       ros::Duration(3).sleep();

       joint4.publish(radspos);	// Move joint 4 with positive and negative values
       ros::Duration(2).sleep();
       joint4.publish(radsneg);
       ros::Duration(2).sleep();
       joint4.publish(radszero);
       ros::Duration(3).sleep();

       joint5.publish(radspos);	// Move joint 5 with positive and negative values
       ros::Duration(2).sleep();
       joint5.publish(radsneg);
       ros::Duration(2).sleep();
       joint5.publish(radszero);
       ros::Duration(3).sleep();

       // Finally, we relax every joint in the proper order. Look out: the arm rests completely, collitions may happen!!
       ROS_INFO("3 SECONDS TO RELAX\n");	
       ros::Duration(3).sleep();
       if (client_service_gripper_relax.call(srv_gripper_relax))
       {
          ROS_INFO("Gripper relaxed");
       }
       else
       {
    	  ROS_ERROR("Gripper relax failed");
       }
       ros::Duration(1).sleep();
       if (client_service_joint5_relax.call(srv_joint5_relax))
       {
          ROS_INFO("Joint 5 relaxed");
       }
       else
       {
    	  ROS_ERROR("Joint 5 relax failed");
       }
       ros::Duration(1).sleep();
       if (client_service_joint4_relax.call(srv_joint4_relax))
       {
          ROS_INFO("Joint 4 relaxed");
       }
       else
       {
    	  ROS_ERROR("Joint 4 relax failed");
       }
       ros::Duration(1).sleep();
       if (client_service_joint3_relax.call(srv_joint3_relax))
       {
          ROS_INFO("Joint 3 relaxed");
       }
       else
       {
    	  ROS_ERROR("Joint 3 relax failed");
       }
       ros::Duration(1).sleep();
       if (client_service_joint2_relax.call(srv_joint2_relax))
       {
          ROS_INFO("Joint 2 relaxed");
       }
       else
       {
    	  ROS_ERROR("Joint 2 relax failed");
       }
       ros::Duration(1).sleep();
       if (client_service_joint1_relax.call(srv_joint1_relax))
       {
          ROS_INFO("Joint 1 relaxed");
       }
       else
       {
    	  ROS_ERROR("Joint 1 relax failed");
       }
       ros::Duration(1).sleep();
    }
    ROS_INFO("Count: %d\n",count);
    count++;
 
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}

    Status API Training Shop Blog About 

