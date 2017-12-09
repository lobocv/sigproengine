#!/bin/bash

BOOST_URL="https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz"
BOOST_INSTALL_DIR=/usr/local
BOOST_INCLUDE=$BOOST_INSTALL_DIR/include/boost

ROS_INSTALL_DIR=/opt/ros/kinetic

echo "This script will set up the development environment. (Boost, OpenCV, Python)"

read -p "Would you like to continue? (y/n)" yn
if [ $yn != "y" ]; then
	exit
fi


## Dependencies ##
apt-get update
apt-get install build-essential python-pip


## BOOST ##

if [ ! -d $BOOST_INCLUDE ]; then
	if [ ! -e "/tmp/boost_1_65_1.tar.gz" ]; then
		echo "Downloading Boost libraries.."
		(cd /tmp && wget $BOOST_URL)
	fi

	if [ ! -d "/tmp/boost_1_65_1" ]; then
		echo "Extracting boost libraries..."
		(cd /tmp && tar xf boost_1_65_1.tar.gz)
	fi

	cd /tmp/boost_1_65_1
	./bootstrap.sh --prefix=$BOOST_INSTALL_DIR --with-python-version=2.7
	./b2 install
	echo "Done installing Boost libraries"
fi

## ROS ##

if [ ! -d ROS_INSTALL_DIR ]; then

	# If ROS build fails, try explicitly setting the release to xenial
        # replace the following line with: sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu xenial main" > /etc/apt/sources.list.d/ros-latest.list'
	sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
	sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
	sudo apt-get update
	sudo apt-get install -y ros-kinetic-desktop-full
	sudo rosdep init
	rosdep update

	echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
    source ~/.bashrc
fi

## OPENCV ##

if [ ! -d "/tmp/opencv-2.4.13.3" ]; then

    sudo apt-get install python2.7
    sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
    sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

    cd /tmp
    if [ ! -e "/tmp/opencv-2.4.13.3.zip" ]; then
        wget -O opencv-2.4.13.3.zip https://github.com/opencv/opencv/archive/2.4.13.3.zip
    fi
    unzip opencv-2.4.13.3.zip
    cd opencv-2.4.13.3
    mkdir release
    cd release
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
    make -j 2
    sudo make install

fi

(ln -s $SANDBOX_WS/sandbox /usr/local/bin && echo "Placing sandbox utility in path (/usr/local/bin)")

