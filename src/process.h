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
        virtual void apply(np::ndarray data);
        virtual std::string json_serialize();


};


#endif // PROCESS_INCLUDED