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

void Process::apply(np::ndarray data) {}



boost::property_tree::ptree Process::json_serialize() {
    boost::property_tree::ptree tree;
    return tree;
    };
