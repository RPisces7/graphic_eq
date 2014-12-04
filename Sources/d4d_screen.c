/******************************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
*******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file      d4d_screen.c
*
* @author    b01119
*
* @version   0.0.9.0
*
* @date      Feb-21-2011
*
* @brief     D4D driver - Entry screen of eGUI/D4D demo application
*
******************************************************************************/

#include "main.h"

#define SCR_ENTRY_TIMEOUT_TICKS 100

static D4D_BOOL firstTime = D4D_TRUE;

/*****************************************************************************
*
* Graphic objects coordination specifications
*
*
*****************************************************************************/

// Declare tabulator table
D4D_DECLARE_TAB_TABLE_BEGIN(scrEntry_txtBxTabTable)
D4D_DECLARE_TAB(20)
D4D_DECLARE_TAB(50)
D4D_DECLARE_TAB_TABLE_END

/*****************************************************************************
*
* Graphic object declarations
*
*
*****************************************************************************/

//D4D_DECLARE_STD_PROGRESS_BAR(eqBar0,0,0,300 - 1, 30, 0)
D4D_DECLARE_LABEL(scrLabel, "                    ", 0, 210, 320 - 1, 30, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED), NULL, FONT_BERLIN_SANS_FBDEMI12, NULL, NULL)


/*****************************************************************************
*
* eGUI/D4D screen declaration
*
*
*****************************************************************************/
// Standard screen declaration
D4D_DECLARE_STD_SCREEN_BEGIN(screen, ScreenEntry_)
	//D4D_DECLARE_SCREEN_OBJECT(eqBar0)
	//D4D_DECLARE_SCREEN_OBJECT(scrLabel)
  
D4D_DECLARE_SCREEN_END()



/*****************************************************************************
*
* Screen system functions
*
*
*****************************************************************************/



// One time called screen function in screen initialization proces
static void ScreenEntry_OnInit()
{
  //Log_AddScreenEvent("Entry", "OnInit");
}

// Screen on Activate function called with each screen activation
static void ScreenEntry_OnActivate()
{
  //Log_AddScreenEvent("Entry", "OnActivate");
}
int cleared = 0;
int pixelPos = 0;
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
//int time = 5000;

int drawPtr = 0;
int lastPixelDrawn = 0;
int trueProgress = 0;
D4D_PROGRESS_BAR_VALUE progress = 0;
// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenEntry_OnMain() {
	if(time.bits.b25ms) {
		int i;
		time.bits.b25ms = 0;
		//update the on screen EQ bars
		if (!cleared){
			//clear screen
			D4D_FillRectXY(0,0,SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, D4D_COLOR_BLACK);
			//tell eq bar components to refresh completely
			for (i = 0; i<NUM_EQ_BARS; i++){
				eqBars[i].drawn = 0;
				eqBars[i].update = 1;
			}
			cleared = 1;
		}
		if(time.bits.b100ms){
			time.bits.b100ms = 0;
			for (i = 0; i<NUM_EQ_BARS; i++){
				if(eqBars[i].update){
					UI_EqBar_Draw(&eqBars[i]);
					eqBars[i].update = 0;
				}
			}
		}
		
		if(time.bits.b200ms) {
			time.bits.b200ms = 0;
			
			
			/*
 			if(textDrawn) {
				//D4D_FillRectXY(0, 210, 320 - 1, 240 - 1, D4D_COLOR_BLACK);
				D4D_SetText(&scrLabel, screenText);
				textDrawn = 0;
			}
			*/
		}

//		diff = (sampleWritePtr - drawPtr + SAMPLE_SIZE) % SAMPLE_SIZE;
//		pixels = diff / 10;
//		//diff = 1;
//		if(pixels > 0) {
//			// Clear part of the wave
//			if(pixelPos + pixels + 10 < SCREEN_WIDTH) {
//				D4D_FillRectXY(pixelPos, 0, pixelPos + pixels + 10, SCREEN_HEIGHT - 1, D4D_COLOR_BLACK);
//			} else {
//				D4D_FillRectXY(pixelPos, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, D4D_COLOR_BLACK);
//				D4D_FillRectXY(0, 0, (pixelPos + pixels + 10) % SCREEN_WIDTH, SCREEN_HEIGHT - 1, D4D_COLOR_BLACK);
//			}
//			D4D_MoveToXY(pixelPos, SCREEN_HEIGHT - (((unsigned int)samples[lastPixelDrawn] * SCREEN_HEIGHT) >> 8) - 1);
//
//			// Draw the wave
//			for(i = 0; i < pixels; i++) {
//				if(pixelPos == 0) {
//					D4D_MoveToXY(0, SCREEN_HEIGHT - (((unsigned int)samples[(drawPtr) % SAMPLE_SIZE] * SCREEN_HEIGHT) >> 8) - 1);
//				} else {
//					D4D_LineToXY(pixelPos, SCREEN_HEIGHT - (((unsigned int)samples[(drawPtr) % SAMPLE_SIZE] * SCREEN_HEIGHT) >> 8) - 1, D4D_LINE_THIN, D4D_COLOR_GREEN);
//				}
//				lastPixelDrawn = (drawPtr) % SAMPLE_SIZE;
//				drawPtr = (drawPtr + divXAxis) % SAMPLE_SIZE;
//				pixelPos++;
//				if(pixelPos >= SCREEN_WIDTH) {
//					pixelPos = 0;
//				}
//			}
//		}
	}
}


/*UI_EQ_BAR* Get_UI_EQ_BAR0(){
	return &eqBar0;
}*/

// Screen on DeActivate function called with each screen deactivation
static void ScreenEntry_OnDeactivate()
{
  //Log_AddScreenEvent("Entry", "OnDeActivate");
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenEntry_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);

  return 0;
}



