/*
 * Rochester Institute of Technology
 * Department of Computer Engineering
 * EECC560
 *
 * Authors: Dan Cheung and Sam Skalicky
 *
 * Description: This program will sample the ADC channel at 1 KHz
 * using the PDB and output it back to the DAC. It uses pin B40
 * on the elevator for ADC input and pin A32 for DAC output.
 *
 * Date: 10/4/2011
 *
 * Revision: 1.0
 *
 * Filename: main.c
 */

#include "derivative.h" /* include peripheral declarations */
#include "stdlib.h"
#include "mcg.h"
#include "uart.h"
#include "io.h"
#include "isr.h"
#include "adc.h"
#include "timers.h"
#include "dac.h"
#include "filters.h"
#include "gpio.h"

volatile unsigned short binAdc;
volatile float decAdc;
volatile float lp1Out, lp1OutRecurs;
volatile int adcConvComplete = 0;
volatile long sampleCounter = 0;
float deltaStep;
volatile int microsecTicks;
int usStart, usTotal;

int main(void) {
	int i, done = 0;
	int s;
	
//	MCG_FLL_RTC_OSC();
	// Enable UART Pins
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR16 = PORT_PCR_MUX(0x03);   //Rx
	PORTC_PCR17 = PORT_PCR_MUX(0x03);	//Tx
	
	// Initialize UART
	uart_init();
	
//    MCG_BLPE();
	CLOCK_100MHZ();
    GPIO_INIT();
    ADC1_INIT();
    DAC1_INIT();
//    lptmr_init();
    PDB_INIT();
    FTM_INIT();

	// Start the PDB (ADC Conversions)
	PDB0_SC |= PDB_SC_SWTRIG_MASK;

    for(;;) {
		int stage;
		// Print out a binary and decimal representation of the resultant ADC conversion.
//		printf("\nBinary ADC conversion result: %b,\tDecimal ADC conversion result: %d\n\r", ADC1_RA, ADC1_RA);
//		
//		// Delay for UART print.
//		for( i = 0; i < 1000000; ++i ){
//    		
//    	}
		if( adcConvComplete && (done == 0) )
		{
			adcConvComplete = 0;
			
			if(sampleCounter == 0)
			{
				deltaStep = 1.0;
			}
			else
			{
				deltaStep = 0.0;
			}
//			lp1Out = lp1Cheby((float)ADC1_RA, &xvlp1[0], &yvlp1[0]); 
//			usStart = microsecTicks;
			// Run first lowpass filter
			lp1Out = lp1Cheby(deltaStep, &xvlp1[0], &yvlp1[0]); 
//			usTotal = microsecTicks - usStart;
//			printf("%i\n\r", usTotal);
//			for(s=0; s<10000; s++);
			// Cascade the filters if desired
//			for(stage=1; stage < LP1_CASCADE; stage++)
//			{
//				lp1OutRecurs = lp1Out;
//				lp1Out = lp1Cheby(lp1OutRecurs, &xvlp1[stage], &yvlp1[stage]);
//			}
			printf("%.6i\n\r", lp1Out);
			
			// Execute filter for finite number of samples
			if(sampleCounter > (SAMPLES_TO_RUN - 1))
			{
				// Halt ADC sampling
				PDB_STOP();
				done = 1;
				printf("Samples Completed: %d.\n\r", sampleCounter);
				GPIOA_PDOR &= GPIO_PIN(LED_GREEN);
			}
			sampleCounter++;
		}
    }

    return 0;
}
