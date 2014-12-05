/*
 * timers.c
 *
 *  Created on: Nov 20, 2014
 *      Author: ral9602
 */
#include "timers.h"

void PDB_INIT(void) {
	//Enable PDB Clock
	SIM_SCGC6 |= SIM_SCGC6_PDB_MASK;
	PDB0_CNT = 0x0000;
	PDB0_MOD = PDB_COUNT;// 1kHz //1041; // 50,000,000 / 1,041 = 48kSPS (for ADC trigger)	
	PDB0_CH1DLY0 = 0;
	PDB0_SC = PDB_SC_PDBEN_MASK | PDB_SC_CONT_MASK | PDB_SC_TRGSEL(0xf)
			| PDB_SC_LDOK_MASK | PDB_SC_PDBIE_MASK;
	PDB0_CH1C1 = PDB_C1_EN(0x01) | PDB_C1_TOS(0x01);		// Enable pre-trigger output of PDB0 to ADC
}

void FTM_INIT(void) {
	// 12.2.13 Enable the Clock to the FTM0 Module
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	
	// 11.4.1 Route the output of TPM channel 0 to the pins
	// Use drive strength enable flag to high drive strength
    PORTA_PCR7  = PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;			// TWR Pin B40
	
	// 39.3.10 Disable Write Protection
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;
	
	// 39.3.4 FTM Counter Value
	// Initialize the CNT to 0 before writing to MOD
	FTM0_CNT = 0;
	
	// 39.3.8 Set the Counter Initial Value to 0
	FTM0_CNTIN = 0;
	
	// 39.3.5 Set the Modulo resister
	FTM0_MOD = FTM0_MOD_VALUE;
	
	// 39.3.3 FTM Setup
	// Set prescale value to 1
	// Chose system clock source
	// Timer Overflow Interrupt Enable
	FTM0_SC = FTM_SC_PS(0) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

	// Enable Interrupt Vector for FTM0
	// See Table 3-4 for FTM0 Interrupt Entry
	// Address: 0x000_013B, Vector: 78, IRQ: 62
	NVICISER1 |= 1 << (62 % 32);
}

void PDB_STOP(void) {
	PDB0_SC &= ~PDB_SC_PDBEN_MASK;
}
