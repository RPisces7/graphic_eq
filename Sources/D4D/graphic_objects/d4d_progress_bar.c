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
* @file      d4d_progress_bar.c
*
* @author    b01119
* 
* @version   0.0.3.0
* 
* @date      Feb-2-2011
* 
* @brief     D4D driver progress_bar object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_PrgrsBarOnMessage(D4D_MESSAGE* pMsg); 

const D4D_OBJECT_SYS_FUNCTION d4d_progress_barSysFunc = 
{
  D4D_OBJECT_PROGRESS_BAR,
  D4D_PrgrsBarOnMessage,
  NULL,
  NULL
};

// temporary structure for bar coordinate calculation
typedef struct 
{
    D4D_POINT position;
    D4D_POINT barXY;
    D4D_SIZE  barSize;
    D4D_COOR resultC1;
    D4D_COOR resultC2;
    D4D_COOR inner_radius;
} D4D_PROGRESS_BAR_TMP_VAL;



//#define _calc (*((D4D_PROGRESS_BAR_TMP_VAL*)d4d_scratchPad))

static D4D_PROGRESS_BAR_TMP_VAL _calc;

static D4D_COLOR D4D_PrgrsBarComputeColorBar(D4D_OBJECT* pThis, D4D_PROGRESS_BAR_VALUE value);

/*******************************************************
*
* calculate pointer endpoint from basepoint, limits and value
*
*******************************************************/

static void D4D_PrgrsBarValue2Coor(D4D_OBJECT* pThis)
{
  D4D_PROGRESS_BAR* pPrgrsBar = D4D_GET_PROGRESS_BAR(pThis);
  
  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pPrgrsBar->scrPos); 
  _calc.barXY.x = (D4D_COOR)(_calc.position.x + D4D_PRGRS_BAR_BAR_OFF);
  _calc.barXY.y = (D4D_COOR)(_calc.position.y + D4D_PRGRS_BAR_BAR_OFF);    
  
  _calc.barSize = pPrgrsBar->scrSize;
  
  _calc.barSize.cx -= (D4D_COOR)(2 * D4D_PRGRS_BAR_BAR_OFF);
  _calc.barSize.cy -= (D4D_COOR)(2 * D4D_PRGRS_BAR_BAR_OFF);
  
  // value and origin coordinates
  _calc.resultC1 = (D4D_COOR)(_calc.barXY.x + D4D_MulDivUU(pPrgrsBar->pData->value, _calc.barSize.cx, D4D_COLOR_PRGRS_BAR_MAX_VAL));
  
  _calc.resultC2 = _calc.resultC1;
  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE  
  
  if(pPrgrsBar->radius - D4D_PRGRS_BAR_BAR_OFF)
  {
    _calc.inner_radius = (D4D_COOR)(pPrgrsBar->radius - D4D_PRGRS_BAR_BAR_OFF);
    
    if((_calc.resultC2 - _calc.inner_radius) < _calc.barXY.x)
      _calc.resultC2 = _calc.barXY.x;
    else
      _calc.resultC2 -= _calc.inner_radius;
    
    if((_calc.barXY.x + _calc.barSize.cx - _calc.resultC2) < (2 * _calc.inner_radius))
      _calc.resultC2 = (D4D_COOR)(_calc.barXY.x + _calc.barSize.cx - (2 * _calc.inner_radius));  
    
    if((_calc.resultC1 - _calc.barXY.x) < (2 * _calc.inner_radius))
      _calc.resultC1 = (D4D_COOR)(_calc.barXY.x + (2 * _calc.inner_radius));  
    
  }
  else
    _calc.inner_radius = 0;
  
#endif  
  
}


/*******************************************************
*
* PROGRESS_BAR Drawing routine
*
*******************************************************/

static void D4D_PrgrsBarOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_PROGRESS_BAR* pPrgrsBar = D4D_GET_PROGRESS_BAR(pMsg->pObject);
    D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pMsg->pObject);
    D4D_COLOR clrBar = pPrgrsBar->pData->colorBar;
    D4D_COLOR clrBarBckg = pScheme_tmp->objectDepend.progressBar.barBckg;
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;

    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif
    
    // bar coordinate calculation
    D4D_PrgrsBarValue2Coor(pMsg->pObject);

    // if progress_bar is disabled draw progress_bar in grey scale
    if(!pMsg->pObject->flags->bits.bEnabled)
    {
      clrBar = D4D_GetGreyScale(clrBar);
      clrBarBckg = D4D_GetGreyScale(clrBarBckg);        
    }
  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE  
  
    // when background redraw is needed
    if(draw.bits.bComplete)
      D4D_RBox(&_calc.position, &pPrgrsBar->scrSize, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw), D4D_ObjectGetBckgFillColor(pMsg->pObject), pPrgrsBar->radius);
    
    if(!pPrgrsBar->pData->drawActiveOnly)
      D4D_FillRRectXY(_calc.resultC2, _calc.barXY.y, (D4D_COOR)(_calc.barXY.x + _calc.barSize.cx - 1), (D4D_COOR)(_calc.barXY.y + _calc.barSize.cy - 1), clrBarBckg, _calc.inner_radius);
    //else
    //  pPrgrsBar->pData->drawActiveOnly = 0;
    
    
    
    // draw active part of the bar
    if(pMsg->pObject->initFlags & D4D_PRGRS_BAR_F_BAR_SCALECOLOR)
      D4D_FillRRectColorScaleXY(_calc.barXY.x, _calc.barXY.y, _calc.resultC1, (D4D_COOR)(_calc.barXY.y + _calc.barSize.cy - 1), pScheme_tmp->objectDepend.progressBar.barFore, clrBar, D4D_DIR_RIGHT, _calc.inner_radius);                      
    else
      D4D_FillRRectXY(_calc.barXY.x, _calc.barXY.y, _calc.resultC1, (D4D_COOR)(_calc.barXY.y + _calc.barSize.cy - 1), clrBar, _calc.inner_radius);
    
#else
    // when background redraw is needed
    if(draw.bits.bComplete)
      D4D_Box(&_calc.position, &pPrgrsBar->scrSize, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw), D4D_ObjectGetBckgFillColor(pMsg->pObject));
    
    if(!pPrgrsBar->pData->drawActiveOnly)
      D4D_FillRectXY(_calc.resultC2, _calc.barXY.y, (D4D_COOR)(_calc.barXY.x + _calc.barSize.cx - 1), (D4D_COOR)(_calc.barXY.y + _calc.barSize.cy - 1), clrBarBckg);
    //else
    //  pPrgrsBar->pData->drawActiveOnly = 0;
    
    
    
    // draw active part of the bar
    if(pMsg->pObject->initFlags & D4D_PRGRS_BAR_F_BAR_SCALECOLOR)
      D4D_FillRectColorScaleXY(_calc.barXY.x, _calc.barXY.y, _calc.resultC1, (D4D_COOR)(_calc.barXY.y + _calc.barSize.cy - 1), pScheme_tmp->objectDepend.progressBar.barFore, clrBar, D4D_DIR_RIGHT);                      
    else
      D4D_FillRectXY(_calc.barXY.x, _calc.barXY.y, _calc.resultC1, (D4D_COOR)(_calc.barXY.y + _calc.barSize.cy - 1), clrBar);

#endif                
    
   }




static D4D_COLOR D4D_PrgrsBarComputeColorBar(D4D_OBJECT* pThis, D4D_PROGRESS_BAR_VALUE value)
{
  D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pThis);
          
  return D4D_GetCrossColor(pScheme_tmp->objectDepend.progressBar.barFore, pScheme_tmp->objectDepend.progressBar.barEnd, D4D_MulDivUU8(value, 255, D4D_COLOR_PRGRS_BAR_MAX_VAL));      
}


/*******************************************************
*
* Set PROGRESS_BAR value
*
*******************************************************/

void D4D_PrgrsBarSetValue(D4D_OBJECT_PTR pThis, D4D_PROGRESS_BAR_VALUE value)
{
    D4D_PROGRESS_BAR* pPrgrsBar = D4D_GET_PROGRESS_BAR(pThis);
    D4D_PROGRESS_BAR_DATA* pData = pPrgrsBar->pData;       
    
    if(pData->value == value)
        return ;
    
    if(pData->value <= value)
      pPrgrsBar->pData->drawActiveOnly = D4D_TRUE;
    else
      pPrgrsBar->pData->drawActiveOnly = D4D_FALSE;
    
    // remember new value
    pData->value = value;
    
    if(pThis->initFlags & (D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_BAR_SCALECOLOR))
    {
      pData->colorBar = D4D_PrgrsBarComputeColorBar((D4D_OBJECT*)pThis, value);
    }
    
    pThis->flags->bits.bRedraw = 1;
}

D4D_PROGRESS_BAR_VALUE D4D_PrgrsBarGetValue(D4D_OBJECT_PTR pThis)
{
    D4D_PROGRESS_BAR* pPrgrsBar = D4D_GET_PROGRESS_BAR(pThis);
    return pPrgrsBar->pData->value;
}

/*******************************************************
*
* Set PROGRESS_BAR bar color
*
*******************************************************/

void D4D_PrgrsBarSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color)
{
    D4D_PROGRESS_BAR* pPrgrsBar = D4D_GET_PROGRESS_BAR(pThis);
    D4D_PROGRESS_BAR_DATA* pData = pPrgrsBar->pData;

    if(pThis->initFlags & (D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_BAR_SCALECOLOR))
      return;
    
    if(pData->colorBar == color)
        return ;
    
    // remember new color
    pData->colorBar = color;
    
    pThis->flags->bits.bRedraw = 1;    
}

D4D_COLOR D4D_PrgrsBarGetBarColor(D4D_OBJECT_PTR pThis)
{
    D4D_PROGRESS_BAR* pPrgrsBar = D4D_GET_PROGRESS_BAR(pThis);
    return pPrgrsBar->pData->colorBar;
}


/*******************************************************
*
* Modify PROGRESS_BAR value by a value
*
*******************************************************/

void D4D_PrgrsBarChangeValue(D4D_OBJECT_PTR pThis, D4D_PROGRESS_BAR_VALUE incr)
{
    D4D_PROGRESS_BAR_VALUE value = D4D_GET_PROGRESS_BAR(pThis)->pData->value;
    
    // change the value
    value += incr;
    
    if(value > D4D_COLOR_PRGRS_BAR_MAX_VAL)
      value = D4D_COLOR_PRGRS_BAR_MAX_VAL;

    D4D_PrgrsBarSetValue(pThis, (D4D_PROGRESS_BAR_VALUE)value);
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
* The main PROGRESS_BAR message handler 
*
*******************************************************/

void D4D_PrgrsBarOnMessage(D4D_MESSAGE* pMsg)
{
    D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pMsg->pObject);
    
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_PrgrsBarOnDraw(pMsg);
        break;       

    
    case D4D_MSG_ONINIT:
        if(!(pMsg->pObject->initFlags & (D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_BAR_SCALECOLOR)))
          D4D_GET_PROGRESS_BAR(pMsg->pObject)->pData->colorBar = pScheme_tmp->objectDepend.progressBar.barFore;                    
        else
          D4D_GET_PROGRESS_BAR(pMsg->pObject)->pData->colorBar = D4D_PrgrsBarComputeColorBar(pMsg->pObject, D4D_GET_PROGRESS_BAR(pMsg->pObject)->pData->value);  
        break;        

    default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
    }
}

