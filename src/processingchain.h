#ifndef PROCESSINGCHAIN_INCLUDED
#define PROCESSINGCHAIN_INCLUDED

#include <boost/python/numpy.hpp>
#include "process.h"

namespace np = boost::python::numpy;
namespace bp = boost::python;


class ProcessingChain {

    public:
        std::vector<Process*> processes;

        void add_process(Process* p);


};


#endif // PROCESSINGCHAIN_INCLUDED