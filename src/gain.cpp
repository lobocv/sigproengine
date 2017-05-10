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



std::string Gain::json_serialize() {
    boost::property_tree::ptree tree;
    std::stringstream s;
    std::string json_str;

    tree.put("points_per_trace", this->points_per_trace);
    tree.put("gain", this->gain);
    boost::property_tree::write_json(s, tree);

    json_str = s.str();

    return json_str;


}
