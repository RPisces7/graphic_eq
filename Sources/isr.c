#include <stdio.h>
#include "derivative.h" 
#include "isr.h"
#include "io.h"
#include "gpio.h"

int time_down=0;
int count_on=0;
extern volatile float lp1Out;
extern volatile long microsecTicks;

// ADC1 Conversion Complete ISR
void ADC1_IRQHandler(void) {
	// Read the result (upper 12-bits). This also clears the Conversion complete flag.
	unsigned short i = ADC1_RA >> 4;
	//binAdc = i;
	//decAdc = i * 3.3 / 65536;
	// Do stuff with the result here.
    //Set DAC output value (12bit)
    DAC1_DAT0L = (char)(i & 0x00FF);    //set low 8 bits
    DAC1_DAT0H = (char)(i >> 8);    //set high 4 bits
    GPIOB_PTOR = GPIO_PIN(9);
    adcConvComplete = 1;
}

void FTM0_ISR(void) {
	FTM0_SC &= ~(FTM_SC_TOF_MASK);
	
	microsecTicks++;
	if(microsecTicks == 0xFFFFFFFF)
	{
		microsecTicks = 0;
	}
}

void PDB_Timer_IRQHandler(void)
{
	// Clear interrupt flag for ADC trigger
	PDB0_SC &= ~(PDB_SC_PDBIF_MASK); 
	GPIOA_PTOR |= 0x00000400;
}
