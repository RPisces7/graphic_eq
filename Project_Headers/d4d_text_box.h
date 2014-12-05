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
* @file      d4d_text_box.h
*
* @author    b01119
* 
* @version   0.0.8.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_TEXTBOX_H
#define __D4D_TEXTBOX_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_textboxSysFunc;
void D4D_TextBoxScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/


  

#ifndef D4D_TXTBX_F_DEFAULT
  #define D4D_TXTBX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_TABSTOP)
#endif

#ifndef D4D_TXTBX_F_SCRLBRS_DEFAULT
  #define D4D_TXTBX_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)
#endif

#ifndef D4D_TXTBX_TXT_PRTY_DEFAULT
  #define D4D_TXTBX_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_TXTBX_FNT_PRTY_DEFAULT
  #define D4D_TXTBX_FNT_PRTY_DEFAULT  ( 0 )
#endif 

#ifndef D4D_TXTBX_SCRLBR_WIDTH
  #define D4D_TXTBX_SCRLBR_WIDTH  ( 16 )
#endif 

#ifndef D4D_TXTBX_SCRLBR_STEP
  #define D4D_TXTBX_SCRLBR_STEP  ( 2 )
#endif
                                                 

typedef struct D4D_TXTBX_DATA_S
{
	D4D_INDEX firstShowedCharIx;
	D4D_CHAR* pTxtArr;			 // Pointer on text array that will use TextBox to store all lines    
	D4D_BOOL redrawText;
}D4D_TXTBX_DATA;

// label configuration (goes to ROM by default)

typedef struct
{
    D4D_STRING textBuff;    // TextBox header text
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4D_COOR radius;          // radius of text box corners
    D4D_COOR* pTabTable;
    D4D_FONT textFontId; 		 // Used font for whole text
    const D4D_BMP*  pBmpIcon; // title icon
    D4D_OBJECT* pScrlBrVer;    
    D4D_TXTBX_DATA* pData;		 // Run time data needed for TextBox 	
} D4D_TEXTBOX;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the TEXTBOX structure from general OBJECT
#define D4D_GET_TEXTBOX(pObj) ((D4D_TEXTBOX*)((pObj)->pParam))
#define D4D_GET_TEXTBOX_DATA(pObj) (D4D_GET_TEXTBOX(pObj)->pData)

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_TEXTBOX(type, name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_TXTBX_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT, D4D_TXTBX_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static D4D_TXTBX_DATA name##_data = { 0, (D4D_CHAR*)pTextArray, D4D_TRUE};\
    extern const D4D_OBJECT name##_scrollBarVer;\
    static type D4D_TEXTBOX name##_params = \
    { \
        { text, sizeof(text), titleFontId, &name##_strPrties}, /* textBuff */ \
        { x, y },   /* scrPos */ \
        { cx, cy }, /* scrSize */ \
        radius,     /* radius */ \
        (D4D_COOR*)pTabTable, /* pTabTable */ \
        fontId,			/* textFontId */ \
        icon,       /* pBmpIcon */\
        (D4D_OBJECT*)&name##_scrollBarVer, /* pScrlBrVer */\
        &name##_data, /* pData */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_textboxSysFunc, &(name##_params), flags, pUser, pScheme)\
    \
    D4D_DECLARE_RSCROLL_BAR_INRAM(name##_scrollBarVer,  20, 100, 100, 20, radius, D4D_TXTBX_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_TextBoxScrollBarsFeedBack, NULL)    


#define D4D_DECLARE_TEXTBOX(name, text, x, y, cx, cy, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_CONST, name, text, x, y, cx, cy, 0, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_TEXTBOX(name, text, x, y, cx, cy, pTextArray, pTabTable, icon,  fontId, titleFontId) \
  D4D_DECLARE_TEXTBOX(name, text, x, y, cx, cy, pTextArray, pTabTable, icon, D4D_TXTBX_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_TEXTBOX_AUTOSIZE(name, text, x, y, pTextArray, pTabTable, icon,  fontId, titleFontId) \ \
  D4D_DECLARE_STD_TEXTBOX(name, text, x, y, 0, 0, pTextArray, pTabTable, icon,  fontId, titleFontId)

#define D4D_DECLARE_TEXTBOX_AUTOSIZE(name, text, x, y, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_TEXTBOX(name, text, x, y, 0, 0, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

// Rounded text box definition
#define D4D_DECLARE_RTEXTBOX(name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_CONST, name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RTEXTBOX(name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon,  fontId, titleFontId) \
  D4D_DECLARE_RTEXTBOX(name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, D4D_TXTBX_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_RTEXTBOX_AUTOSIZE(name, text, x, y, radius, pTextArray, pTabTable, icon,  fontId, titleFontId) \ \
  D4D_DECLARE_STD_RTEXTBOX(name, text, x, y, 0, 0, radius, pTextArray, pTabTable, icon,  fontId, titleFontId)

#define D4D_DECLARE_RTEXTBOX_AUTOSIZE(name, text, x, y, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_RTEXTBOX(name, text, x, y, 0, 0, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)


//IN RAM instantions macros
#define D4D_DECLARE_TEXTBOX_INRAM(name, text, x, y, cx, cy, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_NO_CONST, name, text, x, y, cx, cy, 0, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)
                               
#define D4D_DECLARE_STD_TEXTBOX_INRAM(name, text, x, y, cx, cy, pTextArray, pTabTable, icon,  fontId, titleFontId) \
  D4D_DECLARE_TEXTBOX_INRAM(name, text, x, y, cx, cy, pTextArray, pTabTable, icon, D4D_TXTBX_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_TEXTBOX_AUTOSIZE_INRAM(name, text, x, y, pTextArray, pTabTable, icon,  fontId, titleFontId) \ \
  D4D_DECLARE_STD_TEXTBOX_INRAM(name, text, x, y, 0, 0, pTextArray, pTabTable, icon,  fontId, titleFontId)

#define D4D_DECLARE_TEXTBOX_AUTOSIZE_INRAM(name, text, x, y, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_TEXTBOX_INRAM(name, text, x, y, 0, 0, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)


// Rounded text box definition
#define D4D_DECLARE_RTEXTBOX_INRAM(name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
  _D4D_DECLARE_TEXTBOX(D4D_NO_CONST, name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RTEXTBOX_INRAM(name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon,  fontId, titleFontId) \
  D4D_DECLARE_RTEXTBOX_INRAM(name, text, x, y, cx, cy, radius, pTextArray, pTabTable, icon, D4D_TXTBX_F_DEFAULT, NULL, fontId, titleFontId, NULL, NULL)

#define D4D_DECLARE_STD_RTEXTBOX_AUTOSIZE_INRAM(name, text, x, y, radius, pTextArray, pTabTable, icon,  fontId, titleFontId) \ \
  D4D_DECLARE_STD_RTEXTBOX_INRAM(name, text, x, y, 0, 0, radius, pTextArray, pTabTable, icon,  fontId, titleFontId)

#define D4D_DECLARE_RTEXTBOX_AUTOSIZE_INRAM(name, text, x, y, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg) \
    D4D_DECLARE_RTEXTBOX_INRAM(name, text, x, y, 0, 0, radius, pTextArray, pTabTable, icon, flags, pScheme, fontId, titleFontId, pUser, pOnUsrMsg)

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
void D4D_TextBoxRefreshAll(D4D_OBJECT* pObject);
void D4D_TextBoxChangeText(D4D_OBJECT* pObject, D4D_CHAR* pText);

#endif /* __D4D_TEXTBOX_H */