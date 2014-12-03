/*
 * mcg.c
 *
 */
#include "derivative.h" /* include peripheral declarations */
#include "lptmr.h"

/*
 *	This method configures the Multipurpose Clock Generator (MCG) to run in
 *	Bypassed Low Power External (BLPE) mode. On the TWR-K60N512 board, this
 *	would be 50 MHz.
 *
 */
void MCG_BLPE() {
	// Select External Reference Clock in MCG
	MCG_C1 = MCG_C1_CLKS(2);// | MCG_C1_FRDIV(3);

	// wait for Reference clock to switch to external reference
	while (MCG_S & MCG_S_IREFST_MASK){}
	// Wait for MCGOUT to switch over to the external reference clock
	while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}

	// Transition to bypassed low power external mode
	MCG_C2 = MCG_C2_LP_MASK;
}

void MCG_FLL_RTC_OSC() {
//	MCG_C1 = MCG_C1_CLKS(0);// | MCG_C1_FRDIV(3);
//	// wait for Reference clock to switch to external reference
//	while (MCG_S & MCG_S_IREFST_MASK){}
//	// Wait for MCGOUT to switch over to the external reference clock
//	while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}
//	MCG_C2 = MCG_C2_EREFS_MASK;
	
	// Using the RTC OSC as Ref Clk
	// Configure and enable the RTC OSC
	// select the load caps (application dependent) and the oscillator enable bit
	// note that other bits in this register may need to be set depending on the intended use of
	// the RTC
	RTC_WAR = 0xFF;					// Enable write access
	RTC_RAR = 0xFF;					// Enable read access
//	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
	
	RTC_CR = RTC_CR_SWR_MASK;		// Perform software reset to disable lock
	RTC_CR &= ~RTC_CR_SWR_MASK;
	if (RTC_SR & RTC_SR_TIF_MASK){
	     RTC_TSR = 0x00000000;
	}
	
//	RTC_LR = RTC_LR_CRL_MASK;
	RTC_CR |= RTC_CR_SC16P_MASK | RTC_CR_SC8P_MASK | RTC_CR_OSCE_MASK;
	time_delay_ms(1000);
	// select the RTC oscillator as the MCG reference clock
	SIM_SOPT2 |= SIM_SOPT2_MCGCLKSEL_MASK;
	// ensure MCG_C2 is in the reset state, key item is RANGE = 0 to select the correct FRDIV
	// factor
	MCG_C2 = 0x0;
	// Select the Reference Divider and clear IREFS to select the osc
	// CLKS=0, select the FLL as the clock source for MCGOUTCLK
	// FRDIV=0, set the FLL ref divider to divide by 1
	// IREFS=0, select the external clock
	// IRCLKEN=0, disable IRCLK (can enable if desired)
	// IREFSTEN=0, disable IRC in stop mode (can keep it enabled in stop if desired)
	MCG_C1 = 0x0;
	// wait for Reference clock to switch to external reference
	while (MCG_S & MCG_S_IREFST_MASK){}
	// Wait for clock status bits to update
	while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}
	// Can select the FLL operating range/freq by means of the DRS and DMX32 bits
	// Must first ensure the system clock dividers are set to keep the core and
	// bus clocks within spec.
	// core = FLL (96 MHz), bus = FLL (48 MHz), flexbus = PLL (48 MHz), flash = PLL/4 (24 MHz)
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1)
	| SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
	// In this example DMX32 is set and DRS is set to 3 = 96 MHz from a 32.768 kHz
	// crystal
	MCG_C4 |= MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(3);
}

void CLOCK_100MHZ(void){
	// Bus: 48Mhz
	// Core & System: 96Mhz
	
	/* Switch to FBE Mode */
	/* OSC->CR: ERCLKEN=0,??=0,EREFSTEN=0, */
	OSC_CR = (uint8_t)0x00u;
	/* SIM->SOPT2: MCGCLKSEL=0 TRACECLKSEL=1 */
	SIM_SOPT2 = SIM_SOPT2_TRACECLKSEL_MASK;
	/* MCG->C2: ??=0,??=0,RANGE=2,HGO=0,EREFS=1,LP=0,IRCS=0 */
	MCG_C2 = (uint8_t)0x24u;
	/* MCG->C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCG_C1 = (uint8_t)0x9Au;
	/* MCG->C4: DMX32=0,DRST_DRS=0 */
	MCG_C4 &= (uint8_t)~(uint8_t)0xE0u;
	/* MCG->C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=25(50MHz / 25 = 2MHz)*/
	MCG_C5 = (uint8_t)0x18u;
	/* MCG->C5: PLLCLKEN=1 */
	MCG_C5 |= (uint8_t)0x40u;
	/* Enable the PLL */
	/* MCG->C6: LOLIE=0,PLLS=0,CME=0,VDIV=0 */
	MCG_C6 = (uint8_t)0x00u;
	while((MCG_S & MCG_S_OSCINIT_MASK) == 0u)
		{ /* Check that the oscillator is running */  }
	while((MCG_S & MCG_S_IREFST_MASK) != 0u)
		{ /* Check that the source of the FLL reference clock is the external reference clock. */  }
	while((MCG_S & 0x0Cu) != 0x08u)
		{    /* Wait until external reference clock is selected as MCG output */  }

	/* Switch to PBE Mode */
	/* MCG->C1: CLKS=2,FRDIV=0,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCG_C1 = (uint8_t)0x82u;
	/* MCG_C6: LOLIE=0,PLLS=1,CME=0,VDIV=0 */
	MCG_C6 = (uint8_t)0x40u;
	
	/* Switch to PEE Mode */
	/* MCG_C1: CLKS=0,FRDIV=0,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCG_C1 = (uint8_t)0x02u;
	/* MCG_C5: ??=0,PLLCLKEN=0,PLLSTEN=0,PRDIV=25 */
	MCG_C5 = (uint8_t)0x18u;
	/* MCG_C6: LOLIE=0,PLLS=1,CME=0,VDIV=50 (2MHz * 48 = 96MHz) */
	MCG_C6 = (uint8_t)0x58u;
	while((MCG_S & 0x0Cu) != 0x0Cu) {
		/* Wait until output of the PLL is selected */  }
	while((MCG_S & MCG_S_LOCK_MASK) == 0u)
		{ /* Wait until locked */  }
	/* SIM_CLKDIV1: OUTDIV1=0,OUTDIV2=1,OUTDIV3=3,OUTDIV4=1 */
	SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV3(3) | SIM_CLKDIV1_OUTDIV4(1));
}
