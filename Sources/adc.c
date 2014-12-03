/*
 * adc.c
 *
 *  Created on: Nov 20, 2014
 *      Author: ral9602
 */
#include "adc.h"

void ADC1_INIT(void) {
    unsigned int calib;

    // Turn on ADC1
    SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;

    // Configure CFG Registers
    // Configure ADC to divide 50 MHz down to 6.25 MHz AD Clock, 16-bit single ended
    ADC1_CFG1 |= (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2);

    // Do ADC Calibration for Singled Ended ADC. Do not touch.
    ADC1_SC3 = ADC_SC3_CAL_MASK;
    while ( (ADC1_SC3 & ADC_SC3_CAL_MASK) != 0 );
    calib = ADC1_CLP0;
    calib += ADC1_CLP1;
    calib += ADC1_CLP2;
    calib += ADC1_CLP3;
    calib += ADC1_CLP4;
    calib += ADC1_CLPS;
    calib = calib >> 1;
    calib |= 0x8000;
    ADC1_PG = calib;

	// Configure SC registers.
    // Select hardware trigger.
    ADC1_SC2 |= (1 << 6);


    // Configure SC1A register.
    // Select ADC Channel and enable interrupts. Use ADC1 channel DAD3 (AN3, Pin A27) in single ended mode.
    ADC1_SC1A = 0;
    ADC1_SC1A |= (1 << 6) | (1 << 1) | 1;


	// Enable NVIC interrupt
	NVICISER1 |= NVIC_ISER_SETENA(1<<26);//ADC1 Interrupt
}
