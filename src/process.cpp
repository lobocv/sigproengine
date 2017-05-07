#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "process.h"


namespace np = boost::python::numpy;

const char* Process::getName() {
    return "Process";
}


void Process::setup() {
    this->initialized = true;
}

bool Process::isInitialized(Process* p) {
    if (! this->initialized) {
        std::cout << p->getName() << " is not initialized" << std::endl;
        return false;
        }
    else {
        std::cout << "Applying " << this->getName() << std::endl;
        return true;
    }
}



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



