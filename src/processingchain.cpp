#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>


#include "process.h"
#include "processingchain.h"


namespace np = boost::python::numpy;

void ProcessingChain::add_process(Process* p) {
    this->processes.push_back(std::ref(*p));
}

void ProcessingChain::apply(np::ndarray data) {
    std::vector<std::reference_wrapper<Process>> processes = this->processes;

    std::cout << "Calling Processing Chain Apply" << std::endl;

    for (unsigned int ii=0; ii < processes.size(); ii++) {
        processes[ii].get().apply(data);
    }

    std::cout << "Done Calling Processing Chain Apply" << std::endl;

}

void ProcessingChain::save() {}