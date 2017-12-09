/*! \file sigpro_definitions.h
    \brief Typedefs for commonly used structures.
*/
#ifndef __SIGPRO_DEFINITIONS
#define __SIGPRO_DEFINITIONS


#include "boost/python.hpp"
#include "boost/python/numpy.hpp"

//#define __SSI_DEBUG__

#ifdef __SSI_DEBUG__

#define DEBUG_STDERR(x) (std::cerr << (x))
#define DEBUG_STDOUT(x) (std::cout << (x))
#define CDEBUG_PRINT(x) (printf x)


#else

#define DEBUG_STDERR(x)
#define DEBUG_STDOUT(x)
#define CDEBUG_PRINT(x)

#endif

namespace np = boost::python::numpy;

/*! \var typedef double SIGNAL_DTYPE
    \brief Data type of Radar data arrays.
*/
typedef double SIGNAL_DTYPE;
typedef float DT1_DTYPE;

enum class Orientation { horizontal, vertical };


typedef np::ndarray         SignalArray2D;

#endif