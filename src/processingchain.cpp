#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "process.h"
#include "processingchain.h"


namespace np = boost::python::numpy;
namespace bp = boost::python;


const char* ProcessingChain::getName() {
    return "ProcessingChain";
}


void ProcessingChain::joinChain(Process* p) {
    std::cout << "joingChain" << std::endl;
    this->isNode = true;
}

void ProcessingChain::add_process(Process* p) {
    std::cout << "add_process" << std::endl;
    p->joinChain(this);   
    this->processes.push_back(std::ref(*p));
}

void ProcessingChain::clear() {
    std::cout << "Clearing processing chain" << std::endl;
    this->processes.clear();        // Is this a memory leak?
}


void ProcessingChain::apply(np::ndarray inData) {
    bp::list outDataList;
    outDataList.append(inData);
    this->apply(inData, outDataList);
}

void ProcessingChain::apply(np::ndarray inData, bp::list outDataList) {
    Process* p;
    ProcessingChain* subChain;
    bp::list subchain_outDataList;
    const char* process_name;
    int nodeCount = 0;

    std::cout << "Calling Processing Chain Apply" << std::endl;

    np::ndarray outData = bp::extract<np::ndarray>(outDataList[0]);

    for (unsigned int ii=0; ii < this->processes.size(); ii++) {
        p = &(this->processes[ii].get());
        process_name = p->getName();
        std::cout << "Attempting to call process : " << process_name << " isNode = " << p->isNode << std::endl;

        if (p->enabled) {
            if ( p->isNode ) {
                subChain = static_cast<ProcessingChain*>(p);
                nodeCount += 1;
                subchain_outDataList = bp::extract<bp::list>(outDataList[nodeCount]);
                subChain->apply(inData, subchain_outDataList);
            }
            else {
                p->apply(inData, outData);
                inData = outData;           // Output becomes the input for the next process
            }
        }

    }

    std::cout << "Done Calling Processing Chain Apply" << std::endl;

}

bp::dict ProcessingChain::json_save() {
    bp::dict chain, process;
    bp::list prolist;
    std::vector<std::reference_wrapper<Process>> processes = this->processes;

    std::cout << "Calling Processing Chain JSON Serialize" << std::endl;

    for (unsigned int ii=0; ii < processes.size(); ii++) {
        process = processes[ii].get().json_save();
        prolist.append(process);
    }
    chain["processes"] = prolist;

    std::cout << "Done Calling Processing Chain JSON Serialize" << std::endl;
    return chain;

}

void ProcessingChain::json_load(bp::dict json) {
    std::stringstream ss, sstream;
    std::string process_name;
    boost::property_tree::ptree root, process, parameters;
    Process* p;

    bp::list prolist;
    bp::dict prodict, proparams;

    prolist = bp::extract<bp::list>(json["processes"]);
    

    for (int ii=0; ii < len(prolist); ii++) {
          proparams = bp::extract<bp::dict>(prolist[ii]);
          process_name = bp::extract<std::string>(proparams["name"]);
          std::cout << "Creating process " << process_name << std::endl;
          p = this->process_map[process_name]();
          std::cout << "Initializing process " << process_name << std::endl;
          p->json_load(proparams);
          std::cout << "Adding process to chain" << std::endl;
          this->add_process(p);
    }
}