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
* @file      d4d_screen.h
*
* @author    b01119
* 
* @version   0.0.29.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver screen header file 
*
******************************************************************************/
//#include "../Sources/ui_eqbar.h"

#ifndef __D4D_SCREEN_H
#define __D4D_SCREEN_H

/******************************************************************************
* Constants
******************************************************************************/


#ifndef D4D_COLOR_SCR_DESKTOP
  #define D4D_COLOR_SCR_DESKTOP  D4D_COLOR_WHITE
#endif

#ifndef D4D_COLOR_SCR_OUTLINE
  #define D4D_COLOR_SCR_OUTLINE  D4D_COLOR_LIGHT_GREY
#endif

#ifndef D4D_COLOR_SCR_TITLEBAR
  #define D4D_COLOR_SCR_TITLEBAR  D4D_COLOR_ORANGE
#endif

#ifndef D4D_COLOR_SCR_TILTLETEXT
  #define D4D_COLOR_SCR_TILTLETEXT  D4D_COLOR_WHITE
#endif



#ifndef D4D_COLOR_SCR_EXIT_BTN_FORE
  #define D4D_COLOR_SCR_EXIT_BTN_FORE  D4D_COLOR_WHITE
#endif

#ifndef D4D_COLOR_SCR_EXIT_BTN_BCKG
  #define D4D_COLOR_SCR_EXIT_BTN_BCKG  D4D_COLOR_BRIGHT_RED
#endif



#ifndef D4D_SCR_TITLE_OFF_X
  #define D4D_SCR_TITLE_OFF_X  3
#endif

#ifndef D4D_SCR_TITLE_OFF_Y
  #define D4D_SCR_TITLE_OFF_Y  1
#endif

#ifndef D4D_SCR_HEADER_SIZE_MIN_SIZE
  #define D4D_SCR_HEADER_SIZE_MIN_SIZE  13
#endif
                                             
#ifndef D4D_SCR_TITLE_EXITBTN_MIN_SIZE
  #define D4D_SCR_TITLE_EXITBTN_MIN_SIZE  6
#endif

#ifndef D4D_SCR_EXITBTN_CROSS_SIZE
  #define D4D_SCR_EXITBTN_CROSS_SIZE 2
#endif


#ifndef D4D_SCR_TITLE_EXITBTN_OFFSET
  #define D4D_SCR_TITLE_EXITBTN_OFFSET  3
#endif


typedef struct
{
    // other runtime data
    Byte focusedObjIx;          // index of currently focused object
    
    union 
    {
        D4D_BIT_FIELD all;
    
        struct
        {
            unsigned bInitDone:1;       // screen initialization done
            unsigned bTouchEnable:1;    // Object is enabled for touchscreen
            unsigned bCompleteRedraw:1; // complete redraw pending
        } bits;                
    } flags;
} D4D_SCREEN_DATA;

// screen configuration (goes to ROM by default)
#if D4D_BITFIELD_LSB_ALIGNMENT == D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4D_SCR_F_OUTLINE       (0x01)
  #define D4D_SCR_F_TITLEBAR      (0x02)
  #define D4D_SCR_F_EXIT          (0x04)
  #define D4D_SCR_F_BCKG          (0x08)

  #define D4D_SCR_F_TOUCHENABLE    (0x0200)
#else
  #define D4D_SCR_F_OUTLINE       (0x80)
  #define D4D_SCR_F_TITLEBAR      (0x40)
  #define D4D_SCR_F_EXIT          (0x20)
  #define D4D_SCR_F_BCKG          (0x10)

  #define D4D_SCR_F_TOUCHENABLE    (0x4000)
#endif  

#ifndef D4D_SCR_F_DEFAULT
  #define D4D_SCR_F_DEFAULT        (0)
#endif 

typedef union 
{
    D4D_BIT_FIELD all;
    
    struct
    {
        unsigned bOutline     :1;     // show outline rectagle 
        unsigned bHeader      :1;     // Header box bit
        unsigned bExit        :1;     // Reserved bit 
        unsigned bBackground  :1;     // draw window bakground         
        unsigned bReserve4    :1;     // Reserved bit
        unsigned bReserve5    :1;     // Reserved bit
        unsigned bReserve6    :1;     // Reserved bit
        unsigned bReserve7    :1;     // Reserved bit
    } bits;
} D4D_SCREEN_FLAGS;

struct D4D_STRING_S;

typedef struct D4D_SCREEN_S
{ 
    // object list
    const D4D_OBJECT* const * pObjects; // NULL-terminated array of objects (may lay in ROM)
    
    // event handlers    
    void (*OnInit)(void);       // one-time initialization
    void (*OnMain)(void);       // main screen handler function
    void (*OnActivate)(void);   // called before screen activation
    void (*OnDeactivate)(void); // called before deactivating
    Byte (*OnObjectMsg)(struct D4D_MESSAGE_S* pMsg); // called before object receives the message
    
    // Screen advanced properties
    D4D_POINT position;
    D4D_SIZE size;
    D4D_COOR radius;
    struct D4D_STRING_S textBuff;
    const D4D_BMP* pIcon;
    D4D_SCREEN_FLAGS flags; 
    struct D4D_CLR_SCHEME_S* clrScheme;
    
    // pointer to runtime data
    D4D_SCREEN_DATA* pData;
    
} D4D_SCREEN;


/*********************************************************
*
* static declaration macros
*
*********************************************************/


#define _D4D_DECLARE_SCREEN_BEGIN(type, name, funcPrefix, x ,y, cx, cy, radius, text, fontId, icon, flags, pScheme) \
    extern const D4D_OBJECT * const name##_objects[]; \
    static void funcPrefix##OnInit(); \
    static void funcPrefix##OnMain(); \
    static void funcPrefix##OnActivate(); \
    static void funcPrefix##OnDeactivate(); \
    static Byte funcPrefix##OnObjectMsg(D4D_MESSAGE* pMsg); \
    static D4D_SCREEN_DATA name##_data = { 0,  (D4D_BIT_FIELD)(((D4D_BIT_FIELD)(flags >> 8) & 0x00FF) << D4D_BITFIELD_SHIFT)}; \
    static D4D_STR_PROPERTIES name##_strPrties = { (D4D_FNT_PRTY_TRANSPARENT_YES_MASK) << D4D_BITFIELD_SHIFT, (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)  << D4D_BITFIELD_SHIFT}; \
    type D4D_SCREEN name = \
    { \
        name##_objects, \
        funcPrefix##OnInit, \
        funcPrefix##OnMain, \
        funcPrefix##OnActivate, \
        funcPrefix##OnDeactivate, \
        funcPrefix##OnObjectMsg, \
        {x, y}, \
        {cx, cy}, \
        radius, \
        { text, sizeof(text), fontId,  &name##_strPrties}, /* textBuff */ \
        icon, \
        ((D4D_BIT_FIELD)(flags & 0x00FF) << D4D_BITFIELD_SHIFT), \
        pScheme, \
        &(name##_data) \
    }; \
    const D4D_OBJECT * const name##_objects[] = \
    {

#define D4D_DECLARE_SCREEN_BEGIN(name, funcPrefix, x ,y, cx, cy, text, fontId, icon, flags, pScheme)\
  _D4D_DECLARE_SCREEN_BEGIN(D4D_CONST, name, funcPrefix, x ,y, cx, cy, 0, text, fontId, icon, flags, pScheme)     
     
#define D4D_DECLARE_STD_SCREEN_BEGIN(name, funcPrefix) D4D_DECLARE_SCREEN_BEGIN(name, funcPrefix, 0 ,0, \
          (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), NULL, 0, NULL, 0, NULL)   

#define D4D_DECLARE_STD_PORTRAIT_SCREEN_BEGIN(name, funcPrefix) D4D_DECLARE_SCREEN_BEGIN(name, funcPrefix, 0 ,0, \
          (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), NULL, 0, NULL, D4D_SCR_F_DEFAULT, NULL)   


#define D4D_DECLARE_RSCREEN_BEGIN(name, funcPrefix, x ,y, cx, cy, radius, text, fontId, icon, flags, pScheme)\
  _D4D_DECLARE_SCREEN_BEGIN(D4D_CONST, name, funcPrefix, x ,y, cx, cy, radius, text, fontId, icon, flags, pScheme)          

     
#define D4D_DECLARE_SCREEN_OBJECT(name) &(name),

#define D4D_DECLARE_SCREEN_END() NULL };

#define D4D_EXTERN_SCREEN(name) \
    extern D4D_SCREEN name
               

/**************************************************************//*!
*
* Main driver API Functions
*
******************************************************************/

void D4D_EnableScrTouchScreen(const D4D_SCREEN* pScr, Byte bEnable);
D4D_SCREEN* D4D_GetActiveScreen(void);
void D4D_ActivateScreen(D4D_SCREEN* pNewScreen, Byte bReplaceCurrent);
void D4D_EscapeScreen(void);
void D4D_EscapeToBaseScreen(void);
void D4D_InitScreen(D4D_SCREEN* pScreen);
void D4D_InvalidateScreen(D4D_SCREEN* pScreen, Byte bComplete);
void D4D_SetScreenTextProperties(D4D_SCREEN* pScreen, D4D_TEXT_PROPERTIES property);
void D4D_SetScreenFontProperties(D4D_SCREEN* pScreen, D4D_FONT_PROPERTIES property);

//void Update_EQ_Bars(){

D4D_POINT D4D_GetClientToScreenPoint(D4D_SCREEN* pScreen, D4D_POINT* nClientPoint);
D4D_POINT D4D_GetScreenToClientPoint(D4D_SCREEN* pScreen, D4D_POINT* nScreenPoint);
D4D_SIZE D4D_GetClientScreenSize(D4D_SCREEN* pScreen);



//UI_EQ_BAR* Get_UI_EQ_BAR0(void);
    
#endif /* __D4D_SCREEN_H */   
