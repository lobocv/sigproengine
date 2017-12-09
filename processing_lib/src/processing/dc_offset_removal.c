/*! \file dc_offset_removal.c
	\brief Removes the DC offset from trace data
 	\author Calvin Lobo, Dave Redman
 	\date Nov 7 2017
	
	Functions for computing DC levels and removing them from the trace data.
*/


#include "sigpro_definitions.hpp"
#include "moving_average_filters.h"


/*! Computes the average amplitude in a window.

	@param[in] 		dataIn Input data array
	@param[in] 		points_per_trace Number of sample points in the data array
	@param[in] 		window_length Number of points to compute an average from
	@param[in] 		window_start_idx Index in which to start averaging.
*/
double AverageAmplitude(SIGNAL_DTYPE* dataIn, int points_per_trace, int window_length, int window_start_idx)
{

	int sample_idx = 0;
	int window_idx = 0;
	double sum = 0;

	// Calculate the DC level by averaging over the DC window
	for (window_idx=window_start_idx; window_idx < (window_start_idx + window_length); window_idx++)
	{
		sum += dataIn[window_idx];
	}

	return sum / window_length;


}

/*! Remove the DC level from traces. DC is calculated as the average value over the dc_window_length points

	@param[in] 		dataIn Input data array
	@param[out] 	dataOut Output data array
	@param[in] 		points_per_trace Number of sample points in the data array
	@param[in] 		n_traces Number of traces in the data array
	@param[in] 		dc_window_length Number of points to compute an average DC level from
*/
void SimpleDCOffsetRemoval(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int dc_window_length)
{

	int trace_idx = 0;
	int sample_idx = 0;
	int buffer_idx = 0;
	double dc_level = 0;

	for (trace_idx =0; trace_idx < n_traces; trace_idx++) {

		dc_level = AverageAmplitude(&dataIn[trace_idx * points_per_trace], points_per_trace, dc_window_length, 0);
		// Subtract the DC level from the entire trace
		for (sample_idx = 0; sample_idx < points_per_trace; sample_idx++) {
            buffer_idx = trace_idx * points_per_trace + sample_idx;
			dataOut[buffer_idx] = dataIn[buffer_idx] - dc_level;
		}
	}
}


void EMA_DCOffsetRemoval(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int dc_window_length, ExponentialMovingAverageFilter* filter)
{

	int trace_idx = 0;
	int sample_idx = 0;
	int buffer_idx = 0;
	double dc_level = 0, average_dc = 0;

	for (trace_idx =0; trace_idx < n_traces; trace_idx++) {

		dc_level = AverageAmplitude(&dataIn[trace_idx * points_per_trace], points_per_trace, dc_window_length, 0);

		average_dc = update_ExponentialMovingAverageFilter(filter, dc_level);

		// Subtract the DC level from the entire trace
		for (sample_idx = 0; sample_idx < points_per_trace; sample_idx++) {
		    buffer_idx = trace_idx * points_per_trace + sample_idx;
			dataOut[buffer_idx] = dataIn[buffer_idx] - average_dc;
		}
	}


}
void CMA_DCOffsetRemoval(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int dc_window_length, CummulativeMovingAverageFilter* filter)
{

	int trace_idx = 0;
	int sample_idx = 0;
	int buffer_idx = 0;
	double dc_level = 0, average_dc = 0;

	for (trace_idx =0; trace_idx < n_traces; trace_idx++) {

		dc_level = AverageAmplitude(&dataIn[trace_idx * points_per_trace], points_per_trace, dc_window_length, 0);

		average_dc = update_CummulativeMovingAverageFilter(filter, dc_level);

		// Subtract the DC level from the entire trace
		for (sample_idx = 0; sample_idx < points_per_trace; sample_idx++) {
		    buffer_idx = trace_idx * points_per_trace + sample_idx;
			dataOut[buffer_idx] = dataIn[buffer_idx] - average_dc;
		}
	}


}