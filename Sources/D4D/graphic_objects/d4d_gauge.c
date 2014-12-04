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
* @file      d4d_gauge.c
*
* @author    b01119
* 
* @version   0.0.45.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver gauge object c file 
*
******************************************************************************/


#include "d4d.h"
#include "d4d_private.h"


// Internal API
void D4D_GaugOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_GaugCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_GaugeGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_gaugeSysFunc = 
{
  D4D_OBJECT_GAUGE,
  D4D_GaugOnMessage,
  D4D_GaugCheckCoordinates,
  D4D_GaugeGetTextBuffer
};

// temporary structure for pointer coordinates calculations
typedef struct D4D_GAUGE_VAL2PT_S
{
    D4D_POINT position;
    D4D_POINT hubXY;
    D4D_POINT currPointXY;
    D4D_POINT lastPointXY;   
} D4D_GAUGE_VAL2PT;


#define _calc (*((D4D_GAUGE_VAL2PT*)d4d_scratchPad))

/*******************************************************
*
* calculate pointer endpoint from basepoint, limits and value
*
* global structure _calc is used to exchange in/out data 
*
*******************************************************/

static D4D_POINT D4D_GaugeGetAnglePoint(D4D_GAUGE* pGaug, D4D_GAUGE_VALUE value) {
  D4D_GAUGE_DATA* pData = pGaug->pData;
  D4D_POINT tmp_res;
  Byte angle;
  sByte sin;
  sByte cos;
  
  // Compute angle in allowed angle range from allowed value range 
  angle = (Byte)D4D_MulDivUU8((Byte)(value - pData->limits.valueMin),
                            (Byte)(pData->limits.angleMax - pData->limits.angleMin), 
                            (Byte)(pData->limits.valueMax - pData->limits.valueMin));
  
  
  // Take care direction 
  if(pData->direction == D4D_CLOCK_WISE)
    angle += pData->limits.angleMin;
  else
    angle = (Byte)(pData->limits.angleMax - angle);
  
  // Compute sin value and cos value of computed pointer angle
  sin = D4D_Sin(angle);        
  cos = D4D_Cos(angle);        

 // Compute and fill up result point
  tmp_res = _calc.hubXY;
  tmp_res.x -= D4D_MulDivSU(cos, pGaug->pointerLen, 128);
  tmp_res.y -= D4D_MulDivSU(sin, pGaug->pointerLen, 128);
  
  return tmp_res;
}

static void D4D_GaugValue2Point(D4D_OBJECT* pThis)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;    
    
    _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pGaug->scrPos);
    
     if(pGaug->hubOff.x)
      _calc.hubXY.x = (D4D_COOR)(_calc.position.x + pGaug->hubOff.x+3);
    else
      _calc.hubXY.x = (D4D_COOR)(_calc.position.x + pGaug->scrSize.cx / 2);
    
    if(pGaug->hubOff.y)
      _calc.hubXY.y = (D4D_COOR)(_calc.position.y + pGaug->hubOff.y+3);
    else
      _calc.hubXY.y = (D4D_COOR)(_calc.position.y + pGaug->scrSize.cy / 2);
    
    
    _calc.currPointXY = D4D_GaugeGetAnglePoint(pGaug, pData->value);
    _calc.lastPointXY = D4D_GaugeGetAnglePoint(pGaug, pData->valueLast);
}

/*******************************************************
*
* GAUGE Drawing routine
*
*******************************************************/

static void D4D_GaugOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pMsg->pObject);
    D4D_GAUGE_DATA* pData = pGaug->pData;    
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    
    D4D_CLR_SCHEME *pScheme_tmp = D4D_ObjectGetScheme(pMsg->pObject);
    
    #ifdef D4D_DEBUG 
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif

    D4D_GaugValue2Point(pMsg->pObject);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE    
    
    if(draw.bits.bComplete)
    {
      // draw the bitmap
      if(pGaug->pBmpBkgd != NULL)
        D4D_DrawRBmpXY((D4D_COOR)(_calc.position.x + 1),(D4D_COOR) (_calc.position.y + 1), pGaug->pBmpBkgd, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pGaug->radius);
      else
        D4D_FillRRect(&_calc.position, &pGaug->scrSize, D4D_ObjectGetBckgFillColor(pMsg->pObject), pGaug->radius);
    }
    
    if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
    {
      // draw focus rectangle (drawing or hiding)
      D4D_RRect(&_calc.position, &pGaug->scrSize, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw), pGaug->radius); 
    }
#else
    
    if(draw.bits.bComplete)
    {
      // draw the bitmap
      if(pGaug->pBmpBkgd != NULL)
        D4D_DrawBmpXY((D4D_COOR)(_calc.position.x + 1),(D4D_COOR) (_calc.position.y + 1), pGaug->pBmpBkgd, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
      else
        D4D_FillRect(&_calc.position, &pGaug->scrSize, D4D_ObjectGetBckgFillColor(pMsg->pObject));
    }
    
    if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
    {
      // draw focus rectangle (drawing or hiding)
      D4D_Rect(&_calc.position, &pGaug->scrSize, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw)); 
    }

#endif    
    

    if(!draw.bits.bComplete)
    {
      D4D_MoveTo(&_calc.hubXY);
      D4D_LineTo(&_calc.lastPointXY, (D4D_LINETYPE)((pMsg->pObject->initFlags & D4D_GAUGE_F_THICK_POINTER)? D4D_LINE_THICK:D4D_LINE_THIN), pScheme_tmp->bckg);
    }
    // draw the text
    if(((pMsg->pObject->initFlags & D4D_GAUGE_F_REDRAW_TEXT) || draw.bits.bComplete) && pGaug->textBuff.pText != NULL)
    {
        D4D_POINT tmp_point;
        
        tmp_point.x = (D4D_COOR)(_calc.position.x + pGaug->txtOff.x);
        tmp_point.y = (D4D_COOR)(_calc.position.y + pGaug->txtOff.y);
            
        D4D_DrawTextRect(&tmp_point, (D4D_SIZE*)&d4d_size_zero, &pGaug->textBuff,
            D4D_ObjectGetForeColor(pMsg->pObject, draw), D4D_ObjectGetBckgFillColor(pMsg->pObject));    
    }
    
    // draw the hub circle
    if(pMsg->pObject->initFlags & D4D_GAUGE_F_HUB)
    {      
      if(pMsg->pObject->flags->bits.bEnabled)
        D4D_FillCircle(&_calc.hubXY, D4D_GAUGE_HUB_RADIUS, pScheme_tmp->objectDepend.gauge.hub);
      else
        D4D_FillCircle(&_calc.hubXY, D4D_GAUGE_HUB_RADIUS, D4D_GetGreyScale(pScheme_tmp->objectDepend.gauge.hub));
    }
    // draw the pointer on value
    D4D_MoveTo(&_calc.hubXY);
    
    if(pMsg->pObject->flags->bits.bEnabled)
        D4D_LineTo(&_calc.currPointXY, (D4D_LINETYPE)((pMsg->pObject->initFlags & D4D_GAUGE_F_THICK_POINTER)? D4D_LINE_THICK:D4D_LINE_THIN), pScheme_tmp->objectDepend.gauge.pointer);
      else
        D4D_LineTo(&_calc.currPointXY, (D4D_LINETYPE)((pMsg->pObject->initFlags & D4D_GAUGE_F_THICK_POINTER)? D4D_LINE_THICK:D4D_LINE_THIN), D4D_GetGreyScale(pScheme_tmp->objectDepend.gauge.pointer));
    
    // remember the last value drawn
    pData->valueLast = pData->value;
}

/*******************************************************
*
* Set GAUGE value
*
*******************************************************/

void D4D_GaugSetValue(D4D_OBJECT_PTR pThis, D4D_GAUGE_VALUE value)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;

    if(pGaug->pData->value == value)
        return ;

    // keep within limits    
     value = D4D_LimitS8(value, pData->limits.valueMin, pData->limits.valueMax);      

    // remember new value
    pGaug->pData->value = value;
    pThis->flags->bits.bRedraw = 1;

    // notify user that the value has changed        
    if(pGaug->OnValueChanged)
        pGaug->OnValueChanged(pThis);
}

D4D_GAUGE_VALUE D4D_GaugGetValue(D4D_OBJECT_PTR pThis)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    return pGaug->pData->value;
}


/*******************************************************
*
* Configure GAUGE limits
*
*******************************************************/

void D4D_GaugSetLimits(D4D_OBJECT_PTR pThis, const D4D_GAUGE_LIMITS* pLimits)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    D4D_GAUGE_DATA* pData = pGaug->pData;

    pGaug->pData->limits = *pLimits;
    
    if(pData->limits.valueMin > pData->limits.valueMax)
    {
      D4D_GAUGE_VALUE value_tmp = pData->limits.valueMin;
      
      pData->limits.valueMin = pData->limits.valueMax;
      
      pData->limits.valueMax = value_tmp;
      
      if(pData->direction != D4D_ANTI_CLOCK_WISE)
        pData->direction = D4D_ANTI_CLOCK_WISE;
            
    }

    // keep value within limits
    pData->value = D4D_LimitS8(pData->value, pData->limits.valueMin, pData->limits.valueMax);    

    pThis->flags->bits.bRedrawC = 1;

    // notify user that the value has changed        
    if(pGaug->OnValueChanged)
        pGaug->OnValueChanged(pThis);
}


void D4D_GaugGetLimits(D4D_OBJECT_PTR pThis, D4D_GAUGE_LIMITS* pLimits)
{
    D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
    *pLimits = pGaug->pData->limits;
}

/*******************************************************
*
* Configure GAUGE direction
*
*******************************************************/

void D4D_GaugSetDir(D4D_OBJECT_PTR pThis, D4D_GAUGE_DIR direction)
{
  D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
  D4D_GAUGE_DATA* pData = pGaug->pData;
  
  if(direction != pData->direction)
  {
    pData->valueLast = pData->value;  
    pData->direction = direction;
    
    pThis->flags->bits.bRedrawC = 1;
    
    if(pGaug->OnValueChanged)
        pGaug->OnValueChanged(pThis);  
  }
}

D4D_GAUGE_DIR D4D_GaugGetDir(D4D_OBJECT_PTR pThis)
{
  D4D_GAUGE* pGaug = D4D_GET_GAUGE(pThis);
  return pGaug->pData->direction;  
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
* The main GAUGE message handler 
*
*******************************************************/

void D4D_GaugOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_GaugOnDraw(pMsg);
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
* The GAUGE check coordinators  handler 
*
*******************************************************/

Byte D4D_GaugCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  D4D_GAUGE* pGauge = D4D_GET_GAUGE(pThis);
  
  if((point.x >= pGauge->scrPos.x) && (point.y >= pGauge->scrPos.y))
  { 
    if((point.x <= (pGauge->scrPos.x + pGauge->scrSize.cx)) && (point.y <= (pGauge->scrPos.y + pGauge->scrSize.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_GaugeGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_GAUGE(pThis)->textBuff);  
}
