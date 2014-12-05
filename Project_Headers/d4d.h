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
* @file      d4d.h
*
* @author    r50233
* 
* @version   0.0.72.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver main header file
*
*******************************************************************************/

#ifndef __D4D_H
#define __D4D_H

/******************************************************************************
* Includes
******************************************************************************/

/**************************************************************//*!
*
* Types used by driver (standard HCxx: Byte, Word, LWord, ...)
*
******************************************************************/


/**************************************************************//*!
*
* Implementation header files
*
******************************************************************/
#include "d4d_user_cfg.h"
#include "d4d_types.h"
#include "d4d_base.h"
#include "d4d_object.h"
#include "d4d_low.h"
#include "d4d_bmp.h"
#include "d4d_font.h"
#include "d4d_math.h"
#include "d4d_scheme.h"
#include "d4d_screen.h"
#include "d4d_extsrc.h" 
#include "d4d_picture.h"
#include "d4d_button.h"
#include "d4d_check_box.h"
#include "d4d_gauge.h"
#include "d4d_slider.h"
#include "d4d_label.h"
#include "d4d_icon.h"
#include "d4d_menu.h"
#include "d4d_graph.h"
#include "d4d_scroll_bar.h"
#include "d4d_console.h"
#include "d4d_text_box.h"
#include "d4d_progress_bar.h"



/******************************************************************************
* Constants
******************************************************************************/

/**************************************************************//*!
*
* Basic driver configuration
*
******************************************************************/ 


/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/**************************************************************//*!
*
* Main driver API Functions
*
******************************************************************/

/* General driver API */
D4D_BOOL D4D_Init(D4D_SCREEN* pInitScreen);
void D4D_Poll(void);
void D4D_SetOrientation(D4D_ORIENTATION orient);
void D4D_KeysChanged(D4D_KEYS keys);
void D4D_NewKeyEvent(D4D_KEY_SCANCODE scanCode);
void D4D_TimeTickPut(void);
D4D_POINT D4D_GetTouchScreenCoordinates(void);
void D4D_CheckTouchScreen(void);
void D4D_PutTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y);
void D4D_PutRawTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y);



/* screen API */


/* object API */
void D4D_EnableTabStop(D4D_OBJECT_PTR pObj, Byte bEnable);
void D4D_EnableObject(D4D_OBJECT_PTR pObj, Byte bEnable);
void D4D_EnableTouchScreen(D4D_OBJECT_PTR pObj, Byte bEnable, Byte bFastTouch);
void D4D_ShowObject(D4D_OBJECT_PTR pObject, Byte bShow);    
D4D_OBJECT_PTR D4D_GetFocusedObject(D4D_SCREEN* pScreen);
void D4D_FocusSet(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject);
void D4D_FocusNextObject(D4D_SCREEN* pScreen, Byte bInitialSearch);
void D4D_FocusPrevObject(D4D_SCREEN* pScreen);
void D4D_InvalidateObject(D4D_OBJECT_PTR pObject, Byte bComplete);
void* D4D_GetUserPointer(D4D_OBJECT *pThis);
void D4D_CaptureKeys(D4D_OBJECT_PTR pObj);
D4D_OBJECT* D4D_GetCapturedObject(void);

/* Color Schemes */
D4D_CLR_SCHEME* D4D_ObjectGetScheme(D4D_OBJECT * pObj);
D4D_CLR_SCHEME* D4D_ScreenGetScheme(D4D_SCREEN * pScreen);
D4D_CLR_SCHEME* D4D_GetDefaultScheme(void);
void D4D_SetDefaultScheme(D4D_CLR_SCHEME* pScheme);
D4D_COLOR D4D_ObjectGetForeColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw);
D4D_COLOR D4D_ObjectGetBckgColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw);
D4D_COLOR D4D_ObjectGetForeFillColor(D4D_OBJECT * pObj);
D4D_COLOR D4D_ObjectGetBckgFillColor(D4D_OBJECT * pObj);
                             
/* general helpers */
D4D_KEYS D4D_GetKeys(void);
void D4D_ClearKeysBuffer(void);
void D4D_ClearScreen(D4D_COLOR color);

void D4D_PutPixel(D4D_POINT* ppt, D4D_LINETYPE type, D4D_COLOR color);
void D4D_PutPixelTo(D4D_LINETYPE type, D4D_COLOR color);
void D4D_PutPixelXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE type, D4D_COLOR color);
void D4D_MoveTo(D4D_POINT* ppt);
void D4D_MoveToXY(D4D_COOR x, D4D_COOR y);
D4D_POINT D4D_SwapCoor(D4D_POINT point);
void D4D_LineTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_LineToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_FillRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color);
void D4D_FillRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color);
void D4D_FillRectTo(D4D_POINT* ppt, D4D_COLOR color);
void D4D_FillRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color);
void D4D_FillRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction); // experimental function
void D4D_FillRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction); // same
void D4D_FillRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction); // same
void D4D_FillRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction); // same
void D4D_Rect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_RectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_RectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_RectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_Box(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_BoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_BoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_BoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill);
void D4D_FillCircle(D4D_POINT* pCenter, D4D_COOR r, D4D_COLOR color);
void D4D_FillCircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_COLOR color);
void D4D_Circle(D4D_POINT* pCenter, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_CircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color);
void D4D_DrawBmp(D4D_POINT* ppt, const D4D_BMP* pBmp, D4D_BOOL greyScale);
void D4D_DrawBmpXY(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

  void D4D_RRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_RRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_RRectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_RRectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRectTo(D4D_POINT* ppt, D4D_COLOR color, D4D_COOR radius);
  void D4D_FillRRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color, D4D_COOR radius);
  void D4D_RBox(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_RBoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_RBoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_RBoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius);
  void D4D_FillQuadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_FillQuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_Quadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_QuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant);
  void D4D_FillRRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // experimental function
  void D4D_FillRRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // same
  void D4D_FillRRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // same
  void D4D_FillRRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius); // same

  void D4D_DrawRBmp(D4D_POINT* ppt, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius);
  void D4D_DrawRBmpXY(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius);

#endif

//void D4D_DrawText(D4D_POINT* ppt, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextTab(D4D_POINT* ppt, D4D_STRING* buffText, D4D_COOR* pTabTable, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextTabXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);


void D4D_DrawTextRectTabAdv(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextRectTabAdvTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextRectTabAdvXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
void D4D_DrawTextRectTabAdvToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);

//void D4D_DrawTextRectTab(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextRectTabTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextRectTabXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextRectTabToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd);


//void D4D_DrawTextRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextRectTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd);
//void D4D_DrawTextRectToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd);





D4D_SIZE D4D_GetBmpSize(const D4D_BMP* pBmp);
D4D_COOR D4D_GetBmpWidth(const D4D_BMP* pBmp);
D4D_COOR D4D_GetBmpHeight(const D4D_BMP* pBmp);


void D4D_SetText(D4D_OBJECT_PTR pObject, char* pText);
void D4D_SetTextProperties(D4D_OBJECT_PTR pObject, D4D_TEXT_PROPERTIES property);
void D4D_SetFontProperties(D4D_OBJECT_PTR pObject, D4D_FONT_PROPERTIES property);
D4D_INDEX D4D_GetTextLength(D4D_CHAR* pText);
D4D_COOR D4D_GetTextWidth(D4D_FONT ix, D4D_CHAR* pText);
//D4D_COOR D4D_GetTextBuffWidth(D4D_STRING* text_buffer);
D4D_COOR D4D_GetTextBuffWidthTabAdj(D4D_STRING* text_buffer, D4D_INDEX offset, D4D_TAB* pTab);

Byte D4D_SprintDecU8(Byte val, char *pText, char fill);
Byte D4D_SprintDecS8(sByte val, char *pText, char fill);
Byte D4D_SprintDecU16(Word val, char *pText, char fill);
Byte D4D_SprintDecS16(sWord val, char *pText, char fill);
Byte D4D_SprintDecU32(LWord val, char *pText, char fill);
Byte D4D_SprintDecS32(sLWord val, char *pText, char fill);

void D4D_CalibrateTouchScreen(void);
D4D_TOUCHSCREEN_CALIB D4D_GetTouchScreenCalibration(void);
void D4D_SetTouchScreenCalibration(D4D_TOUCHSCREEN_CALIB newCalib);

D4D_COLOR D4D_GetCrossColor(D4D_COLOR startColor, D4D_COLOR endColor, Byte value);

D4D_COLOR D4D_GetGreyScale(D4D_COLOR color);



#endif /* __D4D_H */









