#include <stdio.h>
#include <iostream>
#include <sstream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "gain.h"
#include "process.h"


namespace np = boost::python::numpy;
namespace bp = boost::python;

const char* Gain::getName() {
    return "Gain";
}


void Gain::setup(float gain, bool enabled) {
    this->gain = gain;
    Process::setup(enabled);
}

void Gain::apply(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData, int points_per_trace) {
    float g = this->gain;
    if (! Process::isInitialized(this)) {
        return;
    }

    for (int ii=0; ii < points_per_trace; ii++) {
        outData[ii] = g * inData[ii];
    }
}



bp::dict Gain::json_save() {
    bp::dict params = Process::json_save();

    params["gain"] = this->gain;
    
    return params;

}

void Gain::json_load(bp::dict params) {
    std::cout << "Loading Gain" << std::endl;
    this->setup(bp::extract<float>(params["gain"]));
    Process::json_load(params);

    }
