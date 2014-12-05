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
* @file      d4dlcdhw_dragonfire_lcdc.h
*
* @author    b01119
* 
* @version   0.0.2.0
* 
* @date      Aug-30-2010
* 
* @brief     D4D driver - dragonfire_lcdc hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4DLCDHW_DRAGONFIRE_LCDC_H
#define __D4DLCDHW_DRAGONFIRE_LCDC_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_dragonfire_lcdc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_dragonfire_lcdc_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
		
		#ifndef D4DLCDHWFB_START_ADDRESS 
			#define D4DLCDHWFB_START_ADDRESS 0x40000000
		#endif	
		   
		#ifndef D4DLCDHWFB_X_MAX 
			#define D4DLCDHWFB_X_MAX 240
		#endif	
		   
		#ifndef D4DLCDHWFB_Y_MAX 
			#define D4DLCDHWFB_Y_MAX 320
		#endif	
				
		#ifndef D4DLCDHWFB_Y_BPP_BYTE 
			#define D4DLCDHWFB_Y_BPP_BYTE 2
		#endif	
		
		
		#ifndef D4DLCDHWFB_MIRROWED
			#define D4DLCDHWFB_MIRROWED 0
		#endif
		
		#ifndef D4DLCDHWFB_PANEL_TYPE	
			#define D4DLCDHWFB_PANEL_TYPE M52277EVB_PANEL
		#endif	
		   
		#ifndef D4DLCDHWFB_PANEL_CLKDIV	
			#define D4DLCDHWFB_PANEL_CLKDIV 7
		#endif	
		   
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
    
    #define MCF_LCDC_LCD_PCR_PANEL_TYPE(x)     (((x)&0x00000003)<<30)
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/



/************************* Structure/Union Data types ************************/
#define VDDEN_ON (*(unsigned long*)(0xC0080000))
//#define USE_m5329evb


typedef enum
{ ACD_DIV_0=0,
  ACD_DIV_1,
  ACD_DIV_2,
  ACD_DIV_3,
  ACD_DIV_4,
  ACD_DIV_5,
  ACD_DIV_6,
  ACD_DIV_7,
  ACD_DIV_8,
  ACD_DIV_9,
  ACD_DIV_10,
  ACD_DIV_11,
  ACD_DIV_12,
  ACD_DIV_13,
  ACD_DIV_14,
  ACD_DIV_15  	
} ACD_DIV;

typedef enum
{ SHARP_12SVGA=0,
  SHARP_35QVGA,
  SHARP_57QVGA,
  SHARP_64VGA,
  SHARP_104VGA,
  M52277EVB_PANEL
} Panel_Size ;


typedef enum 
{ MONO_PANEL=0,
  CSTN_PANEL,
  PANEL_UNUSED,
  TFT_PANEL=3
} LCDC_PANEL_TYPE;

typedef enum
{
 BPP1=0,
 BPP2=1,
 BPP4=2,
 BPP8=3,
 BPP12=4,
 BPP16=5,
 BPP18=6,
 RESERVED=7 
} BPP_TYPE;

  #endif
#endif /* __D4DLCDHW_DRAGONFIRE_LCDC_H */










