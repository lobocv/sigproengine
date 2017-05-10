#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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

std::string ProcessingChain::json_serialize() {
    boost::property_tree::ptree chain, process, prolist;
    std::vector<std::reference_wrapper<Process>> processes = this->processes;
    std::string json;
    std::stringstream sstream;

    std::cout << "Calling Processing Chain JSON Serialize" << std::endl;

    for (unsigned int ii=0; ii < processes.size(); ii++) {
        process = processes[ii].get().json_serialize();
        prolist.push_back(std::make_pair("", process));
    }
    chain.add_child("processes", prolist);

    boost::property_tree::write_json(sstream, chain);
    json = sstream.str();

    std::cout << "Done Calling Processing Chain JSON Serialize" << std::endl;
    return json;

}