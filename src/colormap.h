#ifndef COLORMAP_INCLUDED
#define COLORMAP_INCLUDED

#include <boost/python/numpy.hpp>
#include <boost/property_tree/ptree.hpp>

#include "process.h"

namespace np = boost::python::numpy;
namespace bp = boost::python;


class ColorMap : public Process {

    public:
        const char* getName();
        std::string filepath;
        int points_per_trace;

        void setup(int points_per_trace, std::string filepath, bool enabled=true);
        void apply(np::ndarray inData, np::ndarray outData);
        bp::dict json_save();
        void json_load(bp::dict params);

};

#endif // COLORMAP_INCLUDED