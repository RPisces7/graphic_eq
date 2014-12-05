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
* @file      d4dtch_resistive_mqx.c
*
* @author    b01119
* 
* @version   0.0.5.0
* 
* @date      Mar-11-2011
* 
* @brief     D4D driver - resistive touch screen driver source c file 
*
******************************************************************************/
#include "d4d.h"
#include "d4d_lldapi.h"
#include "d4d_private.h"

#define d4dtch_resistive_mqx_ID 1

#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_resistive_mqx_ID)
  
#include "mqx.h"
#include <fio.h>
#include "tchres.h"
#include "d4dtch_resistive_mqx.h"

/******************************************************************************
* Macros 
******************************************************************************/

/**************************************************************//*!
*
* Local functions
*
******************************************************************/
static unsigned char D4DTCH_Init_Tchres_Mqx(void);
static unsigned char D4DTCH_DeInit_Tchres_Mqx(void);
static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Tchres_Mqx(void);
static unsigned char D4DTCH_GetPositionRaw_Tchres_Mqx (unsigned short *TouchPositionX, 
                                      unsigned short *TouchPositionY);

/**************************************************************//*!
*
* Global variables
*
******************************************************************/
const D4DTCH_FUNCTIONS d4dtch_resistive_mqx = 
{
   D4DTCH_Init_Tchres_Mqx, 
   D4DTCH_GetPositionRaw_Tchres_Mqx,
   D4DTCH_GetRawLimits_Tchres_Mqx,
   D4DTCH_DeInit_Tchres_Mqx
};

/**************************************************************//*!
*
* Local variables
*
******************************************************************/
static FILE_PTR tchscr_dev;

/**************************************************************//*!
*
* Initialize
*
******************************************************************/
static unsigned char D4DTCH_Init_Tchres_Mqx(void)
{
   tchscr_dev = fopen("tchscr:", NULL);
   if( tchscr_dev == NULL )   
    	return 0;
   else
   		return 1;  
}   

/**************************************************************//*!
*
* Deinitialize
*
******************************************************************/
static unsigned char D4DTCH_DeInit_Tchres_Mqx(void)
{
   if( tchscr_dev )
   {     
      fclose( tchscr_dev );
      tchscr_dev = NULL;
   }
   return 1;
}    

/**************************************************************//*!
*
* Initialize
*
******************************************************************/
static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Tchres_Mqx(void)
{
   static D4D_TOUCHSCREEN_LIMITS touchscreen_limits;
   TCHRES_ADC_LIMITS_STRUCT limit_struct;

   if( tchscr_dev )
   {
      _io_ioctl(tchscr_dev, IO_IOCTL_TCHSCR_GET_RAW_LIMITS, &limit_struct);
   
//       *(&touchscreen_limits) = *(D4D_TOUCHSCREEN_LIMITS *)(&limit_struct);
      
      touchscreen_limits.rawDataScale = (unsigned short)limit_struct.FULL_SCALE;
      touchscreen_limits.touchMinX = (unsigned short)limit_struct.X_TOUCH_MIN;
      touchscreen_limits.touchMinY = (unsigned short)limit_struct.Y_TOUCH_MIN;
      touchscreen_limits.touchOffMaxX = (unsigned short)limit_struct.X_TOUCH_MAX;
      touchscreen_limits.touchOffMaxY = (unsigned short)limit_struct.Y_TOUCH_MAX;  
                
      return( &touchscreen_limits );
   }
   return( NULL );
}

//
//-----------------------------------------------------------------------------
// FUNCTION:    LCD_GetTouchScreenPositionRaw
// SCOPE:       Global
// DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y 
//              coordinates if screen touched 
// PARAMETERS:  unsigned int *TouchPositionX    Pointer to X coordinate
//              unsigned int *TouchPositionY    Pointer to Y ccordinate
// RETURNS:     0   no screen touch
//              1   screen touch, 
//-----------------------------------------------------------------------------
//
static unsigned char D4DTCH_GetPositionRaw_Tchres_Mqx (unsigned short *TouchPositionX, 
                                      unsigned short *TouchPositionY)
{ 
   TCHRES_POSITION_STRUCT  position;

   if( tchscr_dev == NULL )
   {
      return 0;
   }
   if (ioctl(tchscr_dev, IO_IOCTL_TCHSCR_GET_POSITION_RAW, &position ) == TCHRES_OK)
   {
      *TouchPositionX = (unsigned short)position.X;
      *TouchPositionY = (unsigned short)position.Y;
      return 1;
   }
   else
   {
      return 0;
   }
}

#endif
