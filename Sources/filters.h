/*
 * filters.h
 *
 *  Created on: Nov 24, 2014
 *      Author: ral9602
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#define SAMPLES_TO_RUN	4096

#define LP1		// 0-20
#define BP2		// 20-40
#define BP3		// 40-60
#define BP4		// 60-125
#define BP5		// 125-160
#define BP6		// 160-200
#define BP7		// 200-250
#define BP8		// 250-315
#define BP9		// 315-400
#define BP10	// 400-500
#define BP11	// 500-630
#define BP12	// 630-800
#define BP13	// 800-1k
#define BP14	// 1k-1.25k
#define BP15	// 1.25k-1.6k
#define BP16	// 1.6k-2k
#define BP17	// 2k-2.5k
#define BP18	// 2.5k-3.15k
#define BP19	// 3.15k-4k
#define BP20	// 4k-5k
#define BP21	// 5k-6.3k
#define BP22	// 6.3k-8k
#define BP23	// 8k-10k
#define BP24	// 10k-12.5k
#define BP25	// 12.5k-16k
#define BP26	// 16k-20k

// ----------------------------------------------
// Bandpass 1
// ----------------------------------------------
#ifdef LP1
#define LP1_FILTER_ORDER	2
#define LP1_CASCADE			1

#define LP1_GAIN			530107.5605	
#define LP1_SCALAR			0.00147
#define LP1_X0				1.0	
#define LP1_X1				2.0
#define LP1_X2				1.0	
#define LP1_Y0				-0.9971302656	
#define LP1_Y1				1.9971227200
#endif

// ----------------------------------------------
// Memory Allocation
// ----------------------------------------------
#ifdef LP1
static int xvlp1[LP1_FILTER_ORDER+1] = {0};
static int yvlp1[LP1_FILTER_ORDER+1] = {0};
#endif

float lp1Cheby(float x, int * xvlp1, int * yvlp1);

#endif /* FILTERS_H_ */
