/*
 * filters.c
 *
 *  Created on: Nov 24, 2014
 *      Author: ral9602
 */
#include "filters.h"

#ifdef BP1
float bp1Cheby(float x, float * xvbp1, float * yvbp1)
{

	int i = 0;
	for(i = 0; i<BP1_FILTER_ORDER; i++)
	{
		xvbp1[i] = xvbp1[i+1];
		yvbp1[i] = yvbp1[i+1];
	}
	
	xvbp1[BP1_FILTER_ORDER] = x / BP1_GAIN;
	
	yvbp1[BP1_FILTER_ORDER] = (BP1_X0 * xvbp1[0]) + (BP1_X1 * xvbp1[1]) + (BP1_X2 * xvbp1[2])
							  + (BP1_X3 * xvbp1[3]) + (BP1_X4 * xvbp1[4]) + (BP1_X5 * xvbp1[5]);
	
	return yvbp1[BP1_FILTER_ORDER];
}
#endif
