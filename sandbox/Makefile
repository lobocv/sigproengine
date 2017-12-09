#	Author: Calvin Lobo
#	Date: September 7 2017
#
#
# Makefile Reference:
#
# 	wildcard *.cpp 							: Returns a list of files that match the pattern *.cpp
# 	patsubst src/%.cpp,bin/%.o,file.cpp 	: Replaces from "src/" with "/bin/" from input "src/file.cpp"
#   foreach d, $(INCLUDE), -I$d				: Adds -I in front of every element of $(INCLUDE)

CXX?=g++

# Required libraries (-l flags)
CATKIN_LIBS=-lsensor_network
GRAPHICS_LIBS=-lradarview
PROCESSING_LIBS=-lempath5
ROS_LIBS=$(shell pkg-config sensor_network --libs-only-l)
OPENCV_LIBS=$(shell pkg-config opencv --libs-only-l)

INCLUDE = include $(ROS_INCLUDE) $(COMMON_UTILS_INCLUDE) $(PROCESSING_LIB_INCLUDE) $(SENSOR_NETWORK_INCLUDE) $(OPENCV_INCLUDE)
INCLUDE_PARAM = $(foreach d, $(INCLUDE), -I$d)

CFLAGS=-fPIC -Wall -std=c++11

BUILD_DIR=build
OUT_DIR=bin
SRC_DIR=src


C_SRC = $(wildcard $(SRC_DIR)/*.c)
CPP_SRC = $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRC))
CPP_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(CPP_SRC))

all: directory_structure display_radar_bag


# Create the output directory for the binaries
directory_structure:
	mkdir -p $(OUT_DIR) $(BUILD_DIR)

# Compile .c and .cpp files to object files
# Take all files matching the pattern src/%.c* and compile them into object files.
# Place the object files in the build directory with the same name but .o extension
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE_PARAM) -c $< -o $@


# Create the executables
display_radar_bag: $(BUILD_DIR)/display_radar_bag.o
	$(CXX) $(CFLAGS) -o $(OUT_DIR)/$@ $^ -L$(SENSOR_NETWORK_LIB) -L$(PROCESSING_LIB_LIB) -L$(ROS_LIB) -L$(OPENCV_LIB) $(PROCESSING_LIBS) $(CATKIN_LIBS) $(ROS_LIBS) $(GRAPHICS_LIBS) $(OPENCV_LIBS)



clean: 
	rm -rv $(OUT_DIR)/* $(BUILD_DIR)/*

test:
	echo $(COMMON_UTILS_INCLUDE)
.PHONY: clean
