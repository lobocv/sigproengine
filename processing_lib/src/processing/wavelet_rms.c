/*! \file wavelet_rms.c
	\brief RMS of a wavelet in a trace
	
	Returns the Root Mean Squared value between two points in a trace.
*/


#include "sigpro_definitions.hpp"
#include <math.h>

/*! Returns the Root Mean Squared value between two points in a trace.

	@param[in] 		dataIn Input data array
	@param[in] 		points_per_trace Number of sample points in the data arrays
	@param[in] 		start_idx Starting index of points to include in the RMS
	@param[in] 		end_idx Final index of points to include in the RMS

 	@returns Root mean squared amplitude value between start_idx and end_idx of the trace
*/
double WaveletRMS(SIGNAL_DTYPE* dataIn, int points_per_trace, int start_idx, int end_idx)
{

	int n_wavelet_points = end_idx - start_idx + 1;
	double squared_sum = 0;

	for (int sample_idx=start_idx; sample_idx <= end_idx; sample_idx++) {
		squared_sum += dataIn[sample_idx] * dataIn[sample_idx];
	}

	return sqrt(squared_sum / n_wavelet_points);
}
