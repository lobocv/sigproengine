#/bin/bash

mkdir -p pylib lib
rm -v ./pylib/*.so
touch ./pylib/__init__.py
cp -v ../processing_lib/pylib/*.so ./pylib
cp -v ../processing_lib/lib/*.so ./lib
cp -v ../common_utils/lib/*.so ./lib
cp -v ../common_utils/pylib/*.so ./pylib
cp -v ../../devel/lib/*_py.so ./pylib
cp -vr ../3rdparty/np_opencv_module/build/np_opencv_module ./pylib

# Rename libsensor_network_py.so to sensor_network_py.so
(cd pylib && mv libsensor_network_py.so sensor_network_py.so)
