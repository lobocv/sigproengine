/*! \file axis.cpp
    \author Calvin Lobo
    \date September 2017
	\brief Drawing of general plot axis.
	
*/


#include "axis.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <sstream>
using namespace std;
using namespace cv;



TickList generateTicks(float minVal, float maxVal, float major_tick_dist, int n_minor_ticks) 
{
    TickList tickList;
    TickDescriptor tick;

    // Compute the distance between minor ticks, given the number of minor ticks we want between major ticks.
    float minor_tick_dist = ( n_minor_ticks > 0 ) ? (major_tick_dist / n_minor_ticks) : major_tick_dist;

    // Determine the first tick (either major or minor)
    float tickVal = ceil(minVal / minor_tick_dist) * minor_tick_dist;

    // Iterate through at minor tick intervals, determine if the tick was
    // major or minor and then add it to the vector.
    do {
        tick.first = (fmod(tickVal, major_tick_dist) == 0) ? major : minor;
        tick.second = tickVal;
        // Add the tick to the list and move onto the next one
        tickList.push_back(tick);
        tickVal += minor_tick_dist;
    } while ( tickVal < maxVal );

    return tickList;
};



void Axis::draw(Mat &img) 
{
    TickList tickList = generateTicks(this->minAxisValue, this->maxAxisValue, this->major_tick_dist, this->n_minor_ticks);

    TickList::iterator it;
    TickDescriptor tick;
    Point innerPoint, outerPoint;
    stringstream tick_name;
    Size text_size;
    double tick_value;
    int tick_height;
    int xtextOffset_px, ytextOffset_px;

    for (it = tickList.begin(); it != tickList.end(); it++) {
        // Iterate through the list of ticks values and convert them to pixel positions of a line segment
        // Inner point refers to the point on the line tick that is closer to the center
        tick = *it;
        tick_height = (tick.first == major) ? tickSizeMajor_px : tickSizeMinor_px;
        tick_value = tick.second;
        // Set inner and outer points of line segment
        if (orientation == Orientation::horizontal) {
            innerPoint.x = (tick_value - minAxisValue) * pixels_per_unit + xOffset_px;
            innerPoint.y = yOffset_px;
            outerPoint.x = (tick_value - minAxisValue) * pixels_per_unit+ xOffset_px;
            outerPoint.y = yOffset_px + tick_height;
        } else {
            innerPoint.x = xOffset_px + tick_height; 
            innerPoint.y = (tick_value - minAxisValue) * pixels_per_unit + yOffset_px;
            outerPoint.x = xOffset_px;
            outerPoint.y = (tick_value - minAxisValue) * pixels_per_unit+ yOffset_px;
        }

        // Draw the line
        line(img, innerPoint, outerPoint, fontColor, lineThickness);

        if (tick.first == major) {
            // Draw text for only the major ticks.
            tick_name << tick.second << unitSymbol;
            // Calculate the text size so we can center it on the tick line.
            text_size = getTextSize(tick_name.str(), fontFace, fontScale, fontThickness, 0);
            if (orientation == Orientation::horizontal) {
                xtextOffset_px = -text_size.width / 2;
                ytextOffset_px = -(text_size.height + tickAxisPad_px);
            } else {
                xtextOffset_px = -(text_size.width + tickAxisPad_px);
                ytextOffset_px = text_size.height / 2;
            }

            putText(img, tick_name.str(), outerPoint + Point(xtextOffset_px, ytextOffset_px),
                    fontFace, fontScale, Scalar::all(255), fontThickness, CV_AA);
            // Clear the string stream
            tick_name.str(string());
        }
    }

}

void Axis::set_orientation(Orientation orientation) {
    this->orientation = orientation;
}

inline float Axis::pixel_to_value(int pixel) {return pixel / pixels_per_unit;}
inline int Axis::value_to_pixel(float value) {return value * pixels_per_unit;}