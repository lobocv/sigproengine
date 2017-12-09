#include "sigpro_constants.h"

#include <boost/python.hpp>

// Macro for defining and exposing constant values via Boost Python
#define DEFINE_CONST(X) scope().attr(#X) = X

BOOST_PYTHON_MODULE(sigpro_constants_py) {
    
        using namespace boost::python;
    
        Py_Initialize();       

        DEFINE_CONST(SIGNAL_AMPLITUDE_MAX);
        DEFINE_CONST(SIGNAL_AMPLITUDE_MMIN);
        DEFINE_CONST(LIGHTSPEED_MPNS);
        
    }