#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "process.h"
#include "processingchain.h"


namespace np = boost::python::numpy;

void ProcessingChain::add_process(Process* p) {
    this->processes.push_back(p);
}