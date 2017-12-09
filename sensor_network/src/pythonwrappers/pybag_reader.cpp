#include "sigpro_definitions.hpp"
#include "bag_reader.hpp"
#include "array_manipulation.hpp"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace np = boost::python::numpy;



BOOST_PYTHON_MODULE(sensor_network_py) {
    
        using namespace boost::python;
    
        Py_Initialize();
        np::initialize();

        // Since some functions are overloaded, we must specify to boost::python which exact method to export
        // Create pointers to the specific methods that are to be exposed. More than overloaded method can be exposed.
        SignalArray2D (*getRadarBuffer_ptr)(std::string, std::string) = &BagReader::getRadarBuffer;


        def("getRadarBuffer",        getRadarBuffer_ptr); 
       
    }