#	Author: Calvin Lobo
#	Date: September 7 2017
#
#
# Makefile Reference:
#
# 	wildcard *.cpp 							: Returns a list of files that match the pattern *.cpp
# 	patsubst src/%.cpp,bin/%.o,file.cpp 	: Replaces from "src/" with "/bin/" from input "src/file.cpp"
#   foreach d, $(INCLUDE), -I$d				: Adds -I in front of every element of $(INCLUDE)

CXX ?= g++

OPENCV_LIBS=-lopencv_core -lopencv_contrib

LIBS=/usr/lib

# We must include sensoft specific header files found in the common_utils repo
INCLUDES=include $(COMMON_UTILS_INCLUDE) $(PYTHON_INCLUDE) $(ROS_INCLUDE)
CFLAGS=-fPIC -std=c++11 --sysroot=$(SYSROOT)

BUILD_DIR=build
LIB_OUT_DIR=lib
PYLIB_OUT_DIR=pylib
SRC_DIR=src

GRAPHICS_SRC_DIR=src/graphics
PROCESSING_SRC_DIR=src/processing
PY_WRAPPERS_SRC_DIR=src/pythonwrappers

# Attach -I to each path in the INCLUDES
INCLUDE_PARAM = $(foreach d, $(INCLUDES), -I$d)

GRAPHICS_MODULES_SRC = $(wildcard $(GRAPHICS_SRC_DIR)/*.cpp)
GRAPHICS_MODULES_OBJ = $(patsubst $(GRAPHICS_SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(GRAPHICS_MODULES_SRC))
GRAPHICS_TARGET_LIB = libradarview.so
GRAPHICS_TARGET_PYLIB= radarview_py.so

PROCESSING_MODULES_SRC = $(wildcard $(PROCESSING_SRC_DIR)/*.c)

PROCESSING_MODULES_OBJ = $(patsubst $(PROCESSING_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(PROCESSING_MODULES_SRC))
PROCESSING_TARGET_LIB=libempath5.so
PROCESSING_TARGET_PYLIB=empath5_py.so

C_SRC = $(wildcard $(SRC_DIR)/*.c)
CPP_SRC = $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRC))
CPP_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(CPP_SRC))

all: directory_structure $(PROCESSING_TARGET_LIB) $(GRAPHICS_TARGET_LIB) $(PROCESSING_TARGET_PYLIB) $(GRAPHICS_TARGET_PYLIB)

processing: directory_structure $(PROCESSING_TARGET_LIB)

graphics: directory_structure $(GRAPHICS_TARGET_LIB)

pythonlibs: processingpy graphicspy

processingpy: directory_structure $(PROCESSING_TARGET_PYLIB)

graphicspy: directory_structure $(GRAPHICS_TARGET_PYLIB) 

# Create the output directory for the binaries
directory_structure:
	mkdir -p $(LIB_OUT_DIR) $(PYLIB_OUT_DIR) $(BUILD_DIR)
	touch $(PYLIB_OUT_DIR)/__init__.py

# Compile .c and .cpp files to object files
# Take all files matching the pattern src/*/%.c* and compile them into object files.
# Place the object files in the build directory with the same name but .o extension
$(BUILD_DIR)/%.o: src/*/%.c*
	$(CXX) $(CFLAGS) $(INCLUDE_PARAM) -c $< -o $@


# Create the executables
$(PROCESSING_TARGET_LIB): $(PROCESSING_MODULES_OBJ)
	$(CXX) $(CFLAGS) -o $(LIB_OUT_DIR)/$@ $^ -shared

$(PROCESSING_TARGET_PYLIB): $(PROCESSING_MODULES_OBJ) $(BUILD_DIR)/pyprocessing.o
	$(CXX) $(CFLAGS) -o $(PYLIB_OUT_DIR)/$@ $^ -shared -L$(COMMON_UTILS_LIB) -lboost_python -lpython2.7 -lboost_numpy -larray_manipulation


$(GRAPHICS_TARGET_LIB): $(GRAPHICS_MODULES_OBJ)
	$(CXX) $(CFLAGS) -o $(LIB_OUT_DIR)/$@ $^ -shared -L$(OPENCV_LIB) $(OPENCV_LIBS)

$(GRAPHICS_TARGET_PYLIB): $(GRAPHICS_MODULES_OBJ) $(BUILD_DIR)/pygraphics.o
	$(CXX) $(CFLAGS) -o $(PYLIB_OUT_DIR)/$@ $^ -shared -L$(OPENCV_LIB) $(OPENCV_LIBS) -L$(COMMON_UTILS_LIB) -lboost_python -lpython2.7 -lboost_numpy  -larray_manipulation


clean: 
	rm -rv $(LIB_OUT_DIR)/* $(PYLIB_OUT_DIR)/* $(BUILD_DIR)/*

.PHONY: clean