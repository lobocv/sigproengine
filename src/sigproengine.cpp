#include <stdio.h>
#include <iostream>


#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "gain.h"
#include "processingchain.h"


namespace bp = boost::python;
namespace np = boost::python::numpy;

void func(void) {
    const int N = 5;

    std::cout << "TEST FUNCTION STARTS\n" << std::endl;


    Gain g;
    Gain G;
    g.setup(N, 10.0);

    ProcessingChain pc;
    pc.add_process(&g);


    bp::tuple shape = bp::make_tuple(N);
    np::dtype dtype = np::dtype::get_builtin<float>();
    np::ndarray data = np::zeros(shape, dtype);


    for (int ii=0; ii<N; ii++) {
        data[ii] = ii;
    }


    pc.apply(data);

    std::cout << bp::extract<char const *>(bp::str(data)) << std::endl;



    std::cout << "\nTEST FUNCTION ENDS" << std::endl;
};



BOOST_PYTHON_MODULE(sigproengine) {

    using namespace boost::python;

    Py_Initialize();
    np::initialize();



    class_<Process>("Process", init<>())
        ;

    class_<Gain, bases<Process> >("Gain", init<>())
        .def("setup", &Gain::setup)
        .def("apply", &Gain::apply)
        ;


    class_<ProcessingChain>("ProcessingChain", init<>())
        .def("add_process", &ProcessingChain::add_process)
        .def("apply", &ProcessingChain::apply)
        ;

    def("func", func);
}
