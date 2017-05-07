#include <stdio.h>
#include <iostream>


#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include "process.h"

namespace bp = boost::python;
namespace np = boost::python::numpy;

void func(void) {

    std::cout << "TEST FUNCTION STARTS\n" << std::endl;

    Gain g;

    const int N = 5;
    bp::tuple shape = bp::make_tuple(N);
    np::dtype dtype = np::dtype::get_builtin<float>();
    np::ndarray data = np::zeros(shape, dtype);


    for (int ii=0; ii<N; ii++) {
        data[ii] = ii;
    }

    g.setup(N, 10.0);
    g.apply(data);

    std::cout << bp::extract<char const *>(bp::str(data)) << std::endl;



    std::cout << "\nTEST FUNCTION ENDS" << std::endl;
};



BOOST_PYTHON_MODULE(sigproengine) {

    using namespace boost::python;

    Py_Initialize();
    np::initialize();



    class_<Gain>("Gain", init<>())
        .def("setup", &Gain::setup)
        .def("apply", &Gain::apply)
        ;

    def("func", func);
}
