/*
 * filters.c
 *
 *  Created on: Nov 24, 2014
 *      Author: ral9602
 */
#include "filters.h"

#ifdef LP1
float lp1Cheby(float x, int * xvlp1, int * yvlp1)
{

	int i = 0;
	for(i = 0; i<LP1_FILTER_ORDER; i++)
	{
		xvlp1[i] = xvlp1[i+1];
		yvlp1[i] = yvlp1[i+1];
	}
	
	xvlp1[LP1_FILTER_ORDER] = x / LP1_GAIN;
	
	yvlp1[LP1_FILTER_ORDER] = (LP1_X0 * (float)xvlp1[0]) + (LP1_X1 * (float)xvlp1[1]) + (LP1_X2 * (float)xvlp1[2])
							  + (LP1_Y0 * (float)yvlp1[0]) + (LP1_Y1 * (float)yvlp1[1]);
	
	return yvlp1[LP1_FILTER_ORDER];
}
#endif
