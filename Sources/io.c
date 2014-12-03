/*
 * File:		io.c
 * Purpose:		Serial Input/Output routines
 *
 * Notes:       TERMINAL_PORT defined in <board>.h
 */

#include "derivative.h" /* include peripheral declarations */
#include "uart.h"

/********************************************************************/
char in_char (void)
{
	return uart_getchar();
}
/********************************************************************/
void
out_char (char ch)
{
	uart_putchar(ch);
}
/********************************************************************/
int
char_present (void)
{
	return uart_getchar_present();
}
/********************************************************************/
