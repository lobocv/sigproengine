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



boost::property_tree::ptree Gain::json_save() {
    boost::property_tree::ptree root, params;

    params.put("points_per_trace", this->points_per_trace);
    params.put("gain", this->gain);
    params.put("enabled", this->enabled);
    root.add_child("Gain", params);

    return root;

}

void Gain::json_load(boost::property_tree::ptree params) {
    std::cout << "Loading Gain" << std::endl;
    this->setup(params.get<int>("points_per_trace"),
                params.get<float>("gain"));
    Process::json_load(params);

    }
