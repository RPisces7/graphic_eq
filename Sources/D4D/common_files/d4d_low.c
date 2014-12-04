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
* @file      d4d_low.c
*
* @author    b01119
* 
* @version   0.0.43.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver low level graphic function c file 
*
******************************************************************************/

/**************************************************************//*!
*
* Include headers
*
******************************************************************/

#include "d4d.h"
#include "d4d_private.h"
#include "d4d_lldapi.h"


/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/**************************************************************//*!
*
* Local variables
*
******************************************************************/
static D4D_ORIENTATION d4d_orientation = D4D_ORIENT_PORTRAIT;

#ifdef D4D_LLD_TCH
  static D4D_TOUCHSCREEN_CALIB d4d_tchCalib = {0, 0, 0, 0, 0};
#endif

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  static void D4D_LCD_QPlot4points(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant);
  static void D4D_LCD_QPlot4pointsfill(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_COLOR color,D4D_QUADRANT quadrant);
  static void D4D_LCD_QPlot8points(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant);
#endif 

/**************************************************************//*!
*
* Initialize
*
******************************************************************/

D4D_BOOL D4D_LCD_Init(void)
{
  if(D4D_LLD_LCD.D4DLCD_Init == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_SetWindow == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_SetOrientation == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_Send_PixelColor == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_Read_Word == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_FlushBuffer == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_Delay_ms == NULL)
    return D4D_FALSE;
  if(D4D_LLD_LCD.D4DLCD_DeInit == NULL)
    return D4D_FALSE;
  
  
  if(!D4D_LLD_LCD.D4DLCD_Init())
    return D4D_FALSE; 
  
  #ifdef D4D_LLD_TCH
    if(D4D_LLD_TCH.D4DTCH_Init == NULL)
      return D4D_FALSE;
    if(D4D_LLD_TCH.D4DTCH_GetPositionRaw == NULL)
      return D4D_FALSE;
    if(D4D_LLD_TCH.D4DTCH_GetRawLimits == NULL)
      return D4D_FALSE;
    if(D4D_LLD_TCH.D4DTCH_DeInit == NULL)
      return D4D_FALSE;
  
    if(!D4D_LLD_TCH.D4DTCH_Init())
      return D4D_FALSE;
  #endif
  
  D4D_LCD_SetOrientation(D4D_ORIENT_PORTRAIT);
  
  #ifdef D4D_LLD_TCH
    // Clear flag to indicate touch screen not calibrated  
    d4d_tchCalib.ScreenCalibrated = 0;
  #endif
  return D4D_TRUE;
}


D4D_ORIENTATION D4D_LCD_GetOrientation(void)
{
  return d4d_orientation;
}

void D4D_LCD_SetOrientation(D4D_ORIENTATION new_orientation)
{
  d4d_orientation = new_orientation;

  (void)D4D_LLD_LCD.D4DLCD_SetOrientation((D4DLCD_ORIENTATION)d4d_orientation);
}

D4D_COOR D4D_DisplayWidth(void)
{
	if (d4d_orientation == D4D_ORIENT_PORTRAIT || d4d_orientation == D4D_ORIENT_PORTRAIT180)
		return (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1);
	else
		return (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE - 1);
}

D4D_COOR D4D_DisplayHeight(void)
{
	if (d4d_orientation == D4D_ORIENT_PORTRAIT || d4d_orientation == D4D_ORIENT_PORTRAIT180)
		return (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE - 1);
	else
		return (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1);
}


D4D_COLOR D4D_LCD_GetGreyScale(D4D_COLOR color)
{
    Byte Red, Green, Blue;

    // Get the separate color components    
    Red = D4D_COLOR_GET_R(color);
    Green = D4D_COLOR_GET_G(color);
    Blue = D4D_COLOR_GET_B(color);
    
    // Get the arithmetic mean of all color components and store it back as individual components
    Green = (Byte)((Word)(Red + Green + Blue) / 3);    
    Red = Green;
    Blue = Green;
    
    // Create the color in GreyScale            
    return D4D_COLOR_RGB(Red, Green, Blue);
}


void D4D_LCD_SwapCoor(D4D_COOR* c1, D4D_COOR* c2)
{
  D4D_COOR tmp;
  
  tmp = *c1;
  *c1 = *c2;
  *c2 = tmp;  
}

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Draving  Functions
  #pragma mark -
#endif  


void D4D_LCD_PutPixel(D4D_COOR x1, D4D_COOR y1, D4D_LINETYPE line_type, D4D_COLOR color)
{
    if (line_type == D4D_LINE_THICK)
    {
        (void)D4D_LLD_LCD.D4DLCD_SetWindow(x1, y1, (unsigned short)(x1+1), (unsigned short)(y1+1));    
        (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(color); (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(color); (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(color); (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);
    }
    else
    {
        (void)D4D_LLD_LCD.D4DLCD_SetWindow(x1, y1, x1, y1);                                     
        (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);                                           
    }
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
}

void D4D_LCD_Box(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color)
{
  LWord i, i_max;

  //Check the coordination if are in right order
  if(x1 > x2)
    D4D_LCD_SwapCoor(&x1, &x2);

  if(y1 > y2)
    D4D_LCD_SwapCoor(&y1, &y2);


  // Set the logic window on LCD
  (void)D4D_LLD_LCD.D4DLCD_SetWindow(x1, y1, x2, y2);                                         
  
  // Count the pixel count
   i_max = (LWord)((LWord)(x2-x1+1)*(LWord)(y2-y1+1));
  
  // send all pixels into LCD
  for (i=0; i < i_max ; ++i) 
    (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(color);                                          
    
  D4D_LLD_LCD.D4DLCD_FlushBuffer();  
}

void D4D_LCD_Rect(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color)   //draw rectangle
{
  D4D_LCD_Line(x1, y1, x2, y1, line_type, color);
  D4D_LCD_Line(x2, y1, x2, y2, line_type, color);
  D4D_LCD_Line(x1, y2, x2, y2, line_type, color);
  D4D_LCD_Line(x1, y1, x1, y2, line_type, color);
}

void D4D_LCD_Line (D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color)     //draw line - bresenham algorithm
{
   sWord delta_x, delta_y;
   sByte ix, iy;

   if((x1 == x2) || (y1 == y2))  // if the line is vertical or horizontal then draw box function is faster than general bresenham algorithm
   {       
      if(line_type == D4D_LINE_THICK)
      {
        if(x1 > x2)
        {
          D4D_LCD_SwapCoor(&x1, &x2);
        }
         
        if(y1 > y2)
        {
          D4D_LCD_SwapCoor(&y1, &y2);
        }
         
        x2++;
        y2++;  
      }
      
      D4D_LCD_Box(x1 ,y1 ,x2 , y2, color);
      
      return;
   }
   
   delta_x = (sWord)(D4D_Abs16((sWord)(x2 - x1)) << 1);
   delta_y = (sWord)(D4D_Abs16((sWord)(y2 - y1)) << 1);

    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    ix = (sByte)((x2 > x1)?1:-1);
    iy = (sByte)((y2 > y1)?1:-1);

    D4D_LCD_PutPixel((D4D_COOR) x1, (D4D_COOR) y1, line_type, color);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        sLWord error = delta_y - (delta_x >> 1);

        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing

            x1 += ix;
            error += delta_y;

            D4D_LCD_PutPixel((D4D_COOR) x1, (D4D_COOR) y1, line_type, color);
        }
    }
    else
    {
        // error may go below zero
        sLWord error = delta_x - (delta_y >> 1);

        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing

            y1 += iy;
            error += delta_x;

            D4D_LCD_PutPixel((D4D_COOR) x1, (D4D_COOR) y1, line_type, color);
        }
    }
 }

// The '(x != 0 && y != 0)' test in the last line of this function
// may be omitted for a performance benefit if the radius of the
// circle is known to be non-zero.
static void D4D_LCD_Plot4points(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_LINETYPE line_type, D4D_COLOR color)
{
  D4D_LCD_PutPixel((D4D_COOR)(cx + x), (D4D_COOR)(cy + y), line_type, color);
  if (x != 0) D4D_LCD_PutPixel((D4D_COOR)(cx - x), (D4D_COOR)(cy + y), line_type, color);
  if (y != 0) D4D_LCD_PutPixel((D4D_COOR)(cx + x), (D4D_COOR)(cy - y), line_type, color);
  if (x != 0 && y != 0) D4D_LCD_PutPixel((D4D_COOR)(cx - x), (D4D_COOR)(cy - y), line_type, color);
}

static void D4D_LCD_Plot4pointsfill(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_COLOR color)
{
  D4D_LCD_Box((D4D_COOR)(cx-x), (D4D_COOR)(cy+y), (D4D_COOR)(cx+x), (D4D_COOR)(cy+y), color);
  D4D_LCD_Box((D4D_COOR)(cx-x), (D4D_COOR)(cy-y), (D4D_COOR)(cx+x), (D4D_COOR)(cy-y), color);
}


static void D4D_LCD_Plot8points(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color)
{
  if(fill) {
    
    D4D_LCD_Plot4pointsfill(cx, cy, x, y, color);
    if (x != y) D4D_LCD_Plot4pointsfill(cx, cy, y, x, color);
  }
  else {
    
    D4D_LCD_Plot4points(cx, cy, x, y, line_type, color);
    if (x != y) D4D_LCD_Plot4points(cx, cy, y, x, line_type, color);
  }
}
 


// 'cx' and 'cy' denote the offset of the circle centre from the origin.
void D4D_LCD_Circle (D4D_COOR cx, D4D_COOR cy, D4D_COOR radius, 
                 D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color)   //draw circle
{
  sWord error =  (sWord)(-1 * (sWord)radius);
  D4D_COOR x = radius;
  D4D_COOR y = 0;
 
  // The following while loop may altered to 'while (x > y)' for a
  // performance benefit, as long as a call to 'plot4points' follows
  // the body of the loop. This allows for the elimination of the
  // '(x != y') test in 'plot8points', providing a further benefit.
  //
  // For the sake of clarity, this is not shown here.
  while (x >= y)
  {
    D4D_LCD_Plot8points(cx, cy, x, y, fill, line_type, color);
 
    error += y;
    ++y;
    error += y;
 
    // The following test may be implemented in assembly language in
    // most machines by testing the carry flag after adding 'y' to
    // the value of 'error' in the previous step, since 'error'
    // nominally has a negative value.
    if (error >= 0)
    {
      --x;
      error -= x;
      error -= x;
    }
  }
}

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Draving Round Corner Functions
  #pragma mark -
#endif

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

void D4D_LCD_RBox(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius)
{
  if(radius)
  {
    //Check the coordination if are in right order
    if(x1 > x2)
      D4D_LCD_SwapCoor(&x1, &x2);

    if(y1 > y2)
      D4D_LCD_SwapCoor(&y1, &y2);

    D4D_LCD_Box(x1, (unsigned short)(y1+radius), x2, (unsigned short)(y2-radius), color);
    D4D_LCD_Box((unsigned short)(x1+radius),y1,(unsigned short)(x2-radius),(unsigned short)(y1+radius), color);
    D4D_LCD_Box((unsigned short)(x1+radius),(unsigned short)(y2-radius),(unsigned short)(x2-radius),y2, color);
    
   
    D4D_LCD_QCircle((unsigned short)(x1+radius), (unsigned short)(y1+radius), radius, D4D_TRUE, D4D_LINE_THIN, color, D4D_QUAD_4);
    D4D_LCD_QCircle((unsigned short)(x2-radius), (unsigned short)(y1+radius), radius, D4D_TRUE, D4D_LINE_THIN, color, D4D_QUAD_1);
    D4D_LCD_QCircle((unsigned short)(x2-radius), (unsigned short)(y2-radius), radius, D4D_TRUE, D4D_LINE_THIN, color, D4D_QUAD_2);
    D4D_LCD_QCircle((unsigned short)(x1+radius), (unsigned short)(y2-radius), radius, D4D_TRUE, D4D_LINE_THIN, color, D4D_QUAD_3);
  }else
    D4D_LCD_Box(x1, y1, x2, y2, color);  
}

void D4D_LCD_RRect(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE line_type, D4D_COLOR color, D4D_COOR radius)   //draw rectangle
{
  if(radius)
  {
    D4D_LCD_Line((unsigned short)(x1+radius), y1, (unsigned short)(x2-radius), y1, line_type, color);
    D4D_LCD_Line(x2, (unsigned short)(y1+radius), x2, (unsigned short)(y2-radius), line_type, color);
    D4D_LCD_Line((unsigned short)(x1+radius), y2, (unsigned short)(x2-radius), y2, line_type, color);
    D4D_LCD_Line(x1, (unsigned short)(y1+radius), x1, (unsigned short)(y2-radius), line_type, color);
      
    D4D_LCD_QCircle((unsigned short)(x1+radius), (unsigned short)(y1+radius), radius, D4D_FALSE, line_type, color, D4D_QUAD_4);
    D4D_LCD_QCircle((unsigned short)(x2-radius), (unsigned short)(y1+radius), radius, D4D_FALSE, line_type, color, D4D_QUAD_1);
    D4D_LCD_QCircle((unsigned short)(x2-radius), (unsigned short)(y2-radius), radius, D4D_FALSE, line_type, color, D4D_QUAD_2);
    D4D_LCD_QCircle((unsigned short)(x1+radius), (unsigned short)(y2-radius), radius, D4D_FALSE, line_type, color, D4D_QUAD_3);
  }else
    D4D_LCD_Rect(x1, y1, x2, y2, line_type, color);  
}


static void D4D_LCD_QPlot4points(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant)
{
  switch(quadrant) 
  {
    case D4D_QUAD_1:
      if (y != 0) D4D_LCD_PutPixel((D4D_COOR)(cx + x), (D4D_COOR)(cy - y), line_type, color);
      break;
    case D4D_QUAD_2: 
      D4D_LCD_PutPixel((D4D_COOR)(cx + x), (D4D_COOR)(cy + y), line_type, color);
      break;
    case D4D_QUAD_3:
      if (x != 0) D4D_LCD_PutPixel((D4D_COOR)(cx - x), (D4D_COOR)(cy + y), line_type, color);
      break;
    case D4D_QUAD_4:
      if (x != 0 && y != 0) D4D_LCD_PutPixel((D4D_COOR)(cx - x), (D4D_COOR)(cy - y), line_type, color);
      break;
  }
}

static void D4D_LCD_QPlot4pointsfill(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_COLOR color,D4D_QUADRANT quadrant)
{
  switch(quadrant) 
  {
    case D4D_QUAD_1:
      D4D_LCD_Box((D4D_COOR)(cx), (D4D_COOR)(cy-y), (D4D_COOR)(cx+x), (D4D_COOR)(cy-y), color);
      break;
    case D4D_QUAD_2:
      D4D_LCD_Box((D4D_COOR)(cx), (D4D_COOR)(cy+y), (D4D_COOR)(cx+x), (D4D_COOR)(cy+y), color);
      break;
    case D4D_QUAD_3:
      D4D_LCD_Box((D4D_COOR)(cx-x), (D4D_COOR)(cy+y), (D4D_COOR)(cx), (D4D_COOR)(cy+y), color);
      break;
    case D4D_QUAD_4:
      D4D_LCD_Box((D4D_COOR)(cx-x), (D4D_COOR)(cy-y), (D4D_COOR)(cx), (D4D_COOR)(cy-y), color);
      break;
  }
}


static void D4D_LCD_QPlot8points(D4D_COOR cx, D4D_COOR cy, D4D_COOR x, D4D_COOR y, D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant)
{
  if(fill) {
    
    D4D_LCD_QPlot4pointsfill(cx, cy, x, y, color, quadrant);
    if (x != y) D4D_LCD_QPlot4pointsfill(cx, cy, y, x, color, quadrant);
  }
  else {
    
    D4D_LCD_QPlot4points(cx, cy, x, y, line_type, color,quadrant);
    if (x != y) D4D_LCD_QPlot4points(cx, cy, y, x, line_type, color, quadrant);
  }
}

void D4D_LCD_QCircle (D4D_COOR cx, D4D_COOR cy, D4D_COOR radius, 
                 D4D_BOOL fill, D4D_LINETYPE line_type, D4D_COLOR color, D4D_QUADRANT quadrant)   //draw circle
{
  sWord error =  (sWord)(-1 * (sWord)radius);
  D4D_COOR x = radius;
  D4D_COOR y = 0;
 
  if(!radius)
  {
    D4D_LCD_PutPixel(cx, cy, line_type, color);
    return;
  }
 
  // The following while loop may altered to 'while (x > y)' for a
  // performance benefit, as long as a call to 'plot4points' follows
  // the body of the loop. This allows for the elimination of the
  // '(x != y') test in 'plot8points', providing a further benefit.
  //
  // For the sake of clarity, this is not shown here.
  while (x >= y)
  {
    D4D_LCD_QPlot8points(cx, cy, x, y, fill, line_type, color, quadrant);
 
    error += y;
    ++y;
    error += y;
 
    // The following test may be implemented in assembly language in
    // most machines by testing the carry flag after adding 'y' to
    // the value of 'error' in the previous step, since 'error'
    // nominally has a negative value.
    if (error >= 0)
    {
      --x;
      error -= x;
      error -= x;
    }
  }
}



#endif 

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Touch Screen Functions
  #pragma mark -
#endif

#ifdef D4D_LLD_TCH
 
Byte D4D_TCH_GetCalibrationStatus(void)
{
  return d4d_tchCalib.ScreenCalibrated;  
}


D4D_TOUCHSCREEN_CALIB D4D_TCH_GetCalibration(void)
{
  return d4d_tchCalib;  
}

void D4D_TCH_SetCalibration(D4D_TOUCHSCREEN_CALIB newCalib)
{
  d4d_tchCalib = newCalib;
  d4d_tchCalib.ScreenCalibrated = 1;  
}


void D4D_TCH_GetCalibratedPosition(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY)
{	
	D4D_COOR Temp;
	
	if (d4d_tchCalib.ScreenCalibrated)
	{
	  // Screen has been touched and calibrated. Is X offset > touch position?
	  if (d4d_tchCalib.TouchScreenXoffset > *TouchPositionX)
	  {
	      // Offset > touch postion. Force Touch position = offset 
	      *TouchPositionX = 0;
	  }else 
	  {
	    // Adjust coordinates using screen calibration data
	    *TouchPositionX = (D4D_COOR)(((*TouchPositionX - d4d_tchCalib.TouchScreenXoffset) * 16) / 
	      d4d_tchCalib.TouchScreenXBitsPerPixelx16);
	    // Transform touch coordinates to display position 
	    if (*TouchPositionX > D4D_SCREEN_SIZE_LONGER_SIDE)
	    {
	        *TouchPositionX = D4D_SCREEN_SIZE_LONGER_SIDE;
	    }        
	  }

	  
	  // Is Y offset > touch position?
	  if (d4d_tchCalib.TouchScreenYoffset > *TouchPositionY)
	  {
	      // Offset > touch postion. Force Touch position = offset 
	      *TouchPositionY = 0;
	  }else 
	  {
	    // Adjust coordinates using screen calibration data
	    *TouchPositionY = (D4D_COOR)(((*TouchPositionY - d4d_tchCalib.TouchScreenYoffset) * 16) / 
	      d4d_tchCalib.TouchScreenYBitsPerPixelx16); 
	    // Transform touch coordinates to display position
	    if (*TouchPositionY > D4D_SCREEN_SIZE_SHORTER_SIDE)
	    {
	        *TouchPositionY = D4D_SCREEN_SIZE_SHORTER_SIDE;
	    } 
	  }
	  
	  // convert coordinations to right orientation of LCD
	  switch(d4d_orientation)
	  {
	    case D4D_ORIENT_PORTRAIT:
	      Temp = *TouchPositionX;
	      *TouchPositionX = *TouchPositionY;
	      *TouchPositionY = (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE - Temp);
	      break;
	      
	    case D4D_ORIENT_PORTRAIT180:
	      Temp = *TouchPositionX;
	      *TouchPositionX = (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE - *TouchPositionY);
	      *TouchPositionY = Temp;
	      break;
	      
	    case D4D_ORIENT_LANDSCAPE:    
	      *TouchPositionX = (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE - *TouchPositionX);
	      *TouchPositionY = (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE - *TouchPositionY);
	      break;
		}  
	}
}

//
//-----------------------------------------------------------------------------
// FUNCTION:    LCD_GetTouchScreenPosition
// SCOPE:       Global
// DESCRIPTION: Reads touch screen and returns X, Y coorinates if screen 
//              touched 
// PARAMETERS:  LWord *TouchPositionX    Pointer to X coordinate
//              LWord *TouchPositionY    Pointer to Y ccordinate
// RETURNS:     0   no screen touch
//              1   screen touch, 
//-----------------------------------------------------------------------------
//
Byte D4D_TCH_GetRawPosition(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY)
{
  // Read raw touch position into *TouchPositionX and *TouchPositionY
  return D4D_LLD_TCH.D4DTCH_GetPositionRaw (TouchPositionX, TouchPositionY);	
}

static void D4D_GetCalibrationPoint(Byte ix, Word* X, Word* Y, D4D_COLOR fore, D4D_COLOR bckg ) 
{
  D4D_COOR tmp_x, tmp_y;
  
  tmp_x = D4D_DisplayWidth();
  tmp_y = D4D_DisplayHeight();
  
  
  switch(ix) 
  {
    case 0:
      tmp_x = D4DTCH_CALIB_CROSS_OFFSET;
      tmp_y = D4DTCH_CALIB_CROSS_OFFSET;      
      break;
    case 1:
      tmp_x /= 2;
      tmp_y /= 2;      
      break;
    case 2:
      tmp_x -= D4DTCH_CALIB_CROSS_OFFSET;
      tmp_y -= D4DTCH_CALIB_CROSS_OFFSET;      
      break;      
  }
  
  D4D_TCH_DrawCalibrationPoint(tmp_x, tmp_y, fore);
  
  while (D4D_LLD_TCH.D4DTCH_GetPositionRaw ((unsigned short*)X, (unsigned short*)Y) == 0)
  {
      D4D_LLD_LCD.D4DLCD_Delay_ms (1);   
  }
 
  D4D_TCH_DrawCalibrationPoint(tmp_x, tmp_y, bckg);  
}

//
//-----------------------------------------------------------------------------
// FUNCTION:    LCD_CalibrateTouchScreen
// SCOPE:       Global
// DESCRIPTION: Reads touchscreen positions in relation to screen positions
//              and calculates calibration constants. These constants are used
//              to calulate 'real' screen positions
// PARAMETERS:  none
// RETURNS:     VOID 
//-----------------------------------------------------------------------------
//
void D4D_TCH_Calibrate(D4D_COLOR fore, D4D_COLOR bckg)
{
    // Declare and initialise local variables
    D4D_STRING tmp_txtbuff;
    D4D_STR_PROPERTIES tmp_str_prty;
    D4D_TOUCHSCREEN_LIMITS* p_tchRawLimits = D4D_LLD_TCH.D4DTCH_GetRawLimits();
    Word X[3];
    Word Y[3];
    char str_touch[10] = "X1:00000";
    
    Byte tmp_i;

    tmp_txtbuff.fontId = D4D_FONT_SYSTEM_DEFAULT;
    tmp_str_prty.font_properties.all = 0;
    tmp_txtbuff.str_properties = &tmp_str_prty;
    
    // Initialise display orientation to LANDSCAPE
    D4D_ClearScreen(bckg);
    
    if(D4D_GetFont(D4D_FONT_SYSTEM_DEFAULT) != NULL)
    {
      tmp_txtbuff.buffSize = 30;
      tmp_txtbuff.pText = "Touch the cross by stylus.";
      D4D_DrawTextXY(10, (D4D_COOR)(D4D_DisplayHeight() - 4 * (D4D_GetFontHeight(D4D_FONT_SYSTEM_DEFAULT)+1) - 10), &tmp_txtbuff, fore, bckg);
    }
    
    // Set touchscreen calibrated flag to FALSE
    d4d_tchCalib.ScreenCalibrated = 0;
    
    while (d4d_tchCalib.ScreenCalibrated == 0)
    {
        // Capture input calibration points
        for(tmp_i=0;tmp_i<3;tmp_i++) 
        {
          
          D4D_GetCalibrationPoint(tmp_i, &X[tmp_i], &Y[tmp_i], fore, bckg);  
          
          if(D4D_GetFont(D4D_FONT_SYSTEM_DEFAULT) != NULL) 
          {            
            D4D_COOR tmp_y_text = (D4D_COOR)(D4D_DisplayHeight() - (3 - tmp_i) * (D4D_GetFontHeight(D4D_FONT_SYSTEM_DEFAULT) + 1) - 5);
            
            tmp_txtbuff.pText = str_touch;
            str_touch[1] = (char)('1' + tmp_i);
            str_touch[0] = 'X';
            str_touch[D4D_SprintDecU16((Word)X[tmp_i],&str_touch[3],' ') + 3] = 0; 
            
            D4D_FillRectXY(10, tmp_y_text, (D4D_COOR)(D4D_GetFontHeight(D4D_FONT_SYSTEM_DEFAULT) * 16), (D4D_COOR)(tmp_y_text + D4D_GetFontWidth(D4D_FONT_SYSTEM_DEFAULT)), bckg);
            
            D4D_DrawTextXY(10, tmp_y_text, &tmp_txtbuff, fore, bckg);
            str_touch[0] = 'Y';
            str_touch[D4D_SprintDecU16((Word)Y[tmp_i],&str_touch[3],' ') + 3] = 0; 
            D4D_DrawTextXY((D4D_COOR)(10 + D4D_GetFontHeight(D4D_FONT_SYSTEM_DEFAULT) * 8), tmp_y_text, &tmp_txtbuff, fore, bckg);
          }
          D4D_LLD_LCD.D4DLCD_Delay_ms(1000);
        }
        
        // convert captured values into Landscape orientation 
        switch(d4d_orientation) 
        {
          Word tmp_coor;
          
          case D4D_ORIENT_LANDSCAPE180:
            tmp_coor = X[0];
            X[0] = X[2];
            X[2] = tmp_coor;
            tmp_coor = Y[0];
            Y[0] = Y[2];
            Y[2] = tmp_coor;
            break;
          case D4D_ORIENT_PORTRAIT:
            tmp_coor = Y[0];
            Y[0] = Y[2];
            Y[2] = tmp_coor;
            break;
          case D4D_ORIENT_PORTRAIT180:
            tmp_coor = X[0];
            X[0] = X[2];
            X[2] = tmp_coor;
            break;  
          case D4D_ORIENT_LANDSCAPE:
          default:
            break;  
        }
        
        // Check X1 > X2 > X3 and Y1 > Y2 > Y3
        if ((X[0] > X[1]) && (X[1] > X[2]) && (Y[0] > Y[1]) && (Y[1] > Y[2]))
        {
            Word dX, dY;
            
            // compute difference in axis X
            dX = (Word)(X[0] - X[2]);

            // compute difference in axis Y
            dY = (Word)(Y[0] - Y[2]);
     
            // Calculate number of (bits / pixel) * 16 for X
             d4d_tchCalib.TouchScreenXBitsPerPixelx16 = (Word)((dX * 16)/(D4D_SCREEN_SIZE_LONGER_SIDE - D4DTCH_CALIB_CROSS_OFFSET * 2));

            // Does number of ADC bits per pixel in X seem reasonable?
            if (d4d_tchCalib.TouchScreenXBitsPerPixelx16 < ((16 * p_tchRawLimits->rawDataScale) / D4D_SCREEN_SIZE_LONGER_SIDE))        
            {
                // Calculate number of (bits / pixel) * 16 for Y
                d4d_tchCalib.TouchScreenYBitsPerPixelx16 = (Word)((dY * 16)/(D4D_SCREEN_SIZE_SHORTER_SIDE - D4DTCH_CALIB_CROSS_OFFSET * 2));

                // Does number of ADC bits per pixel in Y seem reasonable?
                if (d4d_tchCalib.TouchScreenYBitsPerPixelx16 < ((16 * p_tchRawLimits->rawDataScale) / D4D_SCREEN_SIZE_SHORTER_SIDE))
                {
                    // Calculate LCD Y offset
                    d4d_tchCalib.TouchScreenYoffset = (Word)((Y[2] * 16 - D4DTCH_CALIB_CROSS_OFFSET * d4d_tchCalib.TouchScreenYBitsPerPixelx16) / 16);

                    // Calculate LCD X offset
                    d4d_tchCalib.TouchScreenXoffset = (Word)((X[2] * 16 - D4DTCH_CALIB_CROSS_OFFSET * d4d_tchCalib.TouchScreenXBitsPerPixelx16) / 16);

                    // Is screen offset in X and Y reasonable?
                    if (d4d_tchCalib.TouchScreenYoffset < p_tchRawLimits->touchOffMaxY && d4d_tchCalib.TouchScreenXoffset < p_tchRawLimits->touchOffMaxX)
                    {
                        // Set flag to indicate touch screen calibrated
                        d4d_tchCalib.ScreenCalibrated = 1;                        
                    }    
                } 
            }        
        }                
    }
}

//
//-----------------------------------------------------------------------------
// FUNCTION:    LCD_DrawCalibrationPoint
// SCOPE:       Global
// DESCRIPTION: Draws a crosshair on screen
// PARAMETERS:  D4D_COOR X - x coord of crosshair
//              D4D_COOR Y - y coord of crosshair
//              D4D_COLOR Color - crosshair color
// RETURNS:     VOID 
//-----------------------------------------------------------------------------
//
void D4D_TCH_DrawCalibrationPoint (D4D_COOR X, D4D_COOR Y, D4D_COLOR Color)
{
    D4D_LCD_Line((D4D_COOR)(X - 10), Y, (D4D_COOR)(X + 10), Y, 0, Color);
    D4D_LCD_Line(X, (D4D_COOR)(Y - 10), X, (D4D_COOR)(Y + 10), 0, Color);
}

#else

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Dummy Touch Screen Functions
  #pragma mark -
#endif  


void D4D_TCH_Calibrate(D4D_COLOR fore, D4D_COLOR bckg)
{
  D4D_UNUSED(fore);
  D4D_UNUSED(bckg);
}

void D4D_TCH_DrawCalibrationPoint (D4D_COOR X, D4D_COOR Y, D4D_COLOR Color)
{
  D4D_UNUSED(X);
  D4D_UNUSED(Y);
  D4D_UNUSED(Color);
}

#endif