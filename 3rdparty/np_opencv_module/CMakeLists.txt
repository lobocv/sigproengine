# Author: Sudeep Pillai (Sep 14, 2014)
project (numpy-opencv-converter)
cmake_minimum_required (VERSION 2.6.0)

# Include package config, boost-python
find_package(PkgConfig REQUIRED)
include(cmake/boost-python.cmake)

# configure opencv
pkg_check_modules(OpenCV opencv)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include python (use -D flags instead)
# Add a call to find_package requiring Python 2.7 explicitly, otherwise build fails trying to use Python 3 (if it's installed)
# Please see this issue: https://github.com/clemenscorny/brisk/issues/3
find_package(PythonLibs 2.7 REQUIRED)
SET(PYTHON_INCLUDE_DIRS "/usr/include/python2.7")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/np_opencv_module)

file(WRITE ${PROJECT_SOURCE_DIR}/build/np_opencv_module/__init__.py )

# Build np<=>opencv converter library
boost_python_module(np_opencv_converter np_opencv_converter.cpp utils/conversion.cpp)
target_link_libraries(np_opencv_converter boost_system boost_python ${OpenCV_LDFLAGS})

# Build test library
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
boost_python_module(np_opencv_module tests/np_opencv_module.cpp)
target_link_libraries(np_opencv_module boost_system boost_python np_opencv_converter ${OpenCV_LDFLAGS})
