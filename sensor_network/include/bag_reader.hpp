/** 
  * \file bag_reader.cpp
  * \author Calvin Lobo
  * \date September 2017
  *
  * \brief Reads ROS bag files into contigious memory arrays.
  *
  * To read more about ROS  Bag files visit:
  
        http://wiki.ros.org/rosbag
        http://wiki.ros.org/rosbag/Code%20API
    
*/

#include <rosbag/bag.h>
#include <rosbag/view.h>

#include "sigpro_definitions.hpp"
#include "array_manipulation.hpp"
#include "boost/multi_array.hpp"

#include <sensor_network/Radar.h>


namespace BagReader {

    
/*! Reads the ROS Radar messages into a contigious memory block.

	\param[in] 		ROS .bag file that contains Radar messages.
    \param[in] 		topic Name of the topic to read Radar messages from.
    
    \return         Signal trace data as a boost::multi_array.
*/

SignalArray2D getRadarBuffer(rosbag::Bag &bag, const std::string topic)
{
    int N_traces=0, N_samples=0;
    
    // Get a view of the topics that contain Radar messages
    std::vector<std::string> topics;
    topics.push_back(topic);
    rosbag::View view(bag, rosbag::TopicQuery(topics));
    

    // Determine the number of traces in this topic
    N_traces = view.size(); 

    // Determine the number of samples in the first trace of this topic
    // we will assume the following traces all have this many number of samples
    rosbag::View::iterator it = view.begin();
    sensor_network::Radar::ConstPtr trace;
    trace = it->instantiate<sensor_network::Radar>();
    if (trace != NULL) 
        {
            N_samples = trace->data.size();   
        } 


    
    // Allocate a contigious memory buffer for the Radar data
    SIGNAL_DTYPE *tracedata = new SIGNAL_DTYPE[N_traces * N_samples];

    if (N_traces > 0 && N_samples > 0) 
    {
        int message_index=0;
        int sample_number=0;
        // Start iterating the messages from the beginning, placing the data into the memory buffer
        for (it = view.begin(); it != view.end(); message_index++, it++)
        {
            trace = it->instantiate<sensor_network::Radar>();

            for (sample_number=0; sample_number < N_samples; sample_number++)
            {
                tracedata[message_index*N_samples + sample_number] = trace->data[sample_number];
            }
    
        }
    }
    SignalArray2D signal_array = arrayToNdArray(tracedata, N_samples, N_traces);

    return signal_array;
    
}

/*! Reads the ROS Radar messages into a contigious memory block.

	\param[in] 		Full path to ROS .bag file that contains Radar messages.
    \param[in] 		topic Name of the topic to read Radar messages from.
    
    \return         Signal trace data as a boost::multi_array.
*/
SignalArray2D getRadarBuffer(const std::string bagpath, const std::string topic) {
    rosbag::Bag bag(bagpath, rosbag::bagmode::Read);
    SignalArray2D tracedata = getRadarBuffer(bag, topic);
    bag.close();
    return tracedata;
}


}