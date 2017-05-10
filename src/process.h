#ifndef PROCESS_INCLUDED
#define PROCESS_INCLUDED

#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

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
        virtual boost::property_tree::ptree json_serialize();


};


#endif // PROCESS_INCLUDED