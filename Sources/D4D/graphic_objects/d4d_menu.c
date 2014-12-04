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
* @file      d4d_menu.c
*
* @author    b01119
* 
* @version   0.0.35.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver menu object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_MenuOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_MenuCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_MenuGetTextBuffer(D4D_OBJECT* pThis);

static void D4D_MenuOnInit(D4D_MESSAGE* pMsg);

const D4D_OBJECT_SYS_FUNCTION d4d_menuSysFunc = 
{
  D4D_OBJECT_MENU,
  D4D_MenuOnMessage,
  D4D_MenuCheckCoordinates,
  D4D_MenuGetTextBuffer
};

// temporary structure for menu temporary value calculation
typedef struct 
{
    D4D_POINT position;
    D4D_COOR titleBar_y;
    D4D_SIZE maxIcon;
    D4D_MENU_INDEX posCnt;
    D4D_COOR textOff;
    char index_txt[8];
    Byte index_txt_len;
} D4D_MENU_TMP_VAL;

#define _calc (*((D4D_MENU_TMP_VAL*)d4d_scratchPad))




/*******************************************************
*
* MENU Helper computing routines
*
*******************************************************/

static D4D_SIZE D4D_GetIconsMaxSize(D4D_MENU *pMenu)
{
  Byte tmpB;
  D4D_SIZE tmpSize =  {0, 0};
  
  // compute max width of used Icons
  for(tmpB = 0; tmpB < (Byte)(*(pMenu->pItemsCnt)); tmpB++)
  {
    if(pMenu->pItems[tmpB].pIcon != NULL)
    {      
      if(D4D_GetBmpWidth(pMenu->pItems[tmpB].pIcon) > tmpSize.cx)
        tmpSize.cx = D4D_GetBmpWidth(pMenu->pItems[tmpB].pIcon); 
      
      if(D4D_GetBmpHeight(pMenu->pItems[tmpB].pIcon) > tmpSize.cy)
        tmpSize.cy = D4D_GetBmpHeight(pMenu->pItems[tmpB].pIcon); 
    }
  }   
  return tmpSize;
}

static D4D_COOR D4D_GetTitleBarHeight(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  
  if((D4D_GetFontHeight(pMenu->title_text.fontId) > D4D_GetFontHeight(pMenu->indexFontId)) || (!(pThis->initFlags & D4D_MENU_F_INDEX)))
    return (D4D_COOR)(D4D_GetFontHeight(pMenu->title_text.fontId) + 3 + pMenu->radius / 2);
  else
    return (D4D_COOR)(D4D_GetFontHeight(pMenu->indexFontId) + 3 + pMenu->radius / 2);
}

static D4D_MENU_INDEX D4D_GetPositionCount(D4D_OBJECT* pThis)
{
 D4D_MENU* pMenu = D4D_GET_MENU(pThis);
 
 #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities   
 
  if(pMenu->posCnt)
    return pMenu->posCnt;
  
  if(D4D_GetIconsMaxSize(pMenu).cy > (D4D_GetFontHeight(pMenu->itemsFontId)))
    return (D4D_MENU_INDEX)((pMenu->scrSize.cy - D4D_GetTitleBarHeight(pThis)) / (D4D_GetIconsMaxSize(pMenu).cy + 3));
  else       
    return (D4D_MENU_INDEX)((pMenu->scrSize.cy - D4D_GetTitleBarHeight(pThis)) / (D4D_GetFontHeight(pMenu->itemsFontId) + 3));     
  
#else
  
  return pMenu->posCnt;  

#endif  
}


static void D4D_MenuValue2Coor(D4D_OBJECT* pThis)
{
    D4D_MENU* pMenu = D4D_GET_MENU(pThis);
    _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pMenu->scrPos);
    _calc.titleBar_y = D4D_GetTitleBarHeight(pThis);
    _calc.maxIcon = D4D_GetIconsMaxSize(pMenu);
    _calc.posCnt = D4D_GetPositionCount(pThis);
    _calc.textOff = pMenu->textOff;
    
#if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities  
    
    // if count of items position or text offsets are not declared , then use automatic values
    if(!_calc.textOff)
    {
      if(_calc.posCnt > 1)
        _calc.textOff = (D4D_COOR)( (pMenu->scrSize.cy - _calc.titleBar_y) / (_calc.posCnt));
      else
        _calc.textOff = (D4D_COOR)((pMenu->scrSize.cy - _calc.titleBar_y) / 2);
    }
#endif
    
    if(pThis->initFlags & D4D_MENU_F_INDEX)
    {      
      
      // Draw index counter
      _calc.index_txt_len = D4D_SprintDecU8((Byte)(pMenu->pData->ix + 1), _calc.index_txt, (char) 0);
      _calc.index_txt[_calc.index_txt_len++] = '/';
      _calc.index_txt_len += D4D_SprintDecU8((Byte)(*(pMenu->pItemsCnt)), &_calc.index_txt[_calc.index_txt_len], (char) 0);
      _calc.index_txt[_calc.index_txt_len] = 0;

    }

  
}

/*******************************************************
*
* MENU Drawing routine
*
*******************************************************/

static void D4D_MenuOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_MENU* pMenu = D4D_GET_MENU(pMsg->pObject);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;     
    D4D_POINT tmp_point;
    D4D_SIZE tmp_size;
    D4D_STRING tmp_txtbuff;
    D4D_STR_PROPERTIES tmp_str_prty;
    Byte tmpB;
    
    tmp_txtbuff.str_properties = &tmp_str_prty;    
    
    // Get background simple color 
    clrT = D4D_ObjectGetForeFillColor(pMsg->pObject);
    clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);
    
    // Compute all tempoarary values
    D4D_MenuValue2Coor(pMsg->pObject);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  
    // draw just a color rectangle instead of bitmap
    if(draw.bits.bComplete)  
    {      
      if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
        D4D_RBox(&_calc.position, &pMenu->scrSize, D4D_LINE_THIN, clrT, clrB, pMenu->radius);
      else
        D4D_FillRRect(&_calc.position, &pMenu->scrSize, clrB, pMenu->radius);
      
      if(pMenu->pIcon != NULL)
        D4D_DrawRBmpXY((D4D_COOR)(_calc.position.x + 3), (D4D_COOR)(_calc.position.y + 3), pMenu->pIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pMenu->radius);
      
      if(pMenu->title_text.pText)
      {                 
        tmp_point.x = (D4D_COOR)(_calc.position.x + 4 + pMenu->radius/2);
        if(pMenu->pIcon != NULL)
          tmp_point.x += D4D_GetBmpHeight(pMenu->pIcon);
        
        tmp_point.y = (D4D_COOR)(_calc.position.y + 1 ); 
        D4D_DrawTextRect(&tmp_point, (D4D_SIZE*)&d4d_size_zero, &pMenu->title_text, clrT, clrB);        
      }
      
      D4D_MoveToXY((D4D_COOR)(_calc.position.x + 1),\
        (D4D_COOR)(_calc.position.y + _calc.titleBar_y));    
        
      D4D_LineToXY((D4D_COOR)(_calc.position.x + pMenu->scrSize.cx - 1),\
        (D4D_COOR)(_calc.position.y + _calc.titleBar_y), D4D_LINE_THIN, clrT);                
    
    }
    
    if(pMsg->pObject->initFlags & D4D_MENU_F_INDEX)
    {      
      // Draw index counter
      tmp_txtbuff.buffSize = (D4D_INDEX)(((*(pMenu->pItemsCnt)) < 10)? 4:6);
      tmp_size.cx = (D4D_COOR)((tmp_txtbuff.buffSize - 1) * D4D_GetFontWidth(pMenu->indexFontId) + 2);
      tmp_size.cy = (D4D_COOR)(_calc.titleBar_y - 2 - pMenu->radius / 2);
      tmp_point.x = (D4D_COOR)(_calc.position.x + pMenu->scrSize.cx - tmp_size.cx - 2 - pMenu->radius / 2);
      tmp_point.y = (D4D_COOR)(_calc.position.y + 1 + pMenu->radius / 2);
      
      tmp_txtbuff.pText = _calc.index_txt;
      tmp_txtbuff.fontId = pMenu->indexFontId;
      tmp_str_prty.font_properties.all = D4D_MENU_IX_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
      tmp_str_prty.text_properties.all = D4D_MENU_IX_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
      D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);
    }
    
    for(tmpB = 0; (tmpB < _calc.posCnt) && (tmpB < *(pMenu->pItemsCnt)); tmpB++)
    {
      if(((pMenu->pData->ix - pMenu->pData->focus_pos) != tmpB) || (!pMsg->pObject->flags->bits.bEnabled))
      {        
        clrT = D4D_ObjectGetForeFillColor(pMsg->pObject);
        clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);        
      }
      else
      {        
        D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pMsg->pObject);
                                  
        if(pMenu->pData->ix == pMenu->pData->item_select)
          clrT = pScheme_tmp->foreCapture;
        else
          clrT = pScheme_tmp->foreFocus;
       
        clrB = pScheme_tmp->bckgFocus;
  
      }
      
      // Draw menu item text
      if(pMenu->pItems[tmpB + pMenu->pData->focus_pos].text.pText != NULL)
      {
        tmp_point.x = (D4D_COOR)(_calc.position.x + _calc.maxIcon.cx + 4 + pMenu->radius / 2);
        tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff));
        
        tmp_size.cx = (D4D_COOR)(pMenu->scrSize.cx - (_calc.maxIcon.cx + 6 + pMenu->radius / 2 + ((pMenu->pScrlBrVer->flags->bits.bVisible)? D4D_MENU_SCRLBR_WIDTH:0)));
        tmp_size.cy = 0; // to invoke autosize capability in axis Y
        
        tmp_txtbuff.pText = pMenu->pItems[tmpB + pMenu->pData->focus_pos].text.pText;
        tmp_txtbuff.fontId = pMenu->itemsFontId;
        tmp_txtbuff.buffSize = (D4D_INDEX)(D4D_GetTextLength(tmp_txtbuff.pText) + 1);
        tmp_str_prty.font_properties.all = D4D_MENU_ITEM_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
        tmp_str_prty.text_properties.all = D4D_MENU_ITEM_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
        
        
        D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);        
      }
      
      // Draw menu item icon  
      if(_calc.maxIcon.cx)
      {        
        tmp_point.x = (D4D_COOR)(_calc.position.x + 3);
        
        if(_calc.maxIcon.cy < _calc.textOff)        
          tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff) + ((D4D_GetFontHeight(pMenu->itemsFontId) - _calc.maxIcon.cy) / 2));        
        else
          tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff));
        
        if(pMenu->pItems[tmpB + pMenu->pData->focus_pos].pIcon != NULL)
          D4D_DrawRBmp(&tmp_point, pMenu->pItems[tmpB + pMenu->pData->focus_pos].pIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pMenu->radius);
        else
          D4D_FillRRect(&tmp_point, &_calc.maxIcon, D4D_ObjectGetBckgFillColor(pMsg->pObject), pMenu->radius);
      }
    }

#else
    // draw just a color rectangle instead of bitmap
    if(draw.bits.bComplete)  
    {      
      if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
        D4D_Box(&_calc.position, &pMenu->scrSize, D4D_LINE_THIN, clrT, clrB);
      else
        D4D_FillRect(&_calc.position, &pMenu->scrSize, clrB);
      
      if(pMenu->pIcon != NULL)
        D4D_DrawBmpXY((D4D_COOR)(_calc.position.x + 3), (D4D_COOR)(_calc.position.y + 3), pMenu->pIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
      
      if(pMenu->title_text.pText)
      {                 
        tmp_point.x = (D4D_COOR)(_calc.position.x + 4);
        if(pMenu->pIcon != NULL)
          tmp_point.x += D4D_GetBmpHeight(pMenu->pIcon);
        
        tmp_point.y = (D4D_COOR)(_calc.position.y + 1 ); 
        D4D_DrawTextRect(&tmp_point, (D4D_SIZE*)&d4d_size_zero, &pMenu->title_text, clrT, clrB);        
      }
      
      D4D_MoveToXY((D4D_COOR)(_calc.position.x + 1),\
        (D4D_COOR)(_calc.position.y + _calc.titleBar_y));    
        
      D4D_LineToXY((D4D_COOR)(_calc.position.x + pMenu->scrSize.cx - 1),\
        (D4D_COOR)(_calc.position.y + _calc.titleBar_y), D4D_LINE_THIN, clrT);                
    
    }
    
    if(pMsg->pObject->initFlags & D4D_MENU_F_INDEX)
    {      
      // Draw index counter
      tmp_txtbuff.buffSize = (D4D_INDEX)(((*(pMenu->pItemsCnt)) < 10)? 4:6);
      tmp_size.cx = (D4D_COOR)((tmp_txtbuff.buffSize - 1) * D4D_GetFontWidth(pMenu->indexFontId) + 2);
      tmp_size.cy = (D4D_COOR)(_calc.titleBar_y - 2);
      tmp_point.x = (D4D_COOR)(_calc.position.x + pMenu->scrSize.cx - tmp_size.cx - 2);
      tmp_point.y = (D4D_COOR)(_calc.position.y + 1);
      
      tmp_txtbuff.pText = _calc.index_txt;
      tmp_txtbuff.fontId = pMenu->indexFontId;
      tmp_str_prty.font_properties.all = D4D_MENU_IX_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
      tmp_str_prty.text_properties.all = D4D_MENU_IX_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
      D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);
    }
    
    for(tmpB = 0; (tmpB < _calc.posCnt) && (tmpB < *(pMenu->pItemsCnt)); tmpB++)
    {
      if(((pMenu->pData->ix - pMenu->pData->focus_pos) != tmpB) || (!pMsg->pObject->flags->bits.bEnabled))
      {        
        clrT = D4D_ObjectGetForeFillColor(pMsg->pObject);
        clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);        
      }
      else
      {        
        D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pMsg->pObject);
                                  
        if(pMenu->pData->ix == pMenu->pData->item_select)
          clrT = pScheme_tmp->foreCapture;
        else
          clrT = pScheme_tmp->foreFocus;
       
        clrB = pScheme_tmp->bckgFocus;
  
      }
      
      // Draw menu item text
      if(pMenu->pItems[tmpB + pMenu->pData->focus_pos].text.pText != NULL)
      {
        tmp_point.x = (D4D_COOR)(_calc.position.x + _calc.maxIcon.cx + 4);
        tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff));
        
        tmp_size.cx = (D4D_COOR)(pMenu->scrSize.cx - (_calc.maxIcon.cx + 6 + ((pMenu->pScrlBrVer->flags->bits.bVisible)? D4D_MENU_SCRLBR_WIDTH:0)));
        tmp_size.cy = 0; // to invoke autosize capability in axis Y
        
        tmp_txtbuff.pText = pMenu->pItems[tmpB + pMenu->pData->focus_pos].text.pText;
        tmp_txtbuff.fontId = pMenu->itemsFontId;
        tmp_txtbuff.buffSize = (D4D_INDEX)(D4D_GetTextLength(tmp_txtbuff.pText) + 1);
        tmp_str_prty.font_properties.all = D4D_MENU_ITEM_FNT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
        tmp_str_prty.text_properties.all = D4D_MENU_ITEM_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT;
        
        
        D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, clrT, clrB);        
      }
      
      // Draw menu item icon  
      if(_calc.maxIcon.cx)
      {        
        tmp_point.x = (D4D_COOR)(_calc.position.x + 3);
        
        if(_calc.maxIcon.cy < _calc.textOff)        
          tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff) + ((D4D_GetFontHeight(pMenu->itemsFontId) - _calc.maxIcon.cy) / 2));        
        else
          tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 3 + (tmpB * _calc.textOff));
        
        if(pMenu->pItems[tmpB + pMenu->pData->focus_pos].pIcon != NULL)
          D4D_DrawBmp(&tmp_point, pMenu->pItems[tmpB + pMenu->pData->focus_pos].pIcon, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
        else
          D4D_FillRect(&tmp_point, &_calc.maxIcon, D4D_ObjectGetBckgFillColor(pMsg->pObject));
      }
    }  


#endif    
      
}

/*******************************************************
*
* MENU object focus next item routine
*
*******************************************************/
static void D4D_MenuFocusNextItem(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  D4D_MENU_INDEX tmpPosCnt = D4D_GetPositionCount(pThis);
  
  pMenu->pData->item_select = -1;
  pMenu->pData->ix++;
  
  if(*(pMenu->pItemsCnt) >= tmpPosCnt)
  {
    if(pMenu->pData->ix >= *(pMenu->pItemsCnt))
    {
      pMenu->pData->ix = 0;
      pMenu->pData->focus_pos = 0;
      pMenu->pData->item_select = -2;
    }else if(pMenu->pData->ix - pMenu->pData->focus_pos > (tmpPosCnt - 1))
    {
       pMenu->pData->focus_pos++;
       pMenu->pData->item_select = -2;        
    }
    D4D_ScrlBrSetPosition(pMenu->pScrlBrVer, pMenu->pData->focus_pos);
  } else
  {
    if(pMenu->pData->ix >= *(pMenu->pItemsCnt))
    {
      pMenu->pData->ix = 0;        
    }
    
  }  
}

/*******************************************************
*
* MENU object focus previous item routine
*
*******************************************************/
static void D4D_MenuFocusPreviousItem(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  D4D_MENU_INDEX tmpPosCnt = D4D_GetPositionCount(pThis);
  
  pMenu->pData->item_select = -1;
  pMenu->pData->ix--;
  
  if(*(pMenu->pItemsCnt) >= tmpPosCnt)
  {
   if(pMenu->pData->ix < 0)
   {
     pMenu->pData->ix = (D4D_MENU_INDEX)(*(pMenu->pItemsCnt) - 1);
     pMenu->pData->focus_pos = (D4D_MENU_INDEX)(*(pMenu->pItemsCnt) - tmpPosCnt);
     pMenu->pData->item_select = -2;
     
   }else if(pMenu->pData->ix - pMenu->pData->focus_pos < 0)
   {
      pMenu->pData->focus_pos--;
      pMenu->pData->item_select = -2;      
   }
   
   D4D_ScrlBrSetPosition(pMenu->pScrlBrVer, pMenu->pData->focus_pos);
  } else
  {
    if(pMenu->pData->ix < 0)
    {          
      pMenu->pData->ix = (D4D_MENU_INDEX)(*(pMenu->pItemsCnt) - 1);
    }
  }  
}


/*******************************************************
*
* MENU key handling routine
*
*******************************************************/

static void D4D_MenuOnKeyDown(D4D_MESSAGE* pMsg)    
{
    D4D_MENU* pMenu = D4D_GET_MENU(pMsg->pObject);   
    
  if(D4D_GetCapturedObject() == pMsg->pObject)
  {
    if(pMsg->prm.key == D4D_KEY_SCANCODE_DOWN)
    {      
      D4D_MenuFocusNextItem(pMsg->pObject);
      pMsg->pObject->flags->bits.bRedraw = 1;   


    } else if(pMsg->prm.key == D4D_KEY_SCANCODE_UP)
    {      
      D4D_MenuFocusPreviousItem(pMsg->pObject);
      pMsg->pObject->flags->bits.bRedraw = 1; 
        
    } else if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    {    
      pMenu->pData->item_select = pMenu->pData->ix;
      pMsg->pObject->flags->bits.bRedraw = 1;
    }
  }
}

/*******************************************************
*
* MENU key handling routine
*
*******************************************************/

static void D4D_MenuOnKeyUp(D4D_MESSAGE* pMsg)    
{
    D4D_MENU* pMenu = D4D_GET_MENU(pMsg->pObject);

    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    {
      if(D4D_GetCapturedObject() != pMsg->pObject)
      {
        D4D_CaptureKeys(pMsg->pObject);
      }
      else if(pMenu->pData->item_select >= 0)
      {
        if(pMenu->OnClicked)
          pMenu->OnClicked(pMsg->pObject, pMenu->pData->item_select);
        
        pMenu->pData->item_select = -1;
        pMsg->pObject->flags->bits.bRedraw = 1;
      }
    }
    
    if(pMsg->prm.key == D4D_KEY_SCANCODE_ESC)
    {
      D4D_CaptureKeys(NULL);  
    }  
}

/*******************************************************
*
* MENU touch screen handling routine
*
*******************************************************/
static void D4D_MenuOnTouch(D4D_MESSAGE* pMsg)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pMsg->pObject);
  
  if(D4D_GetFocusedObject(pMsg->pScreen) != pMsg->pObject)
  {    
    if(pMsg->nMsgId == D4D_MSG_TOUCHED)
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
  }else
  {
    
    D4D_MenuValue2Coor(pMsg->pObject);
    
    if(D4D_GetTouchScreenCoordinates().x < (pMenu->scrPos.x + pMenu->scrSize.cx - ((pMenu->pScrlBrVer->flags->bits.bVisible)? D4D_MENU_SCRLBR_WIDTH:0)))      
    {
      // Select Menu Item
      if(pMsg->nMsgId == D4D_MSG_TOUCHED)
      {
        D4D_COOR tmp_y;
        Byte tmpB;
        
        tmp_y = (D4D_COOR)(D4D_GetTouchScreenCoordinates().y - pMenu->scrPos.y);
        
        if(tmp_y > _calc.titleBar_y)
        {
          tmp_y -= _calc.titleBar_y;
          // now is in tmp_y offset of y from title bar 
          
          for(tmpB = 0; (tmpB < _calc.posCnt) && (tmpB < *(pMenu->pItemsCnt)); tmpB++)
          {
            if(tmp_y < _calc.textOff)
            {
              // founded touched line   
              if((pMenu->pData->ix - pMenu->pData->focus_pos) != tmpB)
              {
                // Touched line is not focused  
                pMenu->pData->ix = (D4D_MENU_INDEX)(pMenu->pData->focus_pos + tmpB);
              }else
              {
                // Touched line is focused
                pMenu->pData->ix = (D4D_MENU_INDEX)(pMenu->pData->focus_pos + tmpB);
                
                if(pMenu->OnClicked)
                  pMenu->OnClicked(pMsg->pObject, pMenu->pData->ix);
                
                pMenu->pData->item_select = -1;
                  
              }
              break;
            }
            tmp_y -= _calc.textOff;
          }
          pMsg->pObject->flags->bits.bRedraw = 1;    
        }
      }          
    } 
  }  
}

static D4D_BOOL D4D_MenuScrollBarSetup(D4D_OBJECT* pObject)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pObject);
  D4D_SCROLL_BAR* pScrlBr;
  
  
  
  // Try to check if the Menu needs scroll Bar
  D4D_MenuValue2Coor(pObject);
  
  // Switch off possible scroll bar in refresh case
  pMenu->pScrlBrVer->flags->bits.bVisible = 0;
   
  if((D4D_GetPositionCount(pObject) > *(pMenu->pItemsCnt)) || (pObject->initFlags & D4D_MENU_F_SIDEBAR))
  {
    pMenu->pScrlBrVer->flags->bits.bVisible = 1;
    
    // Init child objects coordinations
    pScrlBr = D4D_GET_SCROLL_BAR(pMenu->pScrlBrVer);
  
    pScrlBr->scrPos.x = (D4D_COOR)(_calc.position.x + pMenu->scrSize.cx  - (D4D_MENU_SCRLBR_WIDTH + 2));
    pScrlBr->scrPos.y = (D4D_COOR)(_calc.position.y + _calc.titleBar_y + 1);
    
    pScrlBr->scrPos = D4D_GetScreenToClientPoint( *pObject->pScreen, &pScrlBr->scrPos);
    
    pScrlBr->scrSize.cx = D4D_MENU_SCRLBR_WIDTH + 1;
    pScrlBr->scrSize.cy = (D4D_COOR)(pMenu->scrSize.cy - _calc.titleBar_y - 2);

    pScrlBr->radius = D4D_LimitU(pMenu->radius, 0, D4D_MENU_SCRLBR_WIDTH / 2);
    
    if(*(pMenu->pItemsCnt) < D4D_GetPositionCount(pObject))  
      D4D_ScrlBrSetRange(pMenu->pScrlBrVer, 0, D4D_GetPositionCount(pObject));
    else  
      D4D_ScrlBrSetRange(pMenu->pScrlBrVer, 0, *(pMenu->pItemsCnt));
    D4D_ScrlBrSetStep(pMenu->pScrlBrVer, D4D_GetPositionCount(pObject), D4D_MENU_SCRLBR_STEP);
    D4D_ScrlBrSetPosition(pMenu->pScrlBrVer, 0);
    return D4D_TRUE;
  }
  return D4D_FALSE;
}

static void D4D_MenuOnInit(D4D_MESSAGE* pMsg)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pMsg->pObject);

  // init screen pointer of child objects
  *(pMenu->pScrlBrVer->pScreen) = *(pMsg->pObject->pScreen);

  (void)D4D_MenuScrollBarSetup(pMsg->pObject);

  // set flag to redraw screen
  pMsg->pObject->flags->bits.bRedrawC = 1;
}

void D4D_MenuScrollBarsFeedBack(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position)
{
  D4D_MENU* pMenu = D4D_GET_MENU((D4D_OBJECT*)(pThis->userPointer));

  D4D_UNUSED(old_position);

  pMenu->pData->focus_pos = (D4D_MENU_INDEX)new_position;
  pMenu->pData->ix = (D4D_MENU_INDEX)new_position;
  pMenu->pData->item_select = -2;
  
  ((D4D_OBJECT*)(pThis->userPointer))->flags->bits.bRedraw = 1;  

}

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


/*******************************************************
*
* The MENU Get index function 
*
*******************************************************/

D4D_MENU_INDEX D4D_MenuGetIndex(D4D_OBJECT* pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  
  if(pThis == NULL)
    return 0;
  
  return pMenu->pData->ix;
  
}

/*******************************************************
*
* The MENU Set index function 
*
*******************************************************/

void  D4D_MenuSetIndex(D4D_OBJECT* pThis, D4D_MENU_INDEX ix)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  
  if(pThis == NULL)
    return;
  
  if(ix > (D4D_MENU_INDEX)*pMenu->pItemsCnt)
    ix = (D4D_MENU_INDEX)*pMenu->pItemsCnt;
  
  pMenu->pData->ix = ix;
  pMenu->pData->focus_pos = ix;
  pMenu->pData->item_select = -2;
  
  D4D_ScrlBrSetPosition(pMenu->pScrlBrVer, pMenu->pData->focus_pos);
  
}

/*******************************************************
*
* Get MENU item count
*
*******************************************************/

D4D_MENU_INDEX D4D_MenuGetItemCount(D4D_OBJECT_PTR pThis)
{
   D4D_MENU* pMenu = D4D_GET_MENU(pThis);
   
   return *pMenu->pItemsCnt;
}

/*******************************************************
*
* The MENU find Item by user data function 
*
*******************************************************/
D4D_MENU_INDEX D4D_MenuFindUserDataItem(D4D_OBJECT_PTR pThis, void* pUser)
{
    D4D_MENU* pMenu = D4D_GET_MENU(pThis);
    D4D_MENU_INDEX i_max = D4D_MenuGetItemCount(pThis);
    D4D_MENU_INDEX i;
    
    for(i=0;i< i_max;i++)
    {
      if(pMenu->pItems[i].pUser == pUser)
        return i;
    }
    
    return -1;     
}

/*******************************************************
*
* The MENU get Item user data function 
*
*******************************************************/
void* D4D_MenuGetItemUserData(D4D_OBJECT_PTR pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  return pMenu->pItems[pMenu->pData->ix].pUser;
}

/*******************************************************
*
* The MENU get Item Text function 
*
*******************************************************/
D4D_STRING* D4D_MenuGetItemText(D4D_OBJECT_PTR pThis)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);
  return (D4D_STRING*)&(pMenu->pItems[pMenu->pData->ix].text);
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
* The MENU message handler 
*
*******************************************************/

void D4D_MenuOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
      case D4D_MSG_DRAW:
        D4D_MenuOnDraw(pMsg);
        break;
        
      case D4D_MSG_KEYDOWN:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_MenuOnKeyDown(pMsg);
        break;
      
      case D4D_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_MenuOnKeyUp(pMsg);
        break;        
      
      case D4D_MSG_TOUCHED:
      case D4D_MSG_TOUCH_AUTO:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_MenuOnTouch(pMsg);         
        break;
      
      case D4D_MSG_ONINIT:
        D4D_MenuOnInit(pMsg);
        break;

      default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
    }
    
  // send the message to child object
  // Vertical scroll bar
  SendLocalMessage(pMsg, (D4D_OBJECT*)(D4D_GET_MENU(pMsg->pObject))->pScrlBrVer);
}

/*******************************************************
*
* The MENU check coordinators  handler 
*
*******************************************************/

Byte D4D_MenuCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_MENU* pMenu = D4D_GET_MENU(pThis);  
  
  if((point.x >= pMenu->scrPos.x) && (point.y >= pMenu->scrPos.y))
  {     
    if((point.x <= (pMenu->scrPos.x + pMenu->scrSize.cx)) && (point.y <= (pMenu->scrPos.y + pMenu->scrSize.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_MenuGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_MENU(pThis)->title_text);  
}