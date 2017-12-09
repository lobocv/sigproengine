//
// Created by calvin on 08/11/17.
//

#ifndef SRC_MOVING_AVERAGE_FILTER_H
#define SRC_MOVING_AVERAGE_FILTER_H

#include "sigpro_definitions.hpp"


/*! @brief Filter that computes the moving average using an exponentially decaying history
 *
 *  https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average
 */
struct ExponentialMovingAverageFilter {
	double alpha;
	SIGNAL_DTYPE average;
};

struct ExponentialMovingAverageFilter* new_ExponentialMovingAverageFilter();

void init_ExponentialMovingAverageFilter(ExponentialMovingAverageFilter* filter, double alpha);

void destroy_ExponentialMovingAverageFilter(ExponentialMovingAverageFilter* filter);

double update_ExponentialMovingAverageFilter(ExponentialMovingAverageFilter* filter, SIGNAL_DTYPE value);



/*! @brief Filter that computes the moving average cummulatively (ie equal weighting for all history)
 *
 *  https://en.wikipedia.org/wiki/Moving_average#Cumulative_moving_average
 *  This filter uses the iterative approach described in the above link. Warning, at some point the sum
 *  may overflow.
 */
struct CummulativeMovingAverageFilter {
	int n_data_points;
	SIGNAL_DTYPE average;
};

struct CummulativeMovingAverageFilter* new_CummulativeMovingAverageFilter();

void init_CummulativeMovingAverageFilter(CummulativeMovingAverageFilter* filter);

void destroy_CummulativeMovingAverageFilter(CummulativeMovingAverageFilter* filter);

double update_CummulativeMovingAverageFilter(CummulativeMovingAverageFilter* filter, SIGNAL_DTYPE value);



#endif //SRC_MOVING_AVERAGE_FILTER_H
