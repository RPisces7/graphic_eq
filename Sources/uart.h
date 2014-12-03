/*
 * File:		uart.h
 * Purpose:     Provide Kinetis UART routines for polled serial IO
 *
 * Notes:
 */

#ifndef __UART_H__
#define __UART_H__

/********************************************************************/

void uart_init ();
char uart_getchar ();
void uart_putchar (char);
int uart_getchar_present ();

/********************************************************************/

#endif /* __UART_H__ */
