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

void Process::apply(np::ndarray inData, np::ndarray outData) {
    this->apply(inData, outData);
}

void Process::apply(np::ndarray inData) {
    // Overridden method that uses the same array for in and out data
    return Process::apply(inData, inData);
}

bp::dict Process::json_save() {
    bp::dict tree;
    return tree;
    }

void Process::json_load(bp::dict params) {
    this->setup(bp::extract<bool>(params.get("enabled", true)));
    }
