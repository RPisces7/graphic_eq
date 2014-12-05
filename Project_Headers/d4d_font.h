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
* @file      d4d_font.h
*
* @author    b01119
* 
* @version   0.0.26.0
* 
* @date      Feb-28-2011
* 
* @brief     D4D Driver fonts header file
*
*******************************************************************************/

#ifndef __D4D_FONT_H
#define __D4D_FONT_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/

#define D4D_FONT_PACK_BITORDER 0x01
#define D4D_FONT_PACK_BITORDER_BIGEND 0
#define D4D_FONT_PACK_BITORDER_LITTLEEND 0x01

#define D4D_FONT_PACK_SCANBASED 0x02
#define D4D_FONT_PACK_SCANBASED_ROW 0
#define D4D_FONT_PACK_SCANBASED_COLUMN 0x02

#define D4D_FONT_PACK_SCANPREFERRED 0x04
#define D4D_FONT_PACK_SCANPREFERRED_ROW 0
#define D4D_FONT_PACK_SCANPREFERRED_COLUMN 0x04

#define D4D_FONT_PACK_COMPRESSED 0x08
#define D4D_FONT_PACK_COMPRESSED_OFF 0
#define D4D_FONT_PACK_COMPRESSED_ON 0x08

#define D4D_FONT_PACK_DATA_LEN 0x030
#define D4D_FONT_PACK_DATA_LEN_8B 0x00
#define D4D_FONT_PACK_DATA_LEN_16B 0x10
#define D4D_FONT_PACK_DATA_LEN_32B 0x20

#define D4D_FONT_FLAGS_IX_STYLE 0x01
#define D4D_FONT_FLAGS_IX_STYLE_LINEAR 0x00
#define D4D_FONT_FLAGS_IX_STYLE_NONLINEAR 0x01

#define D4D_FONT_FLAGS_IX_TYPE 0x02
#define D4D_FONT_FLAGS_IX_TYPE_LOOKUP 0x00
#define D4D_FONT_FLAGS_IX_TYPE_MAP 0x02

#define D4D_FONT_FLAGS_FNT_WIDTH 0x04
#define D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE 0x00
#define D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL 0x04


#define D4D_FNT_PRTY_UNDERLINE_NONE         (0x00)
#define D4D_FNT_PRTY_UNDERLINE_LINE         (0x01)
#define D4D_FNT_PRTY_UNDERLINE_DOT          (0x02)
#define D4D_FNT_PRTY_UNDERLINE_RES          (0x03)

#define D4D_FNT_PRTY_STRIKETHROUGH_NONE     (0x00)
#define D4D_FNT_PRTY_STRIKETHROUGH_SINGLE   (0x01)
#define D4D_FNT_PRTY_STRIKETHROUGH_DOUBLE   (0x02)
#define D4D_FNT_PRTY_STRIKETHROUGH_TRIPLE   (0x03)

#define D4D_FNT_PRTY_TRANSPARENT_NO         (0x00)
#define D4D_FNT_PRTY_TRANSPARENT_YES        (0x01)


#if D4D_BITFIELD_LSB_ALIGNMENT == D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4D_FNT_PRTY_UNDERLINE_MASK              (0x03)
  #define D4D_FNT_PRTY_UNDERLINE_NONE_MASK         (0x00)
  #define D4D_FNT_PRTY_UNDERLINE_LINE_MASK         (0x01)
  #define D4D_FNT_PRTY_UNDERLINE_DOT_MASK          (0x02)
  #define D4D_FNT_PRTY_UNDERLINE_RES_MASK          (0x03)
  
  #define D4D_FNT_PRTY_STRIKETHROUGH_MASK          (0x0C)
  #define D4D_FNT_PRTY_STRIKETHROUGH_NONE_MASK     (0x00)
  #define D4D_FNT_PRTY_STRIKETHROUGH_SINGLE_MASK   (0x04)
  #define D4D_FNT_PRTY_STRIKETHROUGH_DOUBLE_MASK   (0x08)
  #define D4D_FNT_PRTY_STRIKETHROUGH_TRIPLE_MASK   (0x0C)
  
  #define D4D_FNT_PRTY_TRANSPARENT_MASK            (0x10)
  #define D4D_FNT_PRTY_TRANSPARENT_NO_MASK         (0x00)
  #define D4D_FNT_PRTY_TRANSPARENT_YES_MASK        (0x10)
  
#else
  #define D4D_FNT_PRTY_UNDERLINE_MASK              (0xC0)
  #define D4D_FNT_PRTY_UNDERLINE_NONE_MASK         (0x00)
  #define D4D_FNT_PRTY_UNDERLINE_LINE_MASK         (0x40)
  #define D4D_FNT_PRTY_UNDERLINE_DOT_MASK          (0x80)
  #define D4D_FNT_PRTY_UNDERLINE_RES_MASK          (0xC0)
  
  #define D4D_FNT_PRTY_STRIKETHROUGH_MASK          (0x30)
  #define D4D_FNT_PRTY_STRIKETHROUGH_NONE_MASK     (0x00)
  #define D4D_FNT_PRTY_STRIKETHROUGH_SINGLE_MASK   (0x10)
  #define D4D_FNT_PRTY_STRIKETHROUGH_DOUBLE_MASK   (0x20)
  #define D4D_FNT_PRTY_STRIKETHROUGH_TRIPLE_MASK   (0x30)
  
  #define D4D_FNT_PRTY_TRANSPARENT_MASK            (0x08)
  #define D4D_FNT_PRTY_TRANSPARENT_NO_MASK         (0x00)
  #define D4D_FNT_PRTY_TRANSPARENT_YES_MASK        (0x08)
#endif  


typedef union 
{
    D4D_BIT_FIELD all;
    
    struct
    {
        unsigned bUnderLine     :2;   // font underline
        unsigned bStrikeThrough :2;   // strike through the text
        unsigned bTransparent   :1;   // trasparent text flag 
        unsigned bReserve0      :1;   // Reserved bit
        unsigned bReserve1      :1;   // Reserved bit
        unsigned bReserve2      :1;   // Reserved bit        
    } bits;
} D4D_FONT_PROPERTIES;

#define D4D_TXT_PRTY_ALIGN_H_LEFT           (0x00)
#define D4D_TXT_PRTY_ALIGN_H_RIGHT          (0x01)
#define D4D_TXT_PRTY_ALIGN_H_CENTER         (0x02)
  
#define D4D_TXT_PRTY_ALIGN_V_TOP            (0x00)
#define D4D_TXT_PRTY_ALIGN_V_BOTTOM         (0x01)
#define D4D_TXT_PRTY_ALIGN_V_CENTER         (0x02)

#if D4D_BITFIELD_LSB_ALIGNMENT == D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4D_TXT_PRTY_ALIGN_H_MASK                (0x03)
  #define D4D_TXT_PRTY_ALIGN_H_LEFT_MASK           (0x00)
  #define D4D_TXT_PRTY_ALIGN_H_RIGHT_MASK          (0x01)
  #define D4D_TXT_PRTY_ALIGN_H_CENTER_MASK         (0x02)
  
  #define D4D_TXT_PRTY_ALIGN_V_MASK                (0x0C)
  #define D4D_TXT_PRTY_ALIGN_V_TOP_MASK            (0x00)
  #define D4D_TXT_PRTY_ALIGN_V_BOTTOM_MASK         (0x04)
  #define D4D_TXT_PRTY_ALIGN_V_CENTER_MASK         (0x08)
    
#else
  #define D4D_TXT_PRTY_ALIGN_H_MASK                (0xc0)
  #define D4D_TXT_PRTY_ALIGN_H_LEFT_MASK           (0x00)
  #define D4D_TXT_PRTY_ALIGN_H_RIGHT_MASK          (0x40)
  #define D4D_TXT_PRTY_ALIGN_H_CENTER_MASK         (0x80)

  #define D4D_TXT_PRTY_ALIGN_V_MASK                (0x30)
  #define D4D_TXT_PRTY_ALIGN_V_TOP_MASK            (0x00)
  #define D4D_TXT_PRTY_ALIGN_V_BOTTOM_MASK         (0x10)
  #define D4D_TXT_PRTY_ALIGN_V_CENTER_MASK         (0x20)
  
#endif

  
typedef union 
{
    D4D_BIT_FIELD all;
    
    struct
    {
        unsigned bAlignHoriz     :2;   // String horizontal alignment in text box
        unsigned bAlignVertic    :2;   // String vertical alignment in text box
        unsigned bReserverd     :4;   // Reserved bits        
    } bits;
} D4D_TEXT_PROPERTIES;



typedef Byte D4D_FONT_PACK;
typedef Byte D4D_FONT_FLAGS;
typedef Byte D4D_FONT_IX;
typedef Word D4D_FONT_OFFSET;
typedef Byte D4D_FONT_REV;
typedef Byte D4D_FONT_DATA;
typedef Byte D4D_FONT;


typedef Byte D4D_FONT_SIZE;

typedef struct
{
  D4D_FONT_SIZE width;
  D4D_FONT_SIZE height;    
}D4D_FONT_SIZES;



typedef struct{
  D4D_FONT_REV revision;       // Font descriptor version number
  D4D_FONT_FLAGS flags;        // linear / non linear , proporcional or not, 
  D4D_FONT_IX startChar;       // start char of used table
  D4D_FONT_IX charCnt;         // end char of used table
  D4D_FONT_IX charDefault;     // index of char that will be printed instead of 
  D4D_FONT_SIZE charSize;      // Font size (size of font loaded in PC)
  D4D_FONT_SIZE charBmpSize;   // Size of font bitmpap for non proportional fonts
  D4D_FONT_SIZE charBaseLine;  // offset from Y0 coordinate to baseline
  D4D_FONT_SIZES charFullSize; // size of biggest char x/y
  D4D_FONT_PACK pack;          // packing condition of individual bitmaps
  D4D_FONT_IX *pIxTable;       // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
  D4D_FONT_OFFSET *pOffTable;  // Offset table - used when proporcial font is set in flags
  D4D_FONT_SIZE *pSizeTable;   // Size table - used when proporcial font is set in flags
  const D4D_FONT_DATA *pFontData; // bitmap/font data array
}D4D_FONT_DESCRIPTOR;

typedef struct
{
  const D4D_FONT  ix_font;
  const D4D_FONT_DESCRIPTOR* pFontDescriptor;
  const D4D_FONT_SIZES scale;           // the scale of the font in both axes values must be > 0
  const D4D_FONT_SIZES charSpacing;     // increment for finest cell in x/y to provide line/char spacing
  const char* fileName;
}D4D_FONT_TYPE;

typedef struct D4D_STR_PROPERTIES_S
{
  D4D_FONT_PROPERTIES font_properties;
  D4D_TEXT_PROPERTIES text_properties;
}D4D_STR_PROPERTIES;

typedef struct D4D_STRING_S 
{
  char *pText;
  D4D_INDEX buffSize;
  D4D_FONT fontId;
  D4D_STR_PROPERTIES *str_properties;
  D4D_INDEX printLen;
  D4D_INDEX printOff;
}D4D_STRING;

typedef struct
{
  D4D_COOR* pTabTable;
  D4D_COOR tabOffset;
}D4D_TAB;

typedef struct{  
  D4D_COOR x; 
  D4D_COOR y;   
  D4D_CHAR* pText;
  D4D_FONT_TYPE* pFontType;
  D4D_TAB* pTab;
  D4D_COLOR colorText; 
  D4D_COLOR colorBack; 
  D4D_FONT_PROPERTIES properties;
  D4D_INDEX textLength;
  D4D_INDEX textOffset;
}D4D_PRINT_STRING;




/******************************************************************************
* Macros 
******************************************************************************/

#define D4D_DECLARE_FONT_TABLE_BEGIN  const D4D_FONT_TYPE d4d_FontTable[] = {

#define D4D_DECLARE_FONT(fontId, font_descriptor, xScale, yScale, charSpace, lineSpace) \
{fontId, &(font_descriptor), { xScale, yScale }, { charSpace, lineSpace }, NULL}, // font info will be replaced by font descriptor

#define D4D_DECLARE_FONT_FILE(fontId, fileName, xScale, yScale, charSpace, lineSpace) \
{fontId, NULL, { xScale, yScale }, { charSpace, lineSpace }, fileName}, // font info will be replaced by font descriptor


#define D4D_DECLARE_FONT_TABLE_END {0, NULL, {0, 0}, {0, 0}, NULL } };


#define D4D_DECLARE_TAB_TABLE_BEGIN(name)  const D4D_COOR name[] = {

#define D4D_DECLARE_TAB(tab) tab, // tab space in pixels

#define D4D_DECLARE_TAB_TABLE_END 0 };


/******************************************************************************
* Global variables
******************************************************************************/

extern const D4D_FONT_TYPE d4d_FontTable[];


/******************************************************************************
* Global functions
******************************************************************************/


D4D_FONT_TYPE* D4D_GetFont(D4D_FONT ix);
D4D_FONT_SIZES D4D_GetFontSize(D4D_FONT ix);
D4D_FONT_SIZE D4D_GetFontHeight(D4D_FONT ix);
D4D_FONT_SIZE D4D_GetFontWidth(D4D_FONT ix);
D4D_FONT_SIZE D4D_GetCharWidth(D4D_FONT ix, char ch);
D4D_FONT_IX D4D_GetCharIndex(D4D_FONT_TYPE* pFontType, char ch);
D4D_CHAR D4D_GetChar(D4D_FONT_TYPE* pFontType, D4D_FONT_IX ix);
D4D_FONT_DATA* D4D_GetCharData(D4D_FONT_TYPE* pFontType, char ch);
D4D_COOR D4D_GetNextTab(D4D_TAB* pTab, D4D_COOR pos);
void D4D_LCD_PrintStr(D4D_PRINT_STRING* p_StrDes);

#endif  /* __D4D_FONT_H */


