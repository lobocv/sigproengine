#	Author: Calvin Lobo
#	Date: September 7 2017
#
#
# Makefile Reference:
#
# 	wildcard *.cpp 							: Returns a list of files that match the pattern *.cpp
# 	patsubst src/%.cpp,bin/%.o,file.cpp 	: Replaces from "src/" with "/bin/" from input "src/file.cpp"
#   foreach d, $(INCLUDE), -I$d				: Adds -I in front of every element of $(INCLUDE)

CCX?=g++

# We must include sensoft specific header files found in the common_utils repo
INCLUDES=include $(PYTHON_INCLUDE) $(BOOST_INCLUDE)
CFLAGS=-fPIC --sysroot=$(SYSROOT) -std=c++11

BUILD_DIR=build
LIB_OUT_DIR=lib
PYLIB_OUT_DIR=pylib
SRC_DIR=src

PY_WRAPPERS_SRC_DIR=$(SRC_DIR)/pythonwrappers

# Attach -I to each path in the INCLUDES
INCLUDE_PARAM = $(foreach d, $(INCLUDES), -I$d)

PYWRAPPERS_MODULES_SRC = $(wildcard $(PY_WRAPPERS_SRC_DIR)/*.c)
PYWRAPPERS_MODULES_OBJ = $(patsubst $(PY_WRAPPERS_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(PY_WRAPPERS_SRC_DIR))


all: directory_structure sigpro_constants_py.so libarray_manipulation.so


# Create the output directory for the binaries
directory_structure:
	mkdir -p $(LIB_OUT_DIR) $(PYLIB_OUT_DIR) $(BUILD_DIR)
	touch $(PYLIB_OUT_DIR)/__init__.py

# Compile .c and .cpp files to object files
# Take all files matching the pattern src/*/%.c* and compile them into object files.
# Place the object files in the build directory with the same name but .o extension
$(BUILD_DIR)/%.o: src/*/%.c*
	$(CXX) $(CFLAGS) $(INCLUDE_PARAM) -c $< -o $@
$(BUILD_DIR)/%.o: src/%.c*
	$(CXX) $(CFLAGS) $(INCLUDE_PARAM) -c $< -o $@


# Create the executables
%_py.so: $(BUILD_DIR)/py_%.o
	$(CXX) $(CFLAGS) -o $(PYLIB_OUT_DIR)/$@ $^ -shared -lboost_python -lpython2.7 -lboost_numpy

libarray_manipulation.so: $(BUILD_DIR)/array_manipulation.o
	$(CXX) $(CFLAGS) -o $(LIB_OUT_DIR)/$@ $^ -shared -lpython2.7 -lboost_numpy

clean: 
	rm -rv $(LIB_OUT_DIR)/* $(PYLIB_OUT_DIR)/* $(BUILD_DIR)/*

.PHONY: clean