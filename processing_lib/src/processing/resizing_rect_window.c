/*! \file resizing_rect_window.c
	\brief Filter/windowing methods
    \author Calvin Lobo
    \date Oct 3rd 2017
*/


#include "resizing_rect_window.h"

struct ResizingRectWindow* new_ResizingRectWindow(int size, int points_per_trace) {
    struct ResizingRectWindow *window = (ResizingRectWindow*) malloc(sizeof(struct ResizingRectWindow));
    window->index = 0;
    window->size = size;
    window->points_per_trace = points_per_trace;
    window->filled_size = 0;
    window->elements = (SIGNAL_DTYPE*) malloc(window->size * sizeof(SIGNAL_DTYPE));
    return window;
}


void destroy_ResizingRectWindow(struct ResizingRectWindow *window) {
    free(window->elements); // Free the allocated data array
    free(window);           // Free the window struct
}

void push_back(ResizingRectWindow* window, SIGNAL_DTYPE value)
{

    // If the filter is full, remove the oldest contribution
    if (window->filled_size == window->size) {
        window->sum -= window->elements[window->index];
    }
    // Add the new value to the history, replacing the oldest value
    window->elements[window->index] = value;
    window->sum += value;

    if (window->filled_size < window->size) {
        window->filled_size++;
    }

    window->index++;

    if (window->index >= window->size) {
        window->index = 0;
    }
}

void clear(ResizingRectWindow* window)
{
    window->sum = 0;
    window->filled_size = 0;
    for (int ii=0; ii< window->size; ii++) {
        window->elements[ii] = 0;
    }
};

double mean(ResizingRectWindow* window)
{
    return window->sum / window->filled_size;
}