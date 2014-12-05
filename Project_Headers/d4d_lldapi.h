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
* @file      d4d_lldapi.h
*
* @author    b01119
* 
* @version   0.0.16.0
* 
* @date      Aug-23-2010
* 
* @brief     D4D driver - resistive touch screen driver function header file 
*
*******************************************************************************/

#ifndef __D4D_LLD_API_H
#define __D4D_LLD_API_H


// LCD low level driver API functions
typedef enum
{
	Portrait	    = D4D_ORIENT_PORTRAIT,
	Portrait180   = D4D_ORIENT_PORTRAIT180,
	Landscape	    = D4D_ORIENT_LANDSCAPE,
	Landscape180  = D4D_ORIENT_LANDSCAPE180
} D4DLCD_ORIENTATION;

typedef enum 
{
  D4DHW_PIN_PULL_UP_ON,
  D4DHW_PIN_PULL_UP_OFF,
  D4DHW_PIN_OUT,
  D4DHW_PIN_IN,  
  D4DHW_PIN_SET_0,
  D4DHW_PIN_SET_1,
  D4DHW_PIN_GET,
  D4DHW_PIN_ADC_ON,
  D4DHW_PIN_ADC_OFF
}D4DHW_PIN_STATE;

typedef enum 
{
  D4DTCH_X_PLUS_PIN,
  D4DTCH_X_MINUS_PIN,
  D4DTCH_Y_PLUS_PIN,
  D4DTCH_Y_MINUS_PIN
}D4DTCHHW_PINS;

typedef enum 
{
  D4DLCD_RESET_PIN,
  D4DLCD_BACKLIGHT_PIN,
  D4DLCD_CS_PIN  
}D4DLCDHW_PINS;

typedef struct D4D_TOUCHSCREEN_LIMITS_S
{
  unsigned short rawDataScale;
  unsigned short touchMinX;
  unsigned short touchMinY;
  unsigned short touchOffMaxX;
  unsigned short touchOffMaxY;
}D4D_TOUCHSCREEN_LIMITS;

typedef struct D4DLCD_FRAMEBUFF_DESC_S
{
  unsigned long fb_start_addr;
	unsigned short lcd_x_max;
	unsigned short lcd_y_max;
	unsigned char bpp_byte;	
}D4DLCD_FRAMEBUFF_DESC;




typedef struct D4DLCD_FUNCTIONS_S
{
  unsigned char (*D4DLCD_Init)(void);
  unsigned char (*D4DLCD_SetWindow)(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  unsigned char (*D4DLCD_SetOrientation)(D4DLCD_ORIENTATION new_orientation);
  unsigned char (*D4DLCD_Send_PixelColor)(unsigned short value);
  unsigned short (*D4DLCD_Read_Word)(void);
  void (*D4DLCD_FlushBuffer)(void);
  void (*D4DLCD_Delay_ms)(unsigned short period);
  unsigned char (*D4DLCD_DeInit)(void);
}D4DLCD_FUNCTIONS;


typedef struct D4DLCDHW_FUNCTIONS_S
{
  unsigned char (*D4DLCDHW_Init)(void);
  unsigned char (*D4DLCDHW_SendDataWord)(unsigned short value);
  unsigned char (*D4DLCDHW_SendCmdWord)(unsigned short cmd);  
  unsigned short (*D4DLCDHW_ReadDataWord)(void);
  unsigned short (*D4DLCDHW_ReadCmdWord)(void);  
  unsigned char (*D4DLCDHW_PinCtl)(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  void (*D4DLCDHW_FlushBuffer)(void);
  unsigned char (*D4DLCDHW_DeInit)(void);
}D4DLCDHW_FUNCTIONS;

typedef struct D4DLCDHWFB_FUNCTIONS_S
{
  unsigned char (*D4DLCDHW_Init)(void);
  unsigned char (*D4DLCDHW_WriteData)(unsigned long addr, unsigned short value);
  unsigned short (*D4DLCDHW_ReadData)(unsigned long addr);
  D4DLCD_FRAMEBUFF_DESC* (*D4DLCDHW_GetFbDescriptor)(void);
  unsigned char (*D4DLCDHW_PinCtl)(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  void (*D4DLCDHW_FlushBuffer)(void);
  unsigned char (*D4DLCDHW_DeInit)(void);
}D4DLCDHWFB_FUNCTIONS;




typedef struct D4DTCH_FUNCTIONS_S
{
  unsigned char (*D4DTCH_Init)(void);
  unsigned char (*D4DTCH_GetPositionRaw)(unsigned short *TouchPositionX, unsigned short *TouchPositionY);  
  D4D_TOUCHSCREEN_LIMITS* (*D4DTCH_GetRawLimits)(void);
  unsigned char (*D4DTCH_DeInit)(void);
}D4DTCH_FUNCTIONS;
          
typedef struct D4DTCHHW_FUNCTIONS_S
{
  unsigned char (*D4DTCHHW_Init)(void);
  unsigned short (*D4DTCHHW_ReadTouchAxis)(D4DTCHHW_PINS pinId);
  D4D_TOUCHSCREEN_LIMITS* (*D4DTCHHW_GetRawLimits)(void);
  unsigned char (*D4DTCHHW_PinCtl)(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);
  unsigned char (*D4DTCHHW_DeInit)(void);
}D4DTCHHW_FUNCTIONS;


// Include the right low level driver for LCD/TFT

#ifndef D4D_LLD_LCD
  #error "You must #define symbol D4D_LLD_LCD in d4d_user_cfg.h header file"
#endif

/*
#ifndef D4D_LLD_LCD_HW
  #error "You must #define symbol D4D_LLD_LCD_HW in d4d_user_cfg.h header file"
#endif
*/
extern const D4DLCD_FUNCTIONS D4D_LLD_LCD;

#ifdef D4D_LLD_TCH
  extern const D4DTCH_FUNCTIONS D4D_LLD_TCH;
#endif

#ifdef D4D_LLD_TCH_HW
  extern const D4DTCHHW_FUNCTIONS D4D_LLD_TCH_HW;
#endif
 
// Touch screen low level driver API functions
#ifdef D4D_LLD_TCH

  /******************************************************************************
  * Global functions
  ******************************************************************************/

#endif

#endif /* __D4D_LLD_API_H */










