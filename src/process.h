#ifndef PROCESS_INCLUDED
#define PROCESS_INCLUDED

#include <boost/python/numpy.hpp>

namespace np = boost::python::numpy;
namespace bp = boost::python;


class Gain {

    public:
        const char* name = "Gain";
        float gain;
        int points_per_trace;

        void setup(int points_per_trace, float gain);
        void apply(np::ndarray  data);

};

#endif // PROCESS_INCLUDED