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
* @file      d4d_slider.c
*
* @author    b01119
* 
* @version   0.0.54.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver slider object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_SldrOnMessage(D4D_MESSAGE* pMsg); 
Byte D4D_SldrCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point); 
D4D_STRING* D4D_SldrGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_sliderSysFunc = 
{
  D4D_OBJECT_SLIDER,
  D4D_SldrOnMessage,
  D4D_SldrCheckCoordinates,
  D4D_SldrGetTextBuffer
};

// temporary structure for bar coordinate calculation
typedef struct 
{
    D4D_POINT pnt1;
    D4D_SIZE  sz1;
    D4D_POINT pnt2;
    D4D_SIZE  sz2;
    D4D_POINT pnt3;
    D4D_SIZE  sz3;
    D4D_DIRECTION dir;
    D4D_BOOL isHorizontal;
    D4D_COOR inner_radius;
    D4D_COOR bar_radius;    
} D4D_SLIDER_TMP_VAL;



//#define _calc (*((D4D_SLIDER_TMP_VAL*)d4d_scratchPad))

D4D_SLIDER_TMP_VAL _calc;

static D4D_COLOR D4D_SldrComputeColorBar(D4D_OBJECT* pThis, D4D_SLIDER_VALUE value);

/*******************************************************
*
* calculate pointer endpoint from basepoint, limits and value
*
*******************************************************/

static void D4D_SldrValue2Coor(D4D_OBJECT* pThis)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_COOR barMin;
    D4D_COOR barMax;
    D4D_COOR c1, c2;
    D4D_POINT tmpBarOff = pSldr->barOff;
    sWord tmp_scale = (sWord)(pSldr->pData->limits.valueMax - pSldr->pData->limits.valueMin);
    
    if(tmp_scale < 0)
      tmp_scale *= -1;
    
    if(pSldr->scrSize.cx > pSldr->scrSize.cy)
      _calc.isHorizontal = D4D_TRUE;
    else
      _calc.isHorizontal = D4D_FALSE;
    
    _calc.sz1 = pSldr->barSize;
    
 #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities    
    
    if(_calc.isHorizontal)        
    {      
      if(!tmpBarOff.x)
        tmpBarOff.x = D4D_SLDR_BAR_OFF_LENGTH;    
      
      if(!tmpBarOff.y)
        tmpBarOff.y = D4D_SLDR_BAR_OFF_WIDTH;          
    }
    else
    {
      if(!tmpBarOff.x)
        tmpBarOff.x = D4D_SLDR_BAR_OFF_WIDTH;    
      
      if(!tmpBarOff.y)
        tmpBarOff.y = D4D_SLDR_BAR_OFF_LENGTH;          
    }
    
    if(!_calc.sz1.cx)
      _calc.sz1.cx = (D4D_COOR)(pSldr->scrSize.cx - (tmpBarOff.x * 2));    
    
    if(!_calc.sz1.cy)
      _calc.sz1.cy = (D4D_COOR)(pSldr->scrSize.cy - (tmpBarOff.y * 2));    
    
#endif
    _calc.pnt1 = D4D_GetClientToScreenPoint(*pThis->pScreen, &pSldr->scrPos);
    _calc.pnt1.x += tmpBarOff.x;
    _calc.pnt1.y += tmpBarOff.y;    
    
    // horizontal?
    if(_calc.isHorizontal)
    {
        barMin = (D4D_COOR)(_calc.pnt1.x);
        barMax = (D4D_COOR)(barMin + _calc.sz1.cx - 1);
    }
    // vertical?
    else
    {
        barMin = (D4D_COOR)(_calc.pnt1.y);
        barMax = (D4D_COOR)(barMin + _calc.sz1.cy - 1);
    }
    
 
    // value and origin coordinates
    c1 = D4D_MulDivUU((D4D_COOR)(pSldr->pData->limits.valueOrg - pSldr->pData->limits.valueMin), (D4D_COOR)(barMax-barMin), 
        (D4D_COOR)tmp_scale);
    
    c2 = D4D_MulDivUU((D4D_COOR)(pSldr->pData->value - pSldr->pData->limits.valueMin), (D4D_COOR)(barMax-barMin), 
        (D4D_COOR)tmp_scale);

    
    #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
      if(tmpBarOff.x > tmpBarOff.y)
        tmp_scale = (sWord)(tmpBarOff.x - tmpBarOff.y);
      else
        tmp_scale = (sWord)(tmpBarOff.y - tmpBarOff.x);
      
      tmp_scale /=2;
      
      tmp_scale = (sWord)(pSldr->radius - tmp_scale);   
      
      if(tmp_scale > 0)  
        _calc.inner_radius = (D4D_COOR)tmp_scale;
      else
        _calc.inner_radius = 0;
      
      _calc.bar_radius = _calc.inner_radius;
      
    #endif
    

    // bar rectangle range need to be "sorted"
    if(c1 > c2)
    {
      D4D_COOR tmp_coor;
      tmp_coor = c2;   
      c2 = c1;
      c1 = tmp_coor;           
    }
    
    _calc.sz2 = _calc.sz1;
    _calc.sz3 = _calc.sz1;
    
    // X grows to right, Y grows to top
    if(_calc.isHorizontal)
    {
      
      if(pSldr->pData->limits.valueOrg > pSldr->pData->value)
        _calc.dir = D4D_DIR_LEFT;
      else    
        _calc.dir = D4D_DIR_RIGHT;
      
      _calc.sz3.cx =  (D4D_COOR)(_calc.sz1.cx - c2);
      _calc.sz1.cx =  c1;
      _calc.sz2.cx =  (D4D_COOR)(c2 - c1 + 1);
      
      _calc.pnt2.x = (D4D_COOR)(_calc.pnt1.x + c1);
      _calc.pnt2.y = _calc.pnt1.y;
      
      _calc.pnt3.x = (D4D_COOR)(_calc.pnt1.x + c2);
      _calc.pnt3.y = _calc.pnt1.y;
      
      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
        
        if(_calc.inner_radius)
        {
          _calc.sz1.cx += _calc.inner_radius * 2;
          _calc.sz3.cx += _calc.inner_radius * 2;
          
          _calc.bar_radius = D4D_LimitU(_calc.bar_radius, 0, (D4D_COOR)(_calc.sz2.cx / 2));
          
          if(_calc.inner_radius != _calc.bar_radius)
          {
            D4D_COOR tmp_coor;
            
            //tmp_coor = D4D_RndCornerGetNonPrintedPxl(_calc.inner_radius, (D4D_COOR)(_calc.pnt2.x + _calc.sz2.cx - (_calc.pnt1.x + _calc.sz1.cx + 0)));
            tmp_coor = (D4D_COOR)(_calc.inner_radius - D4D_RndCornerGetNonPrintedPxl((D4D_COOR)(_calc.inner_radius + 1), (D4D_COOR)(_calc.inner_radius - (_calc.sz2.cx / 2))));
            
            _calc.sz2.cy -= (D4D_COOR)(tmp_coor*2);
            _calc.pnt2.y += tmp_coor;
          }
          
          _calc.sz1.cx = D4D_LimitU(_calc.sz1.cx, (D4D_COOR)(_calc.inner_radius * 2), (D4D_COOR)(barMax - barMin + 1));
          _calc.sz3.cx = D4D_LimitU(_calc.sz3.cx, (D4D_COOR)(_calc.inner_radius * 2), (D4D_COOR)(barMax - barMin + 1));

          _calc.pnt3.x -= (D4D_COOR)(2 * _calc.inner_radius); 
          _calc.pnt3.x = D4D_LimitU(_calc.pnt3.x, _calc.pnt1.x, (D4D_COOR)(barMax- (_calc.inner_radius * 2)));
        }
      #endif
      
    }else
    {
      
      if(pSldr->pData->limits.valueOrg > pSldr->pData->value)
        _calc.dir = D4D_DIR_DOWN;
      else    
        _calc.dir = D4D_DIR_UP;
      
      _calc.pnt3 = _calc.pnt1;
      
      _calc.pnt2 = _calc.pnt1;
      
      _calc.pnt2.y += (D4D_COOR)(_calc.sz1.cy - (c2 + 1));
      
      _calc.pnt1.y += (D4D_COOR)(_calc.sz1.cy - (c1 + 1));
      
      _calc.sz3.cy =  (D4D_COOR)(_calc.sz1.cy - (c2 + 1));
      _calc.sz1.cy =  (D4D_COOR)(c1 + 1);
      _calc.sz2.cy =  (D4D_COOR)(c2 - c1 + 1);            
      
      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
        
        if(_calc.inner_radius)
        {
          _calc.sz1.cy += (D4D_COOR)(_calc.inner_radius * 2);
          _calc.sz3.cy += (D4D_COOR)(_calc.inner_radius * 2);
          
          _calc.bar_radius = D4D_LimitU(_calc.bar_radius, 0, (D4D_COOR)(_calc.sz2.cy / 2));
          
          if(_calc.inner_radius != _calc.bar_radius)
          {
            D4D_COOR tmp_coor;
            
            tmp_coor = (D4D_COOR)(_calc.inner_radius - D4D_RndCornerGetNonPrintedPxl((D4D_COOR)(_calc.inner_radius + 1), (D4D_COOR)(_calc.inner_radius - (_calc.sz2.cy / 2))));
            
            _calc.sz2.cx -= (D4D_COOR)(tmp_coor * 2);
            _calc.pnt2.x += tmp_coor;
          }
          
          _calc.sz1.cy = D4D_LimitU(_calc.sz1.cy, (D4D_COOR)(_calc.inner_radius * 2 + 1), (D4D_COOR)(barMax - barMin + 1));
          _calc.sz3.cy = D4D_LimitU(_calc.sz3.cy, (D4D_COOR)(_calc.inner_radius * 2), (D4D_COOR)(barMax - barMin + 1));

          _calc.pnt1.y -= (D4D_COOR)(2 * _calc.inner_radius); 
          _calc.pnt1.y = D4D_LimitU(_calc.pnt1.y, _calc.pnt3.y, (D4D_COOR)(barMax - (_calc.inner_radius * 2)));
        } 
      #endif
    }
    
      
}


/*******************************************************
*
* SLIDER Drawing routine
*
*******************************************************/

static void D4D_SldrOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pMsg->pObject);
    D4D_COLOR clrT, clrB;
    D4D_COLOR clrBar = pSldr->pData->colorBar;
    D4D_COLOR clrBarBckg = D4D_ObjectGetScheme(pMsg->pObject)->objectDepend.slider.barBckg;
    
    D4D_POINT Position = D4D_GetClientToScreenPoint(pMsg->pScreen, &pSldr->scrPos);
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
   
    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif
    
    // bar coordinate calculation
    D4D_SldrValue2Coor(pMsg->pObject);
    
    clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
    clrB = D4D_ObjectGetBckgColor(pMsg->pObject, draw);
  
    // if slider is disabled draw slider in grey scale
    if(!pMsg->pObject->flags->bits.bEnabled)
    {
      clrBar = D4D_GetGreyScale(clrBar);
      clrBarBckg = D4D_GetGreyScale(clrBarBckg);        
    }
  
  
 #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    // when background redraw is needed
    if(draw.bits.bComplete)
    {
      // draw the bitmap
      if(pSldr->pBmpBkgd != NULL)
          D4D_DrawRBmp(&Position, pSldr->pBmpBkgd, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pSldr->radius);  
      else
          D4D_FillRRect(&Position, &pSldr->scrSize, D4D_ObjectGetBckgFillColor(pMsg->pObject), pSldr->radius);
    }
  
    // draw focus rectangle
    D4D_RRect(&Position, &pSldr->scrSize, D4D_LINE_THIN, clrT, pSldr->radius);
    
    // draw first inactive part
    D4D_FillRRect(&_calc.pnt1, &_calc.sz1, clrBarBckg, _calc.inner_radius);
    
    // draw last inactive part of the bar
    D4D_FillRRect(&_calc.pnt3, &_calc.sz3, clrBarBckg, _calc.inner_radius);
    
    // draw active part of the bar
    if(pMsg->pObject->initFlags & D4D_SLDR_F_BAR_SCALECOLOR)
      D4D_FillRRectColorScale(&_calc.pnt2, &_calc.sz2, D4D_COLOR_SLDR_BAR_START, clrBar, _calc.dir, _calc.bar_radius);                      
    else
      D4D_FillRRect(&_calc.pnt2, &_calc.sz2, clrBar, _calc.inner_radius);    
#else
    // when background redraw is needed
    if(draw.bits.bComplete)
    {
      // draw the bitmap
      if(pSldr->pBmpBkgd != NULL)
          D4D_DrawBmp(&Position, pSldr->pBmpBkgd, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);  
      else
          D4D_FillRect(&Position, &pSldr->scrSize, D4D_ObjectGetBckgFillColor(pMsg->pObject));
    }
  
    // draw focus rectangle
    D4D_Rect(&Position, &pSldr->scrSize, D4D_LINE_THIN, clrT);
    
    // draw first inactive part
    D4D_FillRect(&_calc.pnt1, &_calc.sz1, clrBarBckg);
    
    // draw last inactive part of the bar
    D4D_FillRect(&_calc.pnt3, &_calc.sz3, clrBarBckg);
    
    // draw active part of the bar
    if(pMsg->pObject->initFlags & D4D_SLDR_F_BAR_SCALECOLOR)
      D4D_FillRectColorScale(&_calc.pnt2, &_calc.sz2, D4D_COLOR_SLDR_BAR_START, clrBar, _calc.dir);                      
    else
      D4D_FillRect(&_calc.pnt2, &_calc.sz2, clrBar);    
#endif    
    
    
           
      
    // draw the text
    if(pSldr->textBuff.pText != NULL)
    {
      Position.x += pSldr->txtOff.x;
      Position.y += pSldr->txtOff.y;
          
 #if  D4D_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities

      if(!pSldr->txtOff.x)                                                
      {        
        if(_calc.isHorizontal)                              
          Position.x += (D4D_COOR) ((pSldr->scrSize.cx - (D4D_COOR)(D4D_GetTextWidth(pSldr->textBuff.fontId, pSldr->textBuff.pText))) / 2);
        else
          Position.x += (D4D_COOR) (_calc.pnt1.x + 1);  
      }
      
      if(!pSldr->txtOff.y)
        Position.y += (D4D_COOR) (((pSldr->scrSize.cy - D4D_GetFontHeight(pSldr->textBuff.fontId)) / 2) + 1);
#endif        

      if(pMsg->pObject->initFlags & D4D_SLDR_F_TEXT_AUTOCOLOR)
          clrT = (D4D_COLOR)(~clrBar);
        
      D4D_DrawTextRect(&Position, (D4D_SIZE*)&d4d_size_zero, &pSldr->textBuff, clrT, clrB);
                                         
    }
       
}

/*******************************************************
*
* SLIDER key handling routine
*
*******************************************************/

static void D4D_SldrOnKeyUp(D4D_MESSAGE* pMsg)    
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pMsg->pObject);
    D4D_KEY_SCANCODE tmp_key = pMsg->prm.key;
    // capture the keyboard if enter is pressed
    if(tmp_key == D4D_KEY_SCANCODE_ENTER)
    {
        D4D_CaptureKeys(pMsg->pObject);
        return;
    }
    
    // exit capture
    if(tmp_key == D4D_KEY_SCANCODE_ESC)
    {
        D4D_CaptureKeys(NULL);
        return;
    }
    
    
    if(D4D_GetCapturedObject() == pMsg->pObject)
    {      
      // increment value
      if((tmp_key == D4D_KEY_SCANCODE_RIGHT) || (tmp_key == D4D_KEY_SCANCODE_UP))
      {
          D4D_SldrChangeValue(pMsg->pObject, pSldr->pData->limits.step);
      }
      // decrement value
      else if((tmp_key == D4D_KEY_SCANCODE_LEFT) || (tmp_key == D4D_KEY_SCANCODE_DOWN))
      {
          D4D_SldrChangeValue(pMsg->pObject, (D4D_SLIDER_VALUE)(-pSldr->pData->limits.step));
      } 
    }
}

/*******************************************************
*
* SLIDER Touched Button handling routine
*
*******************************************************/

static void D4D_SldrTouched(D4D_MESSAGE* pMsg)
{
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pMsg->pObject);
    
  if(D4D_GetFocusedObject(pMsg->pScreen) != pMsg->pObject)
  {    
    D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
  }else
  {
    if(D4D_GetCapturedObject() != pMsg->pObject)
    {
       D4D_CaptureKeys(pMsg->pObject);
    }else
    {
      
      if(pSldr->scrSize.cx > pSldr->scrSize.cy)
      {      
        if(D4D_GetTouchScreenCoordinates().x > ((pSldr->scrSize.cx / 2) + pSldr->scrPos.x))
          D4D_SldrChangeValue(pMsg->pObject, pSldr->pData->limits.step);
        else
          D4D_SldrChangeValue(pMsg->pObject, (D4D_SLIDER_VALUE)(-pSldr->pData->limits.step));
      }else
      {
        
        if(D4D_GetTouchScreenCoordinates().y < ((pSldr->scrSize.cy / 2) + pSldr->scrPos.y))
          D4D_SldrChangeValue(pMsg->pObject, pSldr->pData->limits.step);
        else
          D4D_SldrChangeValue(pMsg->pObject, (D4D_SLIDER_VALUE)(-pSldr->pData->limits.step));  
      }
    }
  }
  
  
  
}

static D4D_COLOR D4D_SldrComputeColorBar(D4D_OBJECT* pThis, D4D_SLIDER_VALUE value)
{
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
  Byte shade;    
  D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pThis);

  if(value >= pSldr->pData->limits.valueOrg)   
    shade = D4D_MulDivUU8((Byte)(value - pSldr->pData->limits.valueOrg), 255, \
      (Byte)(pSldr->pData->limits.valueMax - pSldr->pData->limits.valueOrg)); 
  else
    shade = D4D_MulDivUU8((Byte)(pSldr->pData->limits.valueOrg - value), 255, \
      (Byte)(pSldr->pData->limits.valueOrg - pSldr->pData->limits.valueMin));
          
  return D4D_GetCrossColor(pScheme_tmp->objectDepend.slider.barStart, pScheme_tmp->objectDepend.slider.barEnd, shade);
      
}


/*******************************************************
*
* Set SLIDER value
*
*******************************************************/

void D4D_SldrSetValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE value)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_SLIDER_DATA* pData = pSldr->pData;

    // keep value within limits    
    
    value = D4D_LimitS8(value, pData->limits.valueMin, pData->limits.valueMax);     
    
    if(pData->value == value)
        return ;
    
    // remember new value
    pData->value = value;
    
    if(pThis->initFlags & (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR))
      pData->colorBar = D4D_SldrComputeColorBar((D4D_OBJECT*)pThis, value);
    
    pThis->flags->bits.bRedraw = 1;

    // notify user that the value has changed        
    if(pSldr->OnValueChanged)
        pSldr->OnValueChanged(pThis);
}

D4D_SLIDER_VALUE D4D_SldrGetValue(D4D_OBJECT_PTR pThis)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    return pSldr->pData->value;
}

/*******************************************************
*
* Set SLIDER bar color
*
*******************************************************/

void D4D_SldrSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    D4D_SLIDER_DATA* pData = pSldr->pData;

    if(pThis->initFlags & (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR))
      return;
    
    if(pData->colorBar == color)
        return ;
    
    // remember new color
    pData->colorBar = color;
    
    pThis->flags->bits.bRedraw = 1;    
}

D4D_COLOR D4D_SldrGetBarColor(D4D_OBJECT_PTR pThis)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    return pSldr->pData->colorBar;
}


/*******************************************************
*
* Modify SLIDER value by a value
*
*******************************************************/

void D4D_SldrChangeValue(D4D_OBJECT_PTR pThis, D4D_SLIDER_VALUE incr)
{
    D4D_SLIDER_VALUE value;
    
    // change the value
    value = D4D_LimitS8((D4D_SLIDER_VALUE)(D4D_GET_SLIDER(pThis)->pData->value + incr), -128, 127);
    
    D4D_SldrSetValue(pThis, value);
}

/*******************************************************
*
* Configure SLIDER limits
*
*******************************************************/

void D4D_SldrSetLimits(D4D_OBJECT_PTR pThis, const D4D_SLIDER_LIMITS* pLimits)
{
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
  D4D_SLIDER_DATA* pData = pSldr->pData;

  pData->limits = *pLimits;

  if((pData->limits.valueOrg < pData->limits.valueMin) || (pData->limits.valueOrg >= pData->limits.valueMax))
    pData->limits.valueOrg = pData->limits.valueMin;

  D4D_SldrSetValue(pThis, pData->value);    
}

void D4D_SldrGetLimits(D4D_OBJECT_PTR pThis, D4D_SLIDER_LIMITS* pLimits)
{
    D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
    *pLimits = pSldr->pData->limits;
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
* The main SLIDER message handler 
*
*******************************************************/

void D4D_SldrOnMessage(D4D_MESSAGE* pMsg)
{
    D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pMsg->pObject);
    
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_SldrOnDraw(pMsg);
        break;
        
    case D4D_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_SldrOnKeyUp(pMsg);
        break;

    case D4D_MSG_KILLFOCUS:
        D4D_CaptureKeys(NULL);
        break;
    
    case D4D_MSG_ONINIT:
        if(!(pMsg->pObject->initFlags & (D4D_SLDR_F_BAR_AUTOCOLOR | D4D_SLDR_F_BAR_SCALECOLOR)))
          D4D_GET_SLIDER(pMsg->pObject)->pData->colorBar = pScheme_tmp->objectDepend.slider.barFore;                    
        else
          D4D_GET_SLIDER(pMsg->pObject)->pData->colorBar = D4D_SldrComputeColorBar(pMsg->pObject, D4D_GET_SLIDER(pMsg->pObject)->pData->value);  
        break;
        
    case D4D_MSG_TOUCH_AUTO:
    case D4D_MSG_TOUCHED:     
        if(pMsg->pObject->flags->bits.bEnabled)
          D4D_SldrTouched(pMsg);
        break;

    default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
    }
}


/*******************************************************
*
* The SLIDER check coordinators  handler 
*
*******************************************************/

Byte D4D_SldrCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_SLIDER* pSldr = D4D_GET_SLIDER(pThis);
  
  
  
  if((point.x >= pSldr->scrPos.x) && (point.y >= pSldr->scrPos.y))
  { 
    if((point.x <= (pSldr->scrPos.x + pSldr->scrSize.cx)) && (point.y <= (pSldr->scrPos.y + pSldr->scrSize.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_SldrGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_SLIDER(pThis)->textBuff);  
}
