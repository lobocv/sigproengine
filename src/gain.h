#ifndef GAIN_INCLUDED
#define GAIN_INCLUDED

#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

#include "process.h"

namespace np = boost::python::numpy;
namespace bp = boost::python;


class Gain : public Process {

    public:
        virtual const char* getName();
        float gain = 1;
        int points_per_trace;

        void setup(int points_per_trace, float gain);
        void apply(np::ndarray data);
        boost::property_tree::ptree json_serialize();

};

#endif // GAIN_INCLUDED