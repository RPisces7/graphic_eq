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
* @file      d4d_icon.h
*
* @author    b01119
* 
* @version   0.0.38.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D Driver icon object header file
*
*******************************************************************************/

#ifndef __D4D_ICON_H
#define __D4D_ICON_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_iconSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#ifndef D4D_ICON_F_DEFAULT
  #define D4D_ICON_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)
#endif

#ifndef D4D_ICON_TXT_PRTY_DEFAULT
  #define D4D_ICON_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_ICON_FNT_PRTY_DEFAULT
  #define D4D_ICON_FNT_PRTY_DEFAULT  ( 0 )
#endif

#ifndef D4D_ICON_ENABLE_ANIMATION
  #define D4D_ICON_ENABLE_ANIMATION D4D_TRUE
#endif

#ifndef D4D_ICON_ANIMATION_TICK_COUNTER
  #define D4D_ICON_ANIMATION_TICK_COUNTER  ( 4 )
#endif


typedef Byte D4D_ICON_INDEX;

typedef D4D_BMP D4D_ICON_BMPS;

typedef struct
{
    D4D_ICON_INDEX index;      // current value

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE    
    D4D_BOOL animationEnabled;
    D4D_ICON_INDEX tickCounter;
#endif
    
} D4D_ICON_DATA;

// ICON configuration (in ROM by default)

typedef struct
{
    D4D_STRING textBuff;       // ICON text
    D4D_POINT scrPos;           // position on the screen
    D4D_SIZE  scrSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4D_POINT txtOff;           // text coordinates as offset from scrPos
    D4D_COOR radius;            // icon corners radius
    const D4D_ICON_BMPS* const*  pBmpX;           // ICON state X idication bitmaps
    void (*OnValueChanged)(D4D_OBJECT_PTR pThis);
    D4D_ICON_DATA* pData;    
} D4D_ICON;

/******************************************************************************
* Macros 
******************************************************************************/

// getting the ICON structure from general OBJECT
#define D4D_GET_ICON(pObj) ((D4D_ICON*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/
          
#define _D4D_DECLARE_ICON_BEGIN(type, name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
    extern type D4D_ICON_BMPS*  const name##_bmps[];\
    static D4D_ICON_DATA name##_data; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_ICON_FNT_PRTY_DEFAULT  << D4D_BITFIELD_SHIFT, D4D_ICON_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_ICON name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties}, /* textBuff */ \
        { x, y },     /* scrPos     */ \
        { cx, cy },   /* scrSize    */ \
        { tx, ty },   /* txtOff     */ \
        radius,       /* corners radius */ \
        &(name##_bmps[0]),  /* pBmpX     */ \
        pOnValch,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_iconSysFunc, &(name##_params), (flags), pUser, pScheme)\
    \
    type D4D_ICON_BMPS*   const name##_bmps[] = \
    {    

#define D4D_DECLARE_ICON_BMP(pBmp) (pBmp),

#define D4D_DECLARE_ICON_END() NULL };


#define D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_CONST, name, text, x, y, cx, cy, tx, ty, 0, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)
  
#define D4D_DECLARE_STD_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, fontId, onvalch) \
  D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_ICON_BEGIN_AUTOSIZE(name, text, x, y, flags, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  D4D_DECLARE_ICON_BEGIN(name, text, x, y, 0, 0, 0, 0, (flags), pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_ICON_BEGIN_AUTOSIZE(name, text, x, y, fontId, onvalch)\
  D4D_DECLARE_STD_ICON_BEGIN(name, text, x, y, 0, 0, 0, 0, fontId, onvalch)

#define D4D_DECLARE_STD_ICON1(name, text, x, y, cx, cy, tx, ty, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_END()

#define D4D_DECLARE_STD_ICON2(name, text, x, y, cx, cy, tx, ty, pBmp1, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_BMP((pBmp1))\
    D4D_DECLARE_ICON_END()


// Rounded icon definition
#define D4D_DECLARE_RICON_BEGIN(name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_CONST, name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)
  
#define D4D_DECLARE_STD_RICON_BEGIN(name, text, x, y, cx, cy, tx, ty, radius, fontId, onvalch) \
  D4D_DECLARE_RICON_BEGIN(name, text, x, y, cx, cy, tx, ty, radius, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_RICON_BEGIN_AUTOSIZE(name, text, x, y, radius, flags, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  D4D_DECLARE_RICON_BEGIN(name, text, x, y, 0, 0, 0, 0, radius, (flags), pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RICON_BEGIN_AUTOSIZE(name, text, x, y, radius, fontId, onvalch)\
  D4D_DECLARE_STD_RICON_BEGIN(name, text, x, y, 0, 0, 0, 0, radius, fontId, onvalch)



// IN RAM instantions macros
#define D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, tx, ty, 0, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)

#define D4D_DECLARE_STD_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, fontId, onvalch) \
  D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_ICON_BEGIN_AUTOSIZE_INRAM(name, text, x, y, flags, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, 0, 0, 0, 0, (flags), pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_ICON_BEGIN_AUTOSIZE_INRAM(name, text, x, y, fontId, onvalch)\
  D4D_DECLARE_STD_ICON_BEGIN_INRAM(name, text, x, y, 0, 0, 0, 0, fontId, onvalch)

#define D4D_DECLARE_STD_ICON1_INRAM(name, text, x, y, cx, cy, tx, ty, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_END()

#define D4D_DECLARE_STD_ICON2_INRAM(name, text, x, y, cx, cy, tx, ty, pBmp1, pBmp0, font, onvalch) \
    D4D_DECLARE_ICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, (D4D_ICON_F_DEFAULT), NULL, font, NULL, onvalch, NULL)\
    D4D_DECLARE_ICON_BMP((pBmp0))\
    D4D_DECLARE_ICON_BMP((pBmp1))\
    D4D_DECLARE_ICON_END()

// Rounded icon definition
#define D4D_DECLARE_RICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_ICON_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, tx, ty, radius, flags, pScheme, fontId, pUser, pOnValch, pOnUsrMsg)
  
#define D4D_DECLARE_STD_RICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, radius, fontId, onvalch) \
  D4D_DECLARE_RICON_BEGIN_INRAM(name, text, x, y, cx, cy, tx, ty, radius, (D4D_ICON_F_DEFAULT), NULL, fontId, NULL, onvalch, NULL)

#define D4D_DECLARE_RICON_BEGIN_AUTOSIZE_INRAM(name, text, x, y, radius, flags, pScheme, fontId, pUser, onvalch, pOnUsrMsg) \
  D4D_DECLARE_RICON_BEGIN_INRAM(name, text, x, y, 0, 0, 0, 0, radius, (flags), pScheme, fontId, pUser, onvalch, pOnUsrMsg)

#define D4D_DECLARE_STD_RICON_BEGIN_AUTOSIZE_INRAM(name, text, x, y, radius, fontId, onvalch)\
  D4D_DECLARE_STD_RICON_BEGIN_INRAM(name, text, x, y, 0, 0, 0, 0, radius, fontId, onvalch)


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
void D4D_IconSetIndex(D4D_OBJECT_PTR pThis, D4D_ICON_INDEX index);
void D4D_IconChangeIndex(D4D_OBJECT_PTR pThis, sByte incr);
D4D_ICON_INDEX D4D_IconGetIndex(D4D_OBJECT_PTR pThis);

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE
  void D4D_IconAnimationStart(D4D_OBJECT_PTR pThis);
  void D4D_IconAnimationStop(D4D_OBJECT_PTR pThis);
#endif

D4D_ICON_INDEX D4D_IconGetBmpCount(D4D_OBJECT_PTR pThis);
// Obsolete functions, replaced by any general
#define D4D_IconSetText D4D_SetText




#endif /* __D4D_ICON_H */        


