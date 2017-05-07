#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace bp = boost::python;
namespace bn = boost::python::numpy;


class Gain {


    public:
        float gain;
        int points_per_trace;

        void setup(int points_per_trace, float gain) {
            this->gain = gain;
            this->points_per_trace = points_per_trace;
        }

        void apply(float data[]) {
            for (int ii=0; ii < this->points_per_trace; ii++) {
                data[ii] *= this->gain;
            }
        }

};

void func(void) {

    std::cout << "TEST" << std::endl;

    Gain g;
    const int N = 5;

    float data[N] = {1, 2, 3, 4, 5};

    g.setup(N, 2.0);
    g.apply(data);

    for (int ii=0; ii<N; ii++) {
        std::cout << data[ii] << std::endl;
    }

};


BOOST_PYTHON_MODULE(sigproengine) {

    bn::initialize();

    bp::def("func", func);
}
