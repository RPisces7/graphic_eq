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
* @file      d4d_picture.c
*
* @author    r50233
* 
* @version   0.0.15.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver picture object c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

//Internal API
void D4D_PictOnMessage(D4D_MESSAGE* pMsg);
D4D_STRING* D4D_PictGetTextBuffer(D4D_OBJECT* pThis);

const D4D_OBJECT_SYS_FUNCTION d4d_pictSysFunc = 
{
  D4D_OBJECT_PICTURE,
  D4D_PictOnMessage,
  NULL,
  NULL
};

/*******************************************************
*
* PICTURE Drawing routine
*
*******************************************************/

static void D4D_PictOnDraw(D4D_MESSAGE* pMsg)
{
    D4D_PICTURE* pPict = D4D_GET_PICTURE(pMsg->pObject);
    
    // only draw if complete draw is needed
    if(!pMsg->prm.draw.bits.bComplete)
        return;
    
    // draw the bitmap
    if(pPict->pBmp != NULL)
    {
      D4D_POINT tmp_point = D4D_GetClientToScreenPoint(pMsg->pScreen, &pPict->scrPos);  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
      D4D_DrawRBmp(&tmp_point, pPict->pBmp, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled, pPict->radius);
#else        
      D4D_DrawBmp(&tmp_point, pPict->pBmp, (D4D_BOOL)!pMsg->pObject->flags->bits.bEnabled);
#endif        
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
* The main PICTURE message handler 
*
*******************************************************/

void D4D_PictOnMessage(D4D_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4D_MSG_DRAW:
        D4D_PictOnDraw(pMsg);
        break;
        
    default:
        // call the default behavior
        D4D_ObjOnMessage(pMsg);
    }
}