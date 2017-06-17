#include <stdio.h>
#include <iostream>
#include <map>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "gain.h"
#include "colormap.h"
#include "processingchain.h"


namespace bp = boost::python;
namespace np = boost::python::numpy;




void func(void) {
//    const int N = 5;
//
//    std::cout << "TEST FUNCTION STARTS\n" << std::endl;
//
//
//    Gain g;
//    Gain G;
//    g.setup(N, 10.0);
//
//    ProcessingChain pc;
//    pc.add_process(&g);
//
//
//    bp::tuple shape = bp::make_tuple(N);
//    np::dtype dtype = np::dtype::get_builtin<float>();
//    np::ndarray data = np::zeros(shape, dtype);
//
//
//    for (int ii=0; ii<N; ii++) {
//        data[ii] = ii;
//    }
//
//
//    pc.apply(data, data);
//
//    std::cout << bp::extract<char const *>(bp::str(data)) << std::endl;
//
//
//
//    std::cout << "\nTEST FUNCTION ENDS" << std::endl;
};



BOOST_PYTHON_MODULE(sigproengine) {

    using namespace boost::python;

    Py_Initialize();
    np::initialize();

    // Pointer to apply functions in the base class
    void (Process::*ptr_numpy_apply_to)(np::ndarray, np::ndarray) = &Process::apply;
    void (Process::*ptr_numpy_apply)(np::ndarray) = &Process::apply;
    void (Process::*ptr_apply)(SIGNAL_DTYPE*, int) = &Process::apply;
    

    class_<Process>("Process", init<>())
    ;


    // To expose overloaded functions, we must create pointers to each overloaded function and pass them to .def()
    void (ProcessingChain::*ptr_apply_to)(np::ndarray, bp::list) = &ProcessingChain::apply;
//    void (ProcessingChain::*ptr_pc_apply)(np::ndarray, bp::list) = &ProcessingChain::apply;

    class_<ProcessingChain, bases<Process>>("ProcessingChain", init<>())
        .def("add_process", &ProcessingChain::add_process)
        .def("json_save", &ProcessingChain::json_save)
        .def("json_load", &ProcessingChain::json_load)
        .def("apply", ptr_numpy_apply_to)
        .def("apply", ptr_numpy_apply)
        .def("apply", ptr_apply_to)
        .def("apply", ptr_apply)
        .def("clear", &ProcessingChain::clear)
        ;


    void (Gain::*ptr_gain_apply_to)(SIGNAL_DTYPE*, SIGNAL_DTYPE*, int) = &Gain::apply;


    class_<Gain, bases<Process> >("Gain", init<>())
        .def("setup", &Gain::setup)
        .def("apply", ptr_gain_apply_to)
        .def("apply", ptr_numpy_apply_to)
        .def("apply", ptr_numpy_apply)
        .def("apply", ptr_apply)
        .def("json_save", &Gain::json_save)
        .def_readwrite("enabled", &Gain::enabled)
        .def_readonly("gain", &Gain::gain)
        ;

    void (ColorMap::*ptr_colormap_apply_to)(np::ndarray, np::ndarray) = &ColorMap::apply;

    class_<ColorMap, bases<Process> >("ColorMap", init<>())
        .def("setup", &ColorMap::setup)
        .def("apply", ptr_colormap_apply_to)
        .def("apply", ptr_apply)
        .def("json_save", &ColorMap::json_save)
        .def_readwrite("enabled", &ColorMap::enabled)
        .def_readonly("filepath", &ColorMap::filepath)
        ;

    def("func", func);
}
