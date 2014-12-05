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
* @file      d4d_low.h
*
* @author    b01119
* 
* @version   0.0.17.0
* 
* @date      Jan-19-2011
* 
* @brief     D4D driver low level graphic function header file 
*
*******************************************************************************/

#ifndef __D4D_LOW_H
#define __D4D_LOW_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/
//#define RGB(R,G,B) (D4D_COLOR)((((R) & 0x1f) << 11) + (((G) & 0x3f) << 5) + ((B) & 0x1f))

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global D4Drv low level functions
******************************************************************************/

D4D_BOOL D4D_LCD_Init(void); // Hardware init of LCD
D4D_COOR D4D_DisplayHeight(void);
D4D_COOR D4D_DisplayWidth(void);
D4D_ORIENTATION D4D_LCD_GetOrientation(void);
void D4D_LCD_SetOrientation(D4D_ORIENTATION new_orientation);
D4D_COLOR D4D_LCD_GetGreyScale(D4D_COLOR color);
void D4D_LCD_SwapCoor(D4D_COOR* c1, D4D_COOR* c2);

void D4D_LCD_PutPixel(D4D_COOR x1, D4D_COOR y1, D4D_LINETYPE line_type, D4D_COLOR color);
void D4D_LCD_Line(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,D4D_LINETYPE line_type ,D4D_COLOR color);
void D4D_LCD_Box(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color);
void D4D_LCD_Rect(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color);
void D4D_LCD_Circle(D4D_COOR x1, D4D_COOR y1, D4D_COOR r, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  void D4D_LCD_RBox(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius);
  void D4D_LCD_RRect(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color, D4D_COOR radius);
  void D4D_LCD_QCircle (D4D_COOR cx, D4D_COOR cy, D4D_COOR radius, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant);
#endif 



/******************************************************************************
* Global D4Drv Touch Screen low level functions
******************************************************************************/


Byte D4D_TCH_GetCalibrationStatus(void);
D4D_TOUCHSCREEN_CALIB D4D_TCH_GetCalibration(void);
void D4D_TCH_SetCalibration(D4D_TOUCHSCREEN_CALIB newCalib);
void D4D_TCH_GetCalibratedPosition(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);
//Byte D4D_TCH_GetPosition (D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);
Byte D4D_TCH_GetRawPosition(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);
void D4D_TCH_Calibrate(D4D_COLOR fore, D4D_COLOR bckg);
void D4D_TCH_DrawCalibrationPoint (D4D_COOR X, D4D_COOR Y, D4D_COLOR Colour);

#endif /* __D4D_LOW_H */










