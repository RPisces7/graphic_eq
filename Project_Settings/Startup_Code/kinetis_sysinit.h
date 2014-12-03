/*
  FILE    : kinetis_sysinit.h
  PURPOSE : system initialization header for Kinetis ARM architecture
  LANGUAGE: C
  Copyright � 2010 Freescale semiConductor Inc. All Rights Reserved.
*/
#ifndef KINETIS_SYSINIT_H
#define KINETIS_SYSINIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Address of Watchdog Unlock Register (16 bits) */
#define KINETIS_WDOG_UNLOCK_ADDR	0x4005200E

/* Address of Watchdog Status and Control Register High (16 bits) */
#define KINETIS_WDOG_STCTRLH_ADDR	0x40052000

/* Unlocking Watchdog sequence words*/
#define KINETIS_WDOG_UNLOCK_SEQ_1	0xC520
#define KINETIS_WDOG_UNLOCK_SEQ_2	0xD928

/* Word to be written in in STCTRLH after unlocking sequence in order to disable the Watchdog */
#define KINETIS_WDOG_DISABLED_CTRL	0xD2

/* 
	Initializes the Kinetis hardware: e.g. disables the Watchdog
*/
void __init_hardware();

/*
** ===================================================================
**     Method      :  isr_default
**
**     Description :
**         The default ISR.
** ===================================================================
*/
void isr_default(void);

/*
** ===================================================================
**     Method      :  isrINT_NMI
**
**     Description :
**         This ISR services the Non Maskable Interrupt interrupt.
** ===================================================================
*/
void isrINT_NMI(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef KINETIS_SYSINIT_H */
