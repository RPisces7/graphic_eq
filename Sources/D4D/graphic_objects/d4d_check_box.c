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
* @file      d4d_check_box.c
*
* @author    b01119
* 
* @version   0.0.29.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver check box object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_CheckBoxOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_CheckBoxCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_CheckBoxGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_checkBoxSysFunc = 
{
  D4D_OBJECT_CHECKBOX,
  D4D_CheckBoxOnMessage,
  D4D_CheckBoxCheckCoordinates,
  D4D_CheckBoxGetTextBuffer
};

typedef struct 
{
    D4D_POINT Position;
    D4D_SIZE Size;
    D4D_POINT TxtPos;
    D4D_SIZE TxtSize;
    D4D_POINT BmpPos;
    D4D_SIZE  SizeBmpEnabled;
} D4D_CHECKBOX_TMP_VAL;

#define _calc (*((D4D_CHECKBOX_TMP_VAL*)d4d_scratchPad))

    
static void D4D_CheckBoxValue2Coor(D4D_OBJECT* pThis)
{
  D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pThis);
  D4D_COOR tmp_BorderOffset;
  _calc.Size = pCheckB->scrSize;
  _calc.Position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pCheckB->scrPos);
  _calc.SizeBmpEnabled = D4D_GetBmpSize(pCheckB->pBmpChecked);
  
  
#if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
        
    //Check Size of button in axis Y
    
    if(!_calc.Size.cy)
    {
     // if(_calc.SizeBmpEnabled.cy > _calc.Size.cy)
      _calc.Size.cy = (D4D_COOR)(_calc.SizeBmpEnabled.cy + D4D_CHECKBOX_BORDER_OFFSET);
      
      if(pCheckB->textBuff.pText && pCheckB->textBuff.buffSize)
        if((D4D_GetFontHeight(pCheckB->textBuff.fontId) + D4D_CHECKBOX_BORDER_OFFSET) > _calc.Size.cy)
           _calc.Size.cy = (D4D_COOR)(D4D_GetFontHeight(pCheckB->textBuff.fontId) + D4D_CHECKBOX_BORDER_OFFSET);      
      
      _calc.Size.cy += 1; 
    }
    
    tmp_BorderOffset = (D4D_COOR)((_calc.Size.cy - _calc.SizeBmpEnabled.cy) / 2);
    
        //Check Size of button in axis X   
    if(!_calc.Size.cx)
    {
      _calc.Size.cx = (D4D_COOR)(_calc.SizeBmpEnabled.cx + tmp_BorderOffset * 2);
      
      if(pCheckB->textBuff.pText && pCheckB->textBuff.buffSize)
        _calc.Size.cx += (D4D_COOR)(D4D_CHECKBOX_TEXT_OFFSET + D4D_GetFontWidth(pCheckB->textBuff.fontId) * (pCheckB->textBuff.buffSize - 1));
    }
  
#else   
    tmp_BorderOffset = (D4D_COOR)((_calc.Size.cy - _calc.SizeBmpEnabled.cy) / 2);
#endif

    _calc.BmpPos.x = (D4D_COOR)(_calc.Position.x + tmp_BorderOffset);
    _calc.BmpPos.y = (D4D_COOR)(_calc.Position.y + tmp_BorderOffset);

    if(pCheckB->textBuff.pText && pCheckB->textBuff.buffSize)
    {
      _calc.TxtPos.x = (D4D_COOR) (_calc.BmpPos.x + _calc.SizeBmpEnabled.cx + D4D_CHECKBOX_TEXT_OFFSET);      
      _calc.TxtPos.y = (D4D_COOR) (_calc.Position.y + 1);
      _calc.TxtSize.cx = (D4D_COOR)(_calc.Size.cx - (_calc.TxtPos.x - _calc.Position.x + 1));      
      _calc.TxtSize.cy = (D4D_COOR) (_calc.Size.cy - 2);
      
    }
    
  

}


/*******************************************************
*
* CHECK BOX Drawing routine
*
*******************************************************/

static void D4D_CheckBoxOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pMsg->pObject);
    D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);    
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;    
    D4D_COLOR clrT, clrB;
    
    D4D_CheckBoxValue2Coor(pMsg->pObject);
    
    clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
    clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    
    if(draw.bits.bComplete)
    {
      if(!(pMsg->pObject->initFlags & D4D_OBJECT_F_TRANSP_TEXT))
        D4D_FillRRect(&_calc.Position, &_calc.Size, clrB, pCheckB->radius);  
    }
    
    clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);
 
    // normal bitmap (also may be NULL if simple rect button is needed  
    if(pStatus->bits.bChecked)
    {
      if(pCheckB->pBmpChecked != NULL)         
        D4D_DrawRBmp(&_calc.BmpPos, pCheckB->pBmpChecked, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pCheckB->radius);  // draw the bitmap         
    }    
    // no bitmap
    else
    {     
      if(pCheckB->pBmpUnChecked)
      {
        // draw the bitmap
        D4D_DrawRBmp(&_calc.BmpPos, pCheckB->pBmpUnChecked, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pCheckB->radius);   
      }else
      {        
        // draw just a color rectangle instead of bitmap
        D4D_CLR_SCHEME* pScheme = D4D_ObjectGetScheme(pMsg->pObject);
        D4D_FillRRect(&_calc.BmpPos, &_calc.SizeBmpEnabled, pScheme->objectDepend.checkBox.iconBckg, pCheckB->radius);        
      }
    }
    
    if(pMsg->pObject->initFlags & D4D_CHECKBOX_F_ICON_RECTANGLE)
      //D4D_RRectXY((D4D_COOR)(_calc.BmpPos.x - 1), (D4D_COOR)(_calc.BmpPos.y - 1), (D4D_COOR)(_calc.BmpPos.x +_calc.SizeBmpEnabled.cx + 0), (D4D_COOR)(_calc.BmpPos.y + _calc.SizeBmpEnabled.cy + 0), D4D_LINE_THIN, clrT, pCheckB->radius);
      D4D_RRect(&_calc.BmpPos, &_calc.SizeBmpEnabled, D4D_LINE_THIN, clrT, pCheckB->radius);
    
    if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)          
      D4D_RRect(&_calc.Position, &_calc.Size, D4D_LINE_THIN, clrT, pCheckB->radius);
    
#else    
    
    if(draw.bits.bComplete)
    {
      if(!(pMsg->pObject->initFlags & D4D_OBJECT_F_TRANSP_TEXT))
        D4D_FillRect(&_calc.Position, &_calc.Size, clrB);  
    }
    
    clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);
 
    // normal bitmap (also may be NULL if simple rect button is needed  
    if(pStatus->bits.bChecked)
    {
      if(pCheckB->pBmpChecked != NULL)         
        D4D_DrawBmp(&_calc.BmpPos, pCheckB->pBmpChecked, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);  // draw the bitmap         
    }    
    // no bitmap
    else
    {     
      if(pCheckB->pBmpUnChecked)
      {
        // draw the bitmap
        D4D_DrawBmp(&_calc.BmpPos, pCheckB->pBmpUnChecked, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);   
      }else
      {        
        // draw just a color rectangle instead of bitmap
        D4D_CLR_SCHEME* pScheme = D4D_ObjectGetScheme(pMsg->pObject);
        D4D_FillRect(&_calc.BmpPos, &_calc.SizeBmpEnabled, pScheme->objectDepend.checkBox.iconBckg);        
      }
    }
    
    if(pMsg->pObject->initFlags & D4D_CHECKBOX_F_ICON_RECTANGLE)
      //D4D_RectXY((D4D_COOR)(_calc.BmpPos.x - 1), (D4D_COOR)(_calc.BmpPos.y - 1), (D4D_COOR)(_calc.BmpPos.x +_calc.SizeBmpEnabled.cx + 0), (D4D_COOR)(_calc.BmpPos.y + _calc.SizeBmpEnabled.cy + 0), D4D_LINE_THIN, clrT);
      D4D_Rect(&_calc.BmpPos, &_calc.SizeBmpEnabled, D4D_LINE_THIN, clrT);
    
    if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)          
      D4D_Rect(&_calc.Position, &_calc.Size, D4D_LINE_THIN, clrT);

    
#endif    

    // draw the text
    if(pCheckB->textBuff.pText && pCheckB->textBuff.buffSize)
      D4D_DrawTextRect(&_calc.TxtPos, &_calc.TxtSize, &pCheckB->textBuff, clrT, clrB);
    
}

/*******************************************************
*
* CHECK BOX key handling routine
*
*******************************************************/

static void D4D_CheckBoxOnKeyDown(D4D_MESSAGE* pMsg)    
{
    D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);
    
    // invoke "Click" event when enter key is pressed
    if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
    {      
      pStatus->bits.bPressed = 1;
      pMsg->pObject->flags->bits.bRedraw = 1;
    }
}

/*******************************************************
*
* CHECK BOX key handling routine
*
*******************************************************/

static void D4D_CheckBoxOnKeyUp(D4D_MESSAGE* pMsg)    
{
  D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pMsg->pObject);
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);
  
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER && pMsg->pObject->flags->bits.bEnabled)
  {
    if(pStatus->bits.bPressed)
    {
      pStatus->bits.bPressed = 0;      
        pStatus->bits.bChecked = !pStatus->bits.bChecked;  
        if(pCheckB->OnChange)
          pCheckB->OnChange(pMsg->pObject);            
    }
    pMsg->pObject->flags->bits.bRedraw = 1; 
  } 
}

/*******************************************************
*
* CHECK BOX focus handling routine
*
*******************************************************/

static void D4D_CheckBoxKillFocus(D4D_MESSAGE* pMsg)
{
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);
  
  if(pStatus->bits.bPressed)
  {    
    pStatus->bits.bPressed = 0;
    pMsg->pObject->flags->bits.bRedraw = 1;
  }
  
}

/*******************************************************
*
* CHECK BOX Touched Button handling routine
*
*******************************************************/

static void D4D_CheckBoxTouched(D4D_MESSAGE* pMsg)
{  
  if((pMsg->pObject->flags->bits.bTabStop) || (pMsg->pObject->flags->bits.bFastTouch))
  {
    if((D4D_GetFocusedObject(pMsg->pScreen) == pMsg->pObject) || (pMsg->pObject->flags->bits.bFastTouch))
    {               
      D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pMsg->pObject);
      D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pMsg->pObject);

      pStatus->bits.bChecked = !pStatus->bits.bChecked;
      if(pCheckB->OnChange)
          pCheckB->OnChange(pMsg->pObject);
      
      pMsg->pObject->flags->bits.bRedraw = 1;
    }                    
    D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
  }
}

/*******************************************************
*
* Set CHECK BOX value
*
*******************************************************/

void D4D_CheckBoxSetValue(D4D_OBJECT_PTR pThis, Byte value)
{
  D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pThis);
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pThis);
  
  if(!pThis->flags->bits.bEnabled)
    return ;
    
  if((pStatus->bits.bChecked && value) || (!pStatus->bits.bChecked && !value))
    return ;
  
  // remember new value
  pStatus->bits.bChecked = (value)? 1:0;
    
  D4D_InvalidateObject(pThis, D4D_TRUE);

  // notify user that the value has changed        
  if(pCheckB->OnChange)
    pCheckB->OnChange((D4D_OBJECT*)pThis);
}

Byte D4D_CheckBoxGetValue(D4D_OBJECT_PTR pThis)
{
  D4D_CHECKBOX_STATUS* pStatus = D4D_GET_CHECKBOX_STATUS(pThis);
  
  return (Byte)pStatus->bits.bChecked;
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
* The main CHECK BOX message handler 
*
*******************************************************/

void D4D_CheckBoxOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_CheckBoxOnDraw(pMsg);
        break;
        
    case D4D_MSG_KEYDOWN:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_CheckBoxOnKeyDown(pMsg);
        break;

    case D4D_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_CheckBoxOnKeyUp(pMsg);
        break;
        
    case D4D_MSG_KILLFOCUS:
        D4D_CheckBoxKillFocus(pMsg);
        break;
    
    case D4D_MSG_TOUCHED:     
      if(pMsg->pObject->flags->bits.bEnabled)  
        D4D_CheckBoxTouched(pMsg);
      break;

    default:
        // call the default behavior
        D4D_ObjOnMessage(pMsg);
    }
}

/*******************************************************
*
* The CHECK BOX check coordinators  handler 
*
*******************************************************/

Byte D4D_CheckBoxCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_CHECKBOX* pCheckB = D4D_GET_CHECKBOX(pThis);
  
  D4D_CheckBoxValue2Coor(pThis);
  
  if((point.x >= pCheckB->scrPos.x) && (point.y >= pCheckB->scrPos.y))
  {     
    if((point.x <= (pCheckB->scrPos.x + _calc.Size.cx)) && (point.y <= (pCheckB->scrPos.y + _calc.Size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_CheckBoxGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_CHECKBOX(pThis)->textBuff);  
}