#include <stdio.h>
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/refcount.hpp>

#include "process.h"
#include "processingchain.h"


namespace np = boost::python::numpy;
namespace bp = boost::python;


SIGNAL numpy_to_signal(np::ndarray np_array){
    return SIGNAL(reinterpret_cast<SIGNAL_DTYPE*>(np_array.get_data()));
}

np::ndarray signal_to_numpy(SIGNAL outData, int points_per_trace) {
    np::dtype dt = np::dtype::get_builtin<SIGNAL_DTYPE>();
    bp::tuple shape = bp::make_tuple(points_per_trace);
    bp::tuple stride = bp::make_tuple(sizeof(SIGNAL_DTYPE));
    bp::object own;

    return np::from_data(outData, dt, shape, stride, own);
}


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
    #ifdef DEBUG
    std::cout << "Clearing processing chain" << std::endl;
    #endif

    this->processes.clear();        // Is this a memory leak?
}


bp::list ProcessingChain::run(np::ndarray inData) {
    // bp::list outDataList;
    SIGNAL inData_raw = numpy_to_signal(inData);
    // outDataList.append(inData);
    
    return this->run(inData_raw, len(inData));
}


bp::list ProcessingChain::run(np::ndarray inData, np::ndarray outData) {

    SIGNAL inData_raw = numpy_to_signal(inData);
    SIGNAL outData_raw = numpy_to_signal(outData);
    return this->run(inData_raw, outData_raw, len(inData));
}


bp::list ProcessingChain::run(SIGNAL inData, int points_per_trace) {

    // Create a copy of the data
    #ifdef DEBUG
    std::cout << "Allocating new array" << std::endl;
    #endif
    SIGNAL copyData = new SIGNAL_DTYPE[points_per_trace];
    
    return this->run(inData, copyData, points_per_trace);
}

bp::list ProcessingChain::run(SIGNAL inData, SIGNAL outData, int points_per_trace) {
    bp::list outList;
    Process* p;

    #ifdef DEBUG
    std::cout << "Running ProcessingChain" << std::endl;
    #endif

    np::ndarray outData_np = signal_to_numpy(outData, points_per_trace);

    outList.append(outData_np);
    for (unsigned int ii=0; ii < this->processes.size(); ii++) {
        p = &(this->processes[ii].get());
        if (p->enabled) {
            if (p->isNode) {
                ProcessingChain* subChain = static_cast<ProcessingChain*>(p);
                bp::list subChain_outList = subChain->run(inData, points_per_trace);
                if ( len(subChain_outList) == 1 ) {
                    outList.append(subChain_outList[0]);
                } else {
                    outList.append(subChain_outList);
                }
                
            }
            else {
                p->apply(inData, outData, points_per_trace);
            }
            inData = outData;           // Output becomes the input for the next process
        }
    }

    return outList;
}

SIGNAL ProcessingChain::apply(SIGNAL inData, int points_per_trace) {
    // Dummy function, use run() instead.
    return this->apply(inData, inData, points_per_trace);
}


SIGNAL ProcessingChain::apply(SIGNAL inData, SIGNAL outData, int points_per_trace) {
    Process* p;
    ProcessingChain* subChain;
    bp::list subchain_outDataList;
    int nodeCount = 0;

    #ifdef DEBUG
    std::cout << "Calling Processing Chain Apply" << std::endl;
    #endif


    for (unsigned int ii=0; ii < this->processes.size(); ii++) {
        p = &(this->processes[ii].get());
        #ifdef DEBUG
        std::cout << "Attempting to call process : " << p->getName() << " isNode = " << p->isNode << std::endl;
        #endif

        if (p->enabled) {
            if ( p->isNode ) {

                // Create a copy of the data
                #ifdef DEBUG
                std::cout << "Copying Array" << std::endl;
                #endif
                SIGNAL copyData = new SIGNAL_DTYPE[points_per_trace];
                
                for (int jj=0; jj < points_per_trace; jj++) {
                    copyData[jj] = inData[jj];
                }
                subChain = static_cast<ProcessingChain*>(p);
                nodeCount += 1;
                subChain->apply(copyData, copyData, points_per_trace);

                #ifdef DEBUG
                std::cout << "Deleting copied array" << std::endl;
                #endif
                delete copyData;

            }
            else {
                
                p->apply(inData, outData, points_per_trace);
                inData = outData;           // Output becomes the input for the next process
            }
        }

    }

    #ifdef DEBUG
    std::cout << "Done Calling Processing Chain Apply" << std::endl;
    #endif
    return inData;
}

bp::dict ProcessingChain::json_save() {
    bp::dict chain, process;
    bp::list prolist;
    std::vector<std::reference_wrapper<Process>> processes = this->processes;


    for (unsigned int ii=0; ii < processes.size(); ii++) {
        process = processes[ii].get().json_save();
        prolist.append(process);
    }
    chain["processes"] = prolist;

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
          #ifdef DEBUG
          std::cout << "Creating process " << process_name << std::endl;
          #endif
          p = this->process_map[process_name]();
          #ifdef DEBUG
          std::cout << "Initializing process " << process_name << std::endl;
          #endif
          p->json_load(proparams);
          #ifdef DEBUG
          std::cout << "Adding process to chain" << std::endl;
          #endif
          this->add_process(p);
    }
}