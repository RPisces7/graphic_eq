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
* @file      d4d_console.c
*
* @author    b01119
* 
* @version   0.0.13.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver console object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"


// Internal API
void D4D_ConsoleOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_ConsoleCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_ConsoleGetTextBuffer(D4D_OBJECT* pThis);


static void D4D_CnslNewLine(D4D_OBJECT_PTR pObj, D4D_BOOL returnCarrier);
static D4D_BOOL D4D_CnslPutCharInt(D4D_OBJECT_PTR pObj, D4D_CHAR ch);
static void D4D_CnslUpdateScrollbars(D4D_OBJECT* pThis);
static void SendLocalMessage(D4D_MESSAGE* pMsg, D4D_OBJECT* pObj);
static void D4D_CnslOnInit(D4D_MESSAGE* pMsg);
static void D4D_ConsoleTimeTick(D4D_MESSAGE* pMsg);
static D4D_INDEX D4D_CnslFindMaxTextLenght(D4D_CHAR* pText, D4D_FONT font, D4D_INDEX startOffset, D4D_INDEX maxLenght);

const D4D_OBJECT_SYS_FUNCTION d4d_consoleSysFunc = 
{
  D4D_OBJECT_CONSOLE,
  D4D_ConsoleOnMessage,
  D4D_ConsoleCheckCoordinates,
  D4D_ConsoleGetTextBuffer
};

typedef struct 
{
  D4D_POINT position;
  D4D_SIZE size;
  D4D_POINT txtPos;
  D4D_SIZE txtSize;
  D4D_SIZE lineSize;
  D4D_SIZE charArrSize;
  D4D_COOR headerHeight;
} D4D_CNSL_TMP_VAL;

#define _calc (*((D4D_CNSL_TMP_VAL*)d4d_scratchPad))
//static D4D_CNSL_TMP_VAL _calc;
    
static void D4D_CnslValue2Coor(D4D_OBJECT* pThis)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pThis);  
  
  D4D_SIZE scrSize = D4D_GetClientScreenSize(*(pThis->pScreen));
  D4D_BOOL xSizeMax = D4D_TRUE;
  D4D_BOOL ySizeMax = D4D_TRUE;
  _calc.headerHeight = 0;
  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pCnsl->scrPos);
  _calc.size = pCnsl->scrSize;
   
  if(pCnsl->pBmpIcon) 
  {
    _calc.headerHeight = D4D_GetBmpHeight(pCnsl->pBmpIcon);  
  }
  
  if(pCnsl->textBuff.pText) 
  {
    if(D4D_GetFontHeight(pCnsl->textBuff.fontId) > _calc.headerHeight)
      _calc.headerHeight = D4D_GetFontHeight(pCnsl->textBuff.fontId); 
  }
  
  if(_calc.headerHeight)
    _calc.headerHeight += 2;
  else
    _calc.headerHeight = 1;
  
  
   
 #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
      
      if(!_calc.size.cx)
      {
        // compute size of console object automaticcaly
        _calc.size.cx = (D4D_COOR) (D4D_GetFontWidth(pCnsl->textFontId) * (pCnsl->txtArrSize.cx) + 4);
        
        // check if the object size is smaller than screen size
        if((pCnsl->scrPos.x + _calc.size.cx) > scrSize.cx)
        {
          _calc.size.cx = (D4D_COOR)(scrSize.cx - pCnsl->scrPos.x);
        }else
        {
          xSizeMax = D4D_FALSE;
        }        
      }
      
      if(!_calc.size.cy)
      {
        // compute size of console object automaticcaly
        _calc.size.cy = (D4D_COOR) (D4D_GetFontHeight(pCnsl->textFontId) *  pCnsl->txtArrSize.cy + _calc.headerHeight + 2);
        
                // check if the object size is smaller than screen size
        if((pCnsl->scrPos.y + _calc.size.cy) > scrSize.cy)
        {
          _calc.size.cy = (D4D_COOR)(scrSize.cy - pCnsl->scrPos.y);
        }else
        {
          ySizeMax = D4D_FALSE;
        }
                
      }
      
 #endif
 
  _calc.txtPos = _calc.position;
  
  _calc.txtPos.y += (D4D_COOR)(_calc.headerHeight);
  _calc.txtPos.x += 2;
  
  _calc.txtSize = _calc.size;
  _calc.txtSize.cx -= 4;
  _calc.txtSize.cy -= (D4D_COOR)(_calc.headerHeight + 1);
 
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  _calc.txtPos.y += (D4D_COOR)(pCnsl->radius / 2);
  _calc.txtPos.x += (D4D_COOR)(pCnsl->radius / 2);
    
  _calc.txtSize.cx -= (D4D_COOR)(pCnsl->radius / 2);
  _calc.txtSize.cy -= (D4D_COOR)(pCnsl->radius / 2);
#endif 
  
  _calc.charArrSize.cx = (D4D_COOR) (_calc.txtSize.cx / D4D_GetFontWidth(pCnsl->textFontId));
  
  
  // check if the 

  if(_calc.charArrSize.cx < pCnsl->txtArrSize.cx)
  {
    if(ySizeMax == D4D_FALSE)
    {
      if((pCnsl->scrPos.y + _calc.size.cy + D4D_CNSL_SCRLBR_WIDTH) <= scrSize.cy)
        _calc.size.cy += D4D_CNSL_SCRLBR_WIDTH;
      else
      {
        _calc.size.cy = (D4D_COOR)(scrSize.cy - pCnsl->scrPos.y);
        ySizeMax = D4D_TRUE;
      }
      
      _calc.txtSize.cy = (D4D_COOR)(_calc.size.cy - (_calc.headerHeight + 1 + D4D_CNSL_SCRLBR_WIDTH));
      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE      
      _calc.txtSize.cy -= (D4D_COOR)(pCnsl->radius / 2);
      #endif
    }else
      _calc.txtSize.cy -= D4D_CNSL_SCRLBR_WIDTH;    
    
  }else
  {
    _calc.charArrSize.cx = pCnsl->txtArrSize.cx;  
  }
  
  _calc.charArrSize.cy = (D4D_COOR) (_calc.txtSize.cy / D4D_GetFontHeight(pCnsl->textFontId));
  
  if(_calc.charArrSize.cy < pCnsl->txtArrSize.cy)
  {
    if(xSizeMax == D4D_FALSE)
    {
      if((pCnsl->scrPos.x + _calc.size.cx + D4D_CNSL_SCRLBR_WIDTH) <= scrSize.cx)
        _calc.size.cx += D4D_CNSL_SCRLBR_WIDTH;
      else
      {
        _calc.size.cx = (D4D_COOR)(scrSize.cx - pCnsl->scrPos.x);        
        ySizeMax = D4D_TRUE;
        
        if(ySizeMax == D4D_FALSE)
        {
          if((pCnsl->scrPos.y + _calc.size.cy + D4D_CNSL_SCRLBR_WIDTH) <= scrSize.cy)
            _calc.size.cy += D4D_CNSL_SCRLBR_WIDTH;
          else
          {
            _calc.size.cy = (D4D_COOR)(scrSize.cy - pCnsl->scrPos.y);
            ySizeMax = D4D_TRUE;
          }
          
          _calc.txtSize.cy = (D4D_COOR)(_calc.size.cy - (_calc.headerHeight + 1 + D4D_CNSL_SCRLBR_WIDTH));
          
          #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE      
          _calc.txtSize.cy -= (D4D_COOR)(pCnsl->radius / 2);
          #endif
        }else
          _calc.txtSize.cy -= D4D_CNSL_SCRLBR_WIDTH;
        
        _calc.charArrSize.cy = (D4D_COOR) (_calc.txtSize.cy / D4D_GetFontHeight(pCnsl->textFontId));
        
      }
      
      _calc.txtSize.cx = (D4D_COOR)(_calc.size.cx - 4 - D4D_CNSL_SCRLBR_WIDTH);
      
      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE      
      _calc.txtSize.cx -= (D4D_COOR)(pCnsl->radius / 2);
      #endif
    }else
      _calc.txtSize.cx -= D4D_CNSL_SCRLBR_WIDTH;
    
    _calc.charArrSize.cx = (D4D_COOR) (_calc.txtSize.cx / D4D_GetFontWidth(pCnsl->textFontId));
  }else
  {
    _calc.charArrSize.cy = pCnsl->txtArrSize.cy;  
  }

 
}
 
static D4D_INDEX D4D_CnslFindMaxTextLenght(D4D_CHAR* pText, D4D_FONT font, D4D_INDEX startOffset, D4D_INDEX maxLenght)
{
  D4D_INDEX result = _calc.charArrSize.cx;
  D4D_COOR tmp_textWidth; 
  D4D_CHAR tmp_char;
  
  // Get the 
  tmp_char = pText[result];
  pText[result] = 0;
  tmp_textWidth = D4D_GetTextWidth(font, pText);
  pText[result] = tmp_char;
  
  
  while((pText[result] != 0) && ((result + startOffset) < maxLenght))
  {    
    if(tmp_textWidth >= (_calc.txtSize.cx - 4))
    {
      result--;
      break;
    }
    
    result++;
    tmp_textWidth += D4D_GetCharWidth(font, pText[result]);
  }
  
  return result;
}


/*******************************************************
*
* CONSOLE Drawing routine
*
*******************************************************/

static void D4D_ConsoleOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pMsg->pObject);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_INDEX line_cnt, tmp_realLine;
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size, tmp_winOff;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_strPrties;
  D4D_BOOL tmp_font_proporcional = D4D_FALSE;
  
  
  D4D_CnslValue2Coor(pMsg->pObject);
      
  clrT = D4D_ObjectGetForeFillColor(pMsg->pObject);
  clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);  
  
  #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  
  // draw the rectangle around the text
  if(draw.bits.bComplete)  
  {
    D4D_FillRRect(&_calc.position, &_calc.size, clrB, pCnsl->radius);
    
    if(pCnsl->pBmpIcon) {
      // Draw console icon
      D4D_DrawRBmpXY((D4D_COOR)(_calc.position.x + 1), (D4D_COOR)(_calc.position.y + 1 + (_calc.headerHeight - D4D_GetBmpHeight(pCnsl->pBmpIcon))/2), pCnsl->pBmpIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pCnsl->radius);
    }
    
    if(pCnsl->textBuff.pText)
    {
      // draw the title text
      D4D_COOR tmp_x = _calc.position.x;
      
      tmp_x++;
      
      tmp_x += pCnsl->radius;
      
      if(pCnsl->pBmpIcon)
        tmp_x += (D4D_COOR)(D4D_GetBmpWidth(pCnsl->pBmpIcon) + 1);
      
      
      D4D_MoveToXY(tmp_x, (D4D_COOR)(_calc.position.y + 1));
        
      D4D_DrawTextRectToXY((D4D_COOR)(_calc.position.x + _calc.size.cx - 1), (D4D_COOR)(_calc.txtPos.y - 2), &pCnsl->textBuff, clrT, clrB);
    }
    
    if((pCnsl->textBuff.pText) || (pCnsl->pBmpIcon)) 
    {
      //Draw underline of title box
      D4D_MoveToXY(_calc.position.x, (D4D_COOR)(_calc.txtPos.y - 1));
      D4D_LineToXY((D4D_COOR)(_calc.position.x + _calc.size.cx) , (D4D_COOR)(_calc.txtPos.y - 1), D4D_LINE_THIN, clrT);
    }
    
    //D4D_FillRect(&_calc.txtPos, &_calc.txtSize, clrB);
    
    pData->flags.bits.bRedrawAll = 1;
    pData->flags.bits.bRedrawLine = 1;
    pData->flags.bits.bRedrawCursor = 1;
    pData->flags.bits.bRedrawScrollBars = 1;    
  }
  
  if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
     D4D_RRect(&_calc.position, &_calc.size, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw), pCnsl->radius);
  
  #else
  
  // draw the rectangle around the text
  if(draw.bits.bComplete)  
  {
    D4D_FillRect(&_calc.position, &_calc.size, clrB);
    
    if(pCnsl->pBmpIcon) {
      // Draw console icon
      D4D_DrawBmpXY((D4D_COOR)(_calc.position.x + 1), (D4D_COOR)(_calc.position.y + 1 + (_calc.headerHeight - D4D_GetBmpHeight(pCnsl->pBmpIcon))/2), pCnsl->pBmpIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
    }
    
    if(pCnsl->textBuff.pText)
    {
      // draw the title text
      D4D_COOR tmp_x = _calc.position.x;
      
      tmp_x++;
      
      if(pCnsl->pBmpIcon)
        tmp_x += (D4D_COOR)(D4D_GetBmpWidth(pCnsl->pBmpIcon) + 1);
      
      
      D4D_MoveToXY(tmp_x, (D4D_COOR)(_calc.position.y + 1));
        
      D4D_DrawTextRectToXY((D4D_COOR)(_calc.position.x + _calc.size.cx - 1), (D4D_COOR)(_calc.txtPos.y - 2), &pCnsl->textBuff, clrT, clrB);
    }
    
    if((pCnsl->textBuff.pText) || (pCnsl->pBmpIcon)) 
    {
      //Draw underline of title box
      D4D_MoveToXY(_calc.position.x, (D4D_COOR)(_calc.txtPos.y - 1));
      D4D_LineToXY((D4D_COOR)(_calc.position.x + _calc.size.cx) , (D4D_COOR)(_calc.txtPos.y - 1), D4D_LINE_THIN, clrT);
    }
    
    //D4D_FillRect(&_calc.txtPos, &_calc.txtSize, clrB);
    
    pData->flags.bits.bRedrawAll = 1;
    pData->flags.bits.bRedrawLine = 1;
    pData->flags.bits.bRedrawCursor = 1;
    pData->flags.bits.bRedrawScrollBars = 1;    
  }
  
  if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
     D4D_Rect(&_calc.position, &_calc.size, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw));
  
  #endif
  
  
  tmp_winOff.cx = (D4D_COOR)D4D_ScrlBrGetPosition(pCnsl->pScrlBrHor);
  tmp_winOff.cy = (D4D_COOR)D4D_ScrlBrGetPosition(pCnsl->pScrlBrVer);
  tmp_strPrties.font_properties.all = 0;
  tmp_strPrties.text_properties.all = 0;
  tmp_txtbuff.fontId = pCnsl->textFontId;    
  tmp_txtbuff.str_properties = &tmp_strPrties;
  
  
  if(pData->flags.bits.bRedrawAll || pData->flags.bits.bRedrawLine)
  {

    
    
    
    
    // Draw all lines of console
    tmp_point = _calc.txtPos;
    tmp_size.cx = _calc.txtSize.cx;
    tmp_size.cy = D4D_GetFontHeight(pCnsl->textFontId);
    
    
    if(D4D_GetFont(pCnsl->textFontId)->pFontDescriptor->flags & D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL)
      tmp_font_proporcional = D4D_TRUE;
    
    for(line_cnt = 0; line_cnt < _calc.charArrSize.cy; line_cnt++) 
    {
      if(!pData->flags.bits.bRedrawAll)
        if(line_cnt != pData->cursorPos.y)
        {
          tmp_point.y += tmp_size.cy;
          continue;
        }
      
      // Compute the real line in memory
      tmp_realLine = (D4D_INDEX)((pData->lastLineIx + line_cnt + tmp_winOff.cy) % pCnsl->txtArrSize.cy);
      
      // found the right pointer of the text to draw in the text array     
      tmp_txtbuff.pText = (D4D_CHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1) + tmp_winOff.cx);
      
      if((tmp_winOff.cx + _calc.charArrSize.cx) <= pCnsl->txtArrSize.cx)
      {      
        if(tmp_font_proporcional)
          tmp_txtbuff.buffSize = D4D_CnslFindMaxTextLenght(tmp_txtbuff.pText, pCnsl->textFontId, tmp_winOff.cx, pCnsl->txtArrSize.cx);
        else  
          tmp_txtbuff.buffSize = _calc.charArrSize.cx;
        
        tmp_txtbuff.buffSize++;
        
      }
      D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB); 
      
      tmp_point.y += tmp_size.cy; 
    }
  }
  
  // redraw cursor
  if(((D4D_INDEX_DELTA)(pData->cursorPos.y - tmp_winOff.cy) >= 0) && ((pData->cursorPos.y - tmp_winOff.cy) < _calc.charArrSize.cy)) //cursor is on screen - check line
  {
    D4D_BOOL cursorVisible = D4D_FALSE;
    
    // Compute the real line in memory
    tmp_realLine = (D4D_INDEX)((pData->lastLineIx + pData->cursorPos.y) % pCnsl->txtArrSize.cy);
    
    // found the right pointer of the text to draw in the text array     
    tmp_txtbuff.pText = (D4D_CHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1) + tmp_winOff.cx);
      
    
    if(tmp_font_proporcional)
      tmp_txtbuff.buffSize = D4D_CnslFindMaxTextLenght(tmp_txtbuff.pText, pCnsl->textFontId, tmp_winOff.cx, pCnsl->txtArrSize.cx);
    else  
      tmp_txtbuff.buffSize = _calc.charArrSize.cx;
    
    tmp_size.cx = D4D_GetCharWidth(pCnsl->textFontId, ' ');
    tmp_point.x = D4D_GetTextBuffWidthTab(&tmp_txtbuff, NULL);
    
    if(tmp_font_proporcional)
    {
      if(tmp_point.x < (D4D_COOR)( _calc.txtSize.cx - tmp_size.cx))
        cursorVisible = D4D_TRUE;
    }else
    {
      if(pData->cursorPos.x < (D4D_INDEX)(_calc.charArrSize.cx + tmp_winOff.cx))
        cursorVisible = D4D_TRUE;
    }
    
    
    if(cursorVisible && (pData->cursorPos.x >= tmp_winOff.cx)) // and check the axis X
    {
      tmp_point.y = (D4D_COOR)((pData->cursorPos.y - tmp_winOff.cy  + 1) * D4D_GetFontHeight(pCnsl->textFontId));
      
      tmp_point.y -= (D4D_CNSL_CURSOR_HEIGHT + 1); 
      tmp_point.y += _calc.txtPos.y;
      
      tmp_size.cy = D4D_CNSL_CURSOR_HEIGHT;
      tmp_point.x += _calc.txtPos.x;
      
      
      
      // check the state of cursor and selected right color
      if(!pData->flags.bits.bCursorState)      
        // draw cursor
        D4D_FillRect(&tmp_point, &tmp_size, clrT);
        else
      {
        // Draw the original char
        tmp_txtbuff.pText = (D4D_CHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1) + pData->cursorPos.x + tmp_winOff.cx); 
        if(tmp_txtbuff.pText[0])
        {          
          tmp_size.cx = 0;
          tmp_size.cy = D4D_CNSL_CURSOR_HEIGHT;          
          tmp_txtbuff.buffSize = 2;
          D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB); 
        }else
        {
          D4D_FillRect(&tmp_point, &tmp_size, clrB);
        }
      }
    }
  }
  
  
    pData->flags.bits.bRedrawAll = 0;
    pData->flags.bits.bRedrawLine = 0;
    pData->flags.bits.bRedrawCursor = 0;
  
}

static void D4D_CnslOnInit(D4D_MESSAGE* pMsg)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pMsg->pObject);
  D4D_SCROLL_BAR* pScrlBr;
  
  D4D_CnslClearAll(pMsg->pObject);    
  
  // init screen pointer of child objects
  *(pCnsl->pScrlBrVer->pScreen) = *(pMsg->pObject->pScreen);
  *(pCnsl->pScrlBrHor->pScreen) = *(pMsg->pObject->pScreen);
  
  
  // Init child objects coordinations 
  D4D_CnslValue2Coor(pMsg->pObject);
  
  pScrlBr = D4D_GET_SCROLL_BAR(pCnsl->pScrlBrVer);
  
  pScrlBr->scrPos.x = (D4D_COOR)(_calc.txtPos.x + _calc.txtSize.cx + 2);
  pScrlBr->scrPos.y = _calc.txtPos.y;
  
  pScrlBr->scrPos = D4D_GetScreenToClientPoint( pMsg->pScreen, &pScrlBr->scrPos);
  
  pScrlBr->scrSize.cx = D4D_CNSL_SCRLBR_WIDTH;
  pScrlBr->scrSize.cy = (D4D_COOR)(_calc.txtSize.cy + 1);
  
  pScrlBr->radius = D4D_LimitU(pCnsl->radius, 0, D4D_CNSL_SCRLBR_WIDTH / 2);
  
  pScrlBr = D4D_GET_SCROLL_BAR(pCnsl->pScrlBrHor);
  
  pScrlBr->scrPos.x = (D4D_COOR)(_calc.txtPos.x - 1);
  pScrlBr->scrPos.y = (D4D_COOR)(_calc.txtPos.y + _calc.txtSize.cy + 1);
        
  pScrlBr->scrPos = D4D_GetScreenToClientPoint( pMsg->pScreen, &pScrlBr->scrPos);      
        
  pScrlBr->scrSize.cx = (D4D_COOR)(_calc.txtSize.cx + 3);
  pScrlBr->scrSize.cy = D4D_CNSL_SCRLBR_WIDTH;
  
  pScrlBr->radius = D4D_LimitU(pCnsl->radius, 0, D4D_CNSL_SCRLBR_WIDTH / 2);
  
  if(_calc.charArrSize.cx < pCnsl->txtArrSize.cx)
  {    
    D4D_ScrlBrSetRange(pCnsl->pScrlBrHor, 0, pCnsl->txtArrSize.cx);
    D4D_ScrlBrSetStep(pCnsl->pScrlBrHor, _calc.charArrSize.cx, D4D_CNSL_SCRLBR_STEP_H);
    pCnsl->pScrlBrHor->flags->bits.bVisible = 1;
  }
  else
    pCnsl->pScrlBrHor->flags->bits.bVisible = 0;
  
  if(_calc.charArrSize.cy < pCnsl->txtArrSize.cy)
  {
    D4D_ScrlBrSetRange(pCnsl->pScrlBrVer, 0, pCnsl->txtArrSize.cy);
    D4D_ScrlBrSetStep(pCnsl->pScrlBrVer, _calc.charArrSize.cy, D4D_CNSL_SCRLBR_STEP_V);
    pCnsl->pScrlBrVer->flags->bits.bVisible = 1;
  }
  else
    pCnsl->pScrlBrVer->flags->bits.bVisible = 0;
}


static void D4D_ConsoleTimeTick(D4D_MESSAGE* pMsg)
{

  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pMsg->pObject);

  pCnsl->pData->tickCounter++;
  if(pCnsl->pData->tickCounter >= D4D_CNSL_CURSOR_BLINK_TICK_COUNTER)
  {
    pCnsl->pData->tickCounter = 0;
    pCnsl->pData->flags.bits.bRedrawCursor = 1;
    pMsg->pObject->flags->bits.bRedraw = 1;
    
    if(pCnsl->pData->flags.bits.bCursorState)
      pCnsl->pData->flags.bits.bCursorState = 0;
    else
      pCnsl->pData->flags.bits.bCursorState = 1;
  }
}

/**************************************************************//*!
*
*
*
*     THE INTERNAL HELP FUNCTIONS
*
*
*
******************************************************************/

static D4D_BOOL D4D_CnslPutCharInt(D4D_OBJECT_PTR pObj, D4D_CHAR ch) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  D4D_INDEX tmp_realLine;
    
  // Compute the real line in memory
  tmp_realLine = (D4D_INDEX)((pData->lastLineIx + pData->cursorPos.y) % pCnsl->txtArrSize.cy);
  
  if(pData->flags.bits.bCheckLine) {
    pData->flags.bits.bCheckLine = 0;
    if(pData->cursorPos.x) {
      D4D_INDEX i;
      D4D_CHAR* pCh;
      
      pCh = (D4D_CHAR*)(pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1));
      
      for(i = 0; i < pData->cursorPos.x; i++) {
        if(*pCh == 0) {
          *pCh = ' ';
        }
        pCh++;
      }
    }
  }
  
  // found the right pointer of the text to put new char in text array  
  *((D4D_CHAR*)((pCnsl->pTxtArr + tmp_realLine * (pCnsl->txtArrSize.cx + 1)) + pData->cursorPos.x)) = ch;
  
  // move cursor on new char position
  pData->cursorPos.x++;
  
    
  // Check if printed char was last on line
  if(pData->cursorPos.x >= pCnsl->txtArrSize.cx) {
    D4D_CnslNewLine(pObj, D4D_TRUE);
    return D4D_TRUE;
  }
  
  return D4D_FALSE;
}


static void SendLocalMessage(D4D_MESSAGE* pMsg, D4D_OBJECT* pObj)
{
  D4D_MESSAGE tmpMsg = *pMsg;

  // don't send the OnDraw/done messages if the objects are invisible
  if((pMsg->nMsgId == D4D_MSG_DRAW) || (pMsg->nMsgId == D4D_MSG_DRAWDONE))
  {
    
    if((pObj->flags->bits.bVisible == 0) || (D4D_GET_CONSOLE_DATA(pMsg->pObject)->flags.bits.bRedrawScrollBars == 0))
    {
      
      return;  
    }
  }    
  
  
  // don't send the touch scren messages if the objects are untoucheble
  if((pMsg->nMsgId == D4D_MSG_TOUCHED) || (pMsg->nMsgId == D4D_MSG_TOUCH_AUTO) || (pMsg->nMsgId == D4D_MSG_UNTOUCHED))
  {  
    
    if(pMsg->pObject->flags->bits.bTouchEnable == 0)
      return;
    
    if(!pObj->pObjFunc->CheckCoordinates(pObj, D4D_GetTouchScreenCoordinates()))
      return;
    
  }
 
  tmpMsg.pObject = pObj;
 
  if(pObj->pObjFunc->OnSysMessage)
    pObj->pObjFunc->OnSysMessage(&tmpMsg);      
}


void D4D_CnslScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_UNUSED(old_position);
  D4D_UNUSED(new_position);
  
  ((D4D_OBJECT*)(pThis->userPointer))->flags->bits.bRedraw = 1;
  
  D4D_GET_CONSOLE_DATA((D4D_OBJECT*)(pThis->userPointer))->flags.bits.bRedrawAll = 1;
  D4D_GET_CONSOLE_DATA((D4D_OBJECT*)(pThis->userPointer))->flags.bits.bRedrawScrollBars = 1;
  
}


static void D4D_CnslUpdateScrollbars(D4D_OBJECT* pThis)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pThis);
  D4D_CNSL_DATA* pData = D4D_GET_CONSOLE_DATA(pThis);
  
  
  if((pCnsl->pScrlBrHor->flags->bits.bVisible == 0) && (pCnsl->pScrlBrVer->flags->bits.bVisible == 0))
    return;
  
  D4D_CnslValue2Coor(pThis);
  
  if(pCnsl->pScrlBrHor->flags->bits.bVisible)
  {
    if(pData->cursorPos.x < _calc.charArrSize.cx)
      D4D_ScrlBrSetPosition((D4D_OBJECT*)pCnsl->pScrlBrHor, 0);
    else
      D4D_ScrlBrSetPosition((D4D_OBJECT*)pCnsl->pScrlBrHor, (D4D_INDEX)(pData->cursorPos.x - _calc.charArrSize.cx / 2));
  }
  
  if(pCnsl->pScrlBrVer->flags->bits.bVisible)
  {
    if(pData->cursorPos.y < _calc.charArrSize.cy)
      D4D_ScrlBrSetPosition((D4D_OBJECT*)pCnsl->pScrlBrVer, 0);
    else
      D4D_ScrlBrSetPosition((D4D_OBJECT*)pCnsl->pScrlBrVer, (D4D_INDEX)(pData->cursorPos.y - _calc.charArrSize.cy + 1));
  }
  
}

/**************************************************************//*!
*
*
*
*     THE EXTERNAL API FUNCTIONS
*
*
*
******************************************************************/


//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslPutChar
// SCOPE:       Console object external API function
// DESCRIPTION: The function puts one char on cursor position and updates cursor
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_CHAR ch - char that will be puts on console
//              
// RETURNS:     D4D_BOOL: D4D_TRUE - cursor was wrapped 
//                        D4D_FALSE - cursor was not wrapped
//-----------------------------------------------------------------------------
D4D_BOOL D4D_CnslPutChar(D4D_OBJECT_PTR pObj, D4D_CHAR ch) {

D4D_INDEX i, j;
D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);  
D4D_CNSL_DATA* pData = D4D_GET_CONSOLE_DATA(pObj);
D4D_BOOL wrap = D4D_FALSE;
  
  if(ch >= ' ') {
    wrap = D4D_CnslPutCharInt(pObj, ch);   //Put New char
  }else {      
    switch(ch) {
      case '\n': // new line
        D4D_CnslNewLine(pObj, D4D_FALSE);
        wrap = D4D_TRUE;
        break;
      
      case '\r': // return carrier
        pData->cursorPos.x = 0;
        break;
      
      case '\t': // Tab
        j = (D4D_INDEX)(pData->cursorPos.x % pCnsl->tabSize);
        
        if(!j)
          j = pCnsl->tabSize;
        
        for(i = 0; i < j; j++)
          if(D4D_CnslPutCharInt(pObj, ' '))   // if the tab go to new line end operation
          {
            wrap = D4D_TRUE;
            break;
          }
        break;
    }
  }
  
  D4D_CnslUpdateScrollbars((D4D_OBJECT*)pObj);
  pObj->flags->bits.bRedraw = 1;
  pData->flags.bits.bRedrawLine = 1;
  return wrap;
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslPutString
// SCOPE:       Console object external API function
// DESCRIPTION: The function puts string on cursor position and updates cursor
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_CHAR* pText - string that will be puts on console
//              
// RETURNS:     D4D_BOOL: D4D_TRUE - cursor was wrapped 
//                        D4D_FALSE - cursor was not wrapped
//-----------------------------------------------------------------------------

D4D_BOOL D4D_CnslPutString(D4D_OBJECT_PTR pObj, D4D_CHAR* pText) {
  
D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
D4D_BOOL wrap = D4D_FALSE;
D4D_POINT tmp_scrollbars;
  
  if(pText == NULL)
    return D4D_FALSE;
  
  if(pCnsl->pScrlBrHor->flags->bits.bVisible)
    tmp_scrollbars.x = D4D_ScrlBrGetPosition(pCnsl->pScrlBrHor);
  
  tmp_scrollbars.y = D4D_GET_CONSOLE_DATA(pObj)->cursorPos.y;
  
  
  while(*pText) {
    if(D4D_CnslPutChar(pObj, *pText))
      wrap = D4D_TRUE;
    
    pText++;  
  }
  
  if(tmp_scrollbars.y == D4D_GET_CONSOLE_DATA(pObj)->cursorPos.y)
  {
    if(pCnsl->pScrlBrHor->flags->bits.bVisible)
      if(tmp_scrollbars.x != D4D_ScrlBrGetPosition(pCnsl->pScrlBrHor))
        return wrap;
      
    D4D_GET_CONSOLE_DATA(pObj)->flags.bits.bRedrawAll = 0;
  }
  
  return wrap;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslClearLine
// SCOPE:       Console object external API function
// DESCRIPTION: The function clear one line in console 
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_INDEX line - index of line that should be cleared
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_CnslClearLine(D4D_OBJECT_PTR pObj, D4D_INDEX line) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  D4D_INDEX tmp_realLine, ix;
  D4D_CHAR* tmp_charAdrr;
  
  if(line >= pCnsl->txtArrSize.cy)
    return;
  
  // Compute the real line in memory
  tmp_realLine = (D4D_INDEX)((pData->lastLineIx + line) % pCnsl->txtArrSize.cy);
  tmp_charAdrr = (D4D_CHAR*)((tmp_realLine * (pCnsl->txtArrSize.cx + 1)) + pCnsl->pTxtArr);  
    
  for(ix = 0;ix <= pCnsl->txtArrSize.cx; ix++)
    *(tmp_charAdrr++) = 0;  
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslNewLine
// SCOPE:       Console object external API function
// DESCRIPTION: The function move cursor on new line 
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_BOOL returnCarrier - return  carrier flag
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
static void D4D_CnslNewLine(D4D_OBJECT_PTR pObj, D4D_BOOL returnCarrier) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  
  // Check if the new line is last, so whole console will be scrolled by one line
  if((pData->cursorPos.y + 1) >= pCnsl->txtArrSize.cy) {
    // Increment the last line pointer
    pData->lastLineIx++;

    // Check the overroll the buffer runs
    if(pData->lastLineIx >= pCnsl->txtArrSize.cy)
      pData->lastLineIx = 0;
    
    D4D_CnslClearLine(pObj, (D4D_INDEX)(pCnsl->txtArrSize.cy - 1));
    
  }else {
    // Increment the cursor position
    pData->cursorPos.y++;
  }
  
   
  if(returnCarrier) 
  {
    pData->cursorPos.x = 0;
  }else {
    pData->flags.bits.bCheckLine = 1;
  }
  pData->flags.bits.bRedrawAll = 1;
  D4D_CnslUpdateScrollbars((D4D_OBJECT*)pObj);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslGoToXY
// SCOPE:       Console object external API function
// DESCRIPTION: The function move cursor on new position if possible 
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_POINT newPosition - new cursor position
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_CnslGoToXY(D4D_OBJECT_PTR pObj, D4D_POINT newPosition) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  
  if(newPosition.x >= pCnsl->txtArrSize.cx)
    return;
  
  if(newPosition.y >= pCnsl->txtArrSize.cy)
    return;
  
  pData->cursorPos = newPosition;
  
  pData->flags.bits.bCheckLine = 1;
  pData->flags.bits.bRedrawAll = 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslGetCursor
// SCOPE:       Console object external API function
// DESCRIPTION: The function returns the cursor position
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//                           
// RETURNS:     D4D_POINT Position - cursor position
//-----------------------------------------------------------------------------
D4D_POINT D4D_CnslGetCursor(D4D_OBJECT_PTR pObj) 
{
  D4D_CNSL_DATA* pData = D4D_GET_CONSOLE_DATA(pObj);
  
  if(pObj == NULL)
    return d4d_point_zero;
  
  return pData->cursorPos;  
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslEnsureVisible
// SCOPE:       Console object external API function
// DESCRIPTION: The function update automatically the scroll bars to ensure visible position of cursor
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//                           
// RETURNS:     none
//-----------------------------------------------------------------------------

void D4D_CnslEnsureVisible(D4D_OBJECT_PTR pObj)
{
  if(pObj == NULL)
    return;
  
  D4D_CnslUpdateScrollbars((D4D_OBJECT*)pObj);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslSetScrollBarPosition
// SCOPE:       Console object external API function
// DESCRIPTION: The function move cursor on new position if possible 
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              D4D_INDEX hor - new position of horizontal scroll bar
//              D4D_INDEX ver - new position of vertical scroll bar
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_CnslSetScrollBarPosition(D4D_OBJECT_PTR pObj, D4D_INDEX hor, D4D_INDEX ver)
{  
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  
  if(pObj == NULL)
    return;
  
  if(pCnsl->pScrlBrHor->flags->bits.bVisible)  
    D4D_ScrlBrSetPosition((D4D_OBJECT*)pCnsl->pScrlBrHor, hor);   
  
  if(pCnsl->pScrlBrVer->flags->bits.bVisible)
    D4D_ScrlBrSetPosition((D4D_OBJECT*)pCnsl->pScrlBrVer, ver);
  
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslGetScrollBarPosition
// SCOPE:       Console object external API function
// DESCRIPTION: The function returns the scroll bars position
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//                           
// RETURNS:     D4D_POINT Position - scroll bars position
//-----------------------------------------------------------------------------
D4D_POINT D4D_CnslGetScrollBarPosition(D4D_OBJECT_PTR pObj)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_POINT res;
  
  if(pObj == NULL)
    return d4d_point_zero;
  
  res.x = D4D_ScrlBrGetPosition((D4D_OBJECT*)pCnsl->pScrlBrHor);
  res.y = D4D_ScrlBrGetPosition((D4D_OBJECT*)pCnsl->pScrlBrVer);
  
  return res;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_CnslClearAll
// SCOPE:       Console object external API function
// DESCRIPTION: The function clear whole console data 
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on console object
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_CnslClearAll(D4D_OBJECT_PTR pObj) {
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pObj);
  D4D_CNSL_DATA* pData = pCnsl->pData;
  
  LWord i,i_max;
  
  // Init the text array   
  i_max = (LWord)(pCnsl->txtArrSize.cy * (pCnsl->txtArrSize.cx + 1));
      
  for(i=0;i<i_max;i++)
    pCnsl->pTxtArr[i] = 0;
  
  pData->cursorPos.x =0;
  pData->cursorPos.y =0;
  pData->flags.bits.bCheckLine = 0;
  pObj->flags->bits.bRedraw = 1;
  pData->flags.bits.bRedrawAll = 1;
    
}

/**************************************************************//*!
*
*
*
*     THE INTERNAL API FUNCTIONS
*
*
*
******************************************************************/

/*******************************************************
*
* The CONSOLE message handler 
*
*******************************************************/

void D4D_ConsoleOnMessage(D4D_MESSAGE* pMsg)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pMsg->pObject);
  
  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:      
      D4D_ConsoleOnDraw(pMsg);      
      break;
    
    case D4D_MSG_ONINIT:      
      D4D_CnslOnInit(pMsg);
      break;
    
    case D4D_MSG_TOUCHED:                              
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);       
      D4D_CaptureKeys(pMsg->pObject);
      break;
      
     case D4D_MSG_KEYUP:
      if(pMsg->pObject->flags->bits.bEnabled)
      {
        D4D_KEY_SCANCODE tmp_key = pMsg->prm.key;
        // capture the keyboard if enter is pressed
        if(tmp_key == D4D_KEY_SCANCODE_ENTER)
          D4D_CaptureKeys(pMsg->pObject);
        
        // exit capture
        if(tmp_key == D4D_KEY_SCANCODE_ESC)
          D4D_CaptureKeys(NULL);
      }
      break;  
    
    case D4D_MSG_TIMETICK:
      #if (D4D_CNSL_CURSOR_BLINK_TICK_COUNTER > 0)      
        D4D_ConsoleTimeTick(pMsg);
      #endif
      break;
    
    case D4D_MSG_KILLFOCUS:
      D4D_CaptureKeys(NULL);
    case D4D_MSG_SETFOCUS:
    case D4D_MSG_SETCAPTURE:
    case D4D_MSG_KILLCAPTURE:
      pMsg->pObject->flags->bits.bRedrawC = 1;
      break;
    
    
    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
  
  // send the message to child objects
  // Horizontal scroll bar
  SendLocalMessage(pMsg, (D4D_OBJECT*)pCnsl->pScrlBrHor);
  // Vertical scroll bar
  SendLocalMessage(pMsg, (D4D_OBJECT*)pCnsl->pScrlBrVer);
  
  if(pMsg->nMsgId == D4D_MSG_DRAWDONE)  
    D4D_GET_CONSOLE_DATA(pMsg->pObject)->flags.bits.bRedrawScrollBars = 0;
}

/*******************************************************
*
* The CONSOLE check coordinators  handler 
*
*******************************************************/

Byte D4D_ConsoleCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_CONSOLE* pCnsl = D4D_GET_CONSOLE(pThis);
  
  D4D_CnslValue2Coor(pThis);
  
  if((point.x >= pCnsl->scrPos.x) && (point.y >= pCnsl->scrPos.y))
  {
      
    if((point.x <= (pCnsl->scrPos.x + _calc.size.cx)) && (point.y <= (pCnsl->scrPos.y + _calc.size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_ConsoleGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_CONSOLE(pThis)->textBuff);  
}
