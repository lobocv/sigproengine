/*! \file array_manipulation.hpp
    \author Calvin Lobo
    \date September 2017
	\brief Conversion between Boost, NumPy and OpenCV representations of arrays
	
*/

#ifndef __ARRAY_MANIPULATION_DEFINED__
#define __ARRAY_MANIPULATION_DEFINED__

#include "sigpro_definitions.hpp"


SignalArray2D arrayToNdArray(SIGNAL_DTYPE* trace_data, int points_per_trace, int number_of_traces);

SIGNAL_DTYPE* NdArrayToArray(SignalArray2D* trace_data);

#endif