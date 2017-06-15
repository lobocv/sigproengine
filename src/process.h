#ifndef PROCESS_INCLUDED
#define PROCESS_INCLUDED


#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

namespace np = boost::python::numpy;
namespace bp = boost::python;

typedef double SIGNAL_DTYPE;

class Process {


    public:
        bool enabled = true;              // Is the process enabled
        bool isNode = false;              // Is the Process a node in the ProcessingChain

        void setup(bool enabled=true);
        virtual void joinChain(Process* p);

        // Getter methods
        virtual const char* getName();
        virtual bool isInitialized(Process* p);

        // Applying methods
        virtual void apply(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData);
        virtual void apply(SIGNAL_DTYPE* inData);
        virtual void apply(np::ndarray inData, np::ndarray outData);
        virtual void apply(np::ndarray inData);

        // Saving / Loading methods
        virtual bp::dict json_save();
        virtual void json_load(bp::dict params);

    protected:
        bool initialized = false;
        
        
};


#endif // PROCESS_INCLUDED