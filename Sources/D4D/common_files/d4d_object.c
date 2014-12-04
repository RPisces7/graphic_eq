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
* @file      d4d_object.c
*
* @author    b01119
* 
* @version   0.0.3.0
* 
* @date      Jun-14-2010
* 
* @brief     D4D driver object function c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/**************************************************************//*!
*
* invalidate object
*
******************************************************************/

void D4D_InvalidateObject(D4D_OBJECT_PTR pObject, Byte bComplete)
{
    pObject->flags->bits.bRedraw = 1;
    
    if(bComplete)
        pObject->flags->bits.bRedrawC = 1;
}

/**************************************************************//*!
*
* Show / Hide object
*
******************************************************************/

void D4D_ShowObject(D4D_OBJECT_PTR pObject, Byte bShow)
{
    if(bShow)
    {
        pObject->flags->bits.bVisible = 1;
        pObject->flags->bits.bRedraw = 1;
        pObject->flags->bits.bRedrawC = 1;
    }
    else
    {
        pObject->flags->bits.bVisible = 0;
        D4D_InvalidateScreen(D4D_GetActiveScreen(), D4D_TRUE);        
    }
}

/**************************************************************//*!
*
* Get the user pointer / data of of object
*
******************************************************************/

void* D4D_GetUserPointer(D4D_OBJECT *pThis)
{
  return pThis->userPointer;
}


/**************************************************************//*!
*
* enable or diasble touch pad capatibilities for an OBJECT
*
******************************************************************/

void D4D_EnableTouchScreen(D4D_OBJECT_PTR pObj, Byte bEnable, Byte bFastTouch)
{
    pObj->flags->bits.bTouchEnable = bEnable ? 1 : 0;
    pObj->flags->bits.bFastTouch = bFastTouch ? 1 : 0;
    
}

/**************************************************************//*!
*
* Capturing keys
*
******************************************************************/

void D4D_CaptureKeys(D4D_OBJECT_PTR  pObj)
{
    // NOTE: we need to send message, but we may just be in the middle of
    //       message processing (very likely). This may cause problem with
    //       the global d4d_msg object as we are changing it...
    //       We better use the temporary memory to instanitate the message
    
    D4D_MESSAGE* pMsg = (D4D_MESSAGE*) d4d_scratchPad;

    if(d4d_pKeysCapturer == pObj)
      return;

    pMsg->pScreen = D4D_GetActiveScreen();
    
    if(d4d_pKeysCapturer)
    {
        pMsg->nMsgId = D4D_MSG_KILLCAPTURE;
        pMsg->pObject = d4d_pKeysCapturer;
        D4D_SendMessage(pMsg);
        
        d4d_pKeysCapturer->flags->bits.bRedraw = 1;
    }
    
    d4d_pKeysCapturer = (D4D_OBJECT*) pObj;

    if(pObj != NULL)
    {
        pMsg->nMsgId = D4D_MSG_SETCAPTURE;
        pMsg->pObject = d4d_pKeysCapturer;
        D4D_SendMessage(pMsg);
        
        pObj->flags->bits.bRedraw = 1;
    }
}

/**************************************************************//*!
*
* Get Object that is capturing keys
*
******************************************************************/


D4D_OBJECT* D4D_GetCapturedObject(void)
{
  return d4d_pKeysCapturer;
}


/**************************************************************//*!
*
* enable or diasble tab stop for an OBJECT
*
******************************************************************/

void D4D_EnableTabStop(D4D_OBJECT_PTR pObj, Byte bEnable)
{
    pObj->flags->bits.bTabStop = bEnable ? 1 : 0;
}

/**************************************************************//*!
*
* enable or diasble  an OBJECT
*
******************************************************************/

void D4D_EnableObject(D4D_OBJECT_PTR pObj, Byte bEnable)
{
  if((pObj->flags->bits.bEnabled && !bEnable) || (!pObj->flags->bits.bEnabled && bEnable))
  {      
    pObj->flags->bits.bEnabled = bEnable ? 1 : 0;
    pObj->flags->bits.bRedrawC = 1;
  }
}


/**************************************************************//*!
*
* defualt message handler for all OBJECTS
*
******************************************************************/

void D4D_ObjOnMessage(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg); 
}