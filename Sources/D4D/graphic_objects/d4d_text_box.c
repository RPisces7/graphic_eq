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
* @file      d4d_text_box.c
*
* @author    b01119
* 
* @version   0.0.11.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D driver textbox object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"


typedef struct 
{
  D4D_CHAR* pWord;
  D4D_FONT fnt;
  D4D_COOR cur_pos;
  D4D_COOR pixLen;
  D4D_INDEX charCnt;
  D4D_BOOL newLine;
  D4D_TAB* pTab;
  D4D_INDEX offset;
}D4D_TXTBX_WORD_DESC;

// Internal API
void D4D_TextBoxOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_TextBoxCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_TextBoxGetTextBuffer(D4D_OBJECT* pThis);



static D4D_BOOL D4D_TextBoxScrollBarSetup(D4D_OBJECT* pObject);
static D4D_INDEX D4D_TextBoxGetMaxLineChars(D4D_CHAR* pText, D4D_INDEX offset, D4D_TAB* pTab, D4D_COOR len, D4D_FONT fnt);
static D4D_BOOL D4D_TextBoxGetWordSpec(D4D_TXTBX_WORD_DESC* wordDesc);
static D4D_BOOL D4D_TextBoxGetCharsToLine(D4D_TXTBX_WORD_DESC* wordDesc);
static D4D_INDEX D4D_TextBoxGetLineCount(D4D_OBJECT* pThis);
static D4D_INDEX D4D_TextBoxFindLineStartChar(D4D_OBJECT* pThis, D4D_INDEX line);
static void SendLocalMessage(D4D_MESSAGE* pMsg, D4D_OBJECT* pObj);
static void D4D_TextBoxOnInit(D4D_OBJECT* pObject);


const D4D_OBJECT_SYS_FUNCTION d4d_textboxSysFunc = 
{
  D4D_OBJECT_TEXTBOX,
  D4D_TextBoxOnMessage,
  D4D_TextBoxCheckCoordinates,
  D4D_TextBoxGetTextBuffer
};

typedef struct 
{
  D4D_POINT position;
  D4D_SIZE size;
  D4D_POINT txtPos;
  D4D_SIZE txtSize;
  D4D_INDEX lineCount;
} D4D_TXTBX_TMP_VAL;


#define _calc (*((D4D_TXTBX_TMP_VAL*)d4d_scratchPad))
    
static void D4D_TextBoxValue2Coor(D4D_OBJECT* pThis)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);  
  D4D_COOR header_height = 1;
  D4D_SIZE scrSize = D4D_GetClientScreenSize(*(pThis->pScreen));
  
  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pTextBox->scrPos);
  _calc.size = pTextBox->scrSize;
   
  if(pTextBox->pBmpIcon) 
  {
    header_height = D4D_GetBmpHeight(pTextBox->pBmpIcon);  
  }
  
  if(pTextBox->textBuff.pText) 
  {
    if(D4D_GetFontHeight(pTextBox->textBuff.fontId) > header_height)
      header_height = D4D_GetFontHeight(pTextBox->textBuff.fontId); 
  }
  
  if(header_height)
    header_height += 2;


 #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
      
  if(!_calc.size.cx)
  {
    // compute size of textbox object automaticcaly
    // simply get the maximum possible size
    _calc.size.cx = (D4D_COOR)(scrSize.cx - pTextBox->scrPos.x);           
  }
  
  if(!_calc.size.cy)
  {
    // compute size of textbox object automaticcaly
    // simply get the maximum possible size
    _calc.size.cy = (D4D_COOR)(scrSize.cy - pTextBox->scrPos.y);            
  }      
 #endif
 
  _calc.txtPos = _calc.position;
  
  _calc.txtPos.y += header_height;
  _calc.txtPos.x += 1;

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  _calc.txtPos.y += (D4D_COOR)(pTextBox->radius / 2);
  _calc.txtPos.x += (D4D_COOR)(pTextBox->radius / 2);
#endif
  
  _calc.txtSize = _calc.size;
  
  if(pTextBox->pScrlBrVer->flags->bits.bVisible)
    _calc.txtSize.cx -= (D4D_TXTBX_SCRLBR_WIDTH + 4);
  else
    _calc.txtSize.cx -= 4;
  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE   
  _calc.txtSize.cx -= (D4D_COOR)(pTextBox->radius / 2);
  
  _calc.txtSize.cy -= (D4D_COOR)(1 + header_height + pTextBox->radius / 2);
#else
  _calc.txtSize.cy -= (D4D_COOR)(1 + header_height);
#endif
  
  // Compute the lines count in object
  _calc.lineCount = (D4D_COOR) (_calc.txtSize.cy / D4D_GetFontHeight(pTextBox->textFontId));

}

static D4D_BOOL D4D_TextBoxGetWordSpecInt(D4D_CHAR ch, D4D_TXTBX_WORD_DESC* wordDesc)
{
  D4D_COOR tab;
  
  // normal char or any special char
  if(ch <= ' ')
  {
    // special char
    
    if(wordDesc->charCnt) // if any chars are decoded (Word loaded) finish the function
      return D4D_FALSE;
    
    switch(ch)
    {
      case ' ': // space
        wordDesc->pixLen = D4D_GetCharWidth(wordDesc->fnt, ' ');  
        break;
        
      case '\n': // new line
        wordDesc->newLine = D4D_TRUE;
        break;
      
      case '\t': // Tab
        tab = D4D_GetNextTab(wordDesc->pTab, wordDesc->cur_pos);
        
        if(tab)                    
          wordDesc->pixLen = (D4D_COOR)(tab - wordDesc->cur_pos);
        else
          wordDesc->pixLen = D4D_GetCharWidth(wordDesc->fnt, '\t');
        break;
      
      default:
        // unsupported char        
        break;
    }
    
    wordDesc->charCnt++; // increament the one processed char
    return D4D_FALSE;
    
  }else
  {
    // normal char
    wordDesc->pixLen += D4D_GetCharWidth(wordDesc->fnt, ch);
  }
  return D4D_TRUE;
  
}



static D4D_BOOL D4D_TextBoxGetWordSpec(D4D_TXTBX_WORD_DESC* wordDesc)
{
  D4D_CHAR* pTmpWord = wordDesc->pWord;
  wordDesc->newLine = D4D_FALSE;
  wordDesc->pixLen = 0;
  wordDesc->charCnt = 0;
  
  if(pTmpWord == NULL)
    return D4D_TRUE;
  
#if D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE  
  
  if(*pTmpWord == d4d_extsrcLocale.strTranslatePrefix)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = wordDesc->offset;
    D4D_INDEX charCntTmp; 
    
    do
    { 
      charOff += charCnt;
      
      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pTmpWord, (char*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);
      
      
      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if(D4D_TextBoxGetWordSpecInt((D4D_CHAR)d4d_extsrcBuffer[charCntTmp], wordDesc))
        {
          
          wordDesc->charCnt++; // increament the one processed char
        }else
          return D4D_FALSE;
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
  } 
  else
  #endif
  
  {
    pTmpWord += wordDesc->offset;
    
    while(*pTmpWord) // check if the tested strings isn't on end 
    {    
      if(D4D_TextBoxGetWordSpecInt(*pTmpWord, wordDesc))
      {
        
        wordDesc->charCnt++; // increament the one processed char
        pTmpWord++; // increment WORD pointer to next char
      }else
        return D4D_FALSE;
    }
  } 
  
  return D4D_TRUE;
  
}


static D4D_BOOL D4D_TextBoxGetCharsToLine(D4D_TXTBX_WORD_DESC* wordDesc)
{
  D4D_CHAR* pTmpWord = wordDesc->pWord;
  D4D_COOR len, tmp_len;
  
  len = wordDesc->pixLen;
  wordDesc->pixLen = 0;
  wordDesc->newLine = D4D_FALSE;
  wordDesc->charCnt = 0;
  
  if(pTmpWord == NULL)
    return D4D_FALSE;
  
  #if D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE  
  
  if(*pTmpWord == d4d_extsrcLocale.strTranslatePrefix)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = wordDesc->offset;
    D4D_INDEX charCntTmp; 
    
    
    do
    { 
      charOff += charCnt;
      
      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pTmpWord, (char*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);
      
      
      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)      
      {
        tmp_len = D4D_GetCharWidth(wordDesc->fnt, (D4D_CHAR)d4d_extsrcBuffer[charCntTmp]);
      
          // normal char
        if((wordDesc->pixLen + tmp_len) > len)
          return D4D_TRUE;
        
        wordDesc->pixLen += tmp_len;
        wordDesc->charCnt++; // increament the one processed char        
      }
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
  } 
  else
  #endif
  
  {
    pTmpWord += wordDesc->offset;
    
    while(*pTmpWord) // check if the tested strings isn't on end 
    {    
      tmp_len = D4D_GetCharWidth(wordDesc->fnt, *pTmpWord);
      
        // normal char
      if((wordDesc->pixLen + tmp_len) > len)
        return D4D_TRUE;
      
      wordDesc->pixLen += tmp_len;
      wordDesc->charCnt++; // increament the one processed char
      pTmpWord++; // increment WORD pointer to next char
    }
  }
  
  return D4D_TRUE;  
}



 
static D4D_INDEX D4D_TextBoxGetMaxLineChars(D4D_CHAR* pText, D4D_INDEX offset, D4D_TAB* pTab, D4D_COOR len, D4D_FONT fnt)
{
  D4D_TXTBX_WORD_DESC tmp_wordDesc;  
  D4D_COOR tmp_len = 0;
  D4D_INDEX res_charCnt = 0;
  
  tmp_wordDesc.fnt = fnt;
  tmp_wordDesc.pWord = pText;
  tmp_wordDesc.pTab = pTab;
  tmp_wordDesc.cur_pos = 0;
  tmp_wordDesc.offset = offset;
  
  while(!D4D_TextBoxGetWordSpec(&tmp_wordDesc))
  {
    if((tmp_len + tmp_wordDesc.pixLen) > len)
    {
      if(tmp_len == 0)
      {
        // very long word - longer than one line
        tmp_wordDesc.pixLen = len;
        (void)D4D_TextBoxGetCharsToLine(&tmp_wordDesc);
      }else
        return res_charCnt;
    }
    
    
    tmp_len += tmp_wordDesc.pixLen;
    res_charCnt += tmp_wordDesc.charCnt;
    
    if(tmp_wordDesc.newLine)
      return res_charCnt;  
    
    //tmp_wordDesc.pWord = &pText[res_charCnt];
    tmp_wordDesc.offset += tmp_wordDesc.charCnt;
    tmp_wordDesc.cur_pos = tmp_len;    
  }
  
  return (D4D_INDEX)(tmp_wordDesc.charCnt + res_charCnt);
}

static D4D_INDEX D4D_TextBoxGetLineCount(D4D_OBJECT* pThis)
{
  // The function counts with filled _calc variable
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  D4D_CHAR* pText = pTextBox->pData->pTxtArr;
  D4D_INDEX lineCharCnt;
  D4D_INDEX textIx = 0;
  D4D_INDEX lineCnt = 0;
  D4D_TAB tmp_strTab;
  D4D_TAB* pTab = NULL;
  // Try to check if the Text Box needs scroll Bar
  
  if(pTextBox->pTabTable)
  {
    tmp_strTab.pTabTable = pTextBox->pTabTable;
    tmp_strTab.tabOffset = (D4D_COOR)(_calc.txtPos.x - _calc.position.x);
    pTab = &tmp_strTab;  
  }
  
  do
  {    
    lineCharCnt = D4D_TextBoxGetMaxLineChars(pText, textIx, pTab, _calc.txtSize.cx, pTextBox->textFontId);
    textIx += lineCharCnt;
    
    if(lineCharCnt)
      lineCnt++;
  
  }while(lineCharCnt);
  
  return lineCnt;
}


static D4D_INDEX D4D_TextBoxFindLineStartChar(D4D_OBJECT* pThis, D4D_INDEX line)
{
  // The function counts with filled _calc variable
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  D4D_CHAR* pText = pTextBox->pData->pTxtArr;
  D4D_INDEX lineCharCnt;
  D4D_INDEX textIx = 0;
  D4D_INDEX lineCnt = 0;
  D4D_TAB tmp_strTab;
  D4D_TAB* pTab = NULL;
  
  if(pTextBox->pTabTable)
  {
    tmp_strTab.pTabTable = pTextBox->pTabTable;
    tmp_strTab.tabOffset = (D4D_COOR)(_calc.txtPos.x - _calc.position.x);
    pTab = &tmp_strTab;  
  }
  
  do
  {    
    if(line == lineCnt)
      return textIx;
    
    lineCharCnt = D4D_TextBoxGetMaxLineChars(pText, textIx, pTab, _calc.txtSize.cx, pTextBox->textFontId); 
    
    textIx += lineCharCnt;
    
    if(lineCharCnt)
      lineCnt++;
    
  }while(lineCharCnt);
  
  return 0;
}



static D4D_BOOL D4D_TextBoxScrollBarSetup(D4D_OBJECT* pObject)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pObject);
  D4D_SCROLL_BAR* pScrlBr;
  
  
  
  // Try to check if the Text Box needs scroll Bar
  
  // Switch off possible scroll bar in refresh case
  pTextBox->pScrlBrVer->flags->bits.bVisible = 0;
  
   
  D4D_TextBoxValue2Coor(pObject);
  
  if(D4D_TextBoxGetLineCount(pObject) > _calc.lineCount)
  {
    pTextBox->pScrlBrVer->flags->bits.bVisible = 1;
    
    D4D_TextBoxValue2Coor(pObject);
    
    // Init child objects coordinations
    pScrlBr = D4D_GET_SCROLL_BAR(pTextBox->pScrlBrVer);
  
    pScrlBr->scrPos.x = (D4D_COOR)(_calc.txtPos.x + _calc.txtSize.cx + 2);
    pScrlBr->scrPos.y = (D4D_COOR)(_calc.txtPos.y);
    
    pScrlBr->scrPos = D4D_GetScreenToClientPoint( *pObject->pScreen, &pScrlBr->scrPos);
    
    pScrlBr->scrSize.cx = D4D_TXTBX_SCRLBR_WIDTH + 1;
    pScrlBr->scrSize.cy = (D4D_COOR)(_calc.txtSize.cy);

    if(pTextBox->radius > D4D_TXTBX_SCRLBR_WIDTH / 2)
      pScrlBr->radius = (D4D_COOR)(D4D_TXTBX_SCRLBR_WIDTH / 2);
    else
      pScrlBr->radius = pTextBox->radius;
      
    D4D_ScrlBrSetRange(pTextBox->pScrlBrVer, 0, D4D_TextBoxGetLineCount(pObject));
    D4D_ScrlBrSetStep(pTextBox->pScrlBrVer, _calc.lineCount, D4D_TXTBX_SCRLBR_STEP);
    D4D_ScrlBrSetPosition(pTextBox->pScrlBrVer, 0);
    return D4D_TRUE;
  }
  return D4D_FALSE;
}


/*******************************************************
*
* TEXTBOX Drawing routine
*
*******************************************************/

static void D4D_TextBoxOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pMsg->pObject);
  D4D_TXTBX_DATA* pData = pTextBox->pData;
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_INDEX line_cnt;
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_strPrties;
  D4D_TAB tmp_strTab;
  D4D_TAB* pTab = NULL;
  
  D4D_TextBoxValue2Coor(pMsg->pObject);
      
  clrT = D4D_ObjectGetForeFillColor(pMsg->pObject);
  clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);  
  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE 
  
  // draw the rectangle around the text
  if(draw.bits.bComplete)  
  {
    
    D4D_FillRRect(&_calc.position, &_calc.size, clrB, pTextBox->radius);  
    
    if(pTextBox->pBmpIcon) {
      // Draw textbox icon
      D4D_DrawRBmpXY((D4D_COOR)(_calc.position.x + 1), (D4D_COOR)(_calc.position.y + 1), pTextBox->pBmpIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pTextBox->radius);
    }
    
    if(pTextBox->textBuff.pText)
    {
      // draw the title text
      D4D_COOR tmp_x = _calc.position.x;
      
      tmp_x++;
      
      tmp_x += (D4D_COOR)(pTextBox->radius / 2);
      
      if(pTextBox->pBmpIcon)
        tmp_x += (D4D_COOR)(D4D_GetBmpWidth(pTextBox->pBmpIcon) + 1);
      
      
      D4D_MoveToXY(tmp_x, (D4D_COOR)(_calc.position.y + 1));
        
      D4D_DrawTextRectToXY((D4D_COOR)(_calc.position.x + _calc.size.cx - 1 - pTextBox->radius / 2), (D4D_COOR)(_calc.txtPos.y - 2), &pTextBox->textBuff, clrT, clrB);
    }
    
    if((pTextBox->textBuff.pText) || (pTextBox->pBmpIcon)) 
    {
      //Draw underline of title box
      D4D_MoveToXY(_calc.position.x, (D4D_COOR)(_calc.txtPos.y - 1));
      D4D_LineToXY((D4D_COOR)(_calc.position.x + _calc.size.cx) , (D4D_COOR)(_calc.txtPos.y - 1), D4D_LINE_THIN, clrT);
    }
    
    pData->redrawText = D4D_TRUE;
    
  }
  
  if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
     D4D_RRect(&_calc.position, &_calc.size, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw), pTextBox->radius);
#else
  // draw the rectangle around the text
  if(draw.bits.bComplete)  
  {
    
    D4D_FillRect(&_calc.position, &_calc.size, clrB);  
    
    if(pTextBox->pBmpIcon) {
      // Draw textbox icon
      D4D_DrawBmpXY((D4D_COOR)(_calc.position.x + 1), (D4D_COOR)(_calc.position.y + 1), pTextBox->pBmpIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
    }
    
    if(pTextBox->textBuff.pText)
    {
      // draw the title text
      D4D_COOR tmp_x = _calc.position.x;
      
      tmp_x++;
      
      if(pTextBox->pBmpIcon)
        tmp_x += (D4D_COOR)(D4D_GetBmpWidth(pTextBox->pBmpIcon) + 1);
      
      
      D4D_MoveToXY(tmp_x, (D4D_COOR)(_calc.position.y + 1));
        
      D4D_DrawTextRectToXY((D4D_COOR)(_calc.position.x + _calc.size.cx - 1), (D4D_COOR)(_calc.txtPos.y - 2), &pTextBox->textBuff, clrT, clrB);
    }
    
    if((pTextBox->textBuff.pText) || (pTextBox->pBmpIcon)) 
    {
      //Draw underline of title box
      D4D_MoveToXY(_calc.position.x, (D4D_COOR)(_calc.txtPos.y - 1));
      D4D_LineToXY((D4D_COOR)(_calc.position.x + _calc.size.cx) , (D4D_COOR)(_calc.txtPos.y - 1), D4D_LINE_THIN, clrT);
    }
    
    pData->redrawText = D4D_TRUE;
    
  }
  
  if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
     D4D_Rect(&_calc.position, &_calc.size, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw));
#endif  
  
  if(pData->redrawText)
  {
    D4D_INDEX tmp_index = pData->firstShowedCharIx;
    
    pData->redrawText = D4D_FALSE;
  
    tmp_txtbuff.fontId = pTextBox->textFontId;
    tmp_txtbuff.str_properties = &tmp_strPrties;
    
    tmp_strPrties.font_properties.all = 0;
    tmp_strPrties.text_properties.all = 0;
    
    // Draw all lines of textbox
    tmp_point = _calc.txtPos;
    tmp_size.cx = _calc.txtSize.cx;
    tmp_size.cy = D4D_GetFontHeight(pTextBox->textFontId);
    
    tmp_txtbuff.pText = pData->pTxtArr;  
    
    if(pTextBox->pTabTable)
    {
      tmp_strTab.pTabTable = pTextBox->pTabTable;
      tmp_strTab.tabOffset = (D4D_COOR)(_calc.txtPos.x - _calc.position.x);
      pTab = &tmp_strTab;  
    }
    
    
    for(line_cnt = 0; line_cnt < _calc.lineCount; line_cnt++) 
    {

      tmp_txtbuff.buffSize = (Word)(1 + D4D_TextBoxGetMaxLineChars(tmp_txtbuff.pText, tmp_index, pTab, tmp_size.cx, tmp_txtbuff.fontId));
      
      
      D4D_DrawTextRectTabAdv(&tmp_point, &tmp_size, &tmp_txtbuff, tmp_index, pTab, clrT, clrB); 

      tmp_index +=  (tmp_txtbuff.buffSize - 1);
      //tmp_txtbuff.pText += (tmp_txtbuff.buffSize - 1);
      tmp_point.y += tmp_size.cy; 
    }
  }
}

static void D4D_TextBoxOnInit(D4D_OBJECT* pObject)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pObject);

  // init screen pointer of child objects
  *(pTextBox->pScrlBrVer->pScreen) = *(pObject->pScreen);

  pTextBox->pData->firstShowedCharIx = 0;
                                             

  (void)D4D_TextBoxScrollBarSetup(pObject);

  // set flag to redraw screen
  pObject->flags->bits.bRedrawC = 1;  
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



static void SendLocalMessage(D4D_MESSAGE* pMsg, D4D_OBJECT* pObj)
{
  D4D_MESSAGE tmpMsg = *pMsg;

  // don't send the OnDraw/done messages if the objects are invisible
  if((pObj->flags->bits.bVisible == 0) && ((pMsg->nMsgId == D4D_MSG_DRAW) || (pMsg->nMsgId == D4D_MSG_DRAWDONE)))
    return;
  
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

void D4D_TextBoxScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_TXTBX_DATA* pData = D4D_GET_TEXTBOX_DATA((D4D_OBJECT*)(pThis->userPointer));

  D4D_UNUSED(old_position);
  
  // compute the start char of current screen
  pData->firstShowedCharIx = D4D_TextBoxFindLineStartChar((D4D_OBJECT*)(pThis->userPointer), new_position);

  // set flag to redraw screen
  ((D4D_OBJECT*)(pThis->userPointer))->flags->bits.bRedraw = 1;
  pData->redrawText = D4D_TRUE;
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
// FUNCTION:    D4D_TextBoxRefreshAll
// SCOPE:       Text Box object external API function
// DESCRIPTION: The function reinitialize whole object
//              
// PARAMETERS:  D4D_OBJECT* pObject - pointer on text box object
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_TextBoxRefreshAll(D4D_OBJECT* pObject)
{
  if(pObject == NULL)
    return;
  
  D4D_TextBoxOnInit(pObject);    
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_TextBoxChangeText
// SCOPE:       Text Box object external API function
// DESCRIPTION: The function change the pointer of text that will be shown in text box
//              
// PARAMETERS:  D4D_OBJECT* pObject - pointer on text box object
//              D4D_CHAR* pText - pointer on new text that will be shown in text box
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_TextBoxChangeText(D4D_OBJECT* pObject, D4D_CHAR* pText)
{
  D4D_TXTBX_DATA* pData = D4D_GET_TEXTBOX_DATA(pObject);
  
  // check the intput pointers
  if(pObject == NULL)
    return;
  
  if(pText == NULL)
    return;
  
  // change the text pointer
  pData->pTxtArr = pText;
  
  // refresh whole object
  D4D_TextBoxOnInit(pObject);
      
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
* The TEXTBOX message handler 
*
*******************************************************/

void D4D_TextBoxOnMessage(D4D_MESSAGE* pMsg)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pMsg->pObject);
  
  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:      
      D4D_TextBoxOnDraw(pMsg);      
      break;
    
    case D4D_MSG_ONINIT:      
      D4D_TextBoxOnInit(pMsg->pObject);
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
    
    case D4D_MSG_KILLFOCUS:
      D4D_CaptureKeys(NULL);
    case D4D_MSG_SETFOCUS:
    case D4D_MSG_SETCAPTURE:
    case D4D_MSG_KILLCAPTURE:
      pMsg->pObject->flags->bits.bRedraw = 1;
      break;
    
    
    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
  
  // send the message to child object
  // Vertical scroll bar
  SendLocalMessage(pMsg, (D4D_OBJECT*)pTextBox->pScrlBrVer);
}

/*******************************************************
*
* The TEXTBOX check coordinators  handler 
*
*******************************************************/

Byte D4D_TextBoxCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_TEXTBOX* pTextBox = D4D_GET_TEXTBOX(pThis);
  
  D4D_TextBoxValue2Coor(pThis);
  
  if((point.x >= pTextBox->scrPos.x) && (point.y >= pTextBox->scrPos.y))
  {
      
    if((point.x <= (pTextBox->scrPos.x + _calc.size.cx)) && (point.y <= (pTextBox->scrPos.y + _calc.size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_TextBoxGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_TEXTBOX(pThis)->textBuff);  
}
