echo "Making Common Utilities..."
echo "***********************************"
(cd common_utils && make -j all)
echo "***********************************"
echo "Making ROS libraries"
echo "***********************************"
(cd .. && catkin_make)
echo "***********************************"
echo "Making 3rd party dependencies"
echo "***********************************"
(cd 3rdparty/np_opencv_module && mkdir -p build && (cd build && cmake .. && make))
echo "***********************************"
echo "Making Processing Library..."
echo "***********************************"
(cd processing_lib && make -j all)
echo "***********************************"
echo "Setting up Sandbox"
echo "***********************************"
(cd sandbox && ./getlibs.sh)

