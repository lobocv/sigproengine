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

void ProcessingChain::add_process(Process* p) {
    this->processes.push_back(std::ref(*p));
}

void ProcessingChain::apply(np::ndarray data) {
    Process* p;
    std::vector<std::reference_wrapper<Process>> processes = this->processes;

    std::cout << "Calling Processing Chain Apply" << std::endl;

    for (unsigned int ii=0; ii < processes.size(); ii++) {
        p = &processes[ii].get();
        std::cout << "Enabled " << p->enabled << std::endl;
        if (p->enabled) {
            p->apply(data);
        }
    }

    std::cout << "Done Calling Processing Chain Apply" << std::endl;

}

std::string ProcessingChain::json_save() {
    boost::property_tree::ptree chain, process, prolist;
    std::vector<std::reference_wrapper<Process>> processes = this->processes;
    std::string json;
    std::stringstream sstream;

    std::cout << "Calling Processing Chain JSON Serialize" << std::endl;

    for (unsigned int ii=0; ii < processes.size(); ii++) {
        process = processes[ii].get().json_save();
        prolist.push_back(std::make_pair("", process));
    }
    chain.add_child("processes", prolist);

    boost::property_tree::write_json(sstream, chain);
    json = sstream.str();

    std::cout << "Done Calling Processing Chain JSON Serialize" << std::endl;
    return json;

}

void ProcessingChain::json_load(std::string json) {
    std::stringstream ss, sstream;
    std::string process_name;
    boost::property_tree::ptree root, process, parameters;
    Process* p;

    // dump the string into a stream and we can read it into a property tree
    ss << json;
    boost::property_tree::read_json(ss, root);

    // Iterate through each process in the "processes" list
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, root.get_child("processes.")) {
          process = v.second;

         // Read in the process as a ptree
          BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, process){
              process_name = v2.first;
              parameters = v2.second;

              std::cout << "Creating process " << process_name << std::endl;
              p = this->process_map[process_name]();
              std::cout << "Initializing process " << process_name << std::endl;
              p->json_load(parameters);
              std::cout << "Adding process to chain" << std::endl;
              this->add_process(p);

          }


    }
}