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
* @file      d4d_types.h
*
* @author    b01119
* 
* @version   0.0.25.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D Driver global types header file
*
*******************************************************************************/


#ifndef __D4D_TYPES_H
#define __D4D_TYPES_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#define D4D_FALSE 0
#define D4D_TRUE  1


#define  D4D_HC08      1
#define  D4D_HCS08     2
#define  D4D_HC12      10
#define  D4D_HCS12     11
#define  D4D_HCS12X    12
#define  D4D_MCF51     50
#define  D4D_MCF52     51
#define  D4D_MCF53     52
#define  D4D_MCF54     53
#define  D4D_MPC51		 60
#define  D4D_MK        80 

#define D4D_COLOR_SYSTEM_RGB565 16565
#define D4D_COLOR_SYSTEM_RGB332 8332
#define D4D_COLOR_SYSTEM_MONO 21


#ifndef D4D_COLOR_SYSTEM 
  #define D4D_COLOR_SYSTEM D4D_COLOR_SYSTEM_RGB565
#endif


#ifndef NULL
  #define NULL (void*)0
#endif

#ifndef D4D_USE_STANDARD_TYPES
  #define D4D_USE_STANDARD_TYPES D4D_FALSE
  #warning "The D4D is using own standard types Byte, Word etc. , because the D4D_USE_STANDARD_TYPES is not defined!"
#endif  

#if (D4D_USE_STANDARD_TYPES != D4D_TRUE)
  #ifndef Byte
    typedef unsigned char       Byte;
  #endif
  #ifndef sByte
    typedef signed char         sByte;
  #endif
  #ifndef Word
    typedef unsigned short      Word;
  #endif
  #ifndef sWord
    typedef signed short        sWord;
  #endif
  #ifndef LWord
    typedef unsigned long       LWord;
  #endif
  #ifndef sLWord
    typedef signed long         sLWord;
  #endif
  #ifndef DLWord
    typedef unsigned long long  DLWord;
  #endif
  #ifndef sDLWord
    typedef signed long long    sDLWord;
  #endif
#endif


#if (D4D_MCU_TYPE == D4D_HC08) || (D4D_MCU_TYPE == D4D_HCS08)
  typedef Byte D4D_BIT_FIELD; 
  typedef Byte D4D_BOOL;
#elif (D4D_MCU_TYPE == D4D_HC12) || (D4D_MCU_TYPE == D4D_HCS12) || (D4D_MCU_TYPE == D4D_HCS12X)
  typedef Word D4D_BIT_FIELD;
  typedef Word D4D_BOOL;  
#elif (D4D_MCU_TYPE == D4D_MCF51) || (D4D_MCU_TYPE == D4D_MCF52) || (D4D_MCU_TYPE == D4D_MPC51) || (D4D_MCU_TYPE == D4D_MK)
  typedef LWord D4D_BIT_FIELD;
  typedef LWord D4D_BOOL;
#else
  #error "Unsupported MCU type for D4D_BIT_FIELD/D4D_BOOL type!"
#endif 


#ifdef D4D_COOR_TYPE
  typedef D4D_COOR_TYPE D4D_COOR;
#else
  typedef Byte D4D_COOR;  
#endif

typedef struct 
{
    D4D_COOR x;
    D4D_COOR y;
} D4D_POINT;

typedef struct 
{
    D4D_COOR cx;
    D4D_COOR cy;
} D4D_SIZE;

// Color type definition
#if ((D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB565))
  typedef Word D4D_COLOR;
#elif ((D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB332))
  typedef Word D4D_COLOR;
#elif ((D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_MONO))
  typedef Byte D4D_COLOR;  
#else  
  #error D4D_COLOR not defined due to unsupported D4D system color.
#endif

typedef char D4D_CHAR;

typedef Byte D4D_LINETYPE;

typedef Word D4D_INDEX;
typedef sWord D4D_INDEX_DELTA;

typedef struct 
{
    const void *pData;
    const void *pParam;   // pointer to Pallete or filename (if pData == NULL)
} D4D_BMP;

typedef Byte D4D_KEYS;  

typedef Byte D4D_KEY_SCANCODE;


typedef enum
{
  D4D_OBJECT_BUTTON,
  D4D_OBJECT_CHECKBOX,
  D4D_OBJECT_GAUGE,
  D4D_OBJECT_GRAPH,
  D4D_OBJECT_ICON,
  D4D_OBJECT_LABEL,
  D4D_OBJECT_MENU,
  D4D_OBJECT_PICTURE,
  D4D_OBJECT_SLIDER,
  D4D_OBJECT_CONSOLE,
  D4D_OBJECT_SCROLLBAR,
  D4D_OBJECT_TEXTBOX,
  D4D_OBJECT_PROGRESS_BAR,
  D4D_OBJECT_UNKNOWN  
}D4D_OBJECT_TYPE;

typedef enum
{
  D4D_DIR_UP,
  D4D_DIR_DOWN,
  D4D_DIR_RIGHT,
  D4D_DIR_LEFT
}D4D_DIRECTION;

typedef enum
{
  D4D_QUAD_1,
  D4D_QUAD_2,
  D4D_QUAD_3,
  D4D_QUAD_4
}D4D_QUADRANT;

typedef enum
{
	D4D_ORIENT_PORTRAIT		,	
	D4D_ORIENT_PORTRAIT180	,	
	D4D_ORIENT_LANDSCAPE		,	
	D4D_ORIENT_LANDSCAPE180		
} D4D_ORIENTATION;

typedef struct
{
  unsigned char ScreenCalibrated;
  unsigned short TouchScreenXoffset;
  unsigned short TouchScreenYoffset;
  unsigned short TouchScreenXBitsPerPixelx16;
  unsigned short TouchScreenYBitsPerPixelx16;
}D4D_TOUCHSCREEN_CALIB;

typedef struct 
{
	D4D_BOOL touched;
	D4D_POINT point;	 
}D4D_TOUCHSCREEN_EVENT;

/******************************************************************************
* Macros 
******************************************************************************/



#endif /* __D4D_TYPES_H */
