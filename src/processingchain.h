#ifndef PROCESSINGCHAIN_INCLUDED
#define PROCESSINGCHAIN_INCLUDED

#include <boost/python/numpy.hpp>

#include "process.h"

namespace np = boost::python::numpy;
namespace bp = boost::python;


class ProcessingChain {

    public:
        std::vector<std::reference_wrapper<Process>> processes;

        void add_process(Process* p);
        void apply(np::ndarray data);
        std::string json_serialize();
};


#endif // PROCESSINGCHAIN_INCLUDED