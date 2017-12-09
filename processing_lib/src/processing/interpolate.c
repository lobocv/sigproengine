//
// Created by calvin on 08/11/17.
//

#ifndef SRC_INTERPOLATE_H
#define SRC_INTERPOLATE_H

#include "sigpro_definitions.hpp"
#include "interpolate.h"


/*! Linearly interpolate values between two points.

	@param[in] 		x0 x at left point
	@param[in] 		x1 x at right point
	@param[in] 		y0 y at left point
	@param[in] 		y1 y at right point
	@param[out] 	interpolated_output array in which to place interpolated data (inclusive of left and right point)
	@param[in] 		output_length length of interpolated values (includes the input left and right point)
*/
SIGNAL_DTYPE linear_interpolate(int x0, int x1, SIGNAL_DTYPE y0, SIGNAL_DTYPE y1, SIGNAL_DTYPE* interpolated_output, int output_length)
{
	double delta;
	double step;

	delta = (y1 - y0) / double (x1 - x0);
	step = (x1 - x0) / (double) (output_length-1);

	for (int ii=0; ii < output_length; ii++)
	{

		interpolated_output[ii] = y0 + delta * step * ii;
	}
}


/*! Yve's Spline interpolation (Yves Lamontagne, Unversity of Toronto)
 *
 * This spline was developed to emulate the sinc function interpolation. Yves spline works locally to generate a fifth
 * order polynomial for inteprolation between four points (with interpolation occuring between point 2 and 3).
 *
 * We need to compute the fractional distance between point 2 and 3, measured from point 2. Call this  0 < alpha 1.
 * Yves spline defines y(alpha) as follows:
 *
 * y(alpha) = a0 + alpha * (a1 + alpha * (a2 + alpha * (a3 + alpha(a4 - alpha *a3))))

	@param[in] 		y0 first point
	@param[in] 		y1 second point
	@param[in] 		y2 third point
	@param[in] 		y3 fourth point
	@param[out] 		interpolated_output array in which to place interpolated data (inclusive of left and right point)
	@param[in] 		output_length length of interpolated values (includes the input second and third point)
*/
SIGNAL_DTYPE yve_spline_interpolate(SIGNAL_DTYPE y0, SIGNAL_DTYPE y1, SIGNAL_DTYPE y2, SIGNAL_DTYPE y3, SIGNAL_DTYPE* interpolated_output, int output_length)
{
	double a0, a1, a2, a3, a4, f, yt;
	double step;
	int n_interp_pts = output_length -2;

	a0 = y1;
	a1 = 0.5 * (y2 - y0);
	a2 = (11 * y0 - 27 * y1 + 21 * y2 - 5 * y3) / 12.0;
	a3 = (y3 - 3 * y2 + 3 * y1 - y0) / 6.0;
	a4 = (5 * y3 - 15 * y2 + 15 * y1 - 5 * y0) / 12.0;

	interpolated_output[0] = y1;
	for (int ii=0; ii < n_interp_pts; ii++)
	{

		f = ii / ((double) (n_interp_pts + 1));
		yt = a2 + f * (a3 + f * (a4 - f * a3));

		interpolated_output[ii+1] = a0 + f * (a1 + f * yt);
	}
	interpolated_output[n_interp_pts+1] = y2;
}



/*! Returns the total number of points after interpolating by N points.

	@param[in] 		points_per_trace Original number of points in the trace
	@param[in] 		n_interp_points The number of points between each of the original points

*/
int get_interpolated_length(int points_per_trace, int n_interp_points)
{
	return points_per_trace * (n_interp_points+1);
}


/*! Interpolates between points in a trace with the ability to select interpolation method.

	@param[in] 		inData input signal array
	@param[out] 	outData interpolated output signal array
	@param[in] 		points_per_trace Original number of points in the trace
	@param[in] 		n_interp_points The number of points between each of the original points
	@param[in] 		interp_method Name of the method used for interpolation
*/
void interpolate_trace(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData, int points_per_trace, int n_traces, int n_interp_points, const char* interp_method)
{

	SIGNAL_DTYPE* interp_sub_array;
	int interp_sample_idx, sample_idx;
	int interpolated_points_per_trace = get_interpolated_length(points_per_trace, n_interp_points);

	if (strcmp(interp_method, "linear") == 0)
	{
		for (int trace_idx = 0; trace_idx < n_traces; trace_idx++)
		{

			for (int sample_count = 0; sample_count < points_per_trace - 1; sample_count++) {

				// The location of this sample in the input trace buffer and in the interpolated output trace buffer
				sample_idx = (trace_idx * points_per_trace) + sample_count;
				interp_sample_idx = (trace_idx * interpolated_points_per_trace) + (sample_count * (n_interp_points + 1));

				// Call the interpolator to interpolate between the two points and place the results into the output data slice
				linear_interpolate(sample_idx, sample_idx + 1, inData[sample_idx], inData[sample_idx + 1],
							 &outData[interp_sample_idx], n_interp_points + 2);


			}
		}
	}
	else if (strcmp(interp_method, "yves_spline") == 0)
	{
		double y0, y1, y2, y3;

		for (int trace_idx = 0; trace_idx < n_traces; trace_idx++)
		{
			for (int sample_count = 0; sample_count < points_per_trace - 1; sample_count++)
			{
				// The location of this sample in the input trace buffer and in the interpolated output trace buffer
				sample_idx = (trace_idx * points_per_trace) + sample_count;
				interp_sample_idx = (trace_idx * interpolated_points_per_trace) + (sample_count * (n_interp_points + 1));

				// Determine the 4 points used for the spline
				if (sample_count == 0)
				{
					y0 = inData[sample_idx];
					y1 = inData[sample_idx];
					y2 = inData[sample_idx + 1];
					y3 = inData[sample_idx + 2];
				} else
				if (sample_count == (n_interp_points-1))
				{
					y0 = inData[sample_idx - 2];
					y1 = inData[sample_idx - 1];
					y2 = inData[sample_idx];
					y3 = inData[sample_idx];
				}
				else {
					y0 = inData[sample_idx-1];
					y1 = inData[sample_idx];
					y2 = inData[sample_idx+1];
					y3 = inData[sample_idx+2];
				}

				yve_spline_interpolate(y0, y1, y2, y3, &outData[interp_sample_idx], n_interp_points+2);

			}
		}

	}
}

#endif //SRC_INTERPOLATE_H
