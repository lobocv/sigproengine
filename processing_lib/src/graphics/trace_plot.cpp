/*! \file cross_section.cpp
    \author Calvin Lobo
    \date September 2017
	\brief Creation of cross section images from signal data arrays.
	
*/


#include "sigpro_definitions.hpp"
#include "sigpro_constants.h"
#include "trace_plot.hpp"
#include "array_manipulation.hpp"
#include "axis.hpp"
#include "boost/python/numpy.hpp"

#include "rosbag/bag.h"

#include <opencv2/opencv.hpp>


#include <iostream>
using namespace cv;
using namespace std;


TracePlot::TracePlot() {
    timeAxis.xOffset_px = padLeftpx;
    timeAxis.yOffset_px = padToppx;
    amplitudeAxis.xOffset_px = padLeftpx;
    amplitudeAxis.yOffset_px = padToppx;
}

TracePlot::~TracePlot() {}

Mat TracePlot::render(SIGNAL_DTYPE* tracedata, int points_per_trace, double time_sampling_interval_ns, double* markers, int markers_length)
{
	const float max_amplitude_mV = 50.0;
	int trace_offset_x, trace_offset_y;
	double pixels_per_x, pixels_per_y;
	Point p1, p2, marker_center;


	this->points_per_trace = points_per_trace;
	this->time_sampling_interval_ps = time_sampling_interval_ps;


	if (this->orientation == Orientation::vertical) {
		amplitudeAxis.set_orientation(Orientation::horizontal);
		amplitudeAxis.pixels_per_unit = this->plot_width / (2 * max_amplitude_mV);
		timeAxis.set_orientation(Orientation::vertical);
		timeAxis.pixels_per_unit = this->plot_height / (points_per_trace * time_sampling_interval_ns);

		// Calculate scaling factor and offset for drawing the trace
		trace_offset_x = this->plot_width / 2. + padLeftpx;
		trace_offset_y = this->padToppx;
		pixels_per_x = this->plot_width / SIGNAL_AMPLITUDE_MAX ;
		pixels_per_y = this->plot_height / (double) points_per_trace;

	} else {
		amplitudeAxis.set_orientation(Orientation::vertical);
		amplitudeAxis.pixels_per_unit = this->plot_height / (2 * max_amplitude_mV);
		timeAxis.set_orientation(Orientation::horizontal);
		timeAxis.pixels_per_unit = this->plot_width / (points_per_trace * time_sampling_interval_ns);

		// Calculate scaling factor and offset for drawing the trace
		trace_offset_x = this->padLeftpx;
		trace_offset_y = this->plot_height / 2. + padToppx;
		pixels_per_x = this->plot_width / (double) points_per_trace;
		pixels_per_y = this->plot_height / SIGNAL_AMPLITUDE_MAX;
	}


	// Allocate a Mat for plot image, including padding for axis.
	Mat imgPlot(this->getImageHeight(), this->getImageWidth(), CV_8UC3, Scalar(0, 0, 0));

	// Create a subsection of the plot that we can draw the data to
	Rect plotArea = Rect(padLeftpx, padToppx, this->plot_width, this->plot_height);

	// Scale the data from amplitude to pixels in the plot image
	for (int sample_idx=0; sample_idx < points_per_trace-1; sample_idx++)
	{
		if (this->orientation == Orientation::vertical) {
			p1.x = tracedata[sample_idx] * pixels_per_x + trace_offset_x;
			p1.y = pixels_per_y * sample_idx + trace_offset_y;
			p2.x = tracedata[sample_idx+1] * pixels_per_x + trace_offset_x;
			p2.y = pixels_per_y * sample_idx + 1 + trace_offset_y;
		} else {
			p1.x = sample_idx * pixels_per_x + trace_offset_x;
			p1.y = pixels_per_y * tracedata[sample_idx] + trace_offset_y;
			p2.x = (sample_idx+1) * pixels_per_x + trace_offset_x;
			p2.y = pixels_per_y * tracedata[sample_idx + 1] + trace_offset_y;
		}

		if (clipLine(plotArea, p1, p2)) {
			// Only plot if the trace is within the plot area.
			line(imgPlot, p1, p2, Scalar(0, 255, 0), 1);
		}
	}


    for (int marker_idx=0, marker_trace_idx=0; marker_idx < markers_length; marker_idx++)
    {
        marker_trace_idx = markers[marker_idx];
        if (this->orientation == Orientation::vertical) {
			marker_center.x = tracedata[marker_trace_idx] * pixels_per_x + trace_offset_x;
			marker_center.y = pixels_per_y * marker_trace_idx + trace_offset_y;
		} else {
			marker_center.x = marker_trace_idx * pixels_per_x + trace_offset_x;
			marker_center.y = pixels_per_y * tracedata[marker_trace_idx] + trace_offset_y;
		}
        circle(imgPlot, marker_center, 2, Scalar(0, 0, 255), -1);
    }

	// Draw border
	rectangle(imgPlot, plotArea, border_color, border_thickness_px);

	amplitudeAxis.draw(imgPlot);
	timeAxis.draw(imgPlot);


	this->initialized = true;
	return imgPlot;
}

Mat TracePlot::render(SignalArray2D tracedata, double time_sampling_interval_ps, np::ndarray markers)
{
	const Py_intptr_t *array_shape = tracedata.get_shape();
	const Py_intptr_t *markers_shape = markers.get_shape();
	int points_per_trace = array_shape[0];
	int markers_length= markers_shape[0];
	SIGNAL_DTYPE* raw_array = NdArrayToArray(&tracedata);
	double* raw_markers = reinterpret_cast<double*>( markers.get_data());
	return render(raw_array, points_per_trace, time_sampling_interval_ps, raw_markers, markers_length);
}

void TracePlot::setPadding_px(int left, int right, int top, int bottom) {
    this->padLeftpx = left;
    this->padRightpx = right;
    this->padToppx = top;
    this->padBottompx = bottom;

    amplitudeAxis.xOffset_px = padLeftpx;
    amplitudeAxis.yOffset_px = padToppx;
    timeAxis.xOffset_px = padLeftpx;
    timeAxis.yOffset_px = padToppx;
}

int TracePlot::getImageHeight()  { return plot_height + padToppx + padBottompx; }
int TracePlot::getImageWidth()   { return plot_width + padLeftpx + padRightpx; }
bool TracePlot::isInitialized() { return initialized; }