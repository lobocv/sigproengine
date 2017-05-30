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
        int points_per_trace;

        void setup(int points_per_trace, float gain, bool enabled=true);
        void apply(np::ndarray inData, np::ndarray outData);
        bp::dict json_save();
        void json_load(bp::dict params);

};

#endif // GAIN_INCLUDED