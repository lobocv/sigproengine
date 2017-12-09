#include "sigpro_definitions.hpp"
#include "resizing_rect_window.h"
#include "moving_average_filters.h"

void Dewow(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int window_size);

void FindFirstBreakIndex(SIGNAL_DTYPE* dataIn, double* fb_indices, int points_per_trace, int n_traces, float threshold_mV);

void FindZeroCrossings(SIGNAL_DTYPE* trace, int* zeroes_indices,  int points_per_trace, int n_crossings, float threshold_mV, int start_search_idx);

double WaveletRMS(SIGNAL_DTYPE* dataIn, int points_per_trace, int start_idx, int end_idx);

double AverageAmplitude(SIGNAL_DTYPE* dataIn, int points_per_trace, int window_length, int window_start_idx);

void SimpleDCOffsetRemoval(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int dc_window_length);

void EMA_DCOffsetRemoval(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int dc_window_length, ExponentialMovingAverageFilter* filter);

void CMA_DCOffsetRemoval(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, int dc_window_length, CummulativeMovingAverageFilter* filter);

void Envelope(SIGNAL_DTYPE* dataIn, SIGNAL_DTYPE* dataOut, int points_per_trace, int n_traces, float frequency_Mhz, float sampling_interval_ps);
