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
* @file      d4d_scheme.h
*
* @author    b01119
* 
* @version   0.0.20.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D Driver color scheme header file
*
*******************************************************************************/

#ifndef __D4D_SCHEME_H
#define __D4D_SCHEME_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/



#define D4D_COLOR565_GET_R(color) ((Byte)((((color) >> 11) * 33) / 4))
#define D4D_COLOR565_GET_G(color) ((Byte)(((((color) >> 5) & 0x3f) * 65) / 16))
#define D4D_COLOR565_GET_B(color) ((Byte)((((color) & 0x1f) * 33) / 4))

#define D4D_COLOR332_GET_R(color) ((Byte)((((color) >> 5) * 73) / 2))
#define D4D_COLOR332_GET_G(color) ((Byte)(((((color) >> 2) & 0x07) * 73) / 2))
#define D4D_COLOR332_GET_B(color) ((Byte)((((color) & 0x03) * 170) / 2))

#define D4D_COLORMONO_GET_R(color) ((Byte)((color)? 0xFF:0))
#define D4D_COLORMONO_GET_G(color) ((Byte)((color)? 0xFF:0))
#define D4D_COLORMONO_GET_B(color) ((Byte)((color)? 0xFF:0))
#define D4D_COLORMONO_THRESHOLD 128 


#if D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB565

  #define D4D_COLOR_RGB(R,G,B) ((D4D_COLOR)((((R) / 8) << 11) + (((G) / 4) << 5) + ((B) / 8)))

  #define D4D_COLOR_GET_R(color) D4D_COLOR565_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLOR565_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLOR565_GET_B(color)

#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB332

  #define D4D_COLOR_RGB(R,G,B) ((D4D_COLOR)((((R) / 32) << 11) + (((G) / 32) << 5) + ((B) / 64)))
  

  #define D4D_COLOR_GET_R(color) D4D_COLOR332_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLOR332_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLOR332_GET_B(color)

#elif D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_MONO

  #define D4D_COLOR_RGB(R,G,B) ((D4D_COLOR)(((((R) + (G) + (B)) / 3) * 0x100) - 1) & 0xff)

  #define D4D_COLOR_GET_R(color) D4D_COLORMONO_GET_R(color)
  #define D4D_COLOR_GET_G(color) D4D_COLORMONO_GET_G(color)
  #define D4D_COLOR_GET_B(color) D4D_COLORMONO_GET_B(color)

#else
  #error The D4D color system (D4D_COLOR_SYSTEM) is not supported. Select supported one.
#endif


//Colors definition

#define D4D_COLOR_DARK_BLUE     D4D_COLOR_RGB(0, 0, 152)  
#define D4D_COLOR_BRIGHT_BLUE   D4D_COLOR_RGB(222, 219, 255)
#define D4D_COLOR_BLUE          D4D_COLOR_RGB(0, 0, 255)  
#define D4D_COLOR_CYAN          D4D_COLOR_RGB(0, 255, 255)
#define D4D_COLOR_BRIGHT_YELLOW D4D_COLOR_RGB(255, 220, 120)
#define D4D_COLOR_YELLOW        D4D_COLOR_RGB(255, 255, 0) 
#define D4D_COLOR_ORANGE        D4D_COLOR_RGB(255, 152, 96)
#define D4D_COLOR_BRIGHT_RED    D4D_COLOR_RGB(255, 28, 24)  
#define D4D_COLOR_RED           D4D_COLOR_RGB(255, 0, 0)  
#define D4D_COLOR_DARK_RED      D4D_COLOR_RGB(152, 0, 0)  
#define D4D_COLOR_MAGENTA       D4D_COLOR_RGB(255, 0, 255)  
#define D4D_COLOR_BRIGHT_GREEN  D4D_COLOR_RGB(152, 255, 152)
#define D4D_COLOR_GREEN         D4D_COLOR_RGB(0, 255, 0)  
#define D4D_COLOR_DARK_GREEN    D4D_COLOR_RGB(0, 128, 0)  
#define D4D_COLOR_BRIGHT_GREY   D4D_COLOR_RGB(48, 48, 48)  
#define D4D_COLOR_LIGHT_GREY    D4D_COLOR_RGB(120, 120, 120)
#define D4D_COLOR_GREY          D4D_COLOR_RGB(24, 24, 24)   
#define D4D_COLOR_WHITE         D4D_COLOR_RGB(255, 255, 255)
#define D4D_COLOR_BLACK         D4D_COLOR_RGB(0, 0, 0)   


// standard normal color
#ifndef D4D_COLOR_FORE_NORM
  #define D4D_COLOR_FORE_NORM  D4D_COLOR_BLACK
#endif

#ifndef D4D_COLOR_BCKG_NORM
  #define D4D_COLOR_BCKG_NORM  D4D_COLOR_WHITE
#endif

// standard disabled color
#ifndef D4D_COLOR_FORE_DISABLED
  #define D4D_COLOR_FORE_DISABLED D4D_COLOR_GREY
#endif

#ifndef D4D_COLOR_BCKG_DISABLED
  #define D4D_COLOR_BCKG_DISABLED D4D_COLOR_LIGHT_GREY
#endif

// standard focus color
#ifndef D4D_COLOR_FORE_FOCUS
  #define D4D_COLOR_FORE_FOCUS D4D_COLOR_RED
#endif

#ifndef D4D_COLOR_BCKG_FOCUS
  #define D4D_COLOR_BCKG_FOCUS D4D_COLOR_LIGHT_GREY
#endif

// standard capturing color
#ifndef D4D_COLOR_FORE_CAPTURE
  #define D4D_COLOR_FORE_CAPTURE D4D_COLOR_BRIGHT_RED
#endif

#ifndef D4D_COLOR_BCKG_CAPTURE
  #define D4D_COLOR_BCKG_CAPTURE D4D_COLOR_GREY
#endif





/******************************************************************************
* Types
******************************************************************************/

typedef struct 
{
  D4D_COLOR desktop;
  D4D_COLOR outline; 
  D4D_COLOR title_bar;   
  D4D_COLOR title_text;     
  D4D_COLOR exitBtnFore;
  D4D_COLOR exitBtnBckg;
}D4D_CLR_SCHEME_SCR;

typedef struct
{
  D4D_COLOR na;
}D4D_CLR_SCHEME_BTN;

typedef struct
{
  D4D_COLOR hub;
  D4D_COLOR pointer;
}D4D_CLR_SCHEME_GAUG;

typedef struct
{
  D4D_COLOR barBckg;
  D4D_COLOR barFore;
  D4D_COLOR barStart;
  D4D_COLOR barEnd;  
}D4D_CLR_SCHEME_SLDR;

typedef struct
{
  D4D_COLOR na;
}D4D_CLR_SCHEME_ICON;

typedef struct
{
  D4D_COLOR na;
}D4D_CLR_SCHEME_LBL;

typedef struct
{
  D4D_COLOR na;
}D4D_CLR_SCHEME_MENU;

typedef struct
{
  D4D_COLOR iconBckg;
}D4D_CLR_SCHEME_CHECKB;

typedef struct
{
  D4D_COLOR grid;
}D4D_CLR_SCHEME_GRAPH;


typedef struct
{
  D4D_COLOR barBckg;
  D4D_COLOR barFore;
  D4D_COLOR barEnd;  
}D4D_CLR_SCHEME_PRGRS_BAR;

typedef struct
{
  //D4D_CLR_SCHEME_BTN    button;
  D4D_CLR_SCHEME_GAUG   gauge;
  D4D_CLR_SCHEME_SLDR   slider;
  //D4D_CLR_SCHEME_ICON   icon;
  //D4D_CLR_SCHEME_LBL    label;
  //D4D_CLR_SCHEME_MENU   menu;  
  D4D_CLR_SCHEME_CHECKB checkBox;
  D4D_CLR_SCHEME_GRAPH   graph;
  D4D_CLR_SCHEME_PRGRS_BAR progressBar;
}D4D_CLR_SCHEME_OBJ;

typedef struct D4D_CLR_SCHEME_S
{
  D4D_CLR_SCHEME_SCR screen;
  D4D_COLOR bckg;
  D4D_COLOR bckgDis;
  D4D_COLOR bckgFocus;
  D4D_COLOR bckgCapture;  
  D4D_COLOR fore;
  D4D_COLOR foreDis;
  D4D_COLOR foreFocus;
  D4D_COLOR foreCapture;
  D4D_CLR_SCHEME_OBJ objectDepend;
}D4D_CLR_SCHEME;

typedef const D4D_CLR_SCHEME * D4D_CLR_SCHEME_PTR;

/******************************************************************************
* Macros 
******************************************************************************/

#define D4D_DECLARE_CLR_SCHEME(name, \
        scrDesktop, scrOutline, scrTitleBar, scrTitleText, scrExitBtnFore, scrExitBtnBckg,\
        objBckg, objBckgDis, objBckgFocus, objBckgCapture,\
        objFore, objForeDis, objForeFocus, objForeCapture,\
        gaugHub, gaugPointer,\
        sldrBarBckg, sldrBar, sldrBarStart, sldrBarEnd,\
        iconBckg,\
        graphGrid,\
        prgrsBarBarBckg, prgrsBarBar, prgrsBarBarEnd\
        )\
        static const D4D_CLR_SCHEME name = \
        { \
          { scrDesktop, scrOutline, scrTitleBar, scrTitleText, scrExitBtnFore , scrExitBtnBckg}, /* Screen Colors */ \
          objBckg, objBckgDis, objBckgFocus, objBckgCapture,\
          objFore, objForeDis, objForeFocus, objForeCapture,\
          {\
            {gaugHub, gaugPointer },\
            {sldrBarBckg, sldrBar, sldrBarStart, sldrBarEnd },\
            {iconBckg },\
            {graphGrid },\
            {prgrsBarBarBckg, prgrsBarBar, prgrsBarBarEnd }\
          }\
        };

/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/

  
#endif  /* __D4D_SCHEME_H */
