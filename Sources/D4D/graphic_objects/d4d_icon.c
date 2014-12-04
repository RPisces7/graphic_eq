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
* @file      d4d_icon.c
*
* @author    b01119
* 
* @version   0.0.35.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D driver icon object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_IconOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_IconCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_IconGetTextBuffer(D4D_OBJECT* pThis);
D4D_ICON_INDEX D4D_IconGetBmpCount(D4D_OBJECT_PTR pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_iconSysFunc = 
{
  D4D_OBJECT_ICON,
  D4D_IconOnMessage,
  D4D_IconCheckCoordinates,
  D4D_IconGetTextBuffer
};

// temporary structure for icon coordinate calculation
typedef struct 
{
    D4D_POINT position;
    D4D_SIZE size;
    D4D_POINT txtPos;
} D4D_ICON_TMP_VAL;
  
#define _calc (*((D4D_ICON_TMP_VAL*)d4d_scratchPad))

    
static void D4D_IconValue2Coor(D4D_OBJECT* pThis)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  _calc.size = pIcon->scrSize;
  
  
  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pIcon->scrPos);
  _calc.txtPos = _calc.position;
  
  #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
    
    if(!_calc.size.cx)
    {
      _calc.size.cx = D4D_GetBmpWidth(pIcon->pBmpX[pIcon->pData->index]);
    }
    
    if(!_calc.size.cy)
    {
      _calc.size.cy = D4D_GetBmpHeight(pIcon->pBmpX[pIcon->pData->index]);
    }
    
    if(!pIcon->txtOff.x)
    {
      _calc.txtPos.x += (D4D_COOR)((_calc.size.cx - (D4D_GetFontWidth(pIcon->textBuff.fontId) * (pIcon->textBuff.buffSize - 1)))/2);      
    }else
      _calc.txtPos.x += pIcon->txtOff.x;
    
    if(!pIcon->txtOff.y)
    {
      _calc.txtPos.y += (D4D_COOR)(_calc.size.cy + 2);      
    }else
      _calc.txtPos.y += pIcon->txtOff.y;

#endif

  
}

/*******************************************************
*
* ICON Drawing routine
*
*******************************************************/

static void D4D_IconOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_ICON* pIcon = D4D_GET_ICON(pMsg->pObject);    
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_IconValue2Coor(pMsg->pObject);
    
    clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
    clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);
      
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE    
    // when background redraw is needed
        // draw the bitmap        
    if(pIcon->pBmpX[pIcon->pData->index] != NULL)
      D4D_DrawRBmp(&_calc.position, pIcon->pBmpX[pIcon->pData->index], (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pIcon->radius);
    
    // draw the text
    if(pIcon->textBuff.pText != NULL)
      D4D_DrawTextRect(&_calc.txtPos, (D4D_SIZE*)&d4d_size_zero, &pIcon->textBuff, clrT, clrB); 

    if (pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)       
      D4D_RRect(&_calc.position, &_calc.size, D4D_LINE_THIN, clrT, pIcon->radius);  // draw focus rectangle
#else
    // when background redraw is needed
        // draw the bitmap        
    if(pIcon->pBmpX[pIcon->pData->index] != NULL)
      D4D_DrawBmp(&_calc.position, pIcon->pBmpX[pIcon->pData->index], (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
    
    // draw the text
    if(pIcon->textBuff.pText != NULL)
      D4D_DrawTextRect(&_calc.txtPos, (D4D_SIZE*)&d4d_size_zero, &pIcon->textBuff, clrT, clrB); 

    if (pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)       
      D4D_Rect(&_calc.position, &_calc.size, D4D_LINE_THIN, clrT);  // draw focus rectangle

#endif

}

/*******************************************************
*
* Get ICON count
*
*******************************************************/

D4D_ICON_INDEX D4D_IconGetBmpCount(D4D_OBJECT_PTR pThis)
{
   D4D_ICON* pIcon = D4D_GET_ICON(pThis);
   D4D_ICON_INDEX  ix = 0;
   
   
   while(pIcon->pBmpX[ix] != NULL)
   {
     ix++;  
   }
   
   return ix;
}

/*******************************************************
*
* Set ICON value
*
*******************************************************/

void D4D_IconSetIndex(D4D_OBJECT_PTR pThis, D4D_ICON_INDEX index)
{
    D4D_ICON* pIcon = D4D_GET_ICON(pThis);
    D4D_ICON_DATA* pData = pIcon->pData;

    if(pData->index == index)
        return ;
    
    if(D4D_IconGetBmpCount(pThis) == 0)  // There is no loaded bitmaps - go out
        return;
    
    if(index >= D4D_IconGetBmpCount(pThis))  // If value is bigger then count of BMPs
        return;
    
    // remember new value
    pData->index = index;
    
    D4D_InvalidateObject(pThis, D4D_TRUE);

    // notify user that the value has changed        
    if(pIcon->OnValueChanged)
        pIcon->OnValueChanged(pThis);
}


/*******************************************************
*
* Change ICON value
*
*******************************************************/

void D4D_IconChangeIndex(D4D_OBJECT_PTR pThis, sByte incr)
{
  D4D_ICON_INDEX tmp_ix = (D4D_ICON_INDEX)(D4D_GET_ICON(pThis)->pData->index + incr);
  
  if(tmp_ix >= D4D_IconGetBmpCount(pThis))
    tmp_ix = 0;
  
  D4D_IconSetIndex(pThis, tmp_ix);
}


D4D_ICON_INDEX D4D_IconGetIndex(D4D_OBJECT_PTR pThis)
{
    D4D_ICON* pIcon = D4D_GET_ICON(pThis);
    return pIcon->pData->index;
}

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE

/*******************************************************
*
* ICON automatic animation control - Start
*
*******************************************************/
void D4D_IconAnimationStart(D4D_OBJECT_PTR pThis)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  D4D_ICON_DATA* pData = pIcon->pData;

  if(D4D_IconGetBmpCount(pThis))
  {
    pData->animationEnabled = D4D_TRUE;
    pData->tickCounter = 0;
  }
}

/*******************************************************
*
* ICON automatic animation control - Stop
*
*******************************************************/

void D4D_IconAnimationStop(D4D_OBJECT_PTR pThis)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  D4D_ICON_DATA* pData = pIcon->pData;

  pData->animationEnabled = D4D_FALSE;
}

#endif

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
* The main ICON message handler 
*
*******************************************************/

void D4D_IconOnMessage(D4D_MESSAGE* pMsg)
{

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE
  D4D_ICON* pIcon = D4D_GET_ICON(pMsg->pObject);
  D4D_ICON_DATA* pData = pIcon->pData;
#endif

  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_IconOnDraw(pMsg);
      break;
      
      
    case D4D_MSG_TOUCHED:
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
      break;

#if D4D_ICON_ENABLE_ANIMATION == D4D_TRUE
    
    case D4D_MSG_TIMETICK:
      if(pData->animationEnabled)
      {
        if(++(pData->tickCounter) > D4D_ICON_ANIMATION_TICK_COUNTER)
        {
          pData->tickCounter = 0;
          // update animation (change icon)
          D4D_IconChangeIndex(pMsg->pObject, 1);  
        }
      }
      break;
#endif      

    default:
      // call the default behavior of all objects
      D4D_ObjOnMessage(pMsg);
  }
}

/*******************************************************
*
* The ICON check coordinators  handler 
*
*******************************************************/

Byte D4D_IconCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_ICON* pIcon = D4D_GET_ICON(pThis);
  
  D4D_IconValue2Coor(pThis);
  
  if((point.x >= pIcon->scrPos.x) && (point.y >= pIcon->scrPos.y))
  {
    
      
    if((point.x <= (pIcon->scrPos.x + _calc.size.cx)) && (point.y <= (pIcon->scrPos.y + _calc.size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_IconGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_ICON(pThis)->textBuff);  
}

