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
* @file      d4d_console.h
*
* @author    b01119
* 
* @version   0.0.11.0
* 
* @date      Mar-7-2011
* 
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_CONSOLE_H
#define __D4D_CONSOLE_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_consoleSysFunc;
void D4D_CnslScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/


  

#ifndef D4D_CNSL_F_DEFAULT
  #define D4D_CNSL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_TABSTOP)
#endif

#ifndef D4D_CNSL_F_SCRLBRS_DEFAULT
  #define D4D_CNSL_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)
#endif
                  
  

#ifndef D4D_CNSL_TXT_PRTY_DEFAULT
  #define D4D_CNSL_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_CNSL_FNT_PRTY_DEFAULT
  #define D4D_CNSL_FNT_PRTY_DEFAULT  ( 0 )
#endif

#ifndef D4D_CNSL_CURSOR_HEIGHT                  
  #define D4D_CNSL_CURSOR_HEIGHT  ( 1 )
#endif  
 
#ifndef D4D_CNSL_STD_TAB_SIZE                  
  #define D4D_CNSL_STD_TAB_SIZE  ( 8 )
#endif  

#ifndef D4D_CNSL_SCRLBR_WIDTH
  #define D4D_CNSL_SCRLBR_WIDTH  ( 16 )
#endif 

#ifndef D4D_CNSL_SCRLBR_STEP_V
  #define D4D_CNSL_SCRLBR_STEP_V  ( 2 )
#endif 

#ifndef D4D_CNSL_SCRLBR_STEP_H
  #define D4D_CNSL_SCRLBR_STEP_H  ( 8 )
#endif 

#ifndef D4D_CNSL_CURSOR_BLINK_TICK_COUNTER
  #define D4D_CNSL_CURSOR_BLINK_TICK_COUNTER  ( 10 )
#endif


typedef union
{
  D4D_BIT_FIELD all;
  struct
  {
    unsigned bCheckLine;
    unsigned bRedrawAll;
    unsigned bRedrawLine;
    unsigned bRedrawScrollBars;
    unsigned bRedrawCursor;
    unsigned bCursorState; 
  }bits;
}D4D_CNSL_FLAGS;
                                                 

typedef struct D4D_CNSL_DATA_S
{
	D4D_COOR lastLineIx;								// index of last line 
	D4D_POINT cursorPos;
	D4D_CNSL_FLAGS flags;
	D4D_INDEX tickCounter;
	D4D_FONT_PROPERTIES fnt_properties;	// propertis of used font for object strings
}D4D_CNSL_DATA;

// label configuration (goes to ROM by default)

typedef struct
{
    D4D_STRING textBuff;    // Console header text
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4D_COOR radius;         // radius of round corners 
    D4D_CHAR* pTxtArr;			 // Pointer on text array that will use Console to store all lines
    D4D_SIZE txtArrSize;     // Size of two dimensional array for text    
    D4D_FONT textFontId; 		 // Used font for whole text
    D4D_INDEX tabSize;       // TAb Size of console object
    const D4D_BMP*  pBmpIcon; // title icon
    D4D_OBJECT* pScrlBrHor;
    D4D_OBJECT* pScrlBrVer;    
    D4D_CNSL_DATA* pData;		 // Run time data needed for Console 	
} D4D_CONSOLE;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the CONSOLE structure from general OBJECT
#define D4D_GET_CONSOLE(pObj) ((D4D_CONSOLE*)((pObj)->pParam))
#define D4D_GET_CONSOLE_DATA(pObj) (D4D_GET_CONSOLE(pObj)->pData)

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_CONSOLE(type, name, text, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_CNSL_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT, D4D_CNSL_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static D4D_CNSL_DATA name##_data = { line_cnt - 1, {0, 0}, D4D_FALSE, 0, 0};\
    static D4D_CHAR name##_txtArray[line_cnt][char_cnt + 1];\
    extern const D4D_OBJECT name##_scrollBarHor;\
    extern const D4D_OBJECT name##_scrollBarVer;\
    static type D4D_CONSOLE name##_params = \
    { \
        { text, sizeof(text), titleFontId, &name##_strPrties}, /* textBuff */ \
        { x, y },   /* scrPos */ \
        { cx, cy }, /* scrSize */ \
        radius,     /* radius */ \
        (D4D_CHAR*)&name##_txtArray[0][0],		/* pTextArr */ \
        { char_cnt, line_cnt }, /* txtArrSize */ \
        fontId,			/* textFontId */ \
        tabSize,    /* tabSize */ \
        icon,       /* pBmpIcon */\
        (D4D_OBJECT*)&name##_scrollBarHor, /* pScrlBrHor */\
        (D4D_OBJECT*)&name##_scrollBarVer, /* pScrlBrVer */\
        &name##_data, /* pData */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_consoleSysFunc, &(name##_params), flags, pUser, pScheme)\
    \
    D4D_DECLARE_SCROLL_BAR_INRAM(name##_scrollBarHor,  0, 0, 20, 100, D4D_CNSL_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_CnslScrollBarsFeedBack, NULL) \
    \
    D4D_DECLARE_SCROLL_BAR_INRAM(name##_scrollBarVer,  20, 100, 100, 20, D4D_CNSL_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_CnslScrollBarsFeedBack, NULL)    


#define D4D_DECLARE_CONSOLE(name, text, x, y, cx, cy, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_CONST, name, text, x, y, cx, cy, 0, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_CONSOLE(name, text, x, y, cx, cy, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_CONSOLE(name, text, x, y, cx, cy, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, icon, D4D_CNSL_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_CONSOLE_AUTOSIZE(name, text, x, y, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_STD_CONSOLE(name, text, x, y, 0, 0, line_cnt, char_cnt, icon, fontId, titleFontId)

#define D4D_DECLARE_CONSOLE_AUTOSIZE(name, text, x, y, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_CONSOLE(name, text, x, y, 0, 0, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

// Rounded console definition
#define D4D_DECLARE_RCONSOLE(name, text, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_CONST, name, text, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RCONSOLE(name, text, x, y, cx, cy, radius, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_RCONSOLE(name, text, x, y, cx, cy, radius, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, icon, D4D_CNSL_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_RCONSOLE_AUTOSIZE(name, text, x, y, radius, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_STD_RCONSOLE(name, text, x, y, 0, 0, radius, line_cnt, char_cnt, icon, fontId, titleFontId)

#define D4D_DECLARE_RCONSOLE_AUTOSIZE(name, text, x, y, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_RCONSOLE(name, text, x, y, 0, 0, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)



// IN RAM instantions macro
#define D4D_DECLARE_CONSOLE_INRAM(name, text, x, y, cx, cy, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_NO_CONST, name, text, x, y, cx, cy, 0, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_CONSOLE_INRAM(name, text, x, y, cx, cy, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_CONSOLE_INRAM(name, text, x, y, cx, cy, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, icon, D4D_CNSL_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_CONSOLE_AUTOSIZE_INRAM(name, text, x, y, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_STD_CONSOLE_INRAM(name, text, x, y, 0, 0, line_cnt, char_cnt, icon, fontId, titleFontId)

#define D4D_DECLARE_CONSOLE_AUTOSIZE_INRAM(name, text, x, y, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_CONSOLE_INRAM(name, text, x, y, 0, 0, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)
    
// Rounded console definition
#define D4D_DECLARE_RCONSOLE_INRAM(name, text, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_CONSOLE(D4D_NO_CONST, name, text, x, y, cx, cy, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RCONSOLE_INRAM(name, text, x, y, cx, cy, radius, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_RCONSOLE_INRAM(name, text, x, y, cx, cy, radius, line_cnt, char_cnt, D4D_CNSL_STD_TAB_SIZE, icon, D4D_CNSL_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_RCONSOLE_AUTOSIZE_INRAM(name, text, x, y, radius, line_cnt, char_cnt, icon, fontId, titleFontId) \
  D4D_DECLARE_STD_RCONSOLE_INRAM(name, text, x, y, 0, 0, radius, line_cnt, char_cnt, icon, fontId, titleFontId)

#define D4D_DECLARE_RCONSOLE_AUTOSIZE_INRAM(name, text, x, y, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_RCONSOLE_INRAM(name, text, x, y, 0, 0, radius, line_cnt, char_cnt, tabSize, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)    


/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/

D4D_BOOL D4D_CnslPutChar(D4D_OBJECT_PTR pObj, D4D_CHAR ch);
D4D_BOOL D4D_CnslPutString(D4D_OBJECT_PTR pObj, D4D_CHAR* pText);
void D4D_CnslClearLine(D4D_OBJECT_PTR pObj, D4D_INDEX line);
void D4D_CnslGoToXY(D4D_OBJECT_PTR pObj, D4D_POINT newPosition);
void D4D_CnslEnsureVisible(D4D_OBJECT_PTR pObj);
void D4D_CnslClearAll(D4D_OBJECT_PTR pObj);
D4D_POINT D4D_CnslGetCursor(D4D_OBJECT_PTR pObj);
void D4D_CnslSetScrollBarPosition(D4D_OBJECT_PTR pObj, D4D_INDEX hor, D4D_INDEX ver);
D4D_POINT D4D_CnslGetScrollBarPosition(D4D_OBJECT_PTR pObj);
/*********************************************************
*
* Object API
*
*********************************************************/


#endif /* __D4D_CONSOLE_H */
