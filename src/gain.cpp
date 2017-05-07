#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "gain.h"
#include "process.h"


namespace np = boost::python::numpy;

const char* Gain::getName() {
    return "Gain";
}


void Gain::setup(int points_per_trace, float gain) {
    this->gain = gain;
    this->points_per_trace = points_per_trace;
    Process::setup();
}


void Gain::apply(np::ndarray  data) {
    float g = this->gain;

    if (! Process::isInitialized(this)) {
        return;
    }

    for (int ii=0; ii < this->points_per_trace; ii++) {
        data[ii] *= g;
    }
}



