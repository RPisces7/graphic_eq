#include "main.h"
#include "mcg.h"
#include "sim.h"
#include "pit.h"
//#include "../Project_Headers/d4d_screen.h"

// Initialize the TWR-LCD Screen
D4D_EXTERN_SCREEN(screen);
/**************************************************************//*!
 *
 * Global variables
 *
 ******************************************************************/

char screenText[40];

UI_EQ_BAR_VALUE eqValues[NUM_EQ_BARS];
UI_EQ_BAR eqBars[NUM_EQ_BARS];

unsigned char divXAxis = 1; // Divide the X-axis by this value. 1 = every sample drawn. 2 = Every other sample drawn.

char textDrawn = 0;

TIME_FLAGS time;
LWord time100sm_cnt = 0;

unsigned char samples[SAMPLE_SIZE]; // Stored Samples
unsigned volatile short sampleWritePtr; // Index to put the next sample in samples
unsigned volatile short sampleCount; // Number of samples in samples. Maxes at SAMPLE_SIZE
unsigned short samplePtr; // Index to send the next sample wirelessly.

volatile unsigned short binAdc;
volatile float decAdc;
volatile float bp1Out, bp1OutRecurs;
volatile int adcConvComplete = 0;
volatile int sampleCounter = 0;
volatile float deltaStep;
volatile float results[SAMPLES_TO_RUN];
volatile int microsecTicks;

/**************************************************************//*!
 * Prototypes
 ******************************************************************/
static void pit_ch0_callback(void);
void MCU_Init(void);
void Timer_Init(void);
void addSample(unsigned char i);
void setEQValues(UI_EQ_BAR_VALUE * newValues);
void setText(char *c);

/*****************************************************************/

/**************************************************************//*!
 * Init Clock and PLL
 ******************************************************************/
void MCU_Init(void) {
	SIM_Init(SIM_MODULE_CONFIG_LCD);
	PLL_Init(CORE_CLK_MHZ, REF_CLK);
}
/*****************************************************************/

/**************************************************************//*!
 *  Periodic Timer interrupt initialization - 25ms
 ******************************************************************/
#define PIT_PERIOD (D4D_MCU_BUS_CLOCK/1000)*25

void Timer_Init(void) {
	PIT_Init (PIT0,PIT_CH_CONFIG,PIT_PERIOD,1,pit_ch0_callback);
	PDB_INIT();
	FTM_INIT();
	PIT_Enable ();
	PDB0_SC |= PDB_SC_SWTRIG_MASK; //PDB enable
}
/*****************************************************************/

/**************************************************************//*!
 * Periodic IRQ callback function
 ******************************************************************/
static void pit_ch0_callback(void) {
	static Byte actual_time = 0;
	TIME_FLAGS flags;

	actual_time++;
	flags.all = (D4D_BIT_FIELD) (actual_time ^ (actual_time - 1));

	time.all |= flags.all;

	if (flags.bits.b50ms) {
		//  Key_CheckKeys();
		//D4D_CheckTouchScreen();
	}

	if (flags.bits.b100ms) {
		time100sm_cnt++;
	}
}
/********************************************************************/

// This function adds a sample to a buffer.
// Call this function every time you want to draw a sample to the LCD screen (when divXAxis = 1).
void addSample(unsigned char i) {
	// Store only the upper 8-bits into the sample buffer.
	samples[sampleWritePtr] = i;

	// Increment the sample write position pointer.
	sampleWritePtr++;
	if (sampleWritePtr >= SAMPLE_SIZE) {
		sampleWritePtr = 0;
	}
	if ((sampleWritePtr % SAMPLE_SIZE) == samplePtr) {
		samplePtr = samplePtr % SAMPLE_SIZE;
	}
}

void performFilter(){
	int stage;
		// Print out a binary and decimal representation of the resultant ADC conversion.
//		printf("\nBinary ADC conversion result: %b,\tDecimal ADC conversion result: %d\n\r", ADC1_RA, ADC1_RA);
//		
//		// Delay for UART print.
//		for( i = 0; i < 1000000; ++i ){
//    		
//    	}
		if( adcConvComplete )
		{
			adcConvComplete = 0;
			
			if(sampleCounter == 0)
			{
				deltaStep = 1.0;
			}
			else
			{
				deltaStep = 0.0;
			}
//			bp1Out = bp1Cheby(ADC1_RA, xvbp1[0], yvbp1[0]);
			bp1Out = bp1Cheby(deltaStep, &xvbp1[0], &yvbp1[0]); 
			results[sampleCounter] = bp1Out;
			// Cascade the filters if desired
//			for(stage=1; stage < BP1_CASCADE; stage++)
//			{
//				bp1OutRecurs = bp1Out;
//				bp1Out = bp1Cheby(bp1OutRecurs, xvbp1[stage], yvbp1[stage]);
//			}
//			printf("%d\n\r", bp1Out);
			
			if(sampleCounter > (SAMPLES_TO_RUN - 1))
			{
				printf("Sampling Complete.\n\r");
				PDB_STOP();
			}
			sampleCounter++;
		}
}

void updateScreen(){
	int i, counter = 0;
	// Demo Wave (REMOVE THIS CODE IN YOUR PROGRAM)
			if(time.bits.b25ms) {
				//setText("Hello World!");
				// Add a sample to draw. divXAxis = 1 so each sample = 1 pixel.
				//addSample(counter++);
			}
			
			if (time.bits.b1600ms){
				eqValues[0]++;
				eqValues[5]++;
				//eqValues[8]++;
				//eqValues[12]++;
				//eqValues[16]++;
				for (i = 0; i< NUM_EQ_BARS; i++)
				{
					if (eqValues[i] > 254){
						if (i < NUM_EQ_BARS -1){ //if not the last bar
							eqValues[i+1]++;
						}
						eqValues[i] = 0;
					}
				}
				setEQValues(eqValues);
	/*
				if(counter > 25500){
					counter = 0;
				}
				bar0value = counter / 100;
				
				UI_EqBar_Set_Value(&eqBars[0], bar0value);
				printf("bar0value = %d \r\n", bar0value);
	*/
			}

			// Run the TWR-LCD Routine. DO NOT REMOVE
			if(time.bits.b25ms) {
				D4D_Poll(); // D4D poll loop
			}
}

void setText(char *c) {
	char *txt = screenText;
	do {
		if(*txt != *c) {
			textDrawn = 1;
		}
		*txt++ = *c++;
	} while(*c != 0);
}

void setupEQBars(){
	D4D_COOR currX = 1;
	int numBars = 0;
	DECLARE_UI_EQ_BAR(eqBar0, 1,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar1, 33,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar2, 65,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar3, 97,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar4, 129,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar5, 161,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar6, 193,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar7, 225,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar8, 257,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	DECLARE_UI_EQ_BAR(eqBar9, 289,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar10, 161,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar11, 177,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar12, 193,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar13, 209,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar14, 225,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar15, 241,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar16, 257,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar17, 273,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar18, 289,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
//	DECLARE_UI_EQ_BAR(eqBar19, 305,0,EQ_BAR_WIDTH, EQ_BAR_HEIGHT)
	
	eqBars[0] = eqBar0;
	eqBars[1] = eqBar1;
	eqBars[2] = eqBar2;
	eqBars[3] = eqBar3;
	eqBars[4] = eqBar4;
	eqBars[5] = eqBar5;
	eqBars[6] = eqBar6;
	eqBars[7] = eqBar7;
	eqBars[8] = eqBar8;
	eqBars[9] = eqBar9;
//	eqBars[10] = eqBar10;
//	eqBars[11] = eqBar11;
//	eqBars[12] = eqBar12;
//	eqBars[13] = eqBar13;
//	eqBars[14] = eqBar14;
//	eqBars[15] = eqBar15;
//	eqBars[16] = eqBar16;
//	eqBars[17] = eqBar17;
//	eqBars[18] = eqBar18;
//	eqBars[19] = eqBar19;
}

void setEQValues(UI_EQ_BAR_VALUE * newValues){
	int i = 0;
	do{
		UI_EqBar_Set_Value(&eqBars[i], *newValues);
		newValues++;
		i++;
	}while(*newValues != 0 && i < NUM_EQ_BARS);
}

/********************************************************************/
void main(void) {
	int i, counter ,done = 0;
	int s;
	
	UI_EQ_BAR* eqBar0Ptr;
	UI_EQ_BAR_VALUE bar0value = 0;
	// MCU Initialization Clock, etc. Core = 100 MHz, Bus = 50 MHz
	MCU_Init();

	// Periodic Timer interrupt initialization - 25ms
	Timer_Init();

	//UART initialization
	uart_init();

	GPIO_INIT();
	ADC1_INIT();
	DAC1_INIT();
	// Initialize TWR-LCD Screen. DO NOT REMOVE
	D4D_Init(&screen);
	D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
	
	// Initialize custom UI elements
	setupEQBars();
	
	sampleWritePtr = 0;
	samplePtr = 0;
	for (;;) {
		//performFilter();
		updateScreen();
	}
}
