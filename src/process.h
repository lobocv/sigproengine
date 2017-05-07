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


class Gain : public Process {

    public:
        const char* getName();
        float gain = 1;
        int points_per_trace;

        void setup(int points_per_trace, float gain);
        void apply(np::ndarray data);

};

#endif // PROCESS_INCLUDED