//
// Created by calvin on 08/11/17.
//

#include "moving_average_filters.h"


struct ExponentialMovingAverageFilter* new_ExponentialMovingAverageFilter()
{
	struct ExponentialMovingAverageFilter *filter = (ExponentialMovingAverageFilter *) malloc(
			sizeof(struct ExponentialMovingAverageFilter));
	return filter;
}

void init_ExponentialMovingAverageFilter(ExponentialMovingAverageFilter* filter, double alpha)
{
	filter->alpha = alpha;
	filter->average = 0;
}

void destroy_ExponentialMovingAverageFilter(ExponentialMovingAverageFilter* filter) {
	free(filter);
}

double update_ExponentialMovingAverageFilter(ExponentialMovingAverageFilter* filter, SIGNAL_DTYPE value) {
	filter->average = filter->alpha * value + (1 - filter->alpha) * filter->average;
	return filter->average;
}



struct CummulativeMovingAverageFilter* new_CummulativeMovingAverageFilter()
{
	struct CummulativeMovingAverageFilter *filter = (CummulativeMovingAverageFilter *) malloc(
			sizeof(struct CummulativeMovingAverageFilter));
	return filter;
}

void init_CummulativeMovingAverageFilter(CummulativeMovingAverageFilter* filter)
{
	filter->n_data_points = 0;
	filter->average = 0;
}

void destroy_CummulativeMovingAverageFilter(CummulativeMovingAverageFilter* filter) {
	free(filter);
}

double update_CummulativeMovingAverageFilter(CummulativeMovingAverageFilter* filter, SIGNAL_DTYPE value) {

	filter->average = filter->average + (value - filter->average) /  (filter->n_data_points+1);
    filter->n_data_points++;

	return filter->average;
}

