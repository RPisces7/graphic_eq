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
* @file      d4d_base.h
*
* @author    r50233
*
* @version   0.0.85.0
*
* @date      Apr-11-2011
*
* @brief     D4D Driver core and base functions header file
*
*******************************************************************************/

#ifndef __D4D_BASE_H
#define __D4D_BASE_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

#define D4D_VERSION "v2.07"  // to check release history - read the release_notes.txt file

/*********************************************************
*
* D4D constants
*
*********************************************************/

#define D4D_LINE_THIN           0x00
#define D4D_LINE_THICK          0x01

/*********************************************************
*
* general helper constants
*
*********************************************************/

#ifndef D4D_MCU_TYPE
  #define D4D_MCU_TYPE D4D_MCF51
  #warning "There is no definition for D4D_MCU_TYPE in configuration file. The driver is using as default D4D_MCF51."
#endif

#ifndef D4D_SCREEN_HISTORY
  #define D4D_SCREEN_HISTORY 5
#endif

#ifndef D4D_ENABLE_AUTOSIZE
  #define D4D_ENABLE_AUTOSIZE D4D_FALSE
#endif

#ifndef D4D_ROUND_CORNER_ENABLE
  #define D4D_ROUND_CORNER_ENABLE D4D_FALSE
#endif

#ifndef D4D_FONT_TABLE_DISABLED
  #define D4D_FONT_TABLE_DISABLED D4D_TRUE
  #warning "Font table is disabled by default, set the D4D_FONT_TABLE_DISABLED to D4D_TRUE in config file."
#endif

#ifndef D4D_SCREEN_SIZE_LONGER_SIDE
	#define D4D_SCREEN_SIZE_LONGER_SIDE 320
	#warning "The D4D configuration file doesn't contains the resolution information for longer axis. The D4D sets this value to default: 320 pixels."
#endif

#ifndef D4D_SCREEN_SIZE_SHORTER_SIDE
	#define D4D_SCREEN_SIZE_SHORTER_SIDE 240
	#warning "The D4D configuration file doesn't contains the resolution information for shorter axis. The D4D sets this value to default: 240 pixels."
#endif

#if D4D_SCREEN_SIZE_SHORTER_SIDE > D4D_SCREEN_SIZE_LONGER_SIDE
	#warning "The D4D_SCREEN_SIZE_SHORTER_SIDE macro has greater value then D4D_SCREEN_SIZE_LONGER_SIDE in d4d_cfg.h file. Please check!"
#endif

#ifndef D4D_MCU_BUS_CLOCK
  #define D4D_MCU_BUS_CLOCK 25000000L
#endif

#ifndef D4D_COLOR_SYSTEM_FORE
  #define D4D_COLOR_SYSTEM_FORE D4D_COLOR_YELLOW
#endif

#ifndef D4D_COLOR_SYSTEM_BCKG
  #define D4D_COLOR_SYSTEM_BCKG D4D_COLOR_BLUE
#endif

#ifndef D4D_FONT_SYSTEM_DEFAULT
  #define D4D_FONT_SYSTEM_DEFAULT 0
#endif

#ifndef D4D_EXTSRC_TEXT_ENABLE
  #define D4D_EXTSRC_TEXT_ENABLE D4D_FALSE
#endif

#ifndef D4D_EXTSRC_FILE_ENABLE
  #define D4D_EXTSRC_FILE_ENABLE D4D_FALSE
#endif


#ifndef D4D_EXTSRC_TEXT_FLAGCHAR
  #define D4D_EXTSRC_TEXT_FLAGCHAR '^'
#endif

#ifndef D4D_EXTSRC_BUFF_SIZE
  #define D4D_EXTSRC_BUFF_SIZE 64
#endif


//#define D4D_DEBUG D4D_FALSE

/*********************************************************
*
* keys setting  constants
*
*********************************************************/



#ifndef D4D_KEY_UP
  #define D4D_KEY_UP     0x01
#endif
#ifndef D4D_KEY_DOWN
  #define D4D_KEY_DOWN   0x02
#endif
#ifndef D4D_KEY_LEFT
  #define D4D_KEY_LEFT   0x04
#endif
#ifndef D4D_KEY_RIGHT
  #define D4D_KEY_RIGHT  0x08
#endif
#ifndef D4D_KEY_ENTER
  #define D4D_KEY_ENTER  0x10
#endif
#ifndef D4D_KEY_ESC
  #define D4D_KEY_ESC    0x20
#endif


#ifndef D4D_KEY_SCANCODE_UP
  #define D4D_KEY_SCANCODE_UP     0x51
#endif

#ifndef D4D_KEY_SCANCODE_DOWN
  #define D4D_KEY_SCANCODE_DOWN   0x50
#endif

#ifndef D4D_KEY_SCANCODE_LEFT
  #define D4D_KEY_SCANCODE_LEFT   0x4B
#endif

#ifndef D4D_KEY_SCANCODE_RIGHT
  #define D4D_KEY_SCANCODE_RIGHT  0x4D
#endif

#ifndef D4D_KEY_SCANCODE_ENTER
  #define D4D_KEY_SCANCODE_ENTER  0x1C
#endif

#ifndef D4D_KEY_SCANCODE_ESC
  #define D4D_KEY_SCANCODE_ESC    0x01
#endif

#define D4D_KEY_SCANCODE_KEYMASK  0x7F
#define D4D_KEY_SCANCODE_RELEASEMASK  0x80

#ifndef D4D_KEY_FUNC_FOCUS_NEXT
  #define D4D_KEY_FUNC_FOCUS_NEXT    D4D_KEY_SCANCODE_DOWN
#endif

#ifndef D4D_KEY_FUNC_FOCUS_PREV
  #define D4D_KEY_FUNC_FOCUS_PREV    D4D_KEY_SCANCODE_UP
#endif


#ifndef D4D_KEYS_BUFF_LENGTH
  #define D4D_KEYS_BUFF_LENGTH 4
#endif

/*********************************************************
*
* touch screen constants
*
*********************************************************/


#ifndef D4DTCH_CALIB_CROSS_OFFSET
  #define D4DTCH_CALIB_CROSS_OFFSET 30
#endif

/******************************************************************************
* Types
******************************************************************************/

/*********************************************************
*
* general helper types
*
*********************************************************/

#define D4D_BITFIELD_LSB_ALLIGMENT_RIGHT 0
#define D4D_BITFIELD_LSB_ALLIGMENT_LEFT  1


#ifndef D4D_BITFIELD_LSB_ALIGNMENT
  #if (D4D_MCU_TYPE == D4D_MCF52) || (D4D_MCU_TYPE == D4D_MPC51)
    #define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_LEFT
  #elif (D4D_MCU_TYPE == D4D_HC12) || (D4D_MCU_TYPE == D4D_HCS12) || (D4D_MCU_TYPE == D4D_HCS12X)
    #define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #elif (D4D_MCU_TYPE == D4D_MK)
    #define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #else  // for other cases (this should not work for every cases)
    #define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #endif
#endif

#ifndef D4D_BITFIELD_SHIFT
  #if (D4D_MCU_TYPE == D4D_MCF52) || (D4D_MCU_TYPE == D4D_MPC51)
    #define D4D_BITFIELD_SHIFT 24
  #elif (D4D_MCU_TYPE == D4D_HC12) || (D4D_MCU_TYPE == D4D_HCS12) || (D4D_MCU_TYPE == D4D_HCS12X)
    #define D4D_BITFIELD_SHIFT 8
  #elif (D4D_MCU_TYPE == D4D_MK)
    #define D4D_BITFIELD_SHIFT 0
  #else // for other cases (this should not work for every cases)
    #define D4D_BITFIELD_SHIFT 0
  #endif
#endif

#define D4D_ENDIAN_BIG 0
#define D4D_ENDIAN_LITTLE 1


#ifndef D4D_ENDIAN
  #if (D4D_MCU_TYPE == D4D_MK)
    #define D4D_ENDIAN D4D_ENDIAN_LITTLE
  #else  // for other cases (this should not work for every cases)
    #define D4D_ENDIAN D4D_ENDIAN_BIG
  #endif
#endif

#define D4D_SWAP_WORD(x) (((x) << 8) | ((x) >> 8))

#if D4D_ENDIAN == D4D_ENDIAN_BIG
  #define D4D_MODIFY_WORD(x) (x)
#else
  #define D4D_MODIFY_WORD(x) D4D_SWAP_WORD(x)
#endif


typedef struct
{
  D4D_KEY_SCANCODE buff[D4D_KEYS_BUFF_LENGTH];
  Byte readPos;
  Byte writePos;
} D4D_KEYS_BUFFER;

typedef union
{
  D4D_BIT_FIELD all;
  struct
  {
    unsigned bNewTouch:1;
    unsigned bAutorepeat:1;
    unsigned bTouched:1;
    unsigned bUnTouch:1;
    unsigned bNewRawEvent:1;
    unsigned bNewEvent:1;
    unsigned bCheckTouchScreen:1;
  }bits;
  struct
  {
    unsigned bSystemFlags:4;
    unsigned bEvents:3;
  }merged_bits;

}D4D_TOUCHSCREEN_STATUS;

typedef union
{
  D4D_BIT_FIELD all;
  struct
  {
    unsigned bTimeTick:1;
  }bits;
}D4D_SYSTEM_FLAGS;




typedef union D4D_OBJECT_DRAWFLAGS_S
{
    D4D_BIT_FIELD all;

    struct
    {
        unsigned bComplete  :1;   // complete redraw
        unsigned bFocused   :1;    // draw as focused
        unsigned bCapturing :1;  // draw as active (capturing keys)
    } bits;
} D4D_OBJECT_DRAWFLAGS;

/*********************************************************
*
* object messages
*
*********************************************************/

typedef enum
{
    D4D_MSG_DRAW,           // parameter wprm_DRAW
    D4D_MSG_DRAWDONE,       // parameter wprm_DRAW
    D4D_MSG_KEYUP,          // parameter wprm_KEYS
    D4D_MSG_KEYDOWN,        // parameter wprm_KEYS
    D4D_MSG_SETFOCUS,       // no parameter
    D4D_MSG_KILLFOCUS,      // no parameter
    D4D_MSG_SETCAPTURE,     // no parameter
    D4D_MSG_KILLCAPTURE,    // no parameter
    D4D_MSG_ONINIT,         // no parameter
    D4D_MSG_TOUCHED,        // no parameter
    D4D_MSG_TOUCH_AUTO,     // no parameter
    D4D_MSG_UNTOUCHED,      // no parameter
    D4D_MSG_TIMETICK        // no parameter
} D4D_MSGID;

/* the message object */

struct D4D_OBJECT_S;

typedef struct D4D_MESSAGE_S
{
    struct D4D_OBJECT_S* pObject;
    struct D4D_SCREEN_S* pScreen;
    D4D_MSGID   nMsgId;

    union
    {
        D4D_OBJECT_DRAWFLAGS draw;
        D4D_KEY_SCANCODE key;      // there will be stored only code of key without release / press information
                                   // this information is handlad by message ID

    } prm;

} D4D_MESSAGE;

#define D4D_MSG_NOSKIP 0
#define D4D_MSG_SKIP 1


/******************************************************************************
* Macros
******************************************************************************/

/*********************************************************
*
* internal macros
*
*********************************************************/

#define D4D_UNUSED(x) (void)(x);
#define D4D_CONST const
#define D4D_NO_CONST

/*********************************************************
*
* external macros
*
*********************************************************/

#define D4D_DrawTextTab(ppt, buffText, pTab, colorText, colorBkgd) D4D_DrawTextTabXY((ppt)->x, (ppt)->y, buffText, pTab, colorText, colorBkgd)
#define D4D_DrawTextXY(x, y, buffText, colorText, colorBkgd) D4D_DrawTextTabXY(x, y, buffText, NULL, colorText, colorBkgd)
#define D4D_DrawText(ppt, buffText, colorText, colorBkgd) D4D_DrawTextTab(ppt, buffText, NULL, colorText, colorBkgd)

#define D4D_DrawTextRect(ppt, psz, buffText, colorText, colorBkgd) D4D_DrawTextRectTab(ppt, psz, buffText, NULL, colorText, colorBkgd)
#define D4D_DrawTextRectTo(ppt, buffText, colorText, colorBkgd) D4D_DrawTextRectTabTo(ppt, buffText, NULL, colorText, colorBkgd)
#define D4D_DrawTextRectXY(x1, y1, x2, y2, buffText, colorText, colorBkgd) D4D_DrawTextRectTabXY(x1, y1, x2, y2, buffText, NULL, colorText, colorBkgd)
#define D4D_DrawTextRectToXY(x, y, buffText, colorText, colorBkgd) D4D_DrawTextRectTabToXY(x, y, buffText, NULL, colorText, colorBkgd)

#define D4D_DrawTextRectTab(ppt, psz, buffText, pTab, colorText, colorBkgd) D4D_DrawTextRectTabAdv(ppt, psz, buffText, 0, pTab, colorText, colorBkgd)
#define D4D_DrawTextRectTabTo(ppt, buffText, pTab, colorText, colorBkgd) D4D_DrawTextRectTabAdvTo(ppt, buffText, 0, pTab, colorText, colorBkgd)
#define D4D_DrawTextRectTabXY(x1, y1, x2, y2, buffText, pTab, colorText, colorBkgd) D4D_DrawTextRectTabAdvXY(x1, y1, x2, y2, buffText, 0, pTab, colorText, colorBkgd)
#define D4D_DrawTextRectTabToXY(x, y, buffText, pTab, colorText, colorBkgd) D4D_DrawTextRectTabAdvToXY(x, y, buffText, 0, pTab, colorText, colorBkgd)


#define D4D_GetTextBuffWidthTab(text_buffer, pTab) D4D_GetTextBuffWidthTabAdj(text_buffer, 0, pTab)

#define D4D_GetTextBuffWidth(text_buffer) D4D_GetTextBuffWidthTab(text_buffer, NULL)

/*********************************************************
*
* static declaration macros
*
*********************************************************/
#define D4D_DECLARE_BMP(name, pbmp, ppal) \
    const D4D_BMP name = \
    { \
        pbmp, ppal \
    };


#define D4D_DECLARE_BMPFILE(name, filename) \
    const D4D_BMP name = \
    { \
        NULL, filename \
    };


#define D4D_EXTERN_BMP(name) \
    extern const D4D_BMP name

/******************************************************************************
* Global variables
******************************************************************************/

// global shared memory used by multiple modules as temporary storage
#define D4D_SCRATCHPAD_SIZE 36
extern Byte d4d_scratchPad[D4D_SCRATCHPAD_SIZE];

// assert macro
#define D4D_ASSERT(cond) if(!(cond)) for(;;)

/******************************************************************************
* Global functions
******************************************************************************/

#ifdef D4D_INPUT_EVENT_CALLBACK
   extern void D4D_INPUT_EVENT_CALLBACK(void);
#endif

D4D_COOR D4D_RndCornerGetNonPrintedPxl(D4D_COOR radius, D4D_COOR line);

/*********************************************************
*
* internal API
*
*********************************************************/


#endif /* __D4D_BASE_H */










