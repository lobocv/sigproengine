#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

#include "process.h"


namespace np = boost::python::numpy;
namespace bp = boost::python;

const char* Process::getName() {
    return "Un-named Process";
}


void Process::setup(bool enabled) {
    this->initialized = true;
    this->enabled = enabled;
}

void Process::joinChain(Process* p) {
    // Called just before a process joins another chain
}

bool Process::isInitialized(Process* p) {
    return this->initialized;
}


// Numpy array methods: Set points per trace is passed implicitly with the numpy array object
void Process::apply(np::ndarray inData, np::ndarray outData) {
    SIGNAL_DTYPE* inData_raw = reinterpret_cast<SIGNAL_DTYPE*>(inData.get_data());
    SIGNAL_DTYPE* outData_raw = reinterpret_cast<SIGNAL_DTYPE*>(outData.get_data());
    this->apply(inData_raw, outData_raw, len(inData));
}

// Overloadded method that uses the same array for in and out data
void Process::apply(np::ndarray inData) {
    return Process::apply(inData, inData);
}



SIGNAL_DTYPE* Process::apply(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData, int points_per_trace) {
    if ( points_per_trace > 0 ) {
        this->apply(inData, outData, points_per_trace);
    }    
    return inData;
}


SIGNAL_DTYPE* Process::apply(SIGNAL_DTYPE* inData,  int points_per_trace) {
    // Overridden method that uses the same array for in and out data
    return Process::apply(inData, inData, points_per_trace);
}


// void Process::apply(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData) {
    // Implementation of a Process will go here for derived classes
// }



bp::dict Process::json_save() {
    bp::dict params;
    params["name"] = this->getName();
    params["enabled"] = this->enabled;
    return params;
    }

void Process::json_load(bp::dict params) {
    this->setup(bp::extract<bool>(params.get("enabled", true)));
    }
