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
* @file      d4d_font.c
*
* @author    b01119
* 
* @version   0.0.32.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver font c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_lldapi.h"
#include "d4d_private.h"

static D4D_COOR D4D_LCD_PrintChr(D4D_CHAR ch, D4D_PRINT_STRING* p_CharDes);

/**************************************************************//*!
*
* Get selected font information
*
******************************************************************/

D4D_FONT_TYPE* D4D_GetFont(D4D_FONT ix)
{        

#if (D4D_FONT_TABLE_DISABLED == D4D_TRUE)
    D4D_UNUSED(ix);
    return NULL;
#else      
    Byte i=0;
    
    while(d4d_FontTable[i].pFontDescriptor != NULL)
    {
      if(d4d_FontTable[i].ix_font == ix)
        return (D4D_FONT_TYPE*) &d4d_FontTable[i];  
      i++;
    }           
    return NULL;
#endif
}

/**************************************************************//*!
*
* Get selected font size information
*
******************************************************************/

D4D_FONT_SIZES D4D_GetFontSize(D4D_FONT ix)
{    
    D4D_FONT_SIZES tmpFontSize;

    tmpFontSize.height = D4D_GetFontHeight(ix);
    tmpFontSize.width = D4D_GetFontWidth(ix);       
    return tmpFontSize;
}


/**************************************************************//*!
*
* Get selected font height information
*
******************************************************************/

D4D_FONT_SIZE D4D_GetFontHeight(D4D_FONT ix)
{    
    D4D_FONT_TYPE* pFontType = D4D_GetFont(ix);
    D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);                                                              
    
    if(pFontType != NULL)
      return (D4D_FONT_SIZE)((pFontType->scale.height * pFontDescriptor->charFullSize.height) + pFontType->charSpacing.height);
           
    return (D4D_FONT_SIZE) 0;
}

/**************************************************************//*!
*
* Get selected font width information
*
******************************************************************/

D4D_FONT_SIZE D4D_GetFontWidth(D4D_FONT ix)
{    
    D4D_FONT_TYPE* pFontType = D4D_GetFont(ix);
    D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);                                                              
    
    if(pFontType != NULL)
      return (D4D_FONT_SIZE)((pFontType->scale.width * pFontDescriptor->charFullSize.width) + pFontType->charSpacing.width);
           
    return (D4D_FONT_SIZE) 0;
}

/**************************************************************//*!
*
* Get selected font width information
*
******************************************************************/

D4D_FONT_SIZE D4D_GetCharWidth(D4D_FONT ix, char ch)
{    
    D4D_FONT_TYPE* pFontType = D4D_GetFont(ix);
    D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);
    D4D_FONT_SIZE tmp_width = 0;
    
    if(!ch)
      return 0;
    
    if(pFontType != NULL) {
      // Get the raw width of char
      if(pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL) {  
        tmp_width = pFontDescriptor->pSizeTable[D4D_GetCharIndex(pFontType, ch)];
      }else {
        tmp_width = pFontDescriptor->charFullSize.width;        
      }
      // Apply the scale for the char width
      tmp_width *= pFontType->scale.width;
      // Add the char spacing 
      tmp_width += pFontType->charSpacing.width;
    }
    
    // return the char width
    return tmp_width;
}



/**************************************************************//*!
*
* Get "real" index in data font table of used font
*
******************************************************************/

D4D_FONT_IX D4D_GetCharIndex(D4D_FONT_TYPE* pFontType, char ch) {
  
  D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);
  D4D_FONT_IX char_ix = (D4D_FONT_IX)((Byte)ch -  pFontDescriptor->startChar);

  // is the indes table linear or nonlinear?
  if(pFontDescriptor->flags & D4D_FONT_FLAGS_IX_STYLE_NONLINEAR) {    
    // Index table is used? 
    if(pFontDescriptor->pIxTable != NULL) {
      if(pFontDescriptor->flags & D4D_FONT_FLAGS_IX_TYPE_MAP) {
        // Map style of Index Table
        D4D_FONT_IX i;
        // Look for the used char in index map table
        for(i = 0; i < pFontDescriptor->charCnt; i++) {
          if(ch == pFontDescriptor->pIxTable[i])
            return i;
        }
      }else {
        
         // check if the char is set into range of used font
        if(char_ix < pFontDescriptor->charCnt)
          // Look up style of Index Table
          return pFontDescriptor->pIxTable[char_ix];      
      }
    }
  }else 
  {
     // check if the char is set into range of used font
    if(char_ix < pFontDescriptor->charCnt)    
      return char_ix;
  }
  
  // return the "real table index" of used car 
  return pFontDescriptor->charDefault; 
}


/**************************************************************//*!
*
* Get pointer on bitmap of selected char of used font
*
******************************************************************/

D4D_FONT_DATA* D4D_GetCharData(D4D_FONT_TYPE* pFontType, char ch) {
  
  D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);
  D4D_FONT_IX char_ix = D4D_GetCharIndex(pFontType, ch);
 
  
  // check the style of font monospace / proportional
  if(pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL) {
    // Proportinal font is used
    
    // Check if Offset table exist
    if(pFontDescriptor->pOffTable == NULL)
      return NULL;
      
    return (D4D_FONT_DATA*)(pFontDescriptor->pFontData + pFontDescriptor->pOffTable[char_ix]);
    
      
  }else {
    // Monospace font is used
    return (D4D_FONT_DATA*)(pFontDescriptor->pFontData + char_ix * pFontDescriptor->charBmpSize);
  }
}

/**************************************************************//*!
*
* Get "real" index in data font table of used font
*
******************************************************************/
static D4D_BOOL D4D_LCD_GetFntBit(Byte row, Byte clm, D4D_FONT_SIZE char_width, D4D_FONT_DATA* pFntData, D4D_FONT_DESCRIPTOR* pFontDescriptor) {
  
  Word bit_cnt;
  Byte data;
  
  // This is decoder only for fonts that are row scan based and preffered !!!
  if(pFontDescriptor->pack & D4D_FONT_PACK_SCANPREFERRED_COLUMN)
    return 0;
  
  if(pFontDescriptor->pack & D4D_FONT_PACK_SCANBASED_COLUMN)
    return 0;
  
  
  if(pFontDescriptor->pack & D4D_FONT_PACK_COMPRESSED_ON) {
    // data are packed in bit stream
    bit_cnt = (Word)(char_width * row + clm);
    data = pFntData[bit_cnt / 8];
  }else 
  {
    // data aren't packed
    Word byte_cnt; 
    byte_cnt = (Word)((((char_width - 1) / 8) + 1) * row);  // get the start of row index of data
    byte_cnt += (clm / 8);  // add the collumn index move
    data = pFntData[byte_cnt];  // get the data byte
    bit_cnt = (Byte)(clm % 8); // get position of bit in byte for this pixel
  }
  
  if(pFontDescriptor->pack & D4D_FONT_PACK_BITORDER_LITTLEEND)
    return (D4D_BOOL)((data >> (bit_cnt % 8)) & 0x01);   // little endian decoding
  else
    return (D4D_BOOL)((data << (bit_cnt % 8)) & 0x80);   // big endian decoding
}

D4D_COOR D4D_GetNextTab(D4D_TAB* pTab, D4D_COOR pos)
{
  D4D_INDEX ix = 0;

  if(pTab == NULL)
    return 0;
  
  if(pTab->pTabTable == NULL)
    return 0;

  while(pTab->pTabTable[ix])
  {
    if((pos + pTab->tabOffset) < pTab->pTabTable[ix])
      return (D4D_COOR)(pTab->pTabTable[ix] /*- pTab->tabOffset*/);
    
    ix++;
  }

  return 0;

}

/**************************************************************//*!
*
* Print the Char in ASCII in simple format up to 8 columns
*
******************************************************************/
static D4D_COOR D4D_LCD_PrintChr(D4D_CHAR ch, D4D_PRINT_STRING* p_CharDes)
{
  D4D_FONT_SIZE xScale, yScale;               
  LWord row_cnt;
  LWord bit_cnt;
  LWord px_cnt;
  LWord yScale_cnt;
  LWord char_width;
  D4D_FONT_IX ch_ix;
  D4D_COLOR clr;
  D4D_FONT_DATA* pFntData;
  D4D_FONT_TYPE* pFontType = (D4D_FONT_TYPE*)p_CharDes->pFontType;
  D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);
  
  if(ch == '\t')
  {
    D4D_COOR nextTab;
    // print tabulator
    nextTab = D4D_GetNextTab(p_CharDes->pTab, p_CharDes->x);
    
    if(nextTab)
    {
      D4D_LCD_Box(p_CharDes->x, p_CharDes->y, nextTab,\
                     (D4D_COOR)(p_CharDes->y + D4D_GetFontHeight(p_CharDes->pFontType->ix_font) - 1),\
                     p_CharDes->colorBack);
      return (D4D_COOR)(nextTab - p_CharDes->x);
    }
  }
  
  xScale = p_CharDes->pFontType->scale.width;
	yScale = p_CharDes->pFontType->scale.height;

  ch_ix = D4D_GetCharIndex(pFontType, ch);
	pFntData = D4D_GetCharData( pFontType, ch);	 // Get pointer to char bmp data
	
	if(pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL)  
    char_width = pFontDescriptor->pSizeTable[ch_ix];
  else
    char_width = pFontDescriptor->charFullSize.width;
	
	if(p_CharDes->properties.bits.bTransparent)
	{
		for (row_cnt=0; row_cnt<pFontDescriptor->charFullSize.height; ++row_cnt)			// For each vertical row of this character  
  	{
  	  for(bit_cnt = 0; bit_cnt < char_width; bit_cnt++)
      {
		    if(D4D_LCD_GetFntBit((Byte)row_cnt, (Byte)bit_cnt, (Byte)char_width, pFntData, pFontDescriptor))
        {
          D4D_LCD_Box((D4D_COOR)(p_CharDes->x + pFontType->charSpacing.width + bit_cnt * xScale),\
                     (D4D_COOR)(p_CharDes->y + pFontType->charSpacing.height + (row_cnt * yScale)),\
                     (D4D_COOR)(p_CharDes->x + pFontType->charSpacing.width + bit_cnt * xScale + xScale - 1),\
                     (D4D_COOR)(p_CharDes->y + pFontType->charSpacing.height + (row_cnt * yScale) + yScale - 1),\
                     p_CharDes->colorText);
        } 
			}									     
  	}											  
  } 
  else  // if(Transparent)
	{
    (void)D4D_LLD_LCD.D4DLCD_SetWindow(p_CharDes->x, p_CharDes->y, 
               (D4D_COOR)(p_CharDes->x + D4D_GetCharWidth(p_CharDes->pFontType->ix_font, ch) - 1), 
               (D4D_COOR)(p_CharDes->y + D4D_GetFontHeight(p_CharDes->pFontType->ix_font) + 1)); // Set character window

    // add on background box on bottom of character to provide line spacing
		//char_width = D4D_GetCharWidth(p_CharDes->pFontType->ix_font, ch);
		
		for(row_cnt=0; row_cnt < pFontType->charSpacing.height; ++row_cnt) 
		{
		  for(bit_cnt = 0; bit_cnt < (D4D_COOR)(char_width * xScale + pFontType->charSpacing.width); ++bit_cnt)
		  	(void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(p_CharDes->colorBack);	  
		}


    for (row_cnt=0; row_cnt < pFontDescriptor->charFullSize.height; ++row_cnt)			// For each vertical row of this character  
    {
	    for (yScale_cnt=1; yScale_cnt<=yScale; ++yScale_cnt)		// As often as needed by the Y-scale factor (e.g. draws complete line twice if double height font is requested)  
			{
        for (bit_cnt=1; bit_cnt<=pFontType->charSpacing.width; ++bit_cnt)     
				{
					// empty column = always background color
					(void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(p_CharDes->colorBack);								// draw pixel
				}
				
				for(bit_cnt = 0; bit_cnt < char_width; ++bit_cnt)
        {
		      if(D4D_LCD_GetFntBit((Byte)row_cnt, (Byte)bit_cnt, (Byte)char_width, pFntData, pFontDescriptor))
 					  clr = p_CharDes->colorText;
 					else
 					  clr = p_CharDes->colorBack;
 					
 					for (px_cnt=1; px_cnt<=xScale; ++px_cnt)			// as often as needed by the horizotal scale factor (e.g. draws pixel twice if double wide font is requested)  
    				(void)D4D_LLD_LCD.D4DLCD_Send_PixelColor(clr);			// draw pixel
				}
  	  }											// draw this line again if needed for scaling  
		}	 	
	}// next row of this character
	
	return (D4D_COOR)(char_width * xScale + pFontType->charSpacing.width);
}

/**************************************************************//*!
*
* Print the String in ASCII in simple format up to 8 columns
*
******************************************************************/
void D4D_LCD_PrintStr(D4D_PRINT_STRING* p_StrDes)
{
  D4D_INDEX str_len, ch_ix;
  
  D4D_COOR orig_x = p_StrDes->x;
  D4D_COOR base_y, base_x;
  D4D_FONT_TYPE* pFontType = (D4D_FONT_TYPE*)p_StrDes->pFontType;
  D4D_FONT_DESCRIPTOR* pFontDescriptor = (D4D_FONT_DESCRIPTOR*)(pFontType->pFontDescriptor);	
  D4D_CHAR* pText;
  // check if fontdescriptor exist and check the font scale value
  if((pFontDescriptor == NULL) || (pFontType->scale.width == 0) || (pFontType->scale.height == 0))
      return;      
  
#if D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE    

  if(*(p_StrDes->pText) == d4d_extsrcLocale.strTranslatePrefix)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = p_StrDes->textOffset;
    D4D_INDEX charCntTmp; 
    
    do
    {
      charOff += charCnt;
      
      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(p_StrDes->pText, (char*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);
    
        // get the lenght of string 
      str_len = charCnt;     

      if(p_StrDes->textLength)
        if(str_len > p_StrDes->textLength)
          str_len = p_StrDes->textLength;
    
      str_len += (charOff - 1);
    
      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if((!(D4D_CHAR)d4d_extsrcBuffer[charCntTmp]) || (str_len < (charCntTmp + charOff)))
        {
          charCnt = 0;
          break;
        }
        
        p_StrDes->x += D4D_LCD_PrintChr((D4D_CHAR)d4d_extsrcBuffer[charCntTmp], p_StrDes); // print the char and automatically update the coordintaion of next char  
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
    
  
  }else
  {
    pText = p_StrDes->pText;
		
		pText = (D4D_CHAR*)((LWord)pText + p_StrDes->textOffset);
				
    // get the lenght of string 
    str_len = D4D_GetTextLength(p_StrDes->pText);     

    if(p_StrDes->textLength)
      if(str_len > p_StrDes->textLength)
        str_len = p_StrDes->textLength;
    
    // Draw the text
    for (ch_ix=0; ch_ix<str_len; ++ch_ix)	// For each character in the string 	  
      p_StrDes->x += D4D_LCD_PrintChr(*pText++, p_StrDes); // print the char and automatically update the coordintaion of next char 
  }


#else      

  pText = p_StrDes->pText;
		
	pText = (D4D_CHAR*)((LWord)pText + p_StrDes->textOffset);
				
  // get the lenght of string 
  str_len = D4D_GetTextLength(p_StrDes->pText);     

  if(p_StrDes->textLength)
    if(str_len > p_StrDes->textLength)
      str_len = p_StrDes->textLength;
  
  
  // Draw the text
  for (ch_ix=0; ch_ix<str_len; ++ch_ix)	// For each character in the string 	  
    p_StrDes->x += D4D_LCD_PrintChr(*pText++, p_StrDes); // print the char and automatically update the coordintaion of next char 

#endif
	
	// Draw the Underline option	
	switch(p_StrDes->properties.bits.bUnderLine)
	{
	  case D4D_FNT_PRTY_UNDERLINE_LINE:  // Draw the full line
	    base_y = (D4D_COOR)(p_StrDes->y + pFontDescriptor->charBaseLine * pFontType->scale.height);
	    base_x = p_StrDes->x;
	    D4D_FillRectXY(orig_x, base_y, (D4D_COOR)(base_x - 1), (D4D_COOR)(base_y + pFontType->scale.height - 1), p_StrDes->colorText);
	    break;
	    
	  case D4D_FNT_PRTY_UNDERLINE_DOT:  // Draw the dot line
	    base_y = (D4D_COOR)(p_StrDes->y + pFontDescriptor->charBaseLine * pFontType->scale.height);
	    
	    ch_ix = 0;
	    for(base_x = orig_x; base_x < p_StrDes->x; base_x += pFontType->scale.height)
	    {
	      if(++ch_ix % 2)
	        D4D_FillRectXY(base_x, base_y, (D4D_COOR)(base_x + pFontType->scale.width - 1), (D4D_COOR)(base_y + pFontType->scale.height - 1), p_StrDes->colorText);	    
	    }
	    break;
	  default:
	    break;	    
	}
	
	// Draw the Strike Through option	
	if(p_StrDes->properties.bits.bStrikeThrough)
	{
	    Byte line_cnt = 0;
	    base_y = (D4D_COOR)(p_StrDes->y + (pFontDescriptor->charBaseLine - (pFontDescriptor->charSize / 3)) * pFontType->scale.height);
	    base_x = p_StrDes->x;
	    // Draw Strike Through line as many as specified by parameter
	    while(line_cnt++ < p_StrDes->properties.bits.bStrikeThrough)
	    {	      
	      D4D_FillRectXY(orig_x, base_y, (D4D_COOR)(base_x - 1), (D4D_COOR)(base_y - 1), p_StrDes->colorText);
	      base_y -=2;
	    }
	      
	}
	
	D4D_LLD_LCD.D4DLCD_FlushBuffer();
				
}





