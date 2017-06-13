#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "colormap.h"
#include "process.h"


namespace np = boost::python::numpy;
namespace bp = boost::python;

const char* ColorMap::getName() {
    return "ColorMap";
}


void ColorMap::setup(int points_per_trace, std::string filepath, bool enabled) {
    std::ifstream mapfile (filepath);
    std::string line;

    this->filepath = filepath;
    this->points_per_trace = points_per_trace;
    
    if (mapfile.is_open()) {
        while ( getline(mapfile, line) ) {
            std::cout << line << std::endl;
        }
        mapfile.close();
    } else {
        std::cout << "Unable to open colormap file at " << filepath << std::endl;
    }
    Process::setup(enabled);
}

void ColorMap::apply(np::ndarray inData, np::ndarray outData) {

    if (! Process::isInitialized(this)) {
        return;
    }

}



bp::dict ColorMap::json_save() {
    bp::dict params = Process::json_save();

    params["points_per_trace"] = this->points_per_trace;
    params["filepath"] = this->filepath;
    
    return params;

}

void ColorMap::json_load(bp::dict params) {
    std::cout << "Loading ColorMap" << std::endl;
    this->setup(bp::extract<int>(params["points_per_trace"]),
                bp::extract<std::string>(params["filepath"])
                );
    Process::json_load(params);

    }