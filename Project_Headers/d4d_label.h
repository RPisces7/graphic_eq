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
* @file      d4d_label.h
*
* @author    b01119
* 
* @version   0.0.33.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_LABEL_H
#define __D4D_LABEL_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_labelSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#define D4D_LBL_F_TEXTALIGN    0x0300

#define D4D_LBL_F_TEXTALIGN_L  0x0000
#define D4D_LBL_F_TEXTALIGN_R  0x0100
#define D4D_LBL_F_TEXTALIGN_C  0x0200
  

#ifndef D4D_LBL_F_DEFAULT
  #define D4D_LBL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT)
#endif  

#ifndef D4D_LBL_TXT_PRTY_DEFAULT
  #define D4D_LBL_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_LBL_FNT_PRTY_DEFAULT
  #define D4D_LBL_FNT_PRTY_DEFAULT  ( 0 )
#endif


// label configuration (goes to ROM by default)

typedef struct
{
    D4D_STRING textBuff;    // label text
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)    
    D4D_COOR radius;         // corner radius 
} D4D_LABEL;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the LABEL structure from general OBJECT
#define D4D_GET_LABEL(pObj) ((D4D_LABEL*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_LABEL(type, name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg) \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_LBL_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT, D4D_LBL_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_LABEL name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties}, /* textBuff */ \
        { x, y },   /* scrPos */ \
        { cx, cy }, /* scrSize */ \
        radius      /* corner radius */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_labelSysFunc, &(name##_params), flags, pUser, pScheme)
 

#define D4D_DECLARE_LABEL(name, text, x, y, cx, cy, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_CONST, name, text, x, y, cx, cy, 0, flags, pScheme, fontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_LABEL(name, text, x, y, cx, cy, font) \
    D4D_DECLARE_LABEL(name, text, x, y, cx, cy, (D4D_LBL_F_DEFAULT) , NULL, font, NULL, NULL)

#define D4D_DECLARE_LABEL_AUTOSIZE(name, text, x, y, flags, pScheme, font, pUser, pOnUsrMsg) \
    D4D_DECLARE_LABEL(name, text, x, y, 0, 0, flags , pScheme, font, pUser, pOnUsrMsg)
    
#define D4D_DECLARE_STD_LABEL_AUTOSIZE(name, text, x, y, font) \
    D4D_DECLARE_STD_LABEL(name, text, x, y, 0, 0, font)

// Rounded button definition
#define D4D_DECLARE_RLABEL(name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_CONST, name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RLABEL(name, text, x, y, cx, cy, radius, font) \
    D4D_DECLARE_RLABEL(name, text, x, y, cx, cy, radius, (D4D_LBL_F_DEFAULT) , NULL, font, NULL, NULL)

#define D4D_DECLARE_RLABEL_AUTOSIZE(name, text, x, y, radius, flags, pScheme, font, pUser, pOnUsrMsg) \
    D4D_DECLARE_RLABEL(name, text, x, y, 0, 0, radius, flags , pScheme, font, pUser, pOnUsrMsg)
    
#define D4D_DECLARE_STD_RLABEL_AUTOSIZE(name, text, x, y, radius, font) \
    D4D_DECLARE_STD_RLABEL(name, text, x, y, 0, 0, radius, font)


// IN RAM instantions macros 
#define D4D_DECLARE_LABEL_INRAM(name, text, x, y, cx, cy, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_NO_CONST, name, text, x, y, cx, cy, 0, flags, pScheme, fontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_LABEL_INRAM(name, text, x, y, cx, cy, font) \
    D4D_DECLARE_LABEL_INRAM(name, text, x, y, cx, cy, (D4D_LBL_F_DEFAULT) , NULL, font, NULL, NULL)

#define D4D_DECLARE_LABEL_AUTOSIZE_INRAM(name, text, x, y, flags, pScheme, font, pUser, pOnUsrMsg) \
    D4D_DECLARE_LABEL_INRAM(name, text, x, y, 0, 0, flags , pScheme, font, pUser, pOnUsrMsg)
    
#define D4D_DECLARE_STD_LABEL_AUTOSIZE_INRAM(name, text, x, y, font) \
    D4D_DECLARE_STD_LABEL_INRAM(name, text, x, y, 0, 0, font)
    
// Rounded button definition
#define D4D_DECLARE_RLABEL_INRAM(name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_LABEL(D4D_NO_CONST, name, text, x, y, cx, cy, radius, flags, pScheme, fontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RLABEL_INRAM(name, text, x, y, cx, cy, radius, font) \
    D4D_DECLARE_RLABEL_INRAM(name, text, x, y, cx, cy, radius, (D4D_LBL_F_DEFAULT) , NULL, font, NULL, NULL)

#define D4D_DECLARE_RLABEL_AUTOSIZE_INRAM(name, text, x, y, radius, flags, pScheme, font, pUser, pOnUsrMsg) \
    D4D_DECLARE_RLABEL_INRAM(name, text, x, y, 0, 0, radius, flags , pScheme, font, pUser, pOnUsrMsg)
    
#define D4D_DECLARE_STD_RLABEL_AUTOSIZE_INRAM(name, text, x, y, radius, font) \
    D4D_DECLARE_STD_RLABEL_INRAM(name, text, x, y, 0, 0, radius, font)
    

/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/



// Obsolete functions, replaced by any general
#define D4D_LabelSetText D4D_SetText




#endif /* __D4D_LABEL_H */