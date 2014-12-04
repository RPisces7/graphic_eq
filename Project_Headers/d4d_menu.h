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
* @file      d4d_menu.h
*
* @author    b01119
* 
* @version   0.0.36.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D Driver menu object header file
*
*******************************************************************************/

#ifndef __D4D_MENU_H
#define __D4D_MENU_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_menuSysFunc;
void D4D_MenuScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);
/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#define D4D_MENU_F_INDEX  0x0100
#define D4D_MENU_F_SIDEBAR  0x0200

#ifndef D4D_MENU_F_DEFAULT
  #define D4D_MENU_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_MENU_F_INDEX | D4D_MENU_F_SIDEBAR)
#endif

#ifndef D4D_MENU_F_SCRLBRS_DEFAULT
  #define D4D_MENU_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)
#endif

#ifndef D4D_MENU_SCRLBR_WIDTH
  #define D4D_MENU_SCRLBR_WIDTH  ( 16 )
#endif 

#ifndef D4D_MENU_SCRLBR_STEP
  #define D4D_MENU_SCRLBR_STEP  ( 1 )
#endif

#ifndef D4D_MENU_TXT_PRTY_DEFAULT
  #define D4D_MENU_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_MENU_FNT_PRTY_DEFAULT
  #define D4D_MENU_FNT_PRTY_DEFAULT  ( 0 )
#endif

#ifndef D4D_MENU_IX_FNT_PRTY_DEFAULT
  #define D4D_MENU_IX_FNT_PRTY_DEFAULT  ( 0 )
#endif

#ifndef D4D_MENU_IX_TXT_PRTY_DEFAULT
  #define D4D_MENU_IX_TXT_PRTY_DEFAULT  ( D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK )
#endif

#ifndef D4D_MENU_ITEM_FNT_PRTY_DEFAULT
  #define D4D_MENU_ITEM_FNT_PRTY_DEFAULT  ( 0 )
#endif

#ifndef D4D_MENU_ITEM_TXT_PRTY_DEFAULT
  #define D4D_MENU_ITEM_TXT_PRTY_DEFAULT  ( D4D_TXT_PRTY_ALIGN_V_CENTER_MASK | D4D_TXT_PRTY_ALIGN_H_LEFT_MASK )
#endif


typedef sByte D4D_MENU_INDEX;

// Menu configuration (goes to ROM by default)
typedef struct 
{
  D4D_STRING text;
  const D4D_BMP*  pIcon;    // Item Icon
  void* pUser;    
}D4D_MENU_ITEM;

typedef struct 
{
   D4D_MENU_INDEX ix;
   D4D_MENU_INDEX focus_pos;
   D4D_MENU_INDEX item_select; 
} D4D_MENU_DATA;

typedef struct
{
    D4D_STRING title_text;    // menu title text
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4D_COOR  radius;        // radius of corners 
    D4D_FONT  indexFontId;
    D4D_FONT  itemsFontId;
    D4D_MENU_INDEX posCnt; // Number of visiable position
    D4D_COOR textOff; // text offset between individual item on screen
    const D4D_BMP*  pIcon;    // Item Icon
    D4D_MENU_DATA* pData;     // Menu volatile data struct
    const D4D_MENU_ITEM*  pItems;
    const D4D_MENU_INDEX* pItemsCnt;
    
    void (*OnClicked)(D4D_OBJECT* pThis, D4D_MENU_INDEX ix);
    D4D_OBJECT* pScrlBrVer;    
} D4D_MENU;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the MENU structure from general OBJECT
#define D4D_GET_MENU(pObj) ((D4D_MENU*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

    
#define _D4D_DECLARE_MENU_BEGIN(type, name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
    extern type D4D_MENU_ITEM name##_items[];\
    extern type D4D_MENU_INDEX name##_itemsCnt;\
    extern const D4D_OBJECT name##_scrollBarVer;\
    static D4D_MENU_DATA name##_data = { 0, 0, -1};\
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_MENU_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT , D4D_MENU_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_MENU name##_params = \
    { \
        { title_text, sizeof(title_text), title_font, &name##_strPrties},  \
        { x, y },    \
        { cx, cy },  \
        radius,   \
        indexFontId,        \
		    itemsFontId,        \
		    posCnt, \
		    menuItemsOff, \
		    pIcon, \
		    &(name##_data), \
		    &(name##_items[0]),  /* MENU ITEMS     */ \
		    &(name##_itemsCnt), /* all items cnt */ \
		    pOnClick, \
		    (D4D_OBJECT*)&name##_scrollBarVer, \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_menuSysFunc, &(name##_params), flags, pUser, pScheme)\
    \
    D4D_DECLARE_RSCROLL_BAR_INRAM(name##_scrollBarVer,  20, 100, 100, 20, radius, D4D_MENU_F_SCRLBRS_DEFAULT, pScheme, (void*)&name, D4D_MenuScrollBarsFeedBack, NULL) \
    \
    const D4D_MENU_ITEM name##_items[] = { \
    
#define D4D_DECLARE_MENU_ITEM(text, pIcon) { {(text), sizeof(text), 0, NULL}, pIcon, NULL},

#define D4D_DECLARE_MENU_ITEM_FULL(text, pIcon, pUser) { {(text), sizeof(text), 0, NULL}, pIcon, (void*)pUser},


#define _D4D_DECLARE_MENU_END(type, name) { {NULL, 0, 0, NULL}, NULL, NULL} }; \
    type D4D_MENU_INDEX name##_itemsCnt = (D4D_MENU_INDEX)((sizeof(name##_items) / sizeof(D4D_MENU_ITEM)) - 1);



#define D4D_DECLARE_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_CONST, name, title_text, title_font, x, y, cx, cy, 0, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)

#define D4D_DECLARE_MENU_END(name)  _D4D_DECLARE_MENU_END(D4D_CONST, name)

#define D4D_DECLARE_STD_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, pOnClick) \
    D4D_DECLARE_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, NULL, pOnClick, NULL)

#define D4D_DECLARE_STD_MENU_AUTOSIZE_BEGIN(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_STD_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, 0, 0, pIcon, pOnClick)

// Rounded menu definition
#define D4D_DECLARE_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_CONST, name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)

#define D4D_DECLARE_RMENU_END(name)  D4D_DECLARE_MENU_END(name)

#define D4D_DECLARE_STD_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, pOnClick) \
    D4D_DECLARE_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, NULL, pOnClick, NULL)

#define D4D_DECLARE_STD_RMENU_AUTOSIZE_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_STD_RMENU_BEGIN(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, 0, 0, pIcon, pOnClick)


//IN RAM instantions macros
#define D4D_DECLARE_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_NO_CONST, name, title_text, title_font, x, y, cx, cy, 0, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)

#define D4D_DECLARE_MENU_END_INRAM(name)  _D4D_DECLARE_MENU_END(D4D_NO_CONST, name)

#define D4D_DECLARE_STD_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, pOnClick) \
    D4D_DECLARE_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, NULL, pOnClick, NULL)

#define D4D_DECLARE_STD_MENU_AUTOSIZE_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_STD_MENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, 0, 0, pIcon, pOnClick)
    

// Rounded menu definition
#define D4D_DECLARE_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg) \
 _D4D_DECLARE_MENU_BEGIN(D4D_NO_CONST, name, title_text, title_font, x, y, cx, cy, radius, flags, pScheme,\
    indexFontId, itemsFontId, posCnt, menuItemsOff, pIcon, pUser, pOnClick, pOnUsrMsg)

#define D4D_DECLARE_RMENU_END_INRAM(name)  D4D_DECLARE_MENU_END_INRAM(name)

#define D4D_DECLARE_STD_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, pOnClick) \
    D4D_DECLARE_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, (D4D_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, NULL, pOnClick, NULL)

#define D4D_DECLARE_STD_RMENU_AUTOSIZE_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4D_DECLARE_STD_RMENU_BEGIN_INRAM(name, title_text, title_font, x, y, cx, cy, radius, IndexFontId, ItemsFontId, 0, 0, pIcon, pOnClick)    

/******************************************************************************
* Global functions
******************************************************************************/
D4D_MENU_INDEX D4D_MenuGetIndex(D4D_OBJECT* pThis);

void  D4D_MenuSetIndex(D4D_OBJECT* pThis, D4D_MENU_INDEX ix);
D4D_MENU_INDEX D4D_MenuGetItemCount(D4D_OBJECT_PTR pThis);
D4D_MENU_INDEX D4D_MenuFindUserDataItem(D4D_OBJECT_PTR pThis, void* pUser);
void* D4D_MenuGetItemUserData(D4D_OBJECT_PTR pThis);
D4D_STRING* D4D_MenuGetItemText(D4D_OBJECT_PTR pThis);


#endif /* __D4D_MENU_H  */
 



