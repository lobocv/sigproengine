/*! \file axis.hpp
    \author Calvin Lobo
    \date September 2017
	\brief Drawing of general plot axis.
	
*/

#ifndef __AXIS_H__
#define __AXIS_H__

#include "sigpro_definitions.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;


enum TickType { major, minor };

typedef pair<TickType, float> TickDescriptor;
typedef vector<TickDescriptor> TickList;


TickList generateTicks(float minVal, float maxVal, float major_tick_dist, int n_minor_ticks);


class Axis {

protected:
    Orientation orientation = Orientation::horizontal;
    string unitSymbol = "";

public:

    float pixels_per_unit;
    float maxAxisValue;
    float minAxisValue;    
    float major_tick_dist;
    float n_minor_ticks;
    int lineThickness = 1;

    // Font parameters
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.4;
    Scalar fontColor = Scalar::all(255);
    int tickAxisPad_px = 8;
    int fontThickness = 1;

    // Offset in pixels where the minimum axis value is located (usually top left corner of plot)
    int xOffset_px;
    int yOffset_px;

    int tickSizeMajor_px = 10;
    int tickSizeMinor_px = 3;

    void draw(Mat &img);
    void set_orientation(Orientation orientation);
    inline float pixel_to_value(int pixel);
    inline int value_to_pixel(float value);
};


class TimeAxis : public Axis{

public:
	TimeAxis()
	{ this->unitSymbol = "ns";
		this->orientation = Orientation::vertical;
	};
};


class PositionAxis : public Axis {

public:
	PositionAxis()
	{ this->unitSymbol = "m";
		this->orientation = Orientation::horizontal;

	};
};


class AmplitudeAxis : public Axis {

public:
	AmplitudeAxis()
	{ this->unitSymbol = "mV";
		this->orientation = Orientation::vertical;

	};
};

#endif