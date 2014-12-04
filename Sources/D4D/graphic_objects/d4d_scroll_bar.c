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
* @file      d4d_scroll_bar.c
*
* @author    b01119
* 
* @version   0.0.7.0
* 
* @date      Feb-2-2011
* 
* @brief     D4D driver scroll_bar object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"


// Internal API
static void D4D_ScrlBrOnMessage(D4D_MESSAGE* pMsg);
static void D4D_ScrlBrOnKeyDown(D4D_MESSAGE* pMsg);
static void D4D_ScrlBrOnTouch(D4D_MESSAGE* pMsg);

static Byte D4D_ScrlBrCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);

static void D4D_ScrlBrDrawArrow(D4D_POINT* ppt, D4D_COOR size, D4D_DIRECTION dir, D4D_COLOR color);



const D4D_OBJECT_SYS_FUNCTION d4d_scroll_barSysFunc = 
{
  D4D_OBJECT_SCROLLBAR,
  D4D_ScrlBrOnMessage,
  D4D_ScrlBrCheckCoordinates,
  NULL
};

typedef struct 
{
  D4D_POINT position;
  D4D_POINT scrollPos;
  D4D_SIZE scrollSize;
  D4D_POINT refreshPos;
  D4D_SIZE refreshSize;
  D4D_COOR scrollRadius;  
} D4D_SCRLBAR_TMP_VAL;

#define _calc (*((D4D_SCRLBAR_TMP_VAL*)d4d_scratchPad))

    
static void D4D_ScrlBrValue2Coor(D4D_OBJECT* pThis)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pThis);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  
  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pScrlBr->scrPos);
 
 if(pScrlBr->scrSize.cx > pScrlBr->scrSize.cy)
  {
    // horizontal scroll bar
    _calc.refreshPos = _calc.position;
    _calc.refreshPos.x += (D4D_COOR)(pScrlBr->scrSize.cy + 1);
    _calc.refreshPos.y++;
    
    _calc.refreshSize = pScrlBr->scrSize;
    _calc.refreshSize.cx -= (D4D_COOR)((pScrlBr->scrSize.cy * 2) + 2);
    _calc.refreshSize.cy -= 2;
    
    _calc.scrollSize = _calc.refreshSize;

    
    _calc.scrollSize.cy -= 2;
    
    _calc.scrollSize.cx = (D4D_COOR)D4D_MulDivUU16(_calc.refreshSize.cx, pData->page, (D4D_INDEX)(pData->maximum - pData->minimum));
    
    if(_calc.scrollSize.cx < D4D_SCRLBAR_MIN_TAB_SIZE)
      _calc.scrollSize.cx = D4D_SCRLBAR_MIN_TAB_SIZE;
    
    _calc.scrollPos = _calc.refreshPos;
    if(pData->maximum - pData->page - pData->minimum)
      _calc.scrollPos.x += (D4D_COOR)(D4D_MulDivUU16((D4D_COOR)(_calc.refreshSize.cx - _calc.scrollSize.cx - 1), pData->position, (D4D_INDEX)(pData->maximum - pData->page - pData->minimum)) + 1);
    _calc.scrollPos.y++;

       
  }else
  {
    // vertical scroll bar
    _calc.refreshPos = _calc.position;
    _calc.refreshPos.y += (D4D_COOR)(pScrlBr->scrSize.cx + 1);
    _calc.refreshPos.x++;
    
    _calc.refreshSize = pScrlBr->scrSize;
    _calc.refreshSize.cx -= 2;
    _calc.refreshSize.cy -= (D4D_COOR)((pScrlBr->scrSize.cx * 2) + 2);
    
    _calc.scrollSize = _calc.refreshSize;
   
    _calc.scrollSize.cx -= 2;
    
    _calc.scrollSize.cy = (D4D_COOR)D4D_MulDivUU16(_calc.refreshSize.cy, pData->page, (D4D_INDEX)(pData->maximum - pData->minimum));
    
    if(_calc.scrollSize.cy < D4D_SCRLBAR_MIN_TAB_SIZE)
      _calc.scrollSize.cy = D4D_SCRLBAR_MIN_TAB_SIZE;
    
    _calc.scrollPos = _calc.refreshPos;
    if(pData->maximum - pData->page - pData->minimum)
      _calc.scrollPos.y += (D4D_COOR)(D4D_MulDivUU16((D4D_COOR)(_calc.refreshSize.cy - _calc.scrollSize.cy - 1), pData->position, (D4D_INDEX)(pData->maximum - pData->page - pData->minimum)) + 1);
    _calc.scrollPos.x++;
  } 
  
  
  if(pScrlBr->radius)
    _calc.scrollRadius = (D4D_COOR)(pScrlBr->radius - 1);
  else
    _calc.scrollRadius = 0;

}
 


/*******************************************************
*
* SCROLL_BAR Drawing routine
*
*******************************************************/

static void D4D_ScrlBrOnDraw(D4D_MESSAGE* pMsg)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pMsg->pObject);
  D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
  D4D_COLOR clrT, clrB;
  D4D_POINT tmp_point;
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  D4D_SIZE tmp_size;
#endif  
  D4D_ScrlBrValue2Coor(pMsg->pObject);
      
  clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
  clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);  
  
  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE  
  
  // draw the rectangle around the text
  if(draw.bits.bComplete)  
  {
    if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
     D4D_RBox(&_calc.position, &pScrlBr->scrSize, D4D_LINE_THIN, clrT, clrB, pScrlBr->radius);
    else
     D4D_FillRRect(&_calc.position, &pScrlBr->scrSize, clrB, pScrlBr->radius);
    
    if(pScrlBr->scrSize.cx > pScrlBr->scrSize.cy)
    {
      // horizontal scroll bar
      // draw first arrow button left
      tmp_point.x = _calc.position.x;
      tmp_point.y = _calc.position.y; 
      tmp_size.cx = pScrlBr->scrSize.cy;
      tmp_size.cy = pScrlBr->scrSize.cy;
      D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, pScrlBr->radius);
      
      tmp_point.x += (D4D_COOR)(pScrlBr->scrSize.cx - pScrlBr->scrSize.cy);
      D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, pScrlBr->radius);
      
      tmp_point.x = (D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cy / 8);
      tmp_point.y += (D4D_COOR)(pScrlBr->scrSize.cy / 2); 
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cy / 2) - 2),D4D_DIR_LEFT,clrT);
      
      // draw second arrow button right
      tmp_point.x = (D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx - pScrlBr->scrSize.cy / 8);
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cy / 2) - 2),D4D_DIR_RIGHT,clrT);
      
    }else
    {
      // vertical scroll bar
      tmp_point.x = _calc.position.x;
      tmp_point.y = _calc.position.y; 
      tmp_size.cx = pScrlBr->scrSize.cx;
      tmp_size.cy = pScrlBr->scrSize.cx;
      D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, pScrlBr->radius);
      
      tmp_point.y += (D4D_COOR)(pScrlBr->scrSize.cy - pScrlBr->scrSize.cx);
      D4D_RRect(&tmp_point, &tmp_size, D4D_LINE_THIN, clrT, pScrlBr->radius);
      
      tmp_point.y = (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cx / 8);
      tmp_point.x += (D4D_COOR)(pScrlBr->scrSize.cx / 2); 
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cx / 2) - 2),D4D_DIR_UP,clrT);
      
      // draw second arrow button right
      tmp_point.y = (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy - pScrlBr->scrSize.cx / 8);
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cx / 2) - 2),D4D_DIR_DOWN,clrT);
    }
  }
    
  D4D_FillRect(&_calc.refreshPos, &_calc.refreshSize, clrB);
  D4D_FillRRect(&_calc.scrollPos, &_calc.scrollSize, clrT, _calc.scrollRadius);
  
#else
  
  // draw the rectangle around the text
  if(draw.bits.bComplete)  
  {
    if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
     D4D_Box(&_calc.position, &pScrlBr->scrSize, D4D_LINE_THIN, clrT, clrB);
    else
     D4D_FillRect(&_calc.position, &pScrlBr->scrSize, clrB);
    
    if(pScrlBr->scrSize.cx > pScrlBr->scrSize.cy)
    {
      // horizontal scroll bar
      // draw first arrow button left
      D4D_MoveToXY((D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cy), _calc.position.y);
      D4D_LineToXY((D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cy), (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy - 1), D4D_LINE_THIN, clrT);
      tmp_point.x = (D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cy / 4);
      tmp_point.y = (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy / 2); 
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cy / 2) - 1),D4D_DIR_LEFT,clrT);
      
      // draw second arrow button right
      D4D_MoveToXY((D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx - pScrlBr->scrSize.cy), _calc.position.y);
      D4D_LineToXY((D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx - pScrlBr->scrSize.cy), (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy - 1), D4D_LINE_THIN, clrT);
      tmp_point.x = (D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx - pScrlBr->scrSize.cy / 4);
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cy / 2) - 1),D4D_DIR_RIGHT,clrT);
      
    }else
    {
      // vertical scroll bar
      
      // draw first arrow button up
      D4D_MoveToXY(_calc.position.x, (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cx));
      D4D_LineToXY((D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx - 1), (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cx), D4D_LINE_THIN, clrT);
      tmp_point.x = (D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx / 2);
      tmp_point.y = (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cx / 4); 
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cx / 2) - 1),D4D_DIR_UP,clrT);
      
      // draw second arrow button down
      D4D_MoveToXY(_calc.position.x, (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy - pScrlBr->scrSize.cx));
      D4D_LineToXY((D4D_COOR)(_calc.position.x + pScrlBr->scrSize.cx - 1), (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy - pScrlBr->scrSize.cx), D4D_LINE_THIN, clrT);
      tmp_point.y = (D4D_COOR)(_calc.position.y + pScrlBr->scrSize.cy - pScrlBr->scrSize.cx / 4); 
      D4D_ScrlBrDrawArrow( &tmp_point,(D4D_COOR)((pScrlBr->scrSize.cx / 2) - 1),D4D_DIR_DOWN,clrT);
      
    }
  }
    
  D4D_FillRect(&_calc.refreshPos, &_calc.refreshSize, clrB);
  D4D_FillRect(&_calc.scrollPos, &_calc.scrollSize, clrT);

#endif  
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

void D4D_ScrlBrDrawArrow(D4D_POINT* ppt, D4D_COOR size, D4D_DIRECTION dir, D4D_COLOR color) 
{

  D4D_COOR i,j;
  D4D_POINT tmp_point = {0, 0};
  D4D_POINT tmp_pointReal;
  D4D_BOOL swap = D4D_FALSE;
  
  if((dir == D4D_DIR_LEFT) || (dir == D4D_DIR_RIGHT))
    swap = D4D_TRUE;
  
  D4D_PutPixel(&tmp_point, D4D_LINE_THIN, color);
  
  for(i = 0; i < size; i++) 
  {
    if((dir == D4D_DIR_DOWN) || (dir == D4D_DIR_RIGHT))
      tmp_point.y--;
    else
      tmp_point.y++;
        
    tmp_point.x--;
    
    if(swap)
      tmp_pointReal = D4D_SwapCoor(tmp_point);
    else
      tmp_pointReal = tmp_point;
    
    tmp_pointReal.x += ppt->x;
    tmp_pointReal.y += ppt->y;
    
    
    D4D_MoveTo(&tmp_pointReal);
    
    j = tmp_point.x;
    tmp_point.x += (i+1) * 2;
    
    if(swap)
      tmp_pointReal = D4D_SwapCoor(tmp_point);
    else
      tmp_pointReal = tmp_point;
    
    tmp_pointReal.x += ppt->x;
    tmp_pointReal.y += ppt->y;
    
      D4D_LineTo(&tmp_pointReal, D4D_LINE_THIN, color);
    
    tmp_point.x = j;
  }  
}


static void D4D_ScrlBrOnTouch(D4D_MESSAGE* pMsg)
{
  if((pMsg->pObject->flags->bits.bTabStop) || (pMsg->pObject->flags->bits.bFastTouch))
  {    
    if((D4D_GetFocusedObject(pMsg->pScreen) == pMsg->pObject) || (pMsg->pObject->flags->bits.bFastTouch))
    {    
      D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pMsg->pObject);
      D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
      
      if(pMsg->pObject->flags->bits.bTabStop)
        D4D_CaptureKeys(pMsg->pObject);     
      
      if(pScrlBr->scrSize.cx > pScrlBr->scrSize.cy)
      {      
        if(D4D_GetTouchScreenCoordinates().x > ((pScrlBr->scrSize.cx / 2) + pScrlBr->scrPos.x))
          D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(pData->step));
        else
          D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(-1 * pData->step));
      }else
      {
        
        if(D4D_GetTouchScreenCoordinates().y > ((pScrlBr->scrSize.cy / 2) + pScrlBr->scrPos.y))
          D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(pData->step));
        else
          D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(-1 * pData->step));  
      }
      
      
    }
    if(pMsg->pObject->flags->bits.bTabStop)
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
  }
}


static void D4D_ScrlBrOnKeyDown(D4D_MESSAGE* pMsg)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pMsg->pObject);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  
  if(pScrlBr->scrSize.cx > pScrlBr->scrSize.cy)
  {
    if(pMsg->prm.key == D4D_KEY_SCANCODE_LEFT)
      D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(-1 * pData->step));
    else if(pMsg->prm.key == D4D_KEY_SCANCODE_RIGHT)
      D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(pData->step));
  }else
  {
    if(pMsg->prm.key == D4D_KEY_SCANCODE_UP)
      D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(-1 * pData->step));
    else if(pMsg->prm.key == D4D_KEY_SCANCODE_DOWN)
      D4D_ScrlBrChangePosition(pMsg->pObject, (D4D_INDEX_DELTA)(pData->step));  
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
// FUNCTION:    D4D_ScrlBrSetRange
// SCOPE:       ScrlBr object external API function
// DESCRIPTION: The function sets the range of scroll bar scale
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on scroll_bar object
//              D4D_INDEX minimum - minimum value of scroll bar
//              D4D_INDEX maximum - maximum value of scroll bar
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_ScrlBrSetRange(D4D_OBJECT_PTR pObj, D4D_INDEX minimum, D4D_INDEX maximum)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  
  
  if(maximum < minimum)
    return;
  
  pData->minimum = minimum;
  pData->maximum = maximum;  
  
  if(pData->step > pData->maximum)
    pData->step = 1;
  
  if(pData->page > pData->maximum)
    pData->page = 0;
    
  if(pData->position > pData->maximum)
    pData->position = pData->maximum;
  
  if(pData->position < pData->minimum)
    pData->position = pData->minimum;
  
  pObj->flags->bits.bRedraw = 1;  
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_ScrlBrSetStep
// SCOPE:       ScrlBr object external API function
// DESCRIPTION: The function sets the step and page of scroll bar scale
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on scroll_bar object
//              D4D_INDEX page - page value of scroll bar
//              D4D_INDEX step - step value of scroll bar
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_ScrlBrSetStep(D4D_OBJECT_PTR pObj, D4D_INDEX page, D4D_INDEX step)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;

  
  if(step > (pData->maximum - pData->minimum))
    return;
  
  if(page > (pData->maximum - pData->minimum))
    return;

  pData->step = step;
  pData->page = page;
  
  pObj->flags->bits.bRedraw = 1;  
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4D_ScrlBrSetPosition
// SCOPE:       ScrlBr object external API function
// DESCRIPTION: The function sets the scroll bar position
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on scroll_bar object
//              D4D_INDEX position -  value of scroll bar position
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_ScrlBrSetPosition(D4D_OBJECT_PTR pObj, D4D_INDEX position)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  D4D_INDEX old_pos = pData->position;
  
  if(position > (D4D_INDEX)(pData->maximum - pData->page))
    pData->position = (D4D_INDEX)(pData->maximum - pData->page);  
  else if(position < pData->minimum)
    pData->position = pData->minimum;
  else
    pData->position = position;
  
  if(pData->position == old_pos)
    return;
  
  if(pScrlBr->OnChange != NULL)
    pScrlBr->OnChange((D4D_OBJECT*)pObj, old_pos, pData->position);  
  
  pObj->flags->bits.bRedraw = 1;

}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_ScrlBrGetPosition
// SCOPE:       ScrlBr object external API function
// DESCRIPTION: The function gets the scroll bar position
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on scroll_bar object
//              
//              
// RETURNS:     D4D_INDEX - value of scroll bar position
//-----------------------------------------------------------------------------
D4D_INDEX D4D_ScrlBrGetPosition(D4D_OBJECT_PTR pObj)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  
  return pData->position;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4D_ScrlBrChangePosition
// SCOPE:       ScrlBr object external API function
// DESCRIPTION: The function change the scroll bar position
//              
// PARAMETERS:  D4D_OBJECT_PTR pObj - pointer on scroll_bar object
//              D4D_INDEX_DELTA change -  change of scroll bar position
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
void D4D_ScrlBrChangePosition(D4D_OBJECT_PTR pObj, D4D_INDEX_DELTA change)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pObj);
  D4D_SCRLBAR_DATA* pData = pScrlBr->pData;
  
  D4D_INDEX_DELTA tmp_pos = (D4D_INDEX_DELTA)(change + pData->position);
  
  if(tmp_pos < 0)
    tmp_pos = 0;
  
  D4D_ScrlBrSetPosition(pObj, (D4D_INDEX)tmp_pos);

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
* The SCROLL_BAR message handler 
*
*******************************************************/

void D4D_ScrlBrOnMessage(D4D_MESSAGE* pMsg)
{
  switch(pMsg->nMsgId)
  {
    case D4D_MSG_DRAW:
      D4D_ScrlBrOnDraw(pMsg);
      break;
    
    case D4D_MSG_KEYDOWN:
      if(pMsg->pObject->flags->bits.bEnabled)
        D4D_ScrlBrOnKeyDown(pMsg);
      break;
    
    case D4D_MSG_TOUCHED:
    case D4D_MSG_TOUCH_AUTO:
                                  
      if(pMsg->pObject->flags->bits.bEnabled)
        D4D_ScrlBrOnTouch(pMsg);
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
}

/*******************************************************
*
* The SCROLL_BAR check coordinators  handler 
*
*******************************************************/

static Byte D4D_ScrlBrCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_SCROLL_BAR* pScrlBr = D4D_GET_SCROLL_BAR(pThis);
  
  if((point.x >= pScrlBr->scrPos.x) && (point.y >= pScrlBr->scrPos.y))
  {
      
    if((point.x <= (pScrlBr->scrPos.x + pScrlBr->scrSize.cx)) && (point.y <= (pScrlBr->scrPos.y + pScrlBr->scrSize.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}
