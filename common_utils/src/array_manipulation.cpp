//
// Created by calvin on 10/11/17.
//


#include "sigpro_definitions.hpp"
#include "boost/python/numpy.hpp"

namespace np = boost::python::numpy;
namespace bp = boost::python;



/*! Convert the raw signal data array to a numpy::ndarray

    \param[in] 		trace_data          pointer to raw signal data array
    \param[in] 		points_per_trace    Number of sample points per trace
    \param[in] 		number_of_traces    Number of traces in the signal data array
    \return         numpy::ndarray representation of the raw array

*/
SignalArray2D arrayToNdArray(SIGNAL_DTYPE* trace_data, int points_per_trace, int number_of_traces)
{
	np::dtype dtype = np::dtype::get_builtin<SIGNAL_DTYPE>();
	bp::tuple shape = bp::make_tuple(points_per_trace, number_of_traces);
	bp::tuple stride = bp::make_tuple(sizeof(SIGNAL_DTYPE), points_per_trace * sizeof(SIGNAL_DTYPE));

	bp::object own;
	return np::from_data(trace_data, dtype, shape, stride, own);
}

/*! Convert the numpy::ndarray to raw signal data array

    \param[in] 		trace_data          signal data array as a boost::python::numpy::ndarray
    \return         raw array of signal data

*/
SIGNAL_DTYPE* NdArrayToArray(SignalArray2D* trace_data){
	return reinterpret_cast<SIGNAL_DTYPE*>( trace_data->get_data() );
}
