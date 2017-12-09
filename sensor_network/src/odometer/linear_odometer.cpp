/** 
  * \file linear_odometer.cpp
  * \author Calvin Lobo
  * \date September 2017
  *
  * \brief Publishes a stream of linearly increasing odometer messages.
*/

#include "ros/ros.h"
#include "sensor_network/Odometer.h"



int main(int argc, char ** argv) 
{
  int message_queue_size = 1000;
  float ticks_per_meter = 1000.0;
  int direction = 1;
  float step_size_m = 0.1;

  ros::init(argc, argv, "talker");

  ros::NodeHandle n;

  ros::Publisher linear_odometer_pub = n.advertise<sensor_network::Odometer>("linear_odometer", message_queue_size);

  ros::Rate loop_rate(10);


  int step_count  = 0;

  while (ros::ok())
  {
    // Create the Odometer message 
      sensor_network::Odometer msg;

    // Fill the message	
      msg.step_count = step_count;
    msg.tick_count = step_count * step_size_m * direction * ticks_per_meter;

    // Print the message to log
    ROS_INFO("Step count: [%ld]\t Tick count: [%ld]", msg.step_count, msg.tick_count);

      linear_odometer_pub.publish(msg);

      ros::spinOnce();

      loop_rate.sleep();

      ++step_count;
  }


  return 0;
}
