#ifndef __TRACE_PLOT_H__
#define __TRACE_PLOT_H__

#include "sigpro_definitions.hpp"
#include "axis.hpp"
#include "boost/python/numpy.hpp"
#include <opencv2/opencv.hpp>


namespace np = boost::python::numpy;


class TracePlot
{

    public:
        int padLeftpx = 40;
        int padToppx = 20;
        int padRightpx = 10;
        int padBottompx = 10;
        cv::Scalar border_color = Scalar(255, 255, 255);
        int border_thickness_px = 1;

		Orientation orientation = Orientation::vertical;

		int plot_width = 250;
		int plot_height = 250;
        int getImageHeight();
        int getImageWidth();
        bool isInitialized();

        void setPadding_px(int left, int right, int top, int bottom);
		Mat render(SIGNAL_DTYPE* tracedata, int points_per_trace, double time_sampling_interval_ns, double* markers, int markers_length);
		Mat render(SignalArray2D tracedata, double time_sampling_interval_ps, np::ndarray markers);

        ~TracePlot();
        TracePlot();
        TimeAxis timeAxis;
        AmplitudeAxis amplitudeAxis;

    private:

        bool initialized = false;

        int points_per_trace = 0;
        double time_sampling_interval_ps = 0;
};

#endif // __TRACE_PLOT_H__