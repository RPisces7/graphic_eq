/*
 * dac.c
 *
 *  Created on: Nov 20, 2014
 *      Author: ral9602
 */
#include "dac.h"

void DAC1_INIT(void) {
	// Enable DAC clock
	SIM_SCGC2 |= SIM_SCGC2_DAC1_MASK;
	DAC1_C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
	DAC1_C1 = 0;
}
