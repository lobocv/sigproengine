/*! \file cross_section.cpp
    \author Calvin Lobo
    \date September 2017
	\brief Creation of cross section images from signal data arrays.
	
*/


#include "sigpro_definitions.hpp"
#include "sigpro_constants.h"
#include "cross_section.hpp"
#include "array_manipulation.hpp"

#include "boost/python/numpy.hpp"

#include "rosbag/bag.h"

#include <opencv2/opencv.hpp>


using namespace cv;


/*! Convert the raw signal data array to a colored OpenCV Mat object.

	\param[in] 		tracedata pointer to raw signal data array
    \return         OpenCV Mat object populated with colorized trace data
    
*/
Mat CrossSection::render(SIGNAL_DTYPE* tracedata, int points_per_trace, int number_of_traces, double step_size_m, double time_sampling_interval_ns)
{
    positionAxis.pixels_per_unit = 1 / step_size_m;
    timeAxis.pixels_per_unit = 1 / time_sampling_interval_ns;


    this->points_per_trace = points_per_trace;
    this->number_of_traces = number_of_traces;
    this->step_size_m = step_size_m;
    this->time_sampling_interval_ps = time_sampling_interval_ps;

    int imageWidth = this->getImageWidth();
    int imageHeight = this->getImageHeight();

    // Convert the data to Mat
    Mat dataMatrix = Mat(number_of_traces, points_per_trace, CV_64F, tracedata) ;
    // Transpose the matrix because the Mat is Row-major while the SIGNAL_DTYPE* is Column-major
    // There may be a more efficient way load the Mat directly as Column-major.
    cv::transpose(dataMatrix, dataMatrix);

    // Allocate a Mat for plot image, including padding for axis.
    Mat imgPlot(imageHeight, imageWidth, CV_8UC3, Scalar(0, 0, 0));

    // Create a subsection of the plot that we can draw the data to
    Rect plotArea = Rect(padLeftpx, padToppx, number_of_traces, points_per_trace);
    Mat imgData = imgPlot(plotArea);

    // Scale the data from 32 bit float to 8 bit uint, centered around the middle of the range (128)
    Mat scaledDataMatrix;
    convertScaleAbs(dataMatrix, scaledDataMatrix, 256.0 / SIGNAL_AMPLITUDE_MAX, 128);

    // Apply a colormap to the image
    applyColorMap(scaledDataMatrix, imgData, COLORMAP_BONE);

    // Draw border
    rectangle(imgPlot, plotArea, border_color, border_thickness_px);

    positionAxis.draw(imgPlot);
    timeAxis.draw(imgPlot);


    this->initialized = true;
    return imgPlot;
}

/*! Convert the SignalArray2D to a colored OpenCV Mat object.

	\param[in] 		tracedata pointer to SignalArray2D
    \return         OpenCV Mat object populated with colorized trace data

*/
Mat CrossSection::render(SignalArray2D tracedata, double step_size_m, double time_sampling_interval_ps)
{
    const Py_intptr_t *array_shape = tracedata.get_shape();
    int points_per_trace = array_shape[0];
    int number_of_traces =  array_shape[1];
    SIGNAL_DTYPE* raw_array = NdArrayToArray(&tracedata);
    return render(raw_array, points_per_trace, number_of_traces, step_size_m, time_sampling_interval_ps);
}

CrossSection::CrossSection() {
    positionAxis.xOffset_px = padLeftpx;
    positionAxis.yOffset_px = padToppx;
    timeAxis.xOffset_px = padLeftpx;
    timeAxis.yOffset_px = padToppx;
}

CrossSection::~CrossSection() {}

void CrossSection::setPadding_px(int left, int right, int top, int bottom) {
    this->padLeftpx = left;
    this->padRightpx = right;
    this->padToppx = top;
    this->padBottompx = bottom;

    positionAxis.xOffset_px = padLeftpx;
    positionAxis.yOffset_px = padToppx;
    timeAxis.xOffset_px = padLeftpx;
    timeAxis.yOffset_px = padToppx;
}

int CrossSection::getImageHeight()  { return points_per_trace + padToppx + padBottompx; }
int CrossSection::getImageWidth()   { return number_of_traces + padLeftpx + padRightpx; }
bool CrossSection::isInitialized() { return initialized; }