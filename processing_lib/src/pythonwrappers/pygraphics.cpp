#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "sigpro_definitions.hpp"
#include "cross_section.hpp"
#include "trace_plot.hpp"
#include "axis.hpp"
#include <opencv2/opencv.hpp>


namespace np = boost::python::numpy;
namespace bp = boost::python;



BOOST_PYTHON_MODULE(radar_view_py)
{

    using namespace boost::python;

    Py_Initialize();
    np::initialize();

	enum_<Orientation>("Orientation")
	.value("horizontal", Orientation::horizontal)
	.value("vertical", Orientation::vertical)
	;

    // Since some functions are overloaded, we must specify to boost::python which exact method to export
    // Create pointers to the specific methods that are to be exposed. More than overloaded method can be exposed.
    cv::Mat (CrossSection::*np_crosssection_render)(SignalArray2D, double, double) = &CrossSection::render;

    class_<CrossSection>("CrossSection", init<>())
    .def("render", np_crosssection_render)
    .def("setPadding_px", &CrossSection::setPadding_px)
    .def_readwrite("padLeftpx", &CrossSection::padLeftpx)
    .def_readwrite("padToppx", &CrossSection::padToppx)
    .def_readwrite("padRightpx", &CrossSection::padRightpx)
    .def_readwrite("padBottompx", &CrossSection::padBottompx)
    .def_readwrite("positionAxis", &CrossSection::positionAxis)
    .def_readwrite("timeAxis", &CrossSection::timeAxis)
    ;


	cv::Mat (TracePlot::*np_TracePlot_render)(SignalArray2D, double, np::ndarray) = &TracePlot::render;

    class_<TracePlot>("TracePlot", init<>())
	.def("render", np_TracePlot_render)
    .def("setPadding_px", &TracePlot::setPadding_px)
	.def_readwrite("orientation", &TracePlot::orientation)
	.def_readwrite("plot_width", &TracePlot::plot_width)
	.def_readwrite("plot_height", &TracePlot::plot_height)
	.def_readwrite("padLeftpx", &TracePlot::padLeftpx)
    .def_readwrite("padToppx", &TracePlot::padToppx)
    .def_readwrite("padRightpx", &TracePlot::padRightpx)
    .def_readwrite("padBottompx", &TracePlot::padBottompx)
    .def_readwrite("amplitudeAxis", &TracePlot::amplitudeAxis)
    .def_readwrite("timeAxis", &TracePlot::timeAxis)
    ;



    class_<TimeAxis>("TimeAxis", init<>())
    .def_readwrite("minAxisValue", &TimeAxis::minAxisValue)
    .def_readwrite("maxAxisValue", &TimeAxis::maxAxisValue)
    .def_readwrite("xOffset_px", &TimeAxis::xOffset_px)
    .def_readwrite("yOffset_px", &TimeAxis::yOffset_px)
    .def_readwrite("major_tick_dist", &TimeAxis::major_tick_dist)
    .def_readwrite("n_minor_ticks", &TimeAxis::n_minor_ticks)
    ;

    class_<PositionAxis>("PositionAxis", init<>())
    .def_readwrite("minAxisValue", &PositionAxis::minAxisValue)
    .def_readwrite("maxAxisValue", &PositionAxis::maxAxisValue)
    .def_readwrite("xOffset_px", &PositionAxis::xOffset_px)
    .def_readwrite("yOffset_px", &PositionAxis::yOffset_px)
    .def_readwrite("major_tick_dist", &PositionAxis::major_tick_dist)
    .def_readwrite("n_minor_ticks", &PositionAxis::n_minor_ticks)
    ;


    class_<AmplitudeAxis>("AmplitudeAxis", init<>())
    .def_readwrite("minAxisValue", &AmplitudeAxis::minAxisValue)
    .def_readwrite("maxAxisValue", &AmplitudeAxis::maxAxisValue)
    .def_readwrite("xOffset_px", &AmplitudeAxis::xOffset_px)
    .def_readwrite("yOffset_px", &AmplitudeAxis::yOffset_px)
    .def_readwrite("major_tick_dist", &AmplitudeAxis::major_tick_dist)
    .def_readwrite("n_minor_ticks", &AmplitudeAxis::n_minor_ticks)
    ;



//    float pixels_per_unit;
//    float maxAxisValue;
//    float minAxisValue;
//    float major_tick_dist;
//    float n_minor_ticks;
//    Scalar color = Scalar(255, 255, 255);
//    int lineThickness_px = 1;
//    int xOffset_px;
//    int yOffset_px;
//    int tickSizeMajor_px = 10;
//    int tickSizeMinor_px = 3;

}