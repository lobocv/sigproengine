/*! \file resizing_rect_window.h
	\brief Filter/windowing methods
    \author Calvin Lobo
    \date Oct 3rd 2017
*/

#ifndef SRC_FILTER_H
#define SRC_FILTER_H


#include "sigpro_definitions.hpp"


struct ResizingRectWindow {
    int size;
    int filled_size;
    int points_per_trace;
    int index;
    SIGNAL_DTYPE sum;
    SIGNAL_DTYPE* elements;
};

struct ResizingRectWindow* new_ResizingRectWindow(int size, int points_per_trace);
void destroy_ResizingRectWindow(struct ResizingRectWindow *window);

void setup(ResizingRectWindow* window, int size);

void push_back(ResizingRectWindow* window, SIGNAL_DTYPE value);

void clear(ResizingRectWindow* window);

double mean(ResizingRectWindow* window);

#endif //SRC_FILTER_H
