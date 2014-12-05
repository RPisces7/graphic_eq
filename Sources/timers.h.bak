/*
 * timers.h
 *
 *  Created on: Nov 20, 2014
 *      Author: ral9602
 */
#include "derivative.h" /* include peripheral declarations */

#ifndef TIMERS_H_
#define TIMERS_H_

#define CLOCK					96000000
#define FTM_FREQUENCY			1000000
#define FTM0_MOD_VALUE			(CLOCK/FTM_FREQUENCY)
#define PDB_FREQ				1000
#define PDB_COUNT				CLOCK / PDB_FREQ

void PDB_INIT(void);
void PDB_STOP(void);

#endif /* TIMERS_H_ */
