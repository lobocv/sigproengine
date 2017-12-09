#!/bin/bash

HAS_READLINK=`which readlink`
if [ $? -eq 1 ]; then
        read -p "readlink command is not found, would you like to install it? (y/n)" yn

        if [ $yn == "y" ]; then
                echo "installing readlink.."
		apt-get install coreutils
        fi
fi

# Get the full path to the current 
ABS_CURRENT_DIR=$(readlink -e `dirname $0`)

#echo $REL_CURRENT_DIR
#echo $ABS_CURRENT_DIR

echo "Updating environment variables required for building"

export PYTHON_INCLUDE='=/usr/include/python2.7'
export USER_INCLUDE=/usr/local/include

export ROS_LIB='=/opt/ros/'$ROS_DISTRO'/lib'
export ROS_INCLUDE='=/opt/ros/'$ROS_DISTRO'/include'

export OPENCV_LIB='=/usr/lib/x86_64-linux-gnu'

export COMMON_UTILS_INCLUDE=$ABS_CURRENT_DIR/common_utils/include
export COMMON_UTILS_LIB=$ABS_CURRENT_DIR/common_utils/lib

export PROCESSING_LIB_INCLUDE=$ABS_CURRENT_DIR/processing_lib/include
export PROCESSING_LIB_LIB=$ABS_CURRENT_DIR/processing_lib/lib

export SENSOR_NETWORK_INCLUDE=$ABS_CURRENT_DIR/sensor_network/include
export SENSOR_NETWORK_LIB=$ABS_CURRENT_DIR/../devel/lib

export SANDBOX_WS=$ABS_CURRENT_DIR/sandbox



#################################################


export PYTHONPATH=$PYTHONPATH:$SANDBOX_WS/pylib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$COMMON_UTILS_LIB:$SANDBOX_WS/lib
