#include "domain_conversions.hpp"

#include <boost/python.hpp>


BOOST_PYTHON_MODULE(domain_conversions_py) {
    
        using namespace boost::python;
    
        Py_Initialize(),        

        def("TxTime2RxTime",        DepthTimeConversions::TxTime2RxTime); 
        def("RxTime2TxTime",        DepthTimeConversions::RxTime2TxTime);
        def("SampleIdx2TxTime",     DepthTimeConversions::SampleIdx2TxTime);
        def("TxTime2SampleIdx",     DepthTimeConversions::TxTime2SampleIdx);
        def("Depth2TxTime",         DepthTimeConversions::Depth2TxTime);
        def("Depth2RxTime",         DepthTimeConversions::Depth2RxTime);
        def("Depth2SampleIdx",      DepthTimeConversions::Depth2SampleIdx);
        def("TxTime2Depth",         DepthTimeConversions::TxTime2Depth);
        def("RxTime2Depth",         DepthTimeConversions::RxTime2Depth);
        def("SampleIdx2Depth",      DepthTimeConversions::SampleIdx2Depth);
        def("RxTime2SampleIdx",     DepthTimeConversions::RxTime2SampleIdx);
        def("SampleIdx2RxTime",     DepthTimeConversions::SampleIdx2RxTime);
        
    }