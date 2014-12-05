/*
 * File:        uart.c
 * Purpose:     Provide UART routines for serial IO
 *
 * Notes:
 *
 */

#include "derivative.h"
#include "uart.h"

/********************************************************************/
/*
 * Initialize the UART for 8N1 operation, 115200 Baud rate, interrupts
 * disabled, and no hardware flow-control
 *
 * System Clock is 50 MHz
 *
 * @param sysclk: System clock frequency in kHz
 * @param baud: Baud rate in Bps
 */
void uart_init ()
{
	// Enable UART Pins
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR16 = PORT_PCR_MUX(0x03);
	PORTC_PCR17 = PORT_PCR_MUX(0x03);
	
    //set the clock for UART3
	SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
	
    //set baud rate to 115200
    //set SBR[12:8]
	UART3_BDH = 0x00;
    //set SBR[7:0]
	UART3_BDL = 0x1B;
    //set BRFA
    UART3_C4 = 0x04;
	// 8N1 configuration
	UART3_C1 = 0x00;
	// Enable Rx and Tx lines
    UART3_C2 = 0x0C;
}

/********************************************************************/
/*
 * Wait for a character to be received on the specified UART
 *
 *
 * Return Values:
 *  the received character
 */
char uart_getchar ()
{
	while(!uart_getchar_present());
	
	return UART3_D;
}

/********************************************************************/
/*
 * Wait for space in the UART Tx FIFO and then send a character
 *
 * Parameters:
 *  ch			 character to send
 */
void uart_putchar (char ch)
{
	while(!(UART3_S1 & UART_S1_TDRE_MASK));
	
	UART3_D = ch;
}

/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      UART channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart_getchar_present ()
{
	return (UART3_S1 & UART_S1_RDRF_MASK);
}
/********************************************************************/

