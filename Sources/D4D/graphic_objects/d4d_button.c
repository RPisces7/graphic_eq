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
* @file      d4d_button.c
*
* @author    r50233
* 
* @version   0.0.39.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver button object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_BtnOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_BtnCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_BtnGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_btnSysFunc = 
{
  D4D_OBJECT_BUTTON,
  D4D_BtnOnMessage,
  D4D_BtnCheckCoordinates,
  D4D_BtnGetTextBuffer
};

typedef struct 
{
    D4D_POINT Position;
    D4D_SIZE Size;
} D4D_BTN_TMP_VAL;

#define _calc (*((D4D_BTN_TMP_VAL*)d4d_scratchPad))

    
static void D4D_BtnValue2Coor(D4D_OBJECT* pThis)
{
  D4D_BUTTON* pBtn = D4D_GET_BUTTON(pThis);
   
  _calc.Size = pBtn->scrSize;
  _calc.Position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pBtn->scrPos);
  
#if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
        
    //Check Size of button in axis X   
    if(!_calc.Size.cx)
    {
      if(D4D_GetBmpWidth(pBtn->pBmpNormal) > _calc.Size.cx)
         _calc.Size.cx = D4D_GetBmpWidth(pBtn->pBmpNormal);
      
      if(D4D_GetBmpWidth(pBtn->pBmpFocus) > _calc.Size.cx)
         _calc.Size.cx = D4D_GetBmpWidth(pBtn->pBmpFocus);
      
      
      if((D4D_GetFontWidth(pBtn->textBuff.fontId) * (pBtn->textBuff.buffSize - 1) + D4D_BTN_BORDER_OFFSET) > _calc.Size.cx)
         _calc.Size.cx = (D4D_COOR)(D4D_GetFontWidth(pBtn->textBuff.fontId) * (pBtn->textBuff.buffSize - 1) + D4D_BTN_BORDER_OFFSET);
      
      _calc.Size.cx++;
    }
    
    //Check Size of button in axis Y
    
    if(!_calc.Size.cy)
    {
      if(D4D_GetBmpHeight(pBtn->pBmpNormal) > _calc.Size.cy)
         _calc.Size.cy = D4D_GetBmpHeight(pBtn->pBmpNormal);
      
      if(D4D_GetBmpHeight(pBtn->pBmpFocus) > _calc.Size.cy)
         _calc.Size.cy = D4D_GetBmpHeight(pBtn->pBmpFocus);
      
      
      if((D4D_GetFontHeight(pBtn->textBuff.fontId) + D4D_BTN_BORDER_OFFSET) > _calc.Size.cy)
         _calc.Size.cy = (D4D_COOR)(D4D_GetFontHeight(pBtn->textBuff.fontId) + D4D_BTN_BORDER_OFFSET);
      
      _calc.Size.cy++;
    }
    
#endif

  
}


/*******************************************************
*
* BUTTON Drawing routine
*
*******************************************************/

static void D4D_BtnOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_BUTTON* pBtn = D4D_GET_BUTTON(pMsg->pObject);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;    
    const D4D_BMP* pBmp = pBtn->pBmpNormal;
    
    D4D_BtnValue2Coor(pMsg->pObject);
    
    // focused bitmap (may be also NULL)

    clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
    clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);  
     
    if((draw.bits.bFocused) && (pBtn->pBmpFocus))
    {                                                  
      pBmp = pBtn->pBmpFocus;  
    }
      
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    // normal bitmap (also may be NULL if simple rect button is needed  
    if(pBmp != NULL)
    {
        // draw the bitmap
        D4D_DrawRBmp(&_calc.Position, pBmp, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pBtn->radius);
       
        if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
        {          
          D4D_RRect(&_calc.Position, &_calc.Size, D4D_LINE_THIN, clrT, pBtn->radius);
        }
    }
    // no bitmap
    else
    {
        // draw just a color rectangle instead of bitmap
        D4D_RBox(&_calc.Position, &_calc.Size, D4D_LINE_THIN, clrT, clrB, pBtn->radius);        
    }

    // draw the text
    if(pBtn->textBuff.pText)
    {        
       pBtn->textBuff.str_properties->font_properties.bits.bTransparent = D4D_TRUE;
       
       _calc.Position.x++;
       _calc.Position.y++;
       _calc.Size.cx -= 2;
       _calc.Size.cy -= 2;         
       D4D_DrawTextRect(&_calc.Position, &_calc.Size,&pBtn->textBuff, clrT, clrB);
    }
#else
    // normal bitmap (also may be NULL if simple rect button is needed  
    if(pBmp != NULL)
    {
        // draw the bitmap
        D4D_DrawBmp(&_calc.Position, pBmp, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
       
        if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
        {          
          D4D_Rect(&_calc.Position, &_calc.Size, D4D_LINE_THIN, clrT);
        }
    }
    // no bitmap
    else
    {
        // draw just a color rectangle instead of bitmap
        D4D_Box(&_calc.Position, &_calc.Size, D4D_LINE_THIN, clrT, clrB);        
    }

    // draw the text
    if(pBtn->textBuff.pText)
    {        
       _calc.Position.x++;
       _calc.Position.y++;
       _calc.Size.cx -= 2;
       _calc.Size.cy -= 2;         
       D4D_DrawTextRect(&_calc.Position, &_calc.Size,&pBtn->textBuff, clrT, clrB);
    }

#endif    
}

/*******************************************************
*
* BUTTON key handling routine
*
*******************************************************/

static void D4D_BtnOnKeyDown(D4D_MESSAGE* pMsg)    
{                
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
  {      
    D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);
    pStatus->bits.bPressed = 1;
    pMsg->pObject->flags->bits.bRedraw = 1;  
    
  }  
}

/*******************************************************
*
* BUTTON key handling routine
*
*******************************************************/

static void D4D_BtnOnKeyUp(D4D_MESSAGE* pMsg)    
{
                 
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4D_KEY_SCANCODE_ENTER)
  {
    D4D_BUTTON* pBtn = D4D_GET_BUTTON(pMsg->pObject);
    D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);
    
    if(pStatus->bits.bPressed)
    {
      pStatus->bits.bPressed = 0;
      if(pBtn->OnClicked)
          pBtn->OnClicked(pMsg->pObject);
    }
    
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
}

/*******************************************************
*
* BUTTON focus handling routine
*
*******************************************************/

static void D4D_BtnKillFocus(D4D_MESSAGE* pMsg)
{
  D4D_BUTTON_STATUS* pStatus = D4D_GET_BUTTON_STATUS(pMsg->pObject);
  
  if(pStatus->bits.bPressed)
  {    
    pStatus->bits.bPressed = 0;
    pMsg->pObject->flags->bits.bRedraw = 1;
  }
  
}

/*******************************************************
*
* BUTTON Touched Button handling routine
*
*******************************************************/

static void D4D_BtnTouched(D4D_MESSAGE* pMsg)
{ 
  if((pMsg->pObject->flags->bits.bTabStop) || (pMsg->pObject->flags->bits.bFastTouch))
  {    
    if((D4D_GetFocusedObject(pMsg->pScreen) == pMsg->pObject) || (pMsg->pObject->flags->bits.bFastTouch))
    {    
      D4D_BUTTON* pBtn = D4D_GET_BUTTON(pMsg->pObject);
      if(pBtn->OnClicked)
        pBtn->OnClicked(pMsg->pObject);  
    }
    D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
  }
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
* The main BUTTON message handler 
*
*******************************************************/

void D4D_BtnOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_BtnOnDraw(pMsg);
        break;
        
    case D4D_MSG_KEYDOWN:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_BtnOnKeyDown(pMsg);
        break;

    case D4D_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_BtnOnKeyUp(pMsg);
        break;
        
    case D4D_MSG_KILLFOCUS:
        D4D_BtnKillFocus(pMsg);
        break;
    
    case D4D_MSG_TOUCHED:     
      if(pMsg->pObject->flags->bits.bEnabled)
        D4D_BtnTouched(pMsg);
      break;

    default:
        // call the default behavior
        D4D_ObjOnMessage(pMsg);
    }
}

/*******************************************************
*
* The BUTTON check coordinators  handler 
*
*******************************************************/

Byte D4D_BtnCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_BUTTON* pBtn = D4D_GET_BUTTON(pThis);
  
  D4D_BtnValue2Coor(pThis);
  
  if((point.x >= pBtn->scrPos.x) && (point.y >= pBtn->scrPos.y))
  { 
    if((point.x <= (pBtn->scrPos.x + _calc.Size.cx)) && (point.y <= (pBtn->scrPos.y + _calc.Size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}



/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_BtnGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_BUTTON(pThis)->textBuff);  
}
