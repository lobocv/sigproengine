#ifndef __CROSS_SECTION_H__
#define __CROSS_SECTION_H__

#include "sigpro_definitions.hpp"
#include "axis.hpp"
#include "boost/python/numpy.hpp"
#include <opencv2/opencv.hpp>


namespace np = boost::python::numpy;


class CrossSection 
{

    public:
        int padLeftpx = 40;
        int padToppx = 20;
        int padRightpx = 10;
        int padBottompx = 10;
        cv::Scalar border_color = Scalar(255, 255, 255);
        int border_thickness_px = 1;

        cv::Mat render(SIGNAL_DTYPE* tracedata, int points_per_trace, int number_of_traces, double step_size_m, double time_sampling_interval_ps);
        cv::Mat render(SignalArray2D tracedata, double step_size_m, double time_sampling_interval_ps);

        void setPadding_px(int left, int right, int top, int bottom);
        int getImageHeight();
        int getImageWidth();
        bool isInitialized();

        CrossSection();
        ~CrossSection();
        PositionAxis positionAxis;
        TimeAxis timeAxis;

    private:

        bool initialized = false;

        int points_per_trace = 0;
        int number_of_traces = 0;
        double time_sampling_interval_ps = 0;
        double step_size_m = 0;
};

#endif