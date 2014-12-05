/*
 * File:    mcg.c
 * Purpose: Driver for enabling the PLL in 1 of 4 options
 *
 * Notes:
 * Assumes the MCG mode is in the default FEI mode out of reset
 * One of 4 clocking oprions can be selected.
 * One of 16 crystal values can be used
 */

#include "derivative.h" 
#include "common.h"
#include "mcg.h"
#include "lptmr.h"

void PLL_Init (unsigned char clk_option, unsigned char crystal_val)
{
  if (clk_option > 3) { return; } //return if one of the available options is not selected
  if (crystal_val > 24) { return; } // return if one of the available crystal options is not available
//This assumes that the MCG is in default FEI mode out of reset.

// First move to FBE mode
#if (defined(K60_CLK) || defined(ASB817))
     MCG_C2 = 0;
#else
// Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
    MCG_C2 = MCG_C2_RANGE(2) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
#endif

// Select external oscilator and Reference Divider and clear IREFS to start ext osc
// CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);

  /* if we aren't using an osc input we don't need to wait for the osc to init */
#if (!defined(K60_CLK) && !defined(ASB817))

   while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  // wait for oscillator to initialize
#endif

  while (MCG_S & MCG_S_IREFST_MASK){}; // wait for Reference clock Status bit to clear

  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // Wait for clock status bits to show clock source is ext ref clk

// Now in FBE
// Configure PLL Ref Divider, PLLCLKEN=0, PLLSTEN=0, PRDIV=5
// The crystal frequency is used to select the PRDIV value. Only even frequency crystals are supported
// that will produce a 2MHz reference clock to the PLL.
  MCG_C5 = MCG_C5_PRDIV(crystal_val); // Set PLL ref divider to match the crystal used

  // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
  MCG_C6 = 0x0;
// Select the PLL VCO divider and system clock dividers depending on clocking option
  switch (clk_option) {
    case 0:
      // Set system options dividers
      //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
      SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV3(0) | SIM_CLKDIV1_OUTDIV4(1);
      // Set the VCO divider and enable the PLL for 50MHz, LOLIE=0, PLLS=1, CME=0, VDIV=1
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(1); //VDIV = 1 (x25)
      break;
   case 1: // 100MHz
      // Set system options dividers
      //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
      SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
      // Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(26); //VDIV = 26 (x50)
      break;
    case 2: // 96MHz
      // Set system options dividers
      //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
      SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
      // Set the VCO divider and enable the PLL for 96MHz, LOLIE=0, PLLS=1, CME=0, VDIV=24
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24); //VDIV = 24 (x48)
      break;
   case 3:
      // Set system options dividers
      //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
      SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV3(0) | SIM_CLKDIV1_OUTDIV4(1);
      // Set the VCO divider and enable the PLL for 48MHz, LOLIE=0, PLLS=1, CME=0, VDIV=0
      MCG_C6 = MCG_C6_PLLS_MASK; //VDIV = 0 (x24)
      break;
  }
  //b01800
  //while (!(MCG_S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
//b01800
  //while (!(MCG_S & MCG_S_LOCK_MASK)){}; // Wait for LOCK bit to set

// Now running PBE Mode

// Transition into PEE by setting CLKS to 0
// CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG_C1 &= ~MCG_C1_CLKS_MASK;

// Wait for clock status bits to update
  //b01800
  //while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};

// Now running PEE Mode

} //pll_init

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


