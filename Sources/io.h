/*
 * File:		io.h
 * Purpose:		Serial Input/Output routines
 *
 */

#ifndef _IO_H
#define _IO_H

//#define printf io_printf

#define printf ee_printf
#define HAS_FLOAT
#define CVTBUFSIZE 80

/********************************************************************/

char
in_char(void);

void
out_char(char);

int
char_present(void);

int
io_printf(const char *, ... );

int
sprintf(char *, const char *, ... );

/********************************************************************/

#endif
