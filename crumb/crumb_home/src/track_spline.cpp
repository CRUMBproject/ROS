#include "ros/ros.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <math.h>

/**
 * This code makes the robot move tracking a spline.
 * 
 * DISCLAIMER: This is a work-in-progress test code, not finished yet.
 * 
 * This code must be integrated into a ros package, and then called with rosrun,
 * while gazebo is running.
 *
 */
 
 

int main(int argc, char **argv)
{
  ros::init(argc, argv , "track_spline");
  ros::NodeHandle n;

  // Topics of the joints.
  ros::Publisher joint1 = n.advertise<std_msgs::Float64>("/arm_1_joint/command", 1000);
  ros::Publisher joint2 = n.advertise<std_msgs::Float64>("/arm_2_joint/command", 1000);
  ros::Publisher joint3 = n.advertise<std_msgs::Float64>("/arm_3_joint/command", 1000);
  //ros::Publisher joint4 = n.advertise<std_msgs::Float64>("/arm_4_joint/command", 1000);
  //ros::Publisher joint5 = n.advertise<std_msgs::Float64>("/arm_5_joint/command", 1000);
  ros::Rate loop_rate(10);

  /**
   * Main code.
   */

  std_msgs::Float64 theta1;
  std_msgs::Float64 theta2;
  std_msgs::Float64 theta3;
  //std_msgs::Float64 theta4;
  //std_msgs::Float64 theta5;
  theta1.data = 0;
  theta2.data = 0;
  theta3.data = 0;
  //theta4.data = 0;
  //theta5.data = 0;
  float t1,t2,t3, x, z,k, k1, k2,beta;
  //float y[10] = {-0.1347, -0.0586, 0, 0.0586, 0.1347};
  float y = -0.1347;
  float l2=0.04825, l3=0.1423, d2=0.1528;
  int count = 1999, codo=-1;
 
  while (ros::ok() && count<=2000)
  {
	  ROS_INFO("The robot is moving...");

      x=0.1347;
      
      for(int i=0;i<15; i++)
      {
		  
		  z=45*y*(y*y-0.1347*0.1347)+0.1450;
		  //ROS_INFO("x=%f y=%f z=%f i=%d", x, y, z, i);
		  
		  t1= atan2(y,x);
	      k=z*z+d2*d2+(x*cos(t1)+y*sin(t1))*(x*cos(t1)+y*sin(t1))-l3*l3;
	      k1=2*x*cos(t1)*d2+2*y*sin(t1)*d2;
	      k2=2*z*d2;
	      
	      beta=atan2(k1,k2)-atan2(k,codo*sqrt(k1*k1+k2*k2-k*k));
	      t2=beta + 1.2496;
	      
	      t3=t2-asin((-z-d2*sin(beta))/l3);
	      //ROS_INFO("theta1=%f theta2=%f theta3=%f ", t1, t2, t3);
	      ros::Duration(1).sleep();
	      theta1.data=t1;
	      theta2.data=t2;
	      theta3.data=t3;
	      joint1.publish(theta1); /* Waist, [-pi/2, pi/2]  */
		  joint2.publish(theta2); /* Elbow , [-pi/2, pi/2]*/
	      joint3.publish(theta3); /* Shoulder, [-pi/2, pi/2] */
	      
 
	      //ros::spinOnce();
	      //loop_rate.sleep();
	      //ros::Duration(0.0005).sleep();
	      y=y+0.01901; 
	      
	  }
	  y=-0.1347;
	  count++;

  }
  return 0;
}
