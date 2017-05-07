#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "process.h"


namespace np = boost::python::numpy;


void Gain::setup(int points_per_trace, float gain) {
    this->gain = gain;
    this->points_per_trace = points_per_trace;
}

void Gain::apply(np::ndarray  data) {
    float g = this->gain;
    std::cout << "Applying " << this->name << std::endl;
    for (int ii=0; ii < this->points_per_trace; ii++) {
        data[ii] *= g;
    }
}



