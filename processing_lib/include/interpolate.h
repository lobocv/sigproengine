//
// Created by calvin on 08/11/17.
//

#ifndef SRC_INTERPOLATE_H
#define SRC_INTERPOLATE_H

#include "sigpro_definitions.hpp"


SIGNAL_DTYPE linear_interpolate(int x0, int x1, SIGNAL_DTYPE y0, SIGNAL_DTYPE y1, SIGNAL_DTYPE* interpolated_output, int output_length);

int get_interpolated_length(int points_per_trace, int n_interp_points);

void interpolate_trace(SIGNAL_DTYPE* inData, SIGNAL_DTYPE* outData, int points_per_trace, int n_traces, int n_interp_points, const char* interp_method);

#endif //SRC_INTERPOLATE_H
