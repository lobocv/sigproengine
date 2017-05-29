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
        std::vector<std::reference_wrapper<Process>> processes;

        void add_process(Process* p);
        void clear();
        void apply(np::ndarray inData, np::ndarray outData);
        bp::dict json_save();
        void json_load(bp::dict params);

    private:

        class_mapper process_map = { {"Gain", &createInstance<Gain>} };

};


#endif // PROCESSINGCHAIN_INCLUDED