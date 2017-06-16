#ifndef GAIN_INCLUDED
#define GAIN_INCLUDED

#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

#include "process.h"

namespace np = boost::python::numpy;
namespace bp = boost::python;


class Gain : public Process {

    public:
        const char* getName();
        float gain = 1;

        void setup(float gain, bool enabled=true);
        void apply(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData);
        bp::dict json_save();
        void json_load(bp::dict params);

};

#endif // GAIN_INCLUDED