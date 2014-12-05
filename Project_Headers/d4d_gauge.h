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
* @file      d4d_gauge.h
*
* @author    b01119
* 
* @version   0.0.40.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver gauge object header file
*
*******************************************************************************/

#ifndef __D4D_GAUGE_H
#define __D4D_GAUGE_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_gaugeSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_GAUGE_HUB_RADIUS
  #define D4D_GAUGE_HUB_RADIUS 3
#endif  

// standard gauge colors
#ifndef D4D_COLOR_GAUG_HUB
  #define D4D_COLOR_GAUG_HUB        D4D_COLOR_DARK_RED
#endif
#ifndef D4D_COLOR_GAUG_POINTER
  #define D4D_COLOR_GAUG_POINTER    D4D_COLOR_DARK_BLUE
#endif

  

/******************************************************************************
* Types
******************************************************************************/

// should be 8-bit (otherwise change the math)

typedef sByte D4D_GAUGE_VALUE;
typedef Byte D4D_GAUGE_ANGLE;

// definition of gauge direction

typedef Byte D4D_GAUGE_DIR;

#define D4D_CLOCK_WISE 0
#define D4D_ANTI_CLOCK_WISE 1



#define D4D_GAUGE_F_REDRAW_TEXT     0x0100
#define D4D_GAUGE_F_HUB             0x0200
#define D4D_GAUGE_F_THICK_POINTER   0x0400

#ifndef D4D_GAUGE_F_DEFAULT
  #define D4D_GAUGE_F_DEFAULT     (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | \
                                  D4D_OBJECT_F_FOCUSRECT | D4D_GAUGE_F_REDRAW_TEXT | D4D_GAUGE_F_HUB)
#endif

#ifndef D4D_GAUGE_TXT_PRTY_DEFAULT
  #define D4D_GAUGE_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_GAUGE_FNT_PRTY_DEFAULT
  #define D4D_GAUGE_FNT_PRTY_DEFAULT  ( 0 )
#endif


// gauge limits as part of gauge runtime data (in RAM)

typedef struct
{
    D4D_GAUGE_VALUE valueMin;   // minimal value (corresponds to angleMin)
    D4D_GAUGE_VALUE valueMax;   // maximal value (corresponds to angleMax)
    D4D_GAUGE_ANGLE angleMin;             // line angle from 0x00 to 0xFF
    D4D_GAUGE_ANGLE angleMax;             // line angle from 0x00 to 0xFF    
} D4D_GAUGE_LIMITS;

// gauge runtime data (always in RAM)

typedef struct
{
    D4D_GAUGE_VALUE value;      // current value
    D4D_GAUGE_VALUE valueLast;  // last pointer value drawn
    D4D_GAUGE_DIR direction;    // direction of pointer move (D4D_CLOCK_WISE or D4D_ANTI_CLOCK_WISE)
    D4D_GAUGE_LIMITS limits;    // gauge runtime configuration
    
} D4D_GAUGE_DATA;

// gauge configuration (in ROM by default)

typedef struct
{
    D4D_STRING textBuff;       // gauge text
    D4D_POINT scrPos;           // position on the screen
    D4D_SIZE  scrSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4D_COOR  radius;           // radius
    D4D_POINT txtOff;           // text coordinates as offset from scrPos
    D4D_POINT hubOff;           // hub coordinates as offset from scrPos
    D4D_COOR  pointerLen;       // lentgth of the pointer line
    const D4D_BMP*  pBmpBkgd;   // gauge background bitmap
    
    void (*OnValueChanged)(D4D_OBJECT_PTR pThis);
    
    D4D_GAUGE_DATA* pData;
    
} D4D_GAUGE;

/******************************************************************************
* Macros 
******************************************************************************/

// getting the GAUGE structure from general OBJECT
#define D4D_GET_GAUGE(pObj) ((D4D_GAUGE*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_GAUGE(type, name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
    static D4D_GAUGE_DATA name##_data = {0, 1, D4D_CLOCK_WISE, {0, 100, 0, 0x7F } }; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_GAUGE_FNT_PRTY_DEFAULT  << D4D_BITFIELD_SHIFT, D4D_GAUGE_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_GAUGE name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties},      /* pText */ \
        { x, y },       /* scrPos */ \
        { cx, cy },     /* scrSize */ \
        radius,         /* radius */  \
        { tx, ty },     /* txtOff */ \
        { kx, ky },     /* hubOff */ \
        plen,           /* pointerLen */ \
        pBmp,           /* pBmpBkgd */ \
        onvalch,        /* OnValueChanged */ \
        &(name##_data)  /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_gaugeSysFunc, &(name##_params), flags, pUser, pScheme)


#define D4D_DECLARE_GAUGE(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_CONST, name, text, x, y, cx, cy, 0, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_GAUGE(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, pBmp, font, onvalch) \
    D4D_DECLARE_GAUGE(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, font, NULL, onvalch, NULL)

// Rounded gauge definition
#define D4D_DECLARE_RGAUGE(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_CONST, name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RGAUGE(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, pBmp, font, onvalch) \
    D4D_DECLARE_RGAUGE(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, font, NULL, onvalch, NULL)


// IN RAM instantions macros
#define D4D_DECLARE_GAUGE_INRAM(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_NO_CONST, name, text, x, y, cx, cy, 0, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_GAUGE_INRAM(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, pBmp, font, onvalch) \
    D4D_DECLARE_GAUGE_INRAM(name, text, x, y, cx, cy, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, font, NULL, onvalch, NULL)
    
// Rounded gauge definition
#define D4D_DECLARE_RGAUGE_INRAM(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  _D4D_DECLARE_GAUGE(D4D_NO_CONST, name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, flags, pBmp, pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RGAUGE_INRAM(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, pBmp, font, onvalch) \
    D4D_DECLARE_RGAUGE_INRAM(name, text, x, y, cx, cy, radius, tx, ty, kx, ky, plen, (D4D_GAUGE_F_DEFAULT), pBmp, NULL, font, NULL, onvalch, NULL)    

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/

void D4D_GaugSetValue(D4D_OBJECT_PTR pThis, D4D_GAUGE_VALUE value);
D4D_GAUGE_VALUE D4D_GaugGetValue(D4D_OBJECT_PTR pThis);
  
void D4D_GaugSetLimits(D4D_OBJECT_PTR pThis, const D4D_GAUGE_LIMITS* pLimits);
void D4D_GaugGetLimits(D4D_OBJECT_PTR pThis, D4D_GAUGE_LIMITS* pLimits);

void D4D_GaugSetDir(D4D_OBJECT_PTR pThis, D4D_GAUGE_DIR direction);
D4D_GAUGE_DIR D4D_GaugGetDir(D4D_OBJECT_PTR pThis);


// Obsolete functions, replaced by any general
#define D4D_GaugeSetText D4D_SetText




#endif /* __D4D_GAUGE_H */




