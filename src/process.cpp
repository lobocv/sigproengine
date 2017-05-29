#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

#include "process.h"


namespace np = boost::python::numpy;

const char* Process::getName() {
    return "Un-named Process";
}


void Process::setup(bool enabled) {
    this->initialized = true;
    this->enabled = enabled;
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

void Process::apply(np::ndarray inData, np::ndarray outData) {
    this->apply(inData, outData);
}

void Process::apply(np::ndarray inData) {
    // Overridden method that uses the same array for in and out data
    return Process::apply(inData, inData);
}

boost::property_tree::ptree Process::json_save() {
    boost::property_tree::ptree tree;
    return tree;
    };

void Process::json_load(boost::property_tree::ptree params) {
    this->setup(params.get<bool>("enabled", true));
    };
