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
        bool enabled = true;

        void setup(bool enabled=true);
        virtual const char* getName();
        virtual bool isInitialized(Process* p);
        virtual void apply(np::ndarray inData, np::ndarray outData);
        virtual void apply(np::ndarray inData);
        virtual bp::dict json_save();
        virtual void json_load(bp::dict params);


};


#endif // PROCESS_INCLUDED