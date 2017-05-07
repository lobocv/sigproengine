#ifndef PROCESS_INCLUDED
#define PROCESS_INCLUDED

#include <boost/python/numpy.hpp>

namespace np = boost::python::numpy;
namespace bp = boost::python;


class Process {

    protected:
        bool initialized = false;

    public:
        void setup();
        virtual const char* getName();
        virtual bool isInitialized(Process* p);

};


#endif // PROCESS_INCLUDED