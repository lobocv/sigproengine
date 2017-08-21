#ifndef PROCESSINGCHAIN_INCLUDED
#define PROCESSINGCHAIN_INCLUDED

#include <map>
#include <boost/python/numpy.hpp>

#include "process.h"
#include "gain.h"

namespace np = boost::python::numpy;
namespace bp = boost::python;



typedef std::map<std::string, Process*(*)()> class_mapper;
//typedef std::map<std::string, int> class_mapper;


template<typename T> Process* createInstance() {
    return new T;
};



class ProcessingChain : public Process {

    public:
        ~ProcessingChain() = default;
        std::vector<std::reference_wrapper<Process>> processes;
        const char* getName();
        void add_process(Process* p);
        void joinChain(Process* p);
        void clear();
        bp::list run(np::ndarray inData);
        bp::list run(np::ndarray inData, np::ndarray outData);
        bp::list run(SIGNAL inData, SIGNAL outData, int points_per_trace);
        bp::list run(SIGNAL inData, int points_per_trace);
        SIGNAL apply(SIGNAL inData, int points_per_trace);
        SIGNAL apply(SIGNAL inData, SIGNAL outData, int points_per_trace);
        bp::dict json_save();
        void json_load(bp::dict params);

    private:
    
        class_mapper process_map = { {"Gain", &createInstance<Gain>} };

};


#endif // PROCESSINGCHAIN_INCLUDED