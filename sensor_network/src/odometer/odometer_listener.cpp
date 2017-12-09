/** 
  * \file odometer_listener.cpp
  * \author Calvin Lobo
  * \date September 2017
  *
  * \brief Listen to the linear_odometer node and print what we hear.
*/


#include "ros/ros.h"
#include "sensor_network/Odometer.h"


/**
  * Callback for odometer messages read on the wire. Print the message attributes.
*/
void Odometer_Callback(const sensor_network::Odometer::ConstPtr& msg)
{
  ROS_INFO("Step count: [%ld]\t Tick count: [%ld]", msg->step_count, msg->tick_count);
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("linear_odometer", 1000, Odometer_Callback);

  ros::spin();

  return 0;
}
