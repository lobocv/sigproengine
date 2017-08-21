#ifndef GAIN_INCLUDED
#define GAIN_INCLUDED

#include <boost/python/numpy.hpp>
#include "process.h"


namespace np = boost::python::numpy;
namespace bp = boost::python;


class Gain : public Process {

    public:
        ~Gain() = default;
        const char* getName();    
        float gain = 1;

        void setup(float gain, bool enabled=true);
        SIGNAL apply(SIGNAL inData, SIGNAL outData, int points_per_trace);
        bp::dict json_save();
        void json_load(bp::dict params);

};

#endif // GAIN_INCLUDED