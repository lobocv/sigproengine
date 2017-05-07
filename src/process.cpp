#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

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

