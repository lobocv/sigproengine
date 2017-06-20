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
    this->isNode = true;
}

void ProcessingChain::add_process(Process* p) {
    p->joinChain(this);   
    this->processes.push_back(std::ref(*p));
}

void ProcessingChain::clear() {
    std::cout << "Clearing processing chain" << std::endl;
    this->processes.clear();        // Is this a memory leak?
}


void ProcessingChain::apply(np::ndarray inData) {
    // bp::list outDataList;
    SIGNAL inData_raw = SIGNAL(reinterpret_cast<SIGNAL_DTYPE*>(inData.get_data()));
    // outDataList.append(inData);
    
    this->apply(inData_raw, inData_raw, len(inData));
    for (int i=0; i < len(inData); i++) {
        // std::cout << bp::extract<char const*>(inData[i]) << std::endl;
        // std::cout << inData_raw.get()[i]  << std::endl;
        // std::cout << "TEST" << std::endl;
    }
    std::cout << "NUMPY DONE " << std::endl;
}

void ProcessingChain::apply(np::ndarray inData, np::ndarray outData) {
    SIGNAL inData_raw = SIGNAL(reinterpret_cast<SIGNAL_DTYPE*>(inData.get_data()));
    SIGNAL outData_raw = SIGNAL(reinterpret_cast<SIGNAL_DTYPE*>(outData.get_data()));
    this->apply(inData_raw, outData_raw, len(inData));
}


SIGNAL ProcessingChain::apply(SIGNAL inData, int points_per_trace) {
    return this->apply(inData, inData, points_per_trace);
}


SIGNAL ProcessingChain::apply(SIGNAL inData, SIGNAL outData, int points_per_trace) {
    Process* p;
    ProcessingChain* subChain;
    bp::list subchain_outDataList;
    const char* process_name;
    int nodeCount = 0;

    std::cout << "Calling Processing Chain Apply" << std::endl;

    // np::ndarray outData = bp::extract<np::ndarray>(outDataList[0]);
    // SIGNAL outData_raw = SIGNAL(reinterpret_cast<SIGNAL_DTYPE*>(outData.get_data()));    


    for (unsigned int ii=0; ii < this->processes.size(); ii++) {
        p = &(this->processes[ii].get());
        process_name = p->getName();
        std::cout << "Attempting to call process : " << process_name << " isNode = " << p->isNode << std::endl;

        if (p->enabled) {
            if ( p->isNode ) {

                // Create a copy of the data
                std::cout << "Copying Array" << std::endl;
                SIGNAL copyData = new SIGNAL_DTYPE[points_per_trace];
                
                for (int jj=0; jj < points_per_trace; jj++) {
                    copyData[jj] = inData[jj];
                    std::cout << "Array " << jj << " " << copyData[jj] << std::endl;
                }
                std::cout << "Done Copying Array" << std::endl;
                subChain = static_cast<ProcessingChain*>(p);
                nodeCount += 1;
                subChain->apply(copyData, copyData, points_per_trace);

                bp::PyObject
                                
                for (int jj=0; jj < points_per_trace; jj++) {
                    std::cout << "AFTER Array " << jj << " " << copyData[jj] << std::endl;
                }
                delete copyData;

            }
            else {
                
                p->apply(inData, outData, points_per_trace);
                inData = outData;           // Output becomes the input for the next process
            }
        }

    }

    std::cout << "Done Calling Processing Chain Apply" << std::endl;
    return inData;
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