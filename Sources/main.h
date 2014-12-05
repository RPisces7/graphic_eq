#ifndef MAIN_H
#define MAIN_H

#include "d4d.h"
#include "ui_eqBar.h"
#include "derivative.h"
#include "common.h"
#include "fonts.h"
#include "uart.h"
#include "io.h"
#include "stdlib.h"
#include "mcg.h"
#include "isr.h"
#include "adc.h"
#include "timers.h"
#include "dac.h"
#include "filters.h"
#include "gpio.h"

#define SAMPLE_SIZE 10000	// Sample Buffer size in bytes
#define EQ_BAR_HEIGHT 239 //Height of on screen equalizer bar representation in pixels
#define EQ_BAR_WIDTH 30 //Width of on screen equalizer bar representation in pixels
#define NUM_EQ_BARS 10


extern char screenText[40];
extern UI_EQ_BAR_VALUE eqValues[NUM_EQ_BARS];
extern UI_EQ_BAR eqBars[NUM_EQ_BARS];
extern unsigned char divXAxis; // Divide the X-axis by this value. 1 = every sample drawn. 2 = Every other sample drawn.

// Timer struct using PIT0
typedef union
{
  D4D_BIT_FIELD all;

  struct
  {
    unsigned b25ms :1;
    unsigned b50ms : 1;
    unsigned b100ms : 1;
    unsigned b200ms : 1;
    unsigned b400ms : 1;
    unsigned b800ms : 1;
    unsigned b1600ms : 1;
    unsigned b3200ms : 1;
  }bits;

}TIME_FLAGS;

extern char textDrawn;

extern TIME_FLAGS time;					// PIT Timer
extern LWord time100sm_cnt;				// PIT Timer

extern unsigned char samples[];					// ADC Samples
extern unsigned volatile short sampleWritePtr;	// Pointer to where the next ADC sample data will be placed

#endif // MAIN_H
