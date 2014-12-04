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
* @file      d4d_graph.c
*
* @author    b01119
* 
* @version   0.0.22.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver graph object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_GraphOnMessage(D4D_MESSAGE* pMsg);
Byte D4D_GraphCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_STRING* D4D_GraphGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_graphSysFunc = 
{
  D4D_OBJECT_GRAPH,
  D4D_GraphOnMessage,
  D4D_GraphCheckCoordinates,
  D4D_GraphGetTextBuffer
};

// temporary structure for icon coordinate calculation
typedef struct 
{
    D4D_POINT position;
    D4D_POINT graphPos;
    D4D_SIZE  graphSize;
    D4D_POINT valLblOff;
    Byte line_cnt;
    D4D_COOR last_Xpos;
    D4D_COOR curr_Xpos;   
} D4D_GRAPH_TMP_VAL;

#define _calc (*((D4D_GRAPH_TMP_VAL*)d4d_scratchPad))

static void D4D_GraphUpdateTempPosX(D4D_OBJECT* pThis) {
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  
  _calc.last_Xpos = (D4D_COOR)(_calc.graphPos.x + D4D_MulDivUU16((D4D_INDEX)((pData->xPosCnt - 1) % pData->dataShowX), _calc.graphSize.cx, pData->dataShowX));
  _calc.curr_Xpos = (D4D_COOR)(_calc.graphPos.x + D4D_MulDivUU16((D4D_INDEX)(pData->xPosCnt % pData->dataShowX), _calc.graphSize.cx, pData->dataShowX));
  
  if(_calc.curr_Xpos < _calc.last_Xpos)
    _calc.curr_Xpos = (D4D_COOR)(_calc.last_Xpos + D4D_MulDivUU16(1, _calc.graphSize.cx, pData->dataShowX));
}  



static void D4D_GraphValue2Coor(D4D_OBJECT* pThis)
{
  Byte i;
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);

  _calc.valLblOff.y = 0;
  _calc.valLblOff.x = 0;
  
  _calc.position = D4D_GetClientToScreenPoint(*pThis->pScreen, &pGraph->scrPos);

  _calc.graphPos = _calc.position;
  _calc.graphPos.x += D4D_GRAPH_BORDER_OFF;
 
  if((pGraph->textBuff.pText == NULL) || (D4D_GetFont(pGraph->textBuff.fontId) == NULL))
  {
    _calc.graphPos.y += D4D_GRAPH_BORDER_OFF;
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    if(pGraph->radius > D4D_GRAPH_BORDER_OFF)
      _calc.graphPos.y += (D4D_COOR)(pGraph->radius - D4D_GRAPH_BORDER_OFF);
#endif    
  }
  else
    _calc.graphPos.y += (D4D_COOR)(D4D_GetFontHeight(pGraph->textBuff.fontId) * 3 / 2);
 
  if(pThis->initFlags & D4D_GRAPH_F_VALUE_X_TOP)
  { 
    _calc.valLblOff.y = (D4D_COOR)(_calc.graphPos.y - _calc.position.y);
    _calc.graphPos.y += (D4D_COOR)(D4D_GRAPH_VALUE_OFF + D4D_GetFontHeight(pGraph->labelFont));         
  }
  
  if(pThis->initFlags & D4D_GRAPH_F_VALUE_Y_LEFT)
  { 
    _calc.valLblOff.x = (D4D_COOR)(D4D_GRAPH_VALUE_OFF);
    _calc.graphPos.x += (D4D_COOR)(D4D_GRAPH_VALUE_OFF + pGraph->pData->lblSizeY);         
  }
 
  _calc.graphSize.cx = (D4D_COOR)(pGraph->scrSize.cx - (_calc.graphPos.x -_calc.position.x) - D4D_GRAPH_BORDER_OFF);
  
  _calc.graphSize.cy = (D4D_COOR)(pGraph->scrSize.cy  - (_calc.graphPos.y -_calc.position.y) - D4D_GRAPH_BORDER_OFF);

  
  if(pThis->initFlags & D4D_GRAPH_F_VALUE_X_BOTT)
  {
    _calc.graphSize.cy -= (D4D_COOR)(D4D_GRAPH_VALUE_OFF + D4D_GetFontHeight(pGraph->labelFont));  
    _calc.valLblOff.y = (D4D_COOR)(_calc.graphPos.y - _calc.position.y + _calc.graphSize.cy + D4D_GRAPH_VALUE_OFF + 2);  
  }
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  else
    if(pGraph->radius > D4D_GRAPH_BORDER_OFF)
      _calc.graphSize.cy -= (D4D_COOR)(pGraph->radius - D4D_GRAPH_BORDER_OFF);
#endif    

      
  if(pThis->initFlags & D4D_GRAPH_F_VALUE_Y_RIGHT)
  {
    _calc.graphSize.cx -= (D4D_COOR)(D4D_GRAPH_VALUE_OFF +  pGraph->pData->lblSizeY);  
    _calc.valLblOff.x = (D4D_COOR)(_calc.graphPos.x - _calc.position.x + _calc.graphSize.cx + D4D_GRAPH_VALUE_OFF + 2);  
  }
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  else
    if(pGraph->radius > D4D_GRAPH_BORDER_OFF)
      _calc.graphSize.cx -= (D4D_COOR)(pGraph->radius - D4D_GRAPH_BORDER_OFF);
#endif    
  
  i=0;
  while(pGraph->traces[i].pData != NULL)
    i++;
  _calc.line_cnt = i; 
}

static D4D_COOR D4D_GraphGetSizeLblY(D4D_OBJECT* pThis) {
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  
  Byte i, j, k;
  
  if(pGraph->OnNeedLabelText == NULL)
    return (D4D_COOR)(3 * D4D_GetFontWidth(pGraph->labelFont)); // for label will be used numbers from 0 to 255
  
  j = 0;
  
  for(i=0; i < pGraph->grid.y_cnt; i++) 
  {
    k = (Byte)D4D_GetTextLength(pGraph->OnNeedLabelText(pThis,D4D_FALSE, i, 1));
    if(j < k) 
    {
      j = k;  
    }
  }
  
  return (D4D_COOR)(j * D4D_GetFontWidth(pGraph->labelFont));      
}
 
static D4D_COOR D4D_GraphGetGridCoorX(D4D_OBJECT* pThis, Byte gridIx) 
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);  
  return(D4D_COOR)(_calc.graphPos.x + D4D_MulDivUU16(gridIx, _calc.graphSize.cx, (Word)(pGraph->grid.x_cnt + 1)));  
}

static D4D_COOR D4D_GraphGetGridCoorY(D4D_OBJECT* pThis, Byte gridIx) 
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);  
  return(D4D_COOR)(_calc.graphPos.y + D4D_MulDivUU16(gridIx, _calc.graphSize.cy, (Word)(pGraph->grid.y_cnt + 1)));  
}

  
static void D4D_GraphPrintLabel(D4D_OBJECT* pThis, D4D_BOOL axisX, Byte gridIx, D4D_GRAPH_SAMPLE_IX sampleIx) 
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  D4D_COOR GridOffX = (D4D_COOR)(_calc.graphSize.cx / (pGraph->grid.x_cnt + 1));
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  D4D_STRING tmp_txtbuff;
  D4D_STR_PROPERTIES tmp_str_prty;              
  char tmp_str[8];
  char* pStr;

  tmp_str_prty.font_properties.all = D4D_GRAPH_LBL_FNT_PRTY_DEFAULT  << D4D_BITFIELD_SHIFT;
  tmp_str_prty.text_properties.all = 0 << D4D_BITFIELD_SHIFT;
  
  tmp_txtbuff.str_properties = &tmp_str_prty;
    

  if(axisX) {    
    if(gridIx > pGraph->grid.x_cnt)
      return;
  }else {
    if(gridIx > pGraph->grid.y_cnt)
      return;
  }
 
  if(pGraph->OnNeedLabelText == NULL) {    
    if(axisX) {
      tmp_str[D4D_SprintDecU16((Word)(sampleIx), &tmp_str[0], 0)] = 0;
    }else{
      tmp_str[D4D_SprintDecU8((Byte)((256 / (pGraph->grid.y_cnt + 1)) * (pGraph->grid.y_cnt - gridIx + 1)), &tmp_str[0], 0)] = 0;
    }
    
    pStr = tmp_str;
  }else {
    pStr = pGraph->OnNeedLabelText(pThis, axisX, (D4D_INDEX)((axisX)? gridIx : (pGraph->grid.y_cnt - gridIx + 1)), sampleIx);    
  }
  
  if(axisX) {    
    tmp_point.x = D4D_GraphGetGridCoorX(pThis, gridIx);
    tmp_point.x -= (D4D_COOR)(GridOffX / 2);
    tmp_point.y = (D4D_COOR)(_calc.position.y + _calc.valLblOff.y);
    tmp_size.cx = (D4D_COOR)(GridOffX - 1);
  }else {
    // axis Y
    tmp_point.y = D4D_GraphGetGridCoorY(pThis, gridIx);
    tmp_point.y -= (D4D_COOR)(1 + (D4D_GetFontHeight(pGraph->labelFont) / 2));
    tmp_point.x = (D4D_COOR)(_calc.position.x + _calc.valLblOff.x);
    tmp_size.cx = pData->lblSizeY;
  }
  
  tmp_size.cy = 0; // invoke autosize capability in draw function for y axis
  
  tmp_txtbuff.pText = pStr;
  tmp_txtbuff.fontId = pGraph->labelFont;
  tmp_txtbuff.str_properties->text_properties.bits.bAlignHoriz = D4D_TXT_PRTY_ALIGN_H_CENTER; // center aligment
  tmp_txtbuff.buffSize = (D4D_INDEX)(D4D_GetTextLength(tmp_txtbuff.pText) + 1);
  D4D_DrawTextRect(&tmp_point, &tmp_size, &tmp_txtbuff, D4D_ObjectGetForeFillColor(pThis), D4D_ObjectGetBckgFillColor(pThis));      
  
}


static void D4D_GraphDrawEmptyGraph(D4D_OBJECT* pThis) {
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  D4D_COLOR clrT;
  D4D_COOR tmp_coor;
  Byte i;
  
  D4D_GRAPH_SAMPLE_IX tmp_sampleIx = pGraph->pData->xPosCnt;
  
  tmp_sampleIx -= tmp_sampleIx % pGraph->pData->dataShowX;
  
  // Draw graph background box
  D4D_BoxXY((D4D_COOR)(_calc.graphPos.x - 1), (D4D_COOR)(_calc.graphPos.y - 1), (D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 2), (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy + 2), D4D_LINE_THIN, D4D_ObjectGetForeFillColor(pThis), D4D_ObjectGetBckgFillColor(pThis));
  
  clrT = D4D_ObjectGetScheme(pThis)->objectDepend.graph.grid;
 
  if(!pThis->flags->bits.bEnabled)
    clrT = D4D_GetGreyScale(clrT);
  
  // Draw drig lines in X direction
  for(i = 1; i < (pGraph->grid.x_cnt + 1);i++ )
  {
    tmp_coor = D4D_GraphGetGridCoorX(pThis, i);
    D4D_MoveToXY(tmp_coor, _calc.graphPos.y);
    D4D_LineToXY(tmp_coor, (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy + 1), D4D_LINE_THIN, clrT);  
    // Draw values on axis X if are enabled
    
    if(_calc.valLblOff.y)
    {      
      D4D_GraphPrintLabel(pThis, D4D_TRUE, (Byte)i, tmp_sampleIx + (i * (pGraph->pData->dataShowX / (pGraph->grid.x_cnt + 1)))); 
    }
  }
  
  // Draw drig lines in Y direction
  for(i=1; i < (pGraph->grid.y_cnt + 1);i++ )
  {
    D4D_MoveToXY(_calc.graphPos.x, D4D_GraphGetGridCoorY(pThis, i));
    D4D_LineToXY((D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 1), D4D_GraphGetGridCoorY(pThis, i), D4D_LINE_THIN, clrT);  
    if(_calc.valLblOff.x)
    {
      D4D_GraphPrintLabel(pThis, D4D_FALSE, (Byte)i, 1);
    }
  }  
}
  
/*******************************************************
*
* GRAPH Drawing routine
*
*******************************************************/

static void D4D_GraphOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_GRAPH* pGraph = D4D_GET_GRAPH(pMsg->pObject);
    D4D_GRAPH_DATA* pData = pGraph->pData;
    D4D_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4D_COLOR clrT, clrB;
    D4D_COLOR clrGrid = D4D_ObjectGetScheme(pMsg->pObject)->objectDepend.graph.grid;
    D4D_INDEX i;
    D4D_POINT tmp_linePoint;
    
    clrT = D4D_ObjectGetForeColor(pMsg->pObject, draw);
    clrB = D4D_ObjectGetBckgFillColor(pMsg->pObject);
    
    #ifdef D4D_DEBUG
      // sanity check
      D4D_ASSERT(sizeof(_calc) <= D4D_SCRATCHPAD_SIZE);
    #endif
    
    D4D_GraphValue2Coor(pMsg->pObject);
       
    /*****************************************
     * Draw complete object 
     ****************************************/
    if(draw.bits.bComplete)
    {
      if(pMsg->pObject->initFlags & (D4D_GRAPH_F_VALUE_Y_LEFT | D4D_GRAPH_F_VALUE_Y_RIGHT))
        pData->lblSizeY = D4D_GraphGetSizeLblY(pMsg->pObject);
      
      D4D_GraphValue2Coor(pMsg->pObject);
      
      #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
          // Draw main background box
        if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
          D4D_RBox(&_calc.position, &pGraph->scrSize, D4D_LINE_THIN, clrT, clrB, pGraph->radius);
        else
          D4D_FillRRect(&_calc.position, &pGraph->scrSize, clrB, pGraph->radius);
        
        // draw the text
        if(pGraph->textBuff.pText != NULL)
        {
          D4D_SIZE tmp_size;
          tmp_linePoint.x = (D4D_COOR)(_calc.position.x + 1 + pGraph->radius);      
          tmp_linePoint.y = (D4D_COOR)(_calc.position.y + D4D_GetFontHeight(pGraph->textBuff.fontId) / 4);      
          tmp_size.cx = (D4D_COOR)(pGraph->scrSize.cx - 2 - 2 * pGraph->radius); 
          tmp_size.cy = 0; // invoke automat
          D4D_DrawTextRect(&tmp_linePoint, &tmp_size, &pGraph->textBuff, D4D_ObjectGetForeFillColor(pMsg->pObject), D4D_ObjectGetBckgFillColor(pMsg->pObject));
        }
      
      #else
        // Draw main background box
        if(pMsg->pObject->initFlags & D4D_OBJECT_F_FOCUSRECT)
          D4D_Box(&_calc.position, &pGraph->scrSize, D4D_LINE_THIN, clrT, clrB);
        else
          D4D_FillRect(&_calc.position, &pGraph->scrSize, clrB);
 
        // draw the text
        if(pGraph->textBuff.pText != NULL)
        {
          D4D_SIZE tmp_size;
          tmp_linePoint.x = (D4D_COOR)(_calc.position.x + 1);      
          tmp_linePoint.y = (D4D_COOR)(_calc.position.y + D4D_GetFontHeight(pGraph->textBuff.fontId) / 4);      
          tmp_size.cx = (D4D_COOR)(pGraph->scrSize.cx - 2); 
          tmp_size.cy = 0; // invoke automat
          D4D_DrawTextRect(&tmp_linePoint, &tmp_size, &pGraph->textBuff, D4D_ObjectGetForeFillColor(pMsg->pObject), D4D_ObjectGetBckgFillColor(pMsg->pObject));
        }
 
 
      #endif    

      
           
      D4D_GraphDrawEmptyGraph(pMsg->pObject);
      
      
      
      // Sets the last data index to redraw all bytes in Memory on screen in complete redraw action
      if(pData->initData < (pGraph->dataLenght - 1)) {
        pData->xPosCnt = 1;
        pData->lastShowPos = 0;           
      }
      else {
        pData->lastShowPos = (D4D_INDEX)(pData->lastDataPos + 1);
        i = (D4D_COOR)(pData->xPosCnt % pData->dataShowX);
        if(i > pGraph->dataLenght)
          i = pGraph->dataLenght;
        pData->xPosCnt -= (D4D_COOR)i;//(pData->xPosCnt % _calc.graphSize.cx);
      }
      
      D4D_GraphUpdateTempPosX(pMsg->pObject);
      pData->columnDrawed = (D4D_COOR)(_calc.last_Xpos - 1);
      
      i = 1;
      while((D4D_GraphGetGridCoorX(pMsg->pObject,(Byte) i) < _calc.curr_Xpos) && (i < pGraph->grid.x_cnt)) {
        i++;
      };
      pData->drigDrawedX = i;
    }
    
   /*********************************************
   * Draw graph all lines from all pending input record  
   ********************************************/ 
    while(pData->lastShowPos != pData->lastDataPos)
    {            

      D4D_GraphUpdateTempPosX(pMsg->pObject);
      
      // if normla mode of graph is selected then in each lap of run the graph has to be redrawed
      if((pMsg->pObject->initFlags & D4D_GRAPH_F_MODE_MASK) == D4D_GRAPH_F_MODE_NORMAL)      
      {        
        /*********************************************
         * Draw graph in normal mode when it over run 
         ********************************************/
        if(pData->columnDrawed > _calc.curr_Xpos)
        {          
          D4D_GraphDrawEmptyGraph(pMsg->pObject);          
          pData->drigDrawedX = 1;
        }else 
        {
          // Draw values on axis X if are enabled
          if(_calc.valLblOff.y)
          {
            if(_calc.curr_Xpos != pData->columnDrawed) // check if this label was drawed yet on 
            {
              if(D4D_GraphGetGridCoorX(pMsg->pObject, (Byte)pData->drigDrawedX) < _calc.curr_Xpos)
              {
                D4D_GraphPrintLabel(pMsg->pObject, D4D_TRUE, (Byte)pData->drigDrawedX, pData->xPosCnt);
                pData->drigDrawedX++;
              }
            }
          }
        } 
      }
      
      if(pData->columnDrawed > _calc.curr_Xpos) {
        pData->drigDrawedX = 1;
        pData->columnDrawed = (D4D_COOR)(_calc.graphPos.x - 1);
      }

      if((pMsg->pObject->initFlags & D4D_GRAPH_F_MODE_MASK) == D4D_GRAPH_F_MODE_ROLLOVER)
      {
        if(pData->columnDrawed < _calc.curr_Xpos) {
          
         D4D_POINT tmp_Point;            
          
         if(_calc.last_Xpos == _calc.graphPos.x) {
           D4D_MoveToXY((D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 1), _calc.graphPos.y);                   
           D4D_FillRectToXY((D4D_COOR)(_calc.graphPos.x + _calc.graphSize.cx + 1), (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy), clrB);            
         }
         
          // *****  Clear the old drawed traces *****            
          
          tmp_Point.y = _calc.graphPos.y;          
          tmp_Point.x = (D4D_COOR)(pData->columnDrawed + 1);//(D4D_COOR)(_calc.last_Xpos + 1);          
          D4D_MoveTo(&tmp_Point);          
          
          tmp_Point.y += (D4D_COOR)(_calc.graphSize.cy + 1);            
          tmp_Point.x = _calc.curr_Xpos;                                              
          D4D_FillRectTo(&tmp_Point, clrB);
          
          // *****  End of: Clear the old drawed traces *****

          if(pMsg->pObject->flags->bits.bEnabled)
            clrT = clrGrid;
          else
            clrT = D4D_GetGreyScale(clrGrid);
        
                              
          //Draw vertical grid lines if needed          
          for(i = _calc.last_Xpos; i < _calc.curr_Xpos;i++) 
          {           
            if(D4D_GraphGetGridCoorX(pMsg->pObject, (Byte)pData->drigDrawedX) <= i)
            {
              if(pData->drigDrawedX <= pGraph->grid.x_cnt) {                  
                
                tmp_Point.x = (D4D_COOR)(i);
                tmp_Point.y = _calc.graphPos.y;
                D4D_MoveTo(&tmp_Point);
                tmp_Point.y += (D4D_COOR)(_calc.graphSize.cy + 1) ;
                D4D_LineTo(&tmp_Point, D4D_LINE_THIN, clrT);              
                
                if(_calc.valLblOff.y)                                        
                  D4D_GraphPrintLabel(pMsg->pObject, D4D_TRUE, (Byte)pData->drigDrawedX,  pData->xPosCnt);                                       

                pData->drigDrawedX++;
              }
            }
          }
          
          //Draw horizontal grid lines if needed
          for(i=1; i < (pGraph->grid.y_cnt + 1);i++ )
          {
            tmp_Point.y = D4D_GraphGetGridCoorY(pMsg->pObject,(Byte) i);
            tmp_Point.x = _calc.last_Xpos;
            D4D_MoveTo(&tmp_Point);
            tmp_Point.x = _calc.curr_Xpos;
            D4D_LineTo(&tmp_Point, D4D_LINE_THIN, clrT);              
          }
          
          // Draw disjuctive line
            
          tmp_Point.x = (D4D_COOR)(_calc.curr_Xpos + 1);            
          if(tmp_Point.x <= (_calc.graphPos.x + _calc.graphSize.cx)) {
           
            tmp_Point.y = _calc.graphPos.y;
            D4D_MoveTo(&tmp_Point);
            tmp_Point.y += (D4D_COOR)(_calc.graphSize.cy + 1);          
            D4D_LineTo(&tmp_Point, D4D_LINE_THIN, D4D_ObjectGetForeColor(pMsg->pObject, draw));  
          }
        }
      }
      
                          
      
      
      
      // add only a new data
      for(i=0; i < _calc.line_cnt;i++) // for all lines
      {
        tmp_linePoint.x = _calc.last_Xpos;
        
        switch(pGraph->traces[i].type & D4D_GRAPH_TRACE_TYPE_MASK)
        {
          case D4D_GRAPH_TRACE_TYPE_LINE:                            
            // Compute the last position of line
            
            if(pData->xPosCnt == 0)
            {
              tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy);                
              D4D_MoveTo(&tmp_linePoint);                                  
            }else
            {
              D4D_INDEX lastData;
              
              if(pGraph->pData->lastShowPos == 0)
                lastData = (D4D_INDEX)(pGraph->dataLenght - 1);
              else
                lastData = (D4D_INDEX)(pGraph->pData->lastShowPos - 1);  
              
              tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy - D4D_MulDivUU(pGraph->traces[i].pData[lastData], _calc.graphSize.cy, 255));
              D4D_MoveTo(&tmp_linePoint);
            }                            
            break;
            
          case D4D_GRAPH_TRACE_TYPE_DOT:
            tmp_linePoint.x = _calc.curr_Xpos;
            tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy - D4D_MulDivUU(pGraph->traces[i].pData[pData->lastShowPos], _calc.graphSize.cy, 255));                        
            D4D_MoveTo(&tmp_linePoint);
            break;
            
          case D4D_GRAPH_TRACE_TYPE_AREA:
            if(_calc.curr_Xpos <= (D4D_COOR)(_calc.last_Xpos + 1))
              tmp_linePoint.x = _calc.curr_Xpos;  
            tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy);                        
            D4D_MoveTo(&tmp_linePoint);
            break;
            
          case D4D_GRAPH_TRACE_TYPE_AREA_INV:              
            if(_calc.curr_Xpos <= (D4D_COOR)(_calc.last_Xpos + 1))
              tmp_linePoint.x = _calc.curr_Xpos;  
            tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y);                        
            D4D_MoveTo(&tmp_linePoint);
            break;
            
          default:
            break;
        }
        
        // Draw line
        tmp_linePoint.x = _calc.curr_Xpos;
        tmp_linePoint.y = (D4D_COOR)(_calc.graphPos.y + _calc.graphSize.cy - D4D_MulDivUU(pGraph->traces[i].pData[pData->lastShowPos], _calc.graphSize.cy, 255));
        
        if((((pGraph->traces[i].type & D4D_GRAPH_TRACE_TYPE_MASK) == D4D_GRAPH_TRACE_TYPE_AREA) || ((pGraph->traces[i].type & D4D_GRAPH_TRACE_TYPE_MASK) == D4D_GRAPH_TRACE_TYPE_AREA_INV)) && (_calc.curr_Xpos > (D4D_COOR)(_calc.last_Xpos + 1)))
          D4D_FillRectTo(&tmp_linePoint, pGraph->traces[i].color);
        else
          D4D_LineTo(&tmp_linePoint, pGraph->traces[i].line, pGraph->traces[i].color);                    
      }
      
      if(++pData->lastShowPos >= pGraph->dataLenght)
      {
        pData->lastShowPos = 0;
      }
              
      pData->columnDrawed = _calc.curr_Xpos;
      pData->xPosCnt++;
    }        
       
}

/*******************************************************
*
* The GRAPH add new data to one trace function 
*
*******************************************************/
D4D_BOOL D4D_GraphAddTraceData(D4D_OBJECT_PTR pObj, D4D_INDEX trace_ix,  D4D_GRAPH_VALUE value)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  Byte traces_cnt=0;
  Byte i;
  D4D_INDEX lastData;
  
  value = (D4D_GRAPH_VALUE)(((1<<(sizeof(D4D_GRAPH_VALUE)*8))- 1) - value);
  
  if(pGraph->pData->lastDataPos == 0)
    lastData = (D4D_INDEX)(pGraph->dataLenght - 1);
  else
    lastData = (D4D_INDEX)(pGraph->pData->lastDataPos - 1);      
  
  
  while(pGraph->traces[traces_cnt].pData != NULL)
    traces_cnt++;
  
  if(trace_ix > traces_cnt)
    return D4D_FALSE;
 
  for(i=0;i< traces_cnt;i++)
  {
    if(i == trace_ix)
    {
      pGraph->traces[i].pData[pGraph->pData->lastDataPos] = value;
    }else
    {
      if(pGraph->pData->xPosCnt)
        pGraph->traces[i].pData[pGraph->pData->lastDataPos] = 0;
      else
        pGraph->traces[i].pData[pGraph->pData->lastDataPos] = pGraph->traces[i].pData[lastData];
    }    
  }
  
  if(pGraph->pData->initData < (pGraph->dataLenght - 1)) {
    pGraph->pData->initData = (Word)(pGraph->pData->lastDataPos + 1);
  }
  
  if(++pGraph->pData->lastDataPos >= pGraph->dataLenght)
    pGraph->pData->lastDataPos = 0;
  
  
  if(pGraph->pData->lastDataPos == pGraph->pData->lastShowPos)
  {
    if(++pGraph->pData->lastShowPos >= pGraph->dataLenght)
      pGraph->pData->lastShowPos = 0;
    
    pGraph->pData->xPosCnt++;
  }
  
  pObj->flags->bits.bRedraw = 1;
  
  return D4D_TRUE;
}

/*******************************************************
*
* The GRAPH add new data to all lines function 
*
*******************************************************/
void D4D_GraphAddTracesData(D4D_OBJECT_PTR pObj, D4D_GRAPH_VALUE* pValues)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  Byte line_cnt=0;
  Byte i;
  
  while(pGraph->traces[line_cnt].pData != NULL)
    line_cnt++;
  
  for(i=0;i< line_cnt;i++)
  {
      pGraph->traces[i].pData[pGraph->pData->lastDataPos] = (D4D_GRAPH_VALUE)(((1<<((sizeof(D4D_GRAPH_VALUE)) * 8))- 1) - pValues[i]);        
  }
  
  if(pGraph->pData->initData < (pGraph->dataLenght - 1)) {
    pGraph->pData->initData = (Word)(pGraph->pData->lastDataPos + 1);
  }

  
  if(++pGraph->pData->lastDataPos >= pGraph->dataLenght)
    pGraph->pData->lastDataPos = 0;
  
  if(pGraph->pData->lastDataPos == pGraph->pData->lastShowPos)
  {
    if(++pGraph->pData->lastShowPos >= pGraph->dataLenght)
      pGraph->pData->lastShowPos = 0;
    pGraph->pData->xPosCnt++;
  }

  
  pObj->flags->bits.bRedraw = 1;
}

/*******************************************************
*
* The GRAPH clear all data and indexes in graph
*
*******************************************************/
void D4D_GraphClearAll(D4D_OBJECT_PTR pObj)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  pData->xPosCnt = 0;
  pData->lastShowPos = 0;
  pData->lastDataPos = 0;
  pData->initData = 0;
  pObj->flags->bits.bRedrawC = 1;
}

/*******************************************************
*
* The GRAPH get value of current sample index
*
*******************************************************/
LWord D4D_GraphGetSampleIndex(D4D_OBJECT_PTR pObj)
{
  return D4D_GET_GRAPH(pObj)->pData->xPosCnt;  
}


/*******************************************************
*
* The GRAPH get value of current sample index
*
*******************************************************/
D4D_COOR D4D_GraphGetSizeX(D4D_OBJECT_PTR pObj)
{
  D4D_GraphValue2Coor((D4D_OBJECT*)pObj);          
  return _calc.graphSize.cx;  
}

/*******************************************************
*
* The GRAPH set scale of x axis
*
*******************************************************/
D4D_BOOL D4D_GraphSetScaleX(D4D_OBJECT_PTR pObj, Byte mul, Byte div)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  
  if((mul == 0) || (div == 0))
    return D4D_FALSE;
  
  D4D_GraphValue2Coor((D4D_OBJECT*)pObj);
  
  pData->dataShowX = (D4D_INDEX)D4D_MulDivUU16(mul, _calc.graphSize.cx, div);
 
  pObj->flags->bits.bRedrawC = 1;
  
  return D4D_TRUE;  
}

/*******************************************************
*
* The GRAPH set number of samples showen in graph 
*
*******************************************************/
D4D_BOOL D4D_GraphSetDataWidth(D4D_OBJECT_PTR pObj, D4D_INDEX samples)
{
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pObj);
  D4D_GRAPH_DATA* pData = pGraph->pData;
  
  if(samples == 0)
    return D4D_FALSE;
  
  pData->dataShowX = samples;
  
  pObj->flags->bits.bRedrawC = 1;
  
  return D4D_TRUE;  
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
* The main GRAPH message handler 
*
*******************************************************/

void D4D_GraphOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_GraphOnDraw(pMsg);
        break;
                
    case D4D_MSG_TOUCHED:
      D4D_FocusSet(pMsg->pScreen, pMsg->pObject);
      
      break;
        
    case D4D_MSG_ONINIT:
      if(pMsg->pObject->initFlags & (D4D_GRAPH_F_VALUE_Y_LEFT | D4D_GRAPH_F_VALUE_Y_RIGHT))
        D4D_GET_GRAPH(pMsg->pObject)->pData->lblSizeY = D4D_GraphGetSizeLblY(pMsg->pObject); 
      
      D4D_GraphValue2Coor(pMsg->pObject); 
        
      D4D_GET_GRAPH(pMsg->pObject)->pData->initData = 0;     
      D4D_GET_GRAPH(pMsg->pObject)->pData->columnDrawed = (D4D_COOR)(_calc.graphPos.x - 1);
      
      if(!D4D_GET_GRAPH(pMsg->pObject)->pData->dataShowX) {        
        D4D_GET_GRAPH(pMsg->pObject)->pData->dataShowX = _calc.graphSize.cx; // initialization of scale for axis X to scale 1:1
      }
      break;
    
    default:
        // call the default behavior of all objects
        D4D_ObjOnMessage(pMsg);
    }
}

/*******************************************************
*
* The GRAPH check coordinators  handler 
*
*******************************************************/

Byte D4D_GraphCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point)
{
  
  D4D_GRAPH* pGraph = D4D_GET_GRAPH(pThis);
  
  D4D_GraphValue2Coor(pThis);
  
  if((point.x >= pGraph->scrPos.x) && (point.y >= pGraph->scrPos.y))
  {
    
      
    if((point.x <= (pGraph->scrPos.x + _calc.graphSize.cx)) && (point.y <= (pGraph->scrPos.y + _calc.graphSize.cy)))
      return D4D_TRUE;
  } 
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4D_STRING* D4D_GraphGetTextBuffer(D4D_OBJECT* pThis)
{
  return &(D4D_GET_GRAPH(pThis)->textBuff);  
}

