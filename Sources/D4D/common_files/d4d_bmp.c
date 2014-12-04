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
* @file      d4d_bmp.c
*
* @author    b01119
* 
* @version   0.0.18.0
* 
* @date      Jan-24-2011
* 
* @brief     D4D driver bmp decoder functions c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_lldapi.h"
#include "d4d_private.h"

static D4D_COLOR D4D_ConvertColor332(Byte color332);


D4D_BOOL D4D_BmpHeaderCheckEndianism(const D4D_BMP* pBmp)
{
  Word* pHead = (Word*)pBmp->pData;
  
  if(pHead[2] & D4D_BMP_ENDIAN_FLAG)
    return D4D_FALSE;
  else
    return D4D_TRUE;
}


/**************************************************************//*!
*
* Conversion function (UpSampling) from 332 format to D4D Color System
*
******************************************************************/
static D4D_COLOR D4D_ConvertColor332(Byte color332)
{
  #if D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB332
    return color332;
  #else  
    Byte Red, Green, Blue;
    
    //Red = (Byte)((D4D_COLOR332_GET_R(color332) * 9) >> 1);   // new value could be max 28 / possible 31 
    //Green = (Byte)(D4D_COLOR332_GET_G(color332) * 9);       // new value could be max 63 / possible 63
    //Blue = (Byte)((D4D_COLOR332_GET_B(color332) * 21) >> 1); // new value could be max 30 / possible 31

    Red = D4D_COLOR332_GET_R(color332);   // new value could be max 28 / possible 31 
    Green = D4D_COLOR332_GET_G(color332);       // new value could be max 63 / possible 63
    Blue = D4D_COLOR332_GET_B(color332); // new value could be max 30 / possible 31

    return D4D_COLOR_RGB(Red, Green, Blue);
    
  #endif  
}

static void D4D_Draw_2high_Byte(Byte val, Word ColorTable[], Byte pixelCnt, D4D_BOOL GreyScale)
{

  Byte i = 0;
  Byte i_mask = 0x80;
  D4D_COLOR pix_clr;
  pixelCnt++; 
  for(i = 0; i < pixelCnt; i++)
  {
      pix_clr = ColorTable[((val & i_mask)? 1:0)];
      if(GreyScale)
        pix_clr = D4D_LCD_GetGreyScale(pix_clr);  
      (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pix_clr);
      i_mask >>= 1;
  }  
}

static void D4D_Draw_16high_Byte(Byte val, Word ColorTable[], Byte pixelCnt, D4D_BOOL GreyScale)
{

  Byte i = 0;
  Byte i_mask = 0xf0;
  D4D_COLOR pix_clr;
  pixelCnt++;
   
  for(i = 0; i < pixelCnt; i++)
  {
      pix_clr = ColorTable[val & i_mask];
      if(GreyScale)
        pix_clr = D4D_LCD_GetGreyScale(pix_clr);  
      (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pix_clr);
      i_mask >>= 4;
  }  
}


//draw bitmap
void D4D_LCD_Bmp65kNoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                     const Word Pbmp[], 
                     D4D_BOOL Compressed, D4D_BOOL GreyScale)  
{   
    D4D_COLOR pxl_clr, pxl_clr_prv;
    Word  rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr_prv = (D4D_COLOR)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; ++i)           // do for all pixels
    {
      pxl_clr = Pbmp[ix++];           // load the color of pixel in 565 format
      if(GreyScale)
        pxl_clr = D4D_LCD_GetGreyScale(pxl_clr);   // if needed convert it to grayscale
      (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);             // and send color to display
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr_prv == pxl_clr)              // check the two same colors in row
        {
            rpt_cnt = Pbmp[ix++];            // load the count of repeated pixels
            i += rpt_cnt;									         // update overall count of pixels
            while (rpt_cnt--)                                        
                (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);   // write same pixels to display
            pxl_clr = (D4D_COLOR)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr_prv = pxl_clr;                   // store the last drawed pixel
      }
    }
    
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
   
}
     
void D4D_LCD_Bmp256NoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,
                          const Byte Pbmp[], 
                          D4D_BOOL Compressed, D4D_BOOL GreyScale)
{   
    D4D_COLOR pxl_clr;
    Byte pxl_clr8, pxl_clr8_prv, rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr8_prv = (Byte)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; ++i)           // do for all pixels
    {
      pxl_clr8 = Pbmp[ix++];           // load the color of pixel in 332 format
      pxl_clr = D4D_ConvertColor332(pxl_clr8); // convert it from 332 to d4d color system
      if(GreyScale)
        pxl_clr = D4D_LCD_GetGreyScale(pxl_clr);   // if needed convert it to grayscale
      (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);             // and send color to display
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
            i += rpt_cnt;									         // update overall count of pixels
            while (rpt_cnt--)                                        
                (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);   // write same pixels to display
            pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    } 
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();  
}

void D4D_LCD_Bmp256Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[],
                        D4D_BOOL Compressed, D4D_BOOL GreyScale)
{
    D4D_COLOR pxl_clr;
    Byte pxl_clr8, pxl_clr8_prv, rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr8_prv = (Byte)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; ++i)           // do for all pixels
    {
      pxl_clr8 = Pbmp[ix++];           // load the index of pixel color
      pxl_clr = ColorTable[pxl_clr8];  // take the color from color table
      if(GreyScale)
        pxl_clr = D4D_LCD_GetGreyScale(pxl_clr);   // if needed convert it to grayscale
      (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);             // and send color to display
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
            i += rpt_cnt;									         // update overall count of pixels
            while (rpt_cnt--)                                        
                (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);   // write same pixels to display
            pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
}

void D4D_LCD_Bmp16Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale)
{
   
    Byte pxl_clr8, pxl_clr8_prv, rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr8_prv = (Byte)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; i+=2)           // do for all pixels
    {
      pxl_clr8 = Pbmp[ix++];           // load the index of pixel color
      if((ix_max - i) > 1)
        rpt_cnt = 1;
      else
        rpt_cnt = 0;
      D4D_Draw_16high_Byte(pxl_clr8, (Word*)ColorTable, rpt_cnt, GreyScale);
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
            i += (rpt_cnt * 2);									         // update overall count of pixels
            while (rpt_cnt--)                                        
                D4D_Draw_16high_Byte(pxl_clr8, (Word*)ColorTable, 1 , GreyScale);   // write same pixels to display
            pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
 
}

void D4D_LCD_Bmp2Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale)
{
    Byte pxl_clr8, pxl_clr8_prv, rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr8_prv = (Byte)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; i+=8)           // do for all pixels
    {
      pxl_clr8 = Pbmp[ix++];           // load the index of pixel color
      if((ix_max - i) > 7)
        rpt_cnt = 7;
      else
        rpt_cnt = (Byte)(ix_max - i);
      D4D_Draw_2high_Byte(pxl_clr8, (Word*)ColorTable, rpt_cnt , GreyScale);
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
            i += (rpt_cnt * 8);									         // update overall count of pixels
            while (rpt_cnt--)                                        
                D4D_Draw_2high_Byte(pxl_clr8, (Word*)ColorTable, 7 , GreyScale);   // write same pixels to display
            pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
}

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Rounded corners bitmaps print functions
  #pragma mark -
#endif
// Calculate the count of non printed piwels in specified line 

//print rounded corner BMP

//draw bitmap
void D4D_LCD_RBmp65kNoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                     const Word Pbmp[], 
                     D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius)  
{       
  D4D_COLOR pxl_clr, pxl_clr_prv;
  D4D_INDEX rpt_cnt = 0;
  LWord x_ix, y_ix,  non_print_len;
  LWord ix = 0;

    pxl_clr_prv = (D4D_COLOR)~Pbmp[0];
  
  // the main drawing function    
  for (y_ix = 0; y_ix < (y2 - y1 + 1); ++y_ix)           // do for all lines
  {
    if(y_ix < radius)
      non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(y_ix)); // get len of non printed pixels
    else if(((y2 - y1) - y_ix) < radius)
      non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)((y2 - y1) - y_ix)); // get len of non printed pixels
    else
      non_print_len = 0;
    
    for (x_ix = 0; x_ix < (x2 - x1 + 1); ++x_ix)           // do for all pixels on line
    {
  
      if(rpt_cnt == 0)
      {          
        pxl_clr = (D4D_COLOR)Pbmp[ix++];           // load the color of pixel in 332 format
        
        if(GreyScale)
          pxl_clr = D4D_LCD_GetGreyScale(pxl_clr);   // if needed convert it to grayscale
          
        if((x_ix >= non_print_len) && (x_ix <= ((x2 - x1) - non_print_len)))
        {
          // Define window
          (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x1 + x_ix), (D4D_COOR)(y1 + y_ix), (D4D_COOR)(x1 + x_ix + 1), (D4D_COOR)(y1 + y_ix + 1));    

          (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);             // and send color to display
        }
      }
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr_prv == pxl_clr)              // check the two same colors in row
        {
            if(rpt_cnt == 0)
            {
              
              rpt_cnt = (D4D_INDEX)Pbmp[ix++];            // load the count of repeated pixels
              if(rpt_cnt)
                continue;
            }
           
            if(rpt_cnt)
            {
              
              rpt_cnt--; 

              if((x_ix >= non_print_len) && (x_ix <= ((x2 - x1) - non_print_len)))
              {
                // Define window
                (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x1 + x_ix), (D4D_COOR)(y1 + y_ix), (D4D_COOR)(x1 + x_ix + 1), (D4D_COOR)(y1 + y_ix + 1));    
              
                (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);   // write same pixels to display
              }
            }
            
            if(rpt_cnt == 0)
            {                
              pxl_clr = (D4D_COLOR)~Pbmp[ix];            // devalue the last information of drawed pixel
            }
        }
        pxl_clr_prv = pxl_clr;                   // store the last drawed pixel
      }
    }
  } 
  
  D4D_LLD_LCD.D4DLCD_FlushBuffer();
}

void D4D_LCD_RBmp256NoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,
                          const Byte Pbmp[], 
                          D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius)
{   
  D4D_COLOR pxl_clr;
  Byte pxl_clr8, pxl_clr8_prv;
  Byte rpt_cnt = 0;
  LWord x_ix, y_ix,  non_print_len;
  LWord ix = 0;

    pxl_clr8_prv = (Byte)~Pbmp[0];
  
  // the main drawing function    
  for (y_ix = 0; y_ix < (y2 - y1 + 1); ++y_ix)           // do for all lines
  {
    if(y_ix < radius)
      non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(y_ix)); // get len of non printed pixels
    else if(((y2 - y1) - y_ix) < radius)
      non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)((y2 - y1) - y_ix)); // get len of non printed pixels
    else
      non_print_len = 0;
    
    for (x_ix = 0; x_ix < (x2 - x1 + 1); ++x_ix)           // do for all pixels on line
    {
  
      if(rpt_cnt == 0)
      {          
        pxl_clr8 = Pbmp[ix++];           // load the color of pixel in 332 format
        
        pxl_clr = D4D_ConvertColor332(pxl_clr8); // convert it from 332 to d4d color system

        if(GreyScale)
          pxl_clr = D4D_LCD_GetGreyScale(pxl_clr);   // if needed convert it to grayscale
          
        if((x_ix >= non_print_len) && (x_ix <= ((x2 - x1) - non_print_len)))
        {
          // Define window
          (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x1 + x_ix), (D4D_COOR)(y1 + y_ix), (D4D_COOR)(x1 + x_ix + 1), (D4D_COOR)(y1 + y_ix + 1));    

          (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);             // and send color to display
        }
      }
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            if(rpt_cnt == 0)
            {
              
              rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
              if(rpt_cnt)
                continue;
            }
           if(rpt_cnt)
            {                
              rpt_cnt--; 
              if((x_ix >= non_print_len) && (x_ix <= ((x2 - x1) - non_print_len)))
              {
                // Define window
                (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x1 + x_ix), (D4D_COOR)(y1 + y_ix), (D4D_COOR)(x1 + x_ix + 1), (D4D_COOR)(y1 + y_ix + 1));    
              
                (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);   // write same pixels to display
              }
            }
            
            if(rpt_cnt == 0)
            {                
              pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
            }
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
  } 
  
  D4D_LLD_LCD.D4DLCD_FlushBuffer();  
}

void D4D_LCD_RBmp256Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[],
                        D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius)
{ 
  D4D_COLOR pxl_clr;
  Byte pxl_clr8, pxl_clr8_prv;
  Byte rpt_cnt = 0;
  LWord x_ix, y_ix,  non_print_len;
  LWord ix = 0;

    pxl_clr8_prv = (Byte)~Pbmp[0];
  
  // the main drawing function    
  for (y_ix = 0; y_ix < (y2 - y1 + 1); ++y_ix)           // do for all lines
  {
    if(y_ix < radius)
      non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(y_ix)); // get len of non printed pixels
    else if(((y2 - y1) - y_ix) < radius)
      non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)((y2 - y1) - y_ix)); // get len of non printed pixels
    else
      non_print_len = 0;
    
    for (x_ix = 0; x_ix < (x2 - x1 + 1); ++x_ix)           // do for all pixels on line
    {
  
      if(rpt_cnt == 0)
      {          
        pxl_clr8 = Pbmp[ix++];           // load the color of pixel in 332 format
        
        pxl_clr = ColorTable[pxl_clr8];  // take the color from color table

        if(GreyScale)
          pxl_clr = D4D_LCD_GetGreyScale(pxl_clr);   // if needed convert it to grayscale
          
        if((x_ix >= non_print_len) && (x_ix <= ((x2 - x1) - non_print_len)))
        {
          // Define window
          (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x1 + x_ix), (D4D_COOR)(y1 + y_ix), (D4D_COOR)(x1 + x_ix + 1), (D4D_COOR)(y1 + y_ix + 1));    

          (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);             // and send color to display
        }
      }
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            if(rpt_cnt == 0)
            {
              
              rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
              if(rpt_cnt)
                continue;
            }
           if(rpt_cnt)
            {                
              rpt_cnt--; 
              if((x_ix >= non_print_len) && (x_ix <= ((x2 - x1) - non_print_len)))
              {
                // Define window
                (void)D4D_LLD_LCD.D4DLCD_SetWindow ((D4D_COOR)(x1 + x_ix), (D4D_COOR)(y1 + y_ix), (D4D_COOR)(x1 + x_ix + 1), (D4D_COOR)(y1 + y_ix + 1));    
              
                (void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(pxl_clr);   // write same pixels to display
              }
            }
            
            if(rpt_cnt == 0)
            {                
              pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
            }
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
  } 
  
  D4D_LLD_LCD.D4DLCD_FlushBuffer();  

}

/*

void D4D_LCD_RBmp16Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius)
{
   
    Byte pxl_clr8, pxl_clr8_prv, rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr8_prv = (Byte)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; i+=2)           // do for all pixels
    {
      pxl_clr8 = Pbmp[ix++];           // load the index of pixel color
      if((ix_max - i) > 1)
        rpt_cnt = 1;
      else
        rpt_cnt = 0;
      D4D_Draw_16high_Byte(pxl_clr8, (Word*)ColorTable, rpt_cnt, GreyScale);
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
            i += (rpt_cnt * 2);									         // update overall count of pixels
            while (rpt_cnt--)                                        
                D4D_Draw_16high_Byte(pxl_clr8, (Word*)ColorTable, 1 , GreyScale);   // write same pixels to display
            pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
 
}

void D4D_LCD_RBmp2Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius)
{
    Byte pxl_clr8, pxl_clr8_prv, rpt_cnt;
    LWord i, ix_max;
    LWord ix = 0;
    
    // Define window
    (void)D4D_LLD_LCD.D4DLCD_SetWindow (x1, y1, x2, y2);    

    ix_max = (LWord) ((x2 - x1 + 1) * (y2 - y1 + 1)); // Count the overall pixel count

    pxl_clr8_prv = (Byte)~Pbmp[0];
    
    // the main drawing function
    
    for (i=0; i<ix_max; i+=8)           // do for all pixels
    {
      pxl_clr8 = Pbmp[ix++];           // load the index of pixel color
      if((ix_max - i) > 7)
        rpt_cnt = 7;
      else
        rpt_cnt = (Byte)(ix_max - i);
      D4D_Draw_2high_Byte(pxl_clr8, (Word*)ColorTable, rpt_cnt , GreyScale);
      
      if(Compressed)                               // case that bitmap is compressed 
      {
        if (pxl_clr8_prv == pxl_clr8)              // check the two same colors in row
        {
            rpt_cnt = (Byte)Pbmp[ix++];            // load the count of repeated pixels
            i += (rpt_cnt * 8);									         // update overall count of pixels
            while (rpt_cnt--)                                        
                D4D_Draw_2high_Byte(pxl_clr8, (Word*)ColorTable, 7 , GreyScale);   // write same pixels to display
            pxl_clr8 = (Byte)~Pbmp[ix];            // devalue the last information of drawed pixel
        }
        pxl_clr8_prv = pxl_clr8;                   // store the last drawed pixel
      }
    }
    
    D4D_LLD_LCD.D4DLCD_FlushBuffer();
}
*/

#endif

