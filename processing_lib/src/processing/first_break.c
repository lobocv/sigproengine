/*! \file first_break.c
	\brief First break finder
	
	Find the indices in the trace array that correspond to the first point equal to or above the threshold mV level.
 	Find one point for each trace. If there is no value above the threshold, return index zero.
*/


#include <stdio.h>
#include "sigpro_definitions.hpp"
#include "sigpro_constants.h"


/*! Find the first break indices in an array of traces based off of a threshold voltage.

	@param[in] 		dataIn Input data array.
	@param[out] 	fb_indices array of first break indices (length of n_traces).
	@param[in] 		points_per_trace Number of sample points in the data arrays.
	@param[in] 		n_traces Number of traces in the data array.
	@param[in] 		threshold_mV mV threshold level to pick the first break on.
*/
void FindFirstBreakIndex(SIGNAL_DTYPE* dataIn, double* fb_indices, int points_per_trace, int n_traces, float threshold_mV)
{

	int trace_idx = 0;
	int sample_idx = 0;
	int buffer_idx = 0;
	float fb_idx = 0;
	float amplitude_mV;
	int threshold_sign;
	const float max_mV = 50.0;
	double threshold_amplitude = (threshold_mV / max_mV) * SIGNAL_AMPLITUDE_MAX;

	// If the threshold voltage is negative, then flip the sign of the trace amplitude
	if (threshold_mV < 0) {
		threshold_sign = -1;
		threshold_mV *= -1;
	} else {
		threshold_sign = 1;
	}

	for (trace_idx =0; trace_idx < n_traces; trace_idx++) {
		// Set the default value of the first break index to zero.
		fb_idx = 0;

		// Iterate through the trace and find the first point which is equal to or above the threshold
		for (sample_idx=0; sample_idx < points_per_trace; sample_idx++)
		{
			buffer_idx = trace_idx*points_per_trace + sample_idx;

			amplitude_mV =  max_mV * threshold_sign * (dataIn[buffer_idx] / SIGNAL_AMPLITUDE_MAX);

			if (amplitude_mV >= threshold_mV)
				{
					// The index just after the threshold value has been found
					// Linearly interpolate to get the fractional index value at exactly the threshold
					// dataIn[buffer_idx] - dataIn[buffer_idx-1] is the slope (amplitude per index)
					fb_idx = (threshold_amplitude - dataIn[buffer_idx-1] ) /(dataIn[buffer_idx] - dataIn[buffer_idx-1]) + sample_idx;

					break;
				};

		}

		// Record the first break point in the output array. If it was not found, fb_idx is zero.
		fb_indices[trace_idx] = fb_idx;

	}

}
