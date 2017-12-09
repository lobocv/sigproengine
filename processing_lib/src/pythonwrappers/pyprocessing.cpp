#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <iostream>

#include "processes.h"
#include "sigpro_definitions.hpp"
#include "array_manipulation.hpp"
#include "moving_average_filters.h"
#include "interpolate.h"


namespace np = boost::python::numpy;
using namespace std;


class CummulativeMovingAverageFilterPy : public CummulativeMovingAverageFilter {

public:
	CummulativeMovingAverageFilterPy() {
		init_CummulativeMovingAverageFilter(this);
	}

	double update(SIGNAL_DTYPE value) { return update_CummulativeMovingAverageFilter(this, value);}
};


class ExponentialMovingAverageFilterPy : public ExponentialMovingAverageFilter {

public:
	ExponentialMovingAverageFilterPy(double alpha) {
		init_ExponentialMovingAverageFilter(this, alpha);
	}

	double update(SIGNAL_DTYPE value) { return update_ExponentialMovingAverageFilter(this, value);}
};


void FindFirstBreakIndexPy(SignalArray2D dataIn, np::ndarray fb_indices, float threshold_mV)
{

	SIGNAL_DTYPE* dataIn_raw = NdArrayToArray(&dataIn);
	double* fb_indices_raw = reinterpret_cast<double*>(fb_indices.get_data());

	FindFirstBreakIndex(dataIn_raw, fb_indices_raw, dataIn.shape(0), dataIn.shape(1), threshold_mV);
}


double WaveletRMSPy(SignalArray2D dataIn, int start_idx, int end_idx)
{

	SIGNAL_DTYPE* dataIn_raw = NdArrayToArray(&dataIn);

	return WaveletRMS(dataIn_raw, dataIn.shape(1), start_idx, end_idx);
}


double AverageAmplitudePy(SignalArray2D dataIn, int window_length, int window_start_idx)
{
	SIGNAL_DTYPE* dataIn_raw = NdArrayToArray(&dataIn);

	return AverageAmplitude(dataIn_raw, dataIn.shape(0), window_length, window_start_idx);

}

void SimpleDCOffsetRemovalPy(SignalArray2D dataIn, SignalArray2D dataOut, int dc_window_length)
{
	SIGNAL_DTYPE* dataIn_raw = NdArrayToArray(&dataIn);
	SIGNAL_DTYPE* dataOut_raw = NdArrayToArray(&dataOut);

	SimpleDCOffsetRemoval(dataIn_raw, dataOut_raw, dataIn.shape(0), dataIn.shape(1), dc_window_length);
}


void EMA_DCOffsetRemovalPy(SignalArray2D dataIn, SignalArray2D dataOut, int dc_window_length, ExponentialMovingAverageFilterPy* filter)
{
	SIGNAL_DTYPE* dataIn_raw = NdArrayToArray(&dataIn);
	SIGNAL_DTYPE* dataOut_raw = NdArrayToArray(&dataOut);

	EMA_DCOffsetRemoval(dataIn_raw, dataOut_raw, dataIn.shape(0), dataIn.shape(1), dc_window_length, filter);
}

void CMA_DCOffsetRemovalPy(SignalArray2D dataIn, SignalArray2D dataOut, int dc_window_length, CummulativeMovingAverageFilterPy* filter)
{
	SIGNAL_DTYPE* dataIn_raw = NdArrayToArray(&dataIn);
	SIGNAL_DTYPE* dataOut_raw = NdArrayToArray(&dataOut);

	CMA_DCOffsetRemoval(dataIn_raw, dataOut_raw, dataIn.shape(0), dataIn.shape(1), dc_window_length, filter);
}


void InterpolateTracePy(SignalArray2D dataIn, SignalArray2D dataOut, int n_interp_points, std::string interp_type)
{
	SIGNAL_DTYPE* dataIn_raw = reinterpret_cast<SIGNAL_DTYPE*>(dataIn.get_data());
	SIGNAL_DTYPE* dataOut_raw = reinterpret_cast<SIGNAL_DTYPE*>(dataOut.get_data());

	interpolate_trace(dataIn_raw, dataOut_raw, dataIn.shape(0), dataIn.shape(1), n_interp_points, interp_type.c_str());
}

BOOST_PYTHON_MODULE(empath5_py)
{

    using namespace boost::python;

    Py_Initialize();
    np::initialize();


	def("FindFirstBreakIndex", &FindFirstBreakIndexPy);
	def("WaveletRMS", &WaveletRMSPy);
	def("AverageAmplitude", &AverageAmplitudePy);
	def("SimpleDCOffsetRemoval", &SimpleDCOffsetRemovalPy);
	def("EMA_DCOffsetRemoval", &EMA_DCOffsetRemovalPy);
	def("CMA_DCOffsetRemoval", &CMA_DCOffsetRemovalPy);
	def("InterpolateTrace", &InterpolateTracePy);
	def("get_interpolated_length", &get_interpolated_length);

	class_<CummulativeMovingAverageFilterPy>("CummulativeMovingAverageFilter", init<>())
	.def_readonly("average", &CummulativeMovingAverageFilterPy::average)
	.def_readonly("n_data_points", &CummulativeMovingAverageFilterPy::n_data_points)
	.def("update", &CummulativeMovingAverageFilterPy::update)
	;

	class_<ExponentialMovingAverageFilterPy>("ExponentialMovingAverageFilter", init<double>())
	.def_readonly("average", &ExponentialMovingAverageFilterPy::average)
	.def_readonly("alpha", &ExponentialMovingAverageFilterPy::alpha)
	.def("update", &ExponentialMovingAverageFilterPy::update)
	;

};