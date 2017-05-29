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


void Gain::setup(int points_per_trace, float gain, bool enabled) {
    this->gain = gain;
    this->points_per_trace = points_per_trace;
    Process::setup(enabled);
}

void Gain::apply(np::ndarray inData, np::ndarray outData) {
    float g = this->gain;

    if (! Process::isInitialized(this)) {
        return;
    }

    for (int ii=0; ii < this->points_per_trace; ii++) {
        outData[ii] = g * inData[ii];
    }
}



bp::dict Gain::json_save() {
    bp::dict root, params;

    params["points_per_trace"] = this->points_per_trace;
    params["gain"] = this->gain;
    params["enabled"] = this->enabled;
    root["Gain"] = params;

    return root;

}

void Gain::json_load(bp::dict params) {
    std::cout << "Loading Gain" << std::endl;
    this->setup(bp::extract<int>(params["points_per_trace"]),
                bp::extract<float>(params["gain"])
                );
    Process::json_load(params);

    }
