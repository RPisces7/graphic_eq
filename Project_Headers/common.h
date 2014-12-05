/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Include the generic CPU header file
 */
#include "arm_cm4.h"


/*
 * Include the platform specific header file
 */
#include "k60_tower.h"

/*
 * Include the cpu specific header file
 */
#include "MK60N512VMD100.h"

/*
 * Include any toolchain specfic header files
 */
/*
#if (defined(__MWERKS__))
  #include "mwerks.h"
#elif (defined(__DCC__))
  #include "build/wrs/diab.h"
#elif (defined(__ghs__))
  #include "build/ghs/ghs.h"
#elif (defined(__GNUC__))
  #include "build/gnu/gnu.h"
#elif (defined(IAR))
  #include "iar.h"
#else
#warning "No toolchain specific header included"
#endif
*/
/*
 * Common macros
 */
#define IRQ(x)              ((x)-16)
#define ICPR_VALUE(x)       (uint16)(IRQ(x)/32)
#define ICPR_SHIFT(x)       (uint16)(IRQ(x)%32)
#define NVIC_SetIsr(x,ipr)  {                                                 \
                              NVIC_ICPR_REG(NVIC_BASE_PTR,ICPR_VALUE(x)) |=   \
                              (1l << ICPR_SHIFT(x));                          \
                              NVIC_ISER_REG(NVIC_BASE_PTR,ICPR_VALUE(x)) |=   \
                              (1l << ICPR_SHIFT(x));                          \
                              NVIC_IP_REG(NVIC_BASE_PTR,IRQ(x)) |=            \
                              (uint8)((uint8)ipr<<4);                         \
                            }
#define REG_SetVal(reg,mask,val,shift)  { reg = ((reg & ~mask)|(val<<shift)); }

/* defines for peripheral configuration structures 
 *                            */
#define SET(mask)                       mask
#define CLR(mask)                       0

/*
 * System register file
 */
//__root __no_init volatile uint8 SystemRegisterFile[32] @ 0x40041000;


/*
 * Include common utilities
 */
/*
#include "arch_delay.h"
#include "startup.h"
#include "intrinsics.h"
#include "dsp.h"
*/
/********************************************************************/

#endif /* _COMMON_H_ */
