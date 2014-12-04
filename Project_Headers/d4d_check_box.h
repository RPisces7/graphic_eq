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
* @file      d4d_check_box.h
*
* @author    b01119
* 
* @version   0.0.24.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver check box object header file
*
*******************************************************************************/

#ifndef __D4D_CHECKBOX_H
#define __D4D_CHECKBOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_checkBoxSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_CHECKBOX_BORDER_OFFSET
  #define D4D_CHECKBOX_BORDER_OFFSET 3
#endif

#ifndef D4D_CHECKBOX_TEXT_OFFSET
  #define D4D_CHECKBOX_TEXT_OFFSET 4
#endif


// standard check box colors
#ifndef D4D_COLOR_CHECKBOX_ICON_BCKG
  #define D4D_COLOR_CHECKBOX_ICON_BCKG  D4D_COLOR_WHITE
#endif

/******************************************************************************
* Types
******************************************************************************/

// Check box configuration (goes to ROM by default)

#define D4D_CHECKBOX_F_ICON_RECTANGLE  0x0100

#ifndef D4D_CHECKBOX_F_DEFAULT
  #define D4D_CHECKBOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE\
                                  | D4D_OBJECT_F_FOCUSRECT | D4D_CHECKBOX_F_ICON_RECTANGLE )
#endif

#ifndef D4D_CHECKBOX_TXT_PRTY_DEFAULT
  #define D4D_CHECKBOX_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_CHECKBOX_FNT_PRTY_DEFAULT
  #define D4D_CHECKBOX_FNT_PRTY_DEFAULT  ( 0 )
#endif

typedef union
{
  D4D_BIT_FIELD all;
  struct
  {
    unsigned bPressed   :1;
    unsigned bChecked   :1;     
  }bits;
  
}D4D_CHECKBOX_STATUS;


typedef struct 
{
    D4D_STRING textBuff;    // button text
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4D_COOR radius;         // button corners radius
    const D4D_BMP*  pBmpChecked;    // Checked state bitmap
    const D4D_BMP*  pBmpUnChecked;    // Checked state bitmap
    D4D_CHECKBOX_STATUS*  pStatus;    
    void (*OnChange)(D4D_OBJECT* pThis);
} D4D_CHECKBOX;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the CHECK BOX structure from general OBJECT
#define D4D_GET_CHECKBOX(pObj) ((D4D_CHECKBOX*)((pObj)->pParam))
#define D4D_GET_CHECKBOX_STATUS(pObj) ((D4D_CHECKBOX_STATUS*)(D4D_GET_CHECKBOX(pObj)->pStatus))


/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_CHECKBOX(type, name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
    static D4D_CHECKBOX_STATUS name##_status = { 0 }; \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_CHECKBOX_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT, D4D_CHECKBOX_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_CHECKBOX name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties}, /* textBuff */ \
        { x, y },   /* scrPos */ \
        { cx, cy }, /* scrSize */ \
        radius,     /* corners radius */ \
        pbmpChecked,      /* pBmpChecked */ \
        pbmpUnChecked,      /* pBmpUnChecked */ \
        &(name##_status), /* Status clear */ \
        onchange     /* Click event */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_checkBoxSysFunc, &(name##_params), flags, pUser, pScheme)


#define D4D_DECLARE_CHECKBOX(name, text, x, y, cx, cy, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_CONST, name, text, x, y, cx, cy, 0, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

#define D4D_DECLARE_STD_CHECKBOX(name, text, x, y, cx, cy, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_CHECKBOX(name, text, x, y, cx, cy, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)

#define D4D_DECLARE_STD_CHECKBOX_AUTOSIZE(name, text, x, y, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_STD_CHECKBOX(name, text, x, y, 0, 0, pbmpChecked, pbmpUnChecked, fontId, onchange)

// Rounded check box definition

#define D4D_DECLARE_RCHECKBOX(name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_CONST, name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

#define D4D_DECLARE_STD_RCHECKBOX(name, text, x, y, cx, cy, radius, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_RCHECKBOX(name, text, x, y, cx, cy, radius, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)

#define D4D_DECLARE_STD_RCHECKBOX_AUTOSIZE(name, text, x, y, radius, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_STD_RCHECKBOX(name, text, x, y, 0, 0, radius, pbmpChecked, pbmpUnChecked, fontId, onchange)


// IN RAM instantions macros
#define D4D_DECLARE_CHECKBOX_INRAM(name, text, x, y, cx, cy, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_NO_CONST, name, text, x, y, cx, cy, 0, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)
    
#define D4D_DECLARE_STD_CHECKBOX_INRAM(name, text, x, y, cx, cy, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_CHECKBOX_INRAM(name, text, x, y, cx, cy, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)

#define D4D_DECLARE_STD_CHECKBOX_AUTOSIZE_INRAM(name, text, x, y, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_STD_CHECKBOX_INRAM(name, text, x, y, 0, 0, pbmpChecked, pbmpUnChecked, fontId, onchange)


// Rounded check box definition

#define D4D_DECLARE_RCHECKBOX_INRAM(name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg) \
  _D4D_DECLARE_CHECKBOX(D4D_NO_CONST, name, text, x, y, cx, cy, radius, flags, pbmpChecked, pbmpUnChecked, pScheme, fontId, pUser, onchange, pOnUsrMsg)

#define D4D_DECLARE_STD_RCHECKBOX_INRAM(name, text, x, y, cx, cy, radius, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_RCHECKBOX_INRAM(name, text, x, y, cx, cy, radius, (D4D_CHECKBOX_F_DEFAULT), pbmpChecked, pbmpUnChecked, NULL, fontId, NULL, onchange, NULL)

#define D4D_DECLARE_STD_RCHECKBOX_AUTOSIZE_INRAM(name, text, x, y, radius, pbmpChecked, pbmpUnChecked, fontId, onchange) \
    D4D_DECLARE_STD_RCHECKBOX_INRAM(name, text, x, y, 0, 0, radius, pbmpChecked, pbmpUnChecked, fontId, onchange)
    

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* internal API
*
*********************************************************/

void D4D_CheckBoxSetValue(D4D_OBJECT_PTR pThis, Byte value);
Byte D4D_CheckBoxGetValue(D4D_OBJECT_PTR pThis);


// Obsolete functions, replaced by any general
#define D4D_CheckBoxSetText D4D_SetText



#endif /*__D4D_CHECKBOX_H*/












