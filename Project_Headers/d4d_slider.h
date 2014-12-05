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
* @file      d4d_slider.h
*
* @author    b01119
* 
* @version   0.0.41.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver slider object header file
*
*******************************************************************************/

#ifndef __D4D_SLIDER_H
#define __D4D_SLIDER_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_sliderSysFunc;
/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_SLDR_BAR_OFF_LENGTH
  #define D4D_SLDR_BAR_OFF_LENGTH  2
#endif
#ifndef D4D_SLDR_BAR_OFF_WIDTH
  #define D4D_SLDR_BAR_OFF_WIDTH  4
#endif

// standard slider colors
#ifndef D4D_COLOR_SLDR_BAR_FORE  
  #define D4D_COLOR_SLDR_BAR_FORE        D4D_COLOR_DARK_BLUE
#endif
#ifndef D4D_COLOR_SLDR_BAR_BCKG  
  #define D4D_COLOR_SLDR_BAR_BCKG   D4D_COLOR_BRIGHT_GREY
#endif

#ifndef D4D_COLOR_SLDR_BAR_START
  #define D4D_COLOR_SLDR_BAR_START  D4D_COLOR_GREEN
#endif
#ifndef D4D_COLOR_SLDR_BAR_END
  #define D4D_COLOR_SLDR_BAR_END  D4D_COLOR_RED
#endif


/******************************************************************************
* Types
******************************************************************************/

typedef sByte D4D_SLIDER_VALUE;

#define D4D_SLDR_F_BAR_AUTOCOLOR 0x0100
#define D4D_SLDR_F_TEXT_AUTOCOLOR 0x0200
#define D4D_SLDR_F_BAR_SCALECOLOR 0x0400

#define D4D_SLDR_F_AUTOCOLOR 0x0300


#ifndef D4D_SLDR_F_DEFAULT
  #define D4D_SLDR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TRANSP_TEXT)
#endif

#ifndef D4D_SLDR_TXT_PRTY_DEFAULT
  #define D4D_SLDR_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_SLDR_FNT_PRTY_DEFAULT
  #define D4D_SLDR_FNT_PRTY_DEFAULT  ( D4D_FNT_PRTY_TRANSPARENT_YES_MASK )
#endif


// slider limits as part of slider runtime data (in RAM)

typedef struct
{
    D4D_SLIDER_VALUE valueMin;   // minimal value - the maximum input range is 127!!!!
    D4D_SLIDER_VALUE valueMax;   // maximal value 
    D4D_SLIDER_VALUE step;       // keypress delta
    D4D_SLIDER_VALUE valueOrg;   // bar origin value (should be min <= org <= max)
    
} D4D_SLIDER_LIMITS;

// slider runtime data (always in RAM)

typedef struct
{
    D4D_SLIDER_VALUE value;      // current value
    D4D_SLIDER_LIMITS limits;    // slider runtime configuration
    D4D_COLOR colorBar;          // Color v Bar - can be changed
} D4D_SLIDER_DATA;

// slider configuration (in ROM by default)

typedef struct
{
    D4D_STRING textBuff;       // slider text
    D4D_POINT scrPos;           // position on the screen
    D4D_SIZE  scrSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4D_COOR  radius;           // radius
    D4D_POINT txtOff;           // text coordinates as offset from scrPos
    D4D_POINT barOff;           // hub coordinates as offset from scrPos
    D4D_SIZE  barSize;          // value bar size (if cx>cy, then bar is horizontal, otherwise vertical)
    const D4D_BMP*  pBmpBkgd;         // slider background bitmap
    void (*OnValueChanged)(D4D_OBJECT_PTR pThis);
    D4D_SLIDER_DATA* pData;
} D4D_SLIDER;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the SLIDER structure from general OBJECT
#define D4D_GET_SLIDER(pObj) ((D4D_SLIDER*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_SLIDER(type, name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
    static D4D_SLIDER_DATA name##_data = { 0, 0, 100, 1, 0, D4D_COLOR_SLDR_BAR_FORE}; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_SLDR_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT , D4D_SLDR_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_SLIDER name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties}, /* textBuff */ \
        { x, y },    /* scrPos     */ \
        { cx, cy },  /* scrSize    */ \
        radius,      /* radius     */ \
        { tx, ty },  /* txtOff     */ \
        { bx, by },  /* barOff     */ \
        { bcx, bcy }, /* barSize    */ \
        pBmp,        /* pBmpBkgd   */ \
        onvalch,     /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_sliderSysFunc, &(name##_params), flags, pUser, pScheme)


#define D4D_DECLARE_SLIDER(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER(D4D_CONST, name, text, x, y, cx, cy, 0, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_SLIDER(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_SLIDER(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_SLIDER_AUTOSIZE(name, text, x, y, cx, cy, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_SLIDER(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_SLIDER_AUTOSIZE(name, text, x, y, cx, cy, pBmp, font, onvalch) \
    D4D_DECLARE_STD_SLIDER(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, pBmp, font, onvalch)

// Rounded slider definition
#define D4D_DECLARE_RSLIDER(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER(D4D_CONST, name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RSLIDER(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_RSLIDER(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_RSLIDER_AUTOSIZE(name, text, x, y, cx, cy, radius, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_RSLIDER(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RSLIDER_AUTOSIZE(name, text, x, y, cx, cy, radius, pBmp, font, onvalch) \
    D4D_DECLARE_STD_RSLIDER(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, pBmp, font, onvalch)


// IN RAM instantions macros
#define D4D_DECLARE_SLIDER_INRAM(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER( D4D_NO_CONST, name, text, x, y, cx, cy, 0, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_SLIDER_INRAM(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_SLIDER_INRAM(name, text, x, y, cx, cy, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_SLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_SLIDER_INRAM(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_SLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, pBmp, font, onvalch) \
    D4D_DECLARE_STD_SLIDER_INRAM(name, text, x, y, cx, cy, 0, 0, 0, 0, 0, 0, pBmp, font, onvalch)
    
// Rounded slider definition
#define D4D_DECLARE_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_SLIDER(D4D_NO_CONST, name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, pBmp, fontId, onvalch) \
    D4D_DECLARE_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, tx, ty, bx, by, bcx, bcy, (D4D_SLDR_F_DEFAULT), pBmp, NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_RSLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, radius, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg) \
    D4D_DECLARE_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, flags, pBmp, pScheme, font, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RSLIDER_AUTOSIZE_INRAM(name, text, x, y, cx, cy, radius, pBmp, font, onvalch) \
    D4D_DECLARE_STD_RSLIDER_INRAM(name, text, x, y, cx, cy, radius, 0, 0, 0, 0, 0, 0, pBmp, font, onvalch)    

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/

void D4D_SldrSetValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE value);
D4D_SLIDER_VALUE D4D_SldrGetValue(D4D_OBJECT_PTR pThis);
void D4D_SldrChangeValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE incr);

void D4D_SldrSetLimits(D4D_OBJECT_PTR pThis, const D4D_SLIDER_LIMITS* pLimits);
void D4D_SldrGetLimits(D4D_OBJECT_PTR pThis, D4D_SLIDER_LIMITS* pLimits);

void D4D_SldrSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color);
D4D_COLOR D4D_SldrGetBarColor(D4D_OBJECT_PTR pThis);


// Obsolete functions, replaced by any general 
#define D4D_SldrSetText D4D_SetText


#endif /* __D4D_SLIDER_H */











