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
* @file      d4d_button.h
*
* @author    r50233
* 
* @version   0.0.35.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver button object header file
*
*******************************************************************************/

#ifndef __D4D_BUTTON_H
#define __D4D_BUTTON_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_btnSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_BTN_BORDER_OFFSET
  #define D4D_BTN_BORDER_OFFSET 3
#endif

/******************************************************************************
* Types
******************************************************************************/

// button configuration (goes to ROM by default)

#ifndef D4D_BTN_F_DEFAULT
  #define D4D_BTN_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT)
#endif

#ifndef D4D_BTN_TXT_PRTY_DEFAULT
  #define D4D_BTN_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_BTN_FNT_PRTY_DEFAULT
  #define D4D_BTN_FNT_PRTY_DEFAULT  ( 0 )
#endif


typedef union
{
  D4D_BIT_FIELD all;
  struct
  {
    unsigned bPressed   :1;      
  }bits;
  
}D4D_BUTTON_STATUS;


typedef struct 
{
    D4D_STRING textBuff;    // button text
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4D_COOR radius;         // button corners radius
    const D4D_BMP*  pBmpNormal;    // normal state bitmap
    const D4D_BMP*  pBmpFocus;     // focused state bitmap
    D4D_BUTTON_STATUS*  pStatus;    
    void (*OnClicked)(D4D_OBJECT* pThis);
} D4D_BUTTON;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the BUTTON structure from general OBJECT
#define D4D_GET_BUTTON(pObj) ((D4D_BUTTON*)((pObj)->pParam))
#define D4D_GET_BUTTON_STATUS(pObj) ((D4D_BUTTON_STATUS*)(D4D_GET_BUTTON(pObj)->pStatus))


/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_BUTTON(type, name, text, x, y, cx, cy, radius, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
    static D4D_BUTTON_STATUS  name##_status = { 0 }; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_BTN_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT, D4D_BTN_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static const D4D_BUTTON name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties}, /* textBuff */ \
        { x, y },   /* scrPos */ \
        { cx, cy }, /* scrSize */ \
        radius,     /* corners radius */ \
        pbmpN,      /* pBmpNormal */ \
        pbmpF,      /* pBmpFocus */ \
        &(name##_status), /* Status clear */ \
        onclick     /* Click event */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg,  &d4d_btnSysFunc, &(name##_params), flags, pUser, pScheme)

#define D4D_DECLARE_BUTTON(name, text, x, y, cx, cy, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_CONST, name, text, x, y, cx, cy, 0, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

#define D4D_DECLARE_STD_BUTTON(name, text, x, y, cx, cy, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_BUTTON(name, text, x, y, cx, cy, (D4D_BTN_F_DEFAULT), bmpN, bmpF, NULL, fontId, NULL, onclick, NULL)

#define D4D_DECLARE_TXT_BUTTON(name, text, x, y, cx, cy, fontId, onclick) \
    D4D_DECLARE_STD_BUTTON(name, text, x, y, cx, cy,  NULL, NULL, \
        fontId, onclick)

#define D4D_DECLARE_TXT_BUTTON_AUTOSIZE(name, text, x, y, fontId, onclick) \
    D4D_DECLARE_TXT_BUTTON(name, text, x, y, 0, 0, fontId, onclick)

#define D4D_DECLARE_STD_BUTTON_AUTOSIZE(name, text, x, y, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_STD_BUTTON(name, text, x, y, 0, 0, bmpN, bmpF, fontId, onclick)

// Rounded button definition
#define D4D_DECLARE_RBUTTON(name, text, x, y, cx, cy, radius, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_CONST, name, text, x, y, cx, cy, radius, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

#define D4D_DECLARE_STD_RBUTTON(name, text, x, y, cx, cy, radius, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_RBUTTON(name, text, x, y, cx, cy, radius, (D4D_BTN_F_DEFAULT), bmpN, bmpF, NULL, fontId, NULL, onclick, NULL)

#define D4D_DECLARE_TXT_RBUTTON(name, text, x, y, cx, cy, radius, fontId, onclick) \
    D4D_DECLARE_STD_RBUTTON(name, text, x, y, cx, cy,  radius, NULL, NULL, \
        fontId, onclick)

#define D4D_DECLARE_TXT_RBUTTON_AUTOSIZE(name, text, x, y, radius, fontId, onclick) \
    D4D_DECLARE_TXT_RBUTTON(name, text, x, y, 0, 0, radius, fontId, onclick)

#define D4D_DECLARE_STD_RBUTTON_AUTOSIZE(name, text, x, y, radius, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_STD_RBUTTON(name, text, x, y, 0, 0, radius, bmpN, bmpF, fontId, onclick)


// IN RAM instantion macros
#define D4D_DECLARE_BUTTON_INRAM(name, text, x, y, cx, cy, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_NO_CONST, name, text, x, y, cx, cy, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

#define D4D_DECLARE_STD_BUTTON_INRAM(name, text, x, y, cx, cy, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_BUTTON_INRAM(name, text, x, y, cx, cy, (D4D_BTN_F_DEFAULT), bmpN, bmpF, NULL, fontId, NULL, onclick, NULL)

#define D4D_DECLARE_TXT_BUTTON_INRAM(name, text, x, y, cx, cy, fontId, onclick) \
    D4D_DECLARE_STD_BUTTON_INRAM(name, text, x, y, cx, cy,  NULL, NULL, \
        fontId, onclick)

#define D4D_DECLARE_TXT_BUTTON_AUTOSIZE_INRAM(name, text, x, y, fontId, onclick) \
    D4D_DECLARE_TXT_BUTTON_INRAM(name, text, x, y, 0, 0, fontId, onclick)


#define D4D_DECLARE_STD_BUTTON_AUTOSIZE_INRAM(name, text, x, y, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_STD_BUTTON_INRAM(name, text, x, y, 0, 0, bmpN, bmpF, fontId, onclick)
    
    
// Rounded button definition
#define D4D_DECLARE_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg) \
  _D4D_DECLARE_BUTTON(D4D_NO_CONST, name, text, x, y, cx, cy, radius, flags, pbmpN, pbmpF, pScheme, fontId, pUser, onclick, pOnUsrMsg)

#define D4D_DECLARE_STD_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, (D4D_BTN_F_DEFAULT), bmpN, bmpF, NULL, fontId, NULL, onclick, NULL)

#define D4D_DECLARE_TXT_RBUTTON_INRAM(name, text, x, y, cx, cy, radius, fontId, onclick) \
    D4D_DECLARE_STD_RBUTTON_INRAM(name, text, x, y, cx, cy,  radius, NULL, NULL, \
        fontId, onclick)

#define D4D_DECLARE_TXT_RBUTTON_AUTOSIZE_INRAM(name, text, x, y, radius, fontId, onclick) \
    D4D_DECLARE_TXT_RBUTTON_INRAM(name, text, x, y, 0, 0, radius, fontId, onclick)

#define D4D_DECLARE_STD_RBUTTON_AUTOSIZE_INRAM(name, text, x, y, radius, bmpN, bmpF, fontId, onclick) \
    D4D_DECLARE_STD_RBUTTON_INRAM(name, text, x, y, 0, 0, radius, bmpN, bmpF, fontId, onclick)    


/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/

// Obsolete functions, replaced by any general
#define  D4D_BtnSetText D4D_SetText

#endif /*__D4D_BUTTON_H*/












