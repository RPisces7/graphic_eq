/*
 * File:		arm_cm4.h
 * Purpose:		Definitions common to all ARM Cortex M4 processors
 *
 * Notes:
 */

#ifndef _CPU_ARM_CM4_H
#define _CPU_ARM_CM4_H

//#include "common.h"

/*ARM Cortex M4 implementation for interrupt priority shift*/
#define ARM_INTERRUPT_LEVEL_BITS          4

/***********************************************************************/
// function prototypes for arm_cm4.c
/*
void stop (void);
void wait (void);
void write_vtor (int);
void enable_irq (int);
void disable_irq (int);
void set_irq_priority (int, int);
*/
/***********************************************************************/
  /*!< Macro to enable all interrupts. */
#define EnableInterrupts asm(" CPSIE i");

  /*!< Macro to disable all interrupts. */
#define DisableInterrupts asm(" CPSID i");

  /*!< Macro to mask interrupt levels. */
#define MaskInterrupts(level) __set_BASEPRI((level)<<ARM_INTERRUPT_LEVEL_BITS)
/***********************************************************************/


/*
 * Misc. Defines
 */
#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)

#ifdef	NULL
#undef	NULL
#endif
#define NULL	(0)

#ifdef  ON
#undef  ON
#endif
#define ON      (1)

#ifdef  OFF
#undef  OFF
#endif
#define OFF     (0)

/***********************************************************************/
/*
 * The basic data types
 */
typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short	uint16; /* 16 bits */
typedef unsigned long	uint32; /* 32 bits */

typedef signed char		int8;   /*  8 bits */
typedef signed short    int16;  /* 16 bits */
typedef signed long		int32;  /* 32 bits */

typedef volatile int8		vint8;  /*  8 bits */
typedef volatile int16		vint16; /* 16 bits */
typedef volatile int32		vint32; /* 32 bits */

typedef volatile uint8		vuint8;  /*  8 bits */
typedef volatile uint16		vuint16; /* 16 bits */
typedef volatile unsigned long		vuint32; /* 32 bits */

// function prototype for main function
//void main(void);

/***********************************************************************/
#endif	/* _CPU_ARM_CM4_H */

