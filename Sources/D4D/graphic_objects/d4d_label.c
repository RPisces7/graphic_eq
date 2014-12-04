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
* @file      d4d_label.c
*
* @author    b01119
* 
* @version   0.0.33.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver label object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"


// Internal API
void D4D_LabelOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_LabelCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_LabelGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_labelSysFunc = 
{
  D4D_OBJECT_LABEL,
  D4D_LabelOnMessage,
  D4D_LabelCheckCoordinates,
  D4D_LabelGetTextBuffer
};

typedef struct 
{
  D4D_POINT position;
  D4D_SIZE size;
} D4D_LBL_TMP_VAL;

#define _calc (*((D4D_LBL_TMP_VAL*)d4d_scratchPad))

    
static void D4D_LblValue2Coor(D4D_OBJECT* pThis)
{
  D4D_LABEL* pLbl = D4D_GET_LABEL(pThis);

  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pLbl->scrPos);
  _calc.size = pLbl->scrSize;
   
 #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE      
      if(!_calc.size.cx)
      {
        // compute size of label object automaticcaly
        _calc.size.cx = (D4D_COOR) (D4D_GetFontWidth(pLbl->textBuff.fontId) * (pLbl->textBuff.buffSize - 1) + 4 + pLbl->radius);         
      }
      
      if(!_calc.size.cy)
      {
        // compute size of label object automaticcaly
        _calc.size.cy = (D4D_COOR) (D4D_GetFontHeight(pLbl->textBuff.fontId) + 4 + pLbl->radius);        
      }
#else
      if(!_calc.size.cx)
      {
        // compute size of label object automaticcaly
        _calc.size.cx = (D4D_COOR) (D4D_GetFontWidth(pLbl->textBuff.fontId) * (pLbl->textBuff.buffSize - 1) + 4);         
      }
      
      if(!_calc.size.cy)
      {
        // compute size of label object automaticcaly
        _calc.size.cy = (D4D_COOR) (D4D_GetFontHeight(pLbl->textBuff.fontId) + 4);        
      }
#endif      
      
 #endif
}
 


/*******************************************************
*
* LABEL Drawing routine
*
*******************************************************/

static void D4D_LabelOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_LABEL* pLbl = D4D_GET_LABEL(pMsg->pObject);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    
    D4D_LblValue2Coor(pMsg->pObject);
        
    clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
    clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);  
    
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    // draw the rectangle around the text
    if(draw.bits.bComplete)  
    {
      if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
       D4D_RRect(&_calc.position, &_calc.size, D4D_LINE_THIN, clrT, pLbl->radius);
    }
  
    // draw the text
    if(pLbl->textBuff.pText)
    {
      if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
      {        
        _calc.position.x +=(D4D_COOR)(1 + pLbl->radius / 2);
        _calc.position.y +=(D4D_COOR)(1 + pLbl->radius / 2);
        _calc.size.cx -= (D4D_COOR)(2 + pLbl->radius / 2);
        _calc.size.cy -= (D4D_COOR)(2 + pLbl->radius / 2);
      }      
      D4D_DrawTextRect(&_calc.position, &_calc.size, &pLbl->textBuff, clrT, clrB);
    }
#else
    // draw the rectangle around the text
    if(draw.bits.bComplete)  
    {
      if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
       D4D_Rect(&_calc.position, &_calc.size, D4D_LINE_THIN, clrT);
    }
  
    // draw the text
    if(pLbl->textBuff.pText)
    {
      if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
      {        
        _calc.position.x++;
        _calc.position.y++;
        _calc.size.cx -= 2;
        _calc.size.cy -= 2;
      }      
      D4D_DrawTextRect(&_calc.position, &_calc.size, &pLbl->textBuff, clrT, clrB);
    }
#endif    
    
    
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
* The LABEL message handler 
*
*******************************************************/

void D4D_LabelOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_LabelOnDraw(pMsg);
        break;
    case D4D_MSG_TOUCHED:                              
        D4D_FocusSet(pMsg->pScreen, pMsg->pObject);       
        break;
    default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
    }
}

/*******************************************************
*
* The LABEL check coordinators  handler 
*
*******************************************************/

Byte D4D_LabelCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_LABEL* pLbl = D4D_GET_LABEL(pThis);
  D4D_LblValue2Coor(pThis);
  
  if((point.x >= pLbl->scrPos.x) && (point.y >= pLbl->scrPos.y))
  {
      
    if((point.x <= (pLbl->scrPos.x + _calc.size.cx)) && (point.y <= (pLbl->scrPos.y + _calc.size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_LabelGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_LABEL(pThis)->textBuff);  
}
