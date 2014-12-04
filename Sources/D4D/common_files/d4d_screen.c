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
* @file      d4d_screen.c
*
* @author    b01119
* 
* @version   0.0.25.0
* 
* @date      Feb-2-2011
* 
* @brief     D4D driver screen functions c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"


static D4D_COOR D4D_GetScrHeaderSize(D4D_SCREEN* pScreen);
static D4D_COOR D4D_GetScrHeaderTitleOffset(D4D_SCREEN* pScreen);
static D4D_BOOL D4D_GetScrHeaderExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* resultPos, D4D_SIZE* resultSize);
static void D4D_DrawScreenNC(D4D_SCREEN* pScreen, D4D_BOOL active);
static Byte D4D_FindObject(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject);
static Byte D4D_GetObjectCount(D4D_SCREEN* pScreen);
/**************************************************************//*!
*
* Get active screen
*
******************************************************************/

D4D_SCREEN* D4D_GetActiveScreen(void)
{
    // no screen yet shown
    if(d4d_screenHistoryIndex <= 0)
        return NULL;
    
    return d4d_screenHistory[d4d_screenHistoryIndex - 1];
}

/**************************************************************//*!
*
* Change the active screen
*
******************************************************************/
static void D4D_ChangeScreen(D4D_SCREEN* pNewScreen, D4D_SCREEN* pOldScreen)
{    
    D4D_SCREEN_DATA* pData;
    D4D_MESSAGE tmp_msg;
    Byte ix;
    
    if(pNewScreen == NULL)
      return;
    
    if(pOldScreen == pNewScreen)
      return;
    
    tmp_msg.pScreen = pOldScreen;
    
    if(pOldScreen != NULL)
    {
      tmp_msg.nMsgId = D4D_MSG_KILLFOCUS;
      tmp_msg.pObject = (D4D_OBJECT*) pOldScreen->pObjects[pOldScreen->pData->focusedObjIx];
      D4D_SendMessage(&tmp_msg);
              
      // Draw NC screen area as an inactivate
      D4D_DrawScreenNC(pOldScreen, D4D_FALSE);
      
      // call de-activate event
      if(pOldScreen->OnDeactivate != NULL)
          pOldScreen->OnDeactivate();       
    }

    // invalidate the new screen (global complete redraw, not individual objects)
    D4D_InvalidateScreen(pNewScreen, D4D_TRUE);
    
    // if this is the first time activating 
    pData = pNewScreen->pData;  
    
    // initialize the pointers on this screen in all screen objects
    ix = 0;
    tmp_msg.pScreen = pNewScreen;
    tmp_msg.nMsgId = D4D_MSG_ONINIT;
    
    while(pNewScreen->pObjects[ix] != NULL) {
      
      *(pNewScreen->pObjects[ix]->pScreen) = pNewScreen;
      
      
      tmp_msg.pObject = (D4D_OBJECT*)pNewScreen->pObjects[ix];
      
      if(!(tmp_msg.pObject->flags->bits.bInitDone))
      {            
          
          // send the ON INIT message
          D4D_SendMessage(&tmp_msg);
          tmp_msg.pObject->flags->bits.bInitDone = 1;            
      }
      
      ix++;
      
    }
    
    
    
    if(!pData->flags.bits.bInitDone)
    {
        pData->flags.bits.bInitDone = 1;
        
        // user's screen initialization
        if(pNewScreen->OnInit)
            pNewScreen->OnInit();
        
        D4D_FocusNextObject(pNewScreen, D4D_TRUE);
    }else
    {
      tmp_msg.pScreen = pNewScreen;
      tmp_msg.nMsgId = D4D_MSG_SETFOCUS;
      tmp_msg.pObject = (D4D_OBJECT*) pNewScreen->pObjects[pData->focusedObjIx];
      D4D_SendMessage(&tmp_msg);          
    }
    
    // inform the screen it has been activated
    if(pNewScreen->OnActivate)
        pNewScreen->OnActivate();
    
    D4D_ClearKeysBuffer();
  
  #ifdef D4D_LLD_TCH  
    d4d_LastTouchedObj = NULL;
  #endif
}


/**************************************************************//*!
*
* Enter new screen, remember the history if not only replacing
*
******************************************************************/
void D4D_ActivateScreen(D4D_SCREEN* pNewScreen, Byte bReplaceCurrent)
{    
  D4D_SCREEN* pOldScreen;

  // can not activate screen (no space left in history array)
  if(d4d_screenHistoryIndex >= D4D_SCREEN_HISTORY && !bReplaceCurrent)
      return ;
  
  // cancel key capture
  D4D_CaptureKeys(NULL);

  // deactivate current screen (if any)
  pOldScreen = D4D_GetActiveScreen();

  if(pOldScreen != NULL)
  {
      // replacing this screen in history?
      if(bReplaceCurrent)
          d4d_screenHistoryIndex--; // note that we are sure this is >0
  }

  // set the new screen as the active one
  d4d_screenHistory[d4d_screenHistoryIndex] = pNewScreen;
  d4d_screenHistoryIndex++;

  D4D_ChangeScreen(pNewScreen, pOldScreen);    
}

 

/**************************************************************//*!
*
* Return to previous screen recorded in the history
*
******************************************************************/
void D4D_EscapeScreen(void)
{
    D4D_SCREEN* pOldScreen;
    
    // can not escape current screen, it is the top one 
    if(d4d_screenHistoryIndex <= 1)
      return;
    
    // cancel key capture
    D4D_CaptureKeys(NULL);

    pOldScreen = D4D_GetActiveScreen();
    
    // pop the history stack
    if(d4d_screenHistoryIndex > 1)
      d4d_screenHistoryIndex--;
    
    D4D_ChangeScreen(D4D_GetActiveScreen(), pOldScreen);

}


/**************************************************************//*!
*
* Return to previous screen recorded in the history
*
******************************************************************/
void D4D_EscapeToBaseScreen(void)
{
    D4D_SCREEN* pOldScreen;

    // can not escape current screen, it is the top one 
    if(d4d_screenHistoryIndex == 0)
        return;
    
    // cancel key capture
    D4D_CaptureKeys(NULL);

    
    // I can be sure this is not NULL
    pOldScreen = D4D_GetActiveScreen();

    // pop the history stack
    d4d_screenHistoryIndex = 1;    
    
    D4D_ChangeScreen(D4D_GetActiveScreen(), pOldScreen);
}

#if 0
/**************************************************************//*!
*
* Enter new screen, remember the history if not only replacing
*
******************************************************************/

void D4D_ActivateScreen(D4D_SCREEN* pNewScreen, Byte bReplaceCurrent)
{    
  D4D_SCREEN* pOldScreen;
  D4D_SCREEN_DATA* pData;
  
  // cancel key capture
  D4D_CaptureKeys(NULL);

  // can not activate screen (no space left in history array)
  if(d4d_screenHistoryIndex >= D4D_SCREEN_HISTORY && !bReplaceCurrent)
    return ;

  // deactivate current screen (if any)
  pOldScreen = D4D_GetActiveScreen();
  if(pOldScreen != NULL)
  {
    // Draw NC screen area as an inactivate
    D4D_DrawScreenNC(pOldScreen, D4D_FALSE);
    
    // call de-activate event
    if(pOldScreen->OnDeactivate != NULL)
        pOldScreen->OnDeactivate();
    
    // replacing this screen in history?
    if(bReplaceCurrent)
        d4d_screenHistoryIndex--; // note that we are sure this is >0
  }

  // set the new screen as the active one
  d4d_screenHistory[d4d_screenHistoryIndex] = pNewScreen;
  d4d_screenHistoryIndex++;

  // invalidate the new screen (global complete redraw, not individual objects)
  D4D_InvalidateScreen(pNewScreen, D4D_TRUE);
  
  // if this is the first time activating 
  pData = pNewScreen->pData;
  
  if(!pData->flags.bits.bInitDone)
  {
    // Init the screen and its objects
    D4D_InitScreen(pNewScreen);
        
    // find first object with tabstop enabled and set focusedObjOx
    D4D_FocusNextObject(pNewScreen, D4D_TRUE);
  }
  
  // inform the screen it has been activated
  if(pNewScreen->OnActivate)
      pNewScreen->OnActivate();
  
  D4D_ClearKeysBuffer();
  
  #ifdef D4D_LLD_TCH  
    d4d_LastTouchedObj = NULL;
  #endif  
}



/**************************************************************//*!
*
* Return to previous screen recorded in the history
*
******************************************************************/

void D4D_EscapeScreen(void)
{
    D4D_SCREEN* pScreen;

    // cancel key capture
    D4D_CaptureKeys(NULL);

    // can not escape current screen, it is the top one 
    if(d4d_screenHistoryIndex <= 1)
        return;
    
    // I can be sure this is not NULL
    pScreen = D4D_GetActiveScreen();

    // call de-activate event
    if(pScreen->OnDeactivate)
        pScreen->OnDeactivate();
    
    D4D_DrawScreenNC(pScreen, D4D_FALSE);
    
    // pop the history stack
    d4d_screenHistoryIndex--;

    // again, I can be sure this is not NULL
    pScreen = D4D_GetActiveScreen();

    // invalidate the new screen
    D4D_InvalidateScreen(pScreen, D4D_TRUE);    
    
    // inform the screen it has been activated
    if(pScreen->OnActivate)
        pScreen->OnActivate();
  #ifdef D4D_LLD_TCH  
    d4d_LastTouchedObj = NULL;
  #endif  
}


#endif
/**************************************************************//*!
*
* Init screen and all objects on given screen
*
******************************************************************/

void D4D_InitScreen(D4D_SCREEN* pScreen)
{
  D4D_SCREEN_DATA* pData = pScreen->pData;
  D4D_OBJECT* pObj;
  Byte i;

  if(!pData->flags.bits.bInitDone)
  {
    pData->flags.bits.bInitDone = 1;
    
    // user's screen initialization
    if(pScreen->OnInit)
        pScreen->OnInit();
  }

  // prepare message
  d4d_msg.pScreen = pScreen;
  d4d_msg.nMsgId = D4D_MSG_ONINIT;
  
  // redraw objects   
  for(i=0; pScreen->pObjects[i] != NULL; i++)
  {
    pObj = (D4D_OBJECT*) pScreen->pObjects[i];
    
    d4d_msg.pObject = pObj;
    
    // initialize the pointers on this screen in all screen objects
    *(pObj->pScreen) = pScreen;
    
    if(!(pObj->flags->bits.bInitDone))
    {            
      // send the ON INIT message
      D4D_SendMessage(&d4d_msg);
      pObj->flags->bits.bInitDone = 1;            
    }
  }
}


/**************************************************************//*!
*
* Mark all object on screen as "redraw pennding"
*
******************************************************************/

void D4D_InvalidateScreen(D4D_SCREEN* pScreen, Byte bComplete)
{
    D4D_OBJECT* pObj;
    Byte i;

    if(bComplete)
    {
        pScreen->pData->flags.bits.bCompleteRedraw = 1;
    }
    else
    {
        for(i=0; pScreen->pObjects[i] != NULL; i++)
        {
            pObj = (D4D_OBJECT*) pScreen->pObjects[i];
            pObj->flags->bits.bRedraw = 1;
        }
    }
}

/**************************************************************//*!
*
* Gety the count of object in given screen
*
******************************************************************/

static Byte D4D_GetObjectCount(D4D_SCREEN* pScreen)
{
    Byte ix = 0;
    
    while(pScreen->pObjects[ix] != NULL)
        ix++;
    
    return ix;
}

/**************************************************************//*!
*
* Get focused object in a given screen
*
******************************************************************/

D4D_OBJECT_PTR D4D_GetFocusedObject(D4D_SCREEN* pScreen)
{
    if(!pScreen)
        return NULL;
           
    // check if index is not out of range?                
    if(pScreen->pData->focusedObjIx > D4D_GetObjectCount(pScreen))
      return NULL;
    
    // just return the object on a focus index
    return pScreen->pObjects[pScreen->pData->focusedObjIx];
}



 /**************************************************************//*!
*
* Focus next object in given screen
*
******************************************************************/

void D4D_FocusNextObject(D4D_SCREEN* pScreen, Byte bInitialSearch)
{
    D4D_SCREEN_DATA* pData = pScreen->pData;
    const D4D_OBJECT* const* pObjects = pScreen->pObjects;
    Byte ix = pData->focusedObjIx;
    
    // sanity check of list of objects - contains Screen any object?
    if(pObjects[ix] == NULL)
      return;

    // currently focused object already has a tabstop    
    if(bInitialSearch)
      if(pObjects[ix]->flags->bits.bTabStop && pObjects[ix]->flags->bits.bEnabled)
        return ;
    
    do
    {
        // get next object
        ix++;
        
        // wrap around in the array if end is reached
        if(pObjects[ix] == NULL)
            ix = 0;
        
        // object with focus enabled?
        if(pObjects[ix]->flags->bits.bTabStop && pObjects[ix]->flags->bits.bEnabled)
            break;
        
      // avoid endless loop if no focused object can be found
    } while(ix != pData->focusedObjIx);
    
    if(ix != pData->focusedObjIx)
    {        
      // invalidate object which is loosing focus
      pObjects[pData->focusedObjIx]->flags->bits.bRedraw = 1;
      
      // prepare message KILLFOCUS
      d4d_msg.pScreen = pScreen;
      d4d_msg.nMsgId = D4D_MSG_KILLFOCUS;
      d4d_msg.pObject = (D4D_OBJECT*) pObjects[pData->focusedObjIx];
      D4D_SendMessage(&d4d_msg);
      
      
      // invalidate object which is getting focus
      pObjects[ix]->flags->bits.bRedraw = 1;
      
      // move the focus
      pData->focusedObjIx = ix;
      
      // prepare message
      d4d_msg.pScreen = pScreen;
      d4d_msg.nMsgId = D4D_MSG_SETFOCUS;
      d4d_msg.pObject = (D4D_OBJECT*) pObjects[ix];
      D4D_SendMessage(&d4d_msg);
    }
}

/**************************************************************//*!
*
* Focus previous object in given screen
*
******************************************************************/

void D4D_FocusPrevObject(D4D_SCREEN* pScreen)
{
    D4D_SCREEN_DATA* pData = pScreen->pData;
    const D4D_OBJECT* const* pObjects = pScreen->pObjects;
    Byte ix = pData->focusedObjIx;
    
    // sanity check of list of objects - contains Screen any object?
    if(pObjects[ix] == NULL)
      return;
    
    do
    {
        // wrap around in the array if begin is reached
        if(ix == 0)
        {           
            // assuming at least one object in the screen always
            ix = D4D_GetObjectCount(pScreen);
            // but better be robust
            if(ix) ix--;
        }
        else
        {
            // just get previous object
            ix--;
        }
        
        // object with focus enabled?
        if(pObjects[ix]->flags->bits.bTabStop && pObjects[ix]->flags->bits.bEnabled)
            break;
        
      // avoid endless loop if no focused object can be found
    } while(ix != pData->focusedObjIx);
    
    if(ix != pData->focusedObjIx)
    {        
      // invalidate object which is loosing focus
      pObjects[pData->focusedObjIx]->flags->bits.bRedraw = 1;
      
      // prepare message KILLFOCUS
      d4d_msg.pScreen = pScreen;
      d4d_msg.nMsgId = D4D_MSG_KILLFOCUS;
      d4d_msg.pObject = (D4D_OBJECT*) pObjects[pData->focusedObjIx];
      D4D_SendMessage(&d4d_msg);
      
      
      // invalidate object which is getting focus
      pObjects[ix]->flags->bits.bRedraw = 1;
      
      // move the focus
      pData->focusedObjIx = ix;
      
      // prepare message
      d4d_msg.pScreen = pScreen;
      d4d_msg.nMsgId = D4D_MSG_SETFOCUS;
      d4d_msg.pObject = (D4D_OBJECT*) pObjects[ix];
      D4D_SendMessage(&d4d_msg);
    }
}

/**************************************************************//*!
*
* Find object's index
*
******************************************************************/

static Byte D4D_FindObject(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject)
{
    Byte ix;
    
    for(ix = 0; pScreen->pObjects[ix] != NULL; ix++)
        if(pScreen->pObjects[ix] == pObject)
            return ix;
        
    // not found
    return 0xff;
}

/**************************************************************//*!
*
* Focus selected object in given screen
*
******************************************************************/

void D4D_FocusSet(D4D_SCREEN* pScreen, D4D_OBJECT_PTR pObject)
{
    D4D_SCREEN_DATA* pData = pScreen->pData;
    const D4D_OBJECT* const* pObjects = pScreen->pObjects;
    Byte ix = pData->focusedObjIx;
    Byte i;
    
    if(pScreen == NULL)
      return;
    
    // check if object is really item of the current screen
    i = D4D_FindObject(pScreen, pObject);
    if(i == 0xff)
      return;
    
    if(i == ix) // is selected object same as focused?
      return;
  
    if(!pObject->flags->bits.bTabStop  || !pObject->flags->bits.bEnabled) // is this object selectable?
      return;
    
    // invalidate object which is loosing focus
    pObjects[ix]->flags->bits.bRedraw = 1;
    
    // prepare message
    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_KILLFOCUS;
    d4d_msg.pObject = (D4D_OBJECT*) pObjects[ix];
    D4D_SendMessage(&d4d_msg);
    
    // invalidate object which is getting focus
    pObjects[i]->flags->bits.bRedraw = 1;
    
    // move the focus
    pData->focusedObjIx = i;
    
    // prepare message
    d4d_msg.pScreen = pScreen;
    d4d_msg.nMsgId = D4D_MSG_SETFOCUS;
    d4d_msg.pObject = (D4D_OBJECT*)  pObjects[i];
    D4D_SendMessage(&d4d_msg);
        
}


/**************************************************************//*!
*
* enable or diasble touch pad capatibilities for an SCREEN
*
******************************************************************/
#ifdef D4D_LLD_TCH
  void D4D_EnableScrTouchScreen(const D4D_SCREEN* pScr, Byte bEnable)
  {
      pScr->pData->flags.bits.bTouchEnable = bEnable ? 1 : 0;    
  }
#else
  void D4D_EnableScrTouchScreen(const D4D_SCREEN* pScr, Byte bEnable)
  {
    D4D_UNUSED(pScr);
    D4D_UNUSED(bEnable);
  }
#endif  


/**************************************************************//*!
*
* Set Screen text font properties
*
******************************************************************/

void D4D_SetScreenFontProperties(D4D_SCREEN* pScreen, D4D_FONT_PROPERTIES property) 
{
  if(pScreen->textBuff.str_properties->font_properties.all == property.all)
    return;     // There is no change needed
  
  pScreen->textBuff.str_properties->font_properties.all = property.all;
  D4D_InvalidateScreen(pScreen, D4D_FALSE);
}

/**************************************************************//*!
*
* Set Object text properties
*
******************************************************************/

void D4D_SetScreenTextProperties(D4D_SCREEN* pScreen, D4D_TEXT_PROPERTIES property) 
{
  if(pScreen->textBuff.str_properties->text_properties.all == property.all)
    return;     // There is no change needed
  
  pScreen->textBuff.str_properties->text_properties.all = property.all;
  D4D_InvalidateScreen(pScreen, D4D_FALSE);

}



/**************************************************************//*!
*
* Compute size of screen header 
*
******************************************************************/

static D4D_COOR D4D_GetScrHeaderSize(D4D_SCREEN* pScreen)
{
  D4D_COOR tmp_size = 0;
  
  if(pScreen == NULL)
    return 0;
  
  if(pScreen->flags.bits.bHeader || pScreen->textBuff.pText)
  {
    tmp_size = (D4D_COOR)(D4D_SCR_TITLE_OFF_Y * 2 + D4D_GetFontHeight(pScreen->textBuff.fontId));
  }

  if(pScreen->pIcon != NULL)
    if(tmp_size < (D4D_GetBmpHeight(pScreen->pIcon) + D4D_SCR_TITLE_OFF_Y * 2 + 1))
      tmp_size = (D4D_COOR)(D4D_GetBmpHeight(pScreen->pIcon) + D4D_SCR_TITLE_OFF_Y * 2 + 1);          
  
  if(pScreen->flags.bits.bHeader) 
    if(tmp_size < D4D_SCR_HEADER_SIZE_MIN_SIZE)
      tmp_size = D4D_SCR_HEADER_SIZE_MIN_SIZE;   
        
  return tmp_size;  
}

/**************************************************************//*!
*
* Compute offset of screen header title
*
******************************************************************/

static D4D_COOR D4D_GetScrHeaderTitleOffset(D4D_SCREEN* pScreen)
{
  D4D_COOR tmp_off = D4D_SCR_TITLE_OFF_X;
  
  if(pScreen == NULL)
    return 0;
  
  if(pScreen->pIcon != NULL)    
    tmp_off = (D4D_COOR)(D4D_GetBmpWidth(pScreen->pIcon) + D4D_SCR_TITLE_OFF_X * 2);          
  
  #if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
    tmp_off += (pScreen->radius * 2);
  #endif  
      
  return tmp_off;  
}


/**************************************************************//*!
*
* Compute position of exit button on screen header title
*
******************************************************************/

static D4D_BOOL D4D_GetScrHeaderExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* resultPos, D4D_SIZE* resultSize)
{        
  D4D_COOR tmp_coor = D4D_GetScrHeaderSize(pScreen);
  
  resultPos->x = 0;
  resultPos->y = 0;
  
  resultSize->cx = 0;
  resultSize->cy = 0;
  
  if(pScreen == NULL)
    return D4D_FALSE;
  
  if((tmp_coor - D4D_SCR_TITLE_EXITBTN_OFFSET * 2) <= 0)
    return D4D_FALSE;

  tmp_coor -= D4D_SCR_TITLE_EXITBTN_OFFSET * 2;
  
  if(tmp_coor < D4D_SCR_TITLE_EXITBTN_MIN_SIZE)
    return D4D_FALSE;
  
  resultSize->cx = tmp_coor;
  resultSize->cy = tmp_coor;
   
  resultPos->x = (D4D_COOR)(pScreen->position.x + pScreen->size.cx - tmp_coor - D4D_SCR_TITLE_EXITBTN_OFFSET);
  resultPos->y = (D4D_COOR)(pScreen->position.y + D4D_SCR_TITLE_EXITBTN_OFFSET);
  
  return D4D_TRUE;
}

/**************************************************************//*!
*
* Redraw non client screen area - Header, background, title, etc.
*
******************************************************************/

static void D4D_DrawScreenNC(D4D_SCREEN* pScreen, D4D_BOOL active)
{
  D4D_CLR_SCHEME *pScheme = D4D_ScreenGetScheme(pScreen);
  D4D_COLOR clr;
  
#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE  
  
  if(active)
  {    
    if(pScreen->flags.bits.bBackground)
    {    
      D4D_FillRRect(&pScreen->position, &pScreen->size, pScheme->screen.desktop, pScreen->radius);
    }
    
    if(pScreen->flags.bits.bOutline)
    {
      D4D_RRect(&(pScreen->position), &(pScreen->size), D4D_LINE_THIN, pScheme->screen.outline, pScreen->radius);
    }
  }

  if(pScreen->flags.bits.bHeader)
  {
    clr = pScheme->screen.title_bar;
    
    if(!active)
      clr = D4D_GetGreyScale(clr);    
    
    D4D_FillRRectXY((D4D_COOR)(pScreen->position.x + 1), (D4D_COOR)(pScreen->position.y + 1), (D4D_COOR)(pScreen->size.cx + pScreen->position.x - 1), (D4D_COOR)(D4D_GetScrHeaderSize(pScreen) + pScreen->position.y - 1), clr, pScreen->radius);    
  }
  
  if(pScreen->pIcon != NULL)
  {
    D4D_DrawRBmpXY((D4D_COOR)(pScreen->position.x + D4D_SCR_TITLE_OFF_X), (D4D_COOR)(pScreen->position.y + (D4D_GetScrHeaderSize(pScreen) - D4D_GetBmpHeight(pScreen->pIcon)) / 2), pScreen->pIcon, (D4D_BOOL)!active, pScreen->radius);  
  }
  
  if(pScreen->textBuff.pText)
  {        
    clr = pScheme->screen.title_text;
    
    if(!active)
      clr = D4D_GetGreyScale(clr);    
    
    D4D_DrawTextXY((D4D_COOR)(pScreen->position.x + D4D_GetScrHeaderTitleOffset(pScreen)), (D4D_COOR)(pScreen->position.y + (D4D_GetScrHeaderSize(pScreen) + pScreen->radius - D4D_GetFontHeight(pScreen->textBuff.fontId)) / 2), 
        &pScreen->textBuff, clr, 0);
    }
  
  if(pScreen->flags.bits.bExit)
  {
    D4D_POINT tmp_pos;
    D4D_SIZE tmp_size;
    clr = pScheme->screen.exitBtnFore;
    
    if(!active)
      clr = D4D_GetGreyScale(clr);
    
    if(D4D_GetScrHeaderExitBtnCoor(pScreen, &tmp_pos, &tmp_size))
    {         
      D4D_RBox(&tmp_pos, &tmp_size, D4D_LINE_THIN, clr, pScheme->screen.exitBtnBckg, pScreen->radius);
      
      tmp_pos.x += (D4D_COOR)(D4D_SCR_EXITBTN_CROSS_SIZE + pScreen->radius / 2);
      tmp_pos.y += (D4D_COOR)(D4D_SCR_EXITBTN_CROSS_SIZE + pScreen->radius / 2);    
      
      D4D_MoveTo(&tmp_pos);
      tmp_pos.x += tmp_size.cx - (2 * D4D_SCR_EXITBTN_CROSS_SIZE  + pScreen->radius);
      tmp_pos.y += tmp_size.cy - (2 * D4D_SCR_EXITBTN_CROSS_SIZE  + pScreen->radius);    
      D4D_LineTo(&tmp_pos, D4D_LINE_THIN, clr);
      
      tmp_pos.y -= tmp_size.cy - (2 * D4D_SCR_EXITBTN_CROSS_SIZE  + pScreen->radius);    
      
      D4D_MoveTo(&tmp_pos);
      tmp_pos.x -= tmp_size.cx - (2 * D4D_SCR_EXITBTN_CROSS_SIZE  + pScreen->radius);
      tmp_pos.y += tmp_size.cy - (2 * D4D_SCR_EXITBTN_CROSS_SIZE  + pScreen->radius);    
      D4D_LineTo(&tmp_pos, D4D_LINE_THIN, clr);
    }
  }
  
#else

  if(active)
  {    
    if(pScreen->flags.bits.bBackground)
    {    
      D4D_FillRect(&pScreen->position, &pScreen->size, pScheme->screen.desktop);
    }
    
    if(pScreen->flags.bits.bOutline)
    {
      D4D_Rect(&(pScreen->position), &(pScreen->size), D4D_LINE_THIN, pScheme->screen.outline);
    }
  }

  if(pScreen->flags.bits.bHeader)
  {
    clr = pScheme->screen.title_bar;
    
    if(!active)
      clr = D4D_GetGreyScale(clr);    
    
    D4D_FillRectXY((D4D_COOR)(pScreen->position.x + 1), (D4D_COOR)(pScreen->position.y + 1), (D4D_COOR)(pScreen->size.cx + pScreen->position.x - 1), (D4D_COOR)(D4D_GetScrHeaderSize(pScreen) + pScreen->position.y - 1), clr);    
  }
  
  if(pScreen->pIcon != NULL)
  {
    D4D_DrawBmpXY((D4D_COOR)(pScreen->position.x + D4D_SCR_TITLE_OFF_X), (D4D_COOR)(pScreen->position.y + (D4D_GetScrHeaderSize(pScreen) - D4D_GetBmpHeight(pScreen->pIcon)) / 2), pScreen->pIcon, (D4D_BOOL)!active);  
  }
  
  if(pScreen->textBuff.pText)
  {        
    clr = pScheme->screen.title_text;
    
    if(!active)
      clr = D4D_GetGreyScale(clr);    
    
    D4D_DrawTextXY((D4D_COOR)(pScreen->position.x + D4D_GetScrHeaderTitleOffset(pScreen)), (D4D_COOR)(pScreen->position.y + (D4D_GetScrHeaderSize(pScreen) - D4D_GetFontHeight(pScreen->textBuff.fontId)) / 2), 
        &pScreen->textBuff, clr, 0);
    }
  
  if(pScreen->flags.bits.bExit)
  {
    D4D_POINT tmp_pos;
    D4D_SIZE tmp_size;
    clr = pScheme->screen.exitBtnFore;
    
    if(!active)
      clr = D4D_GetGreyScale(clr);
    
    if(D4D_GetScrHeaderExitBtnCoor(pScreen, &tmp_pos, &tmp_size))
    {         
      D4D_Box(&tmp_pos, &tmp_size, D4D_LINE_THIN, clr, pScheme->screen.exitBtnBckg);
      
      tmp_pos.x += D4D_SCR_EXITBTN_CROSS_SIZE;
      tmp_pos.y += D4D_SCR_EXITBTN_CROSS_SIZE;    
      
      D4D_MoveTo(&tmp_pos);
      tmp_pos.x += tmp_size.cx - 2 * D4D_SCR_EXITBTN_CROSS_SIZE;
      tmp_pos.y += tmp_size.cy - 2 * D4D_SCR_EXITBTN_CROSS_SIZE;    
      D4D_LineTo(&tmp_pos, D4D_LINE_THIN, clr);
      
      tmp_pos.y -= tmp_size.cy - 2 * D4D_SCR_EXITBTN_CROSS_SIZE;    
      
      D4D_MoveTo(&tmp_pos);
      tmp_pos.x -= tmp_size.cx - 2 * D4D_SCR_EXITBTN_CROSS_SIZE;
      tmp_pos.y += tmp_size.cy - 2 * D4D_SCR_EXITBTN_CROSS_SIZE;    
      D4D_LineTo(&tmp_pos, D4D_LINE_THIN, clr);
    }
  }
#endif  
    
}
 
/**************************************************************//*!
*
* Redraw all objects that need it on given screen
*
******************************************************************/

void D4D_RedrawScreen(D4D_SCREEN* pScreen)
{
    D4D_SCREEN_DATA* pData = pScreen->pData;
    D4D_OBJECT* pFocus = (D4D_OBJECT*) D4D_GetFocusedObject(pScreen);
    D4D_OBJECT* pObj;
    Byte i;
    
    D4D_BOOL tmpCompleteRedraw = pData->flags.bits.bCompleteRedraw;
    D4D_BOOL tmpTimeTick = d4d_systemFlags.bits.bTimeTick;
    
    pData->flags.bits.bCompleteRedraw = 0;
    d4d_systemFlags.bits.bTimeTick = 0;
    
    // prepare message
    d4d_msg.pScreen = pScreen;
    
    // redraw screen non client area if necessary
    if(tmpCompleteRedraw)
    {
      D4D_DrawScreenNC(pScreen, D4D_TRUE);
    }
    
    
    // redraw objects   
    for(i=0; pScreen->pObjects[i] != NULL; i++)
    {
        pObj = (D4D_OBJECT*) pScreen->pObjects[i];
        
        if(!(pObj->flags->bits.bVisible))
          continue;
        
        d4d_msg.pObject = pObj;
        d4d_msg.prm.draw.all = 0;
        
        if(!(pObj->flags->bits.bInitDone))
        {            
            d4d_msg.nMsgId = D4D_MSG_ONINIT;
            // send the ON INIT message
            D4D_SendMessage(&d4d_msg);
            pObj->flags->bits.bInitDone = 1;            
        }
        
        if(tmpTimeTick)
        {
          d4d_msg.nMsgId = D4D_MSG_TIMETICK;
          // send the TIMETICK message
          D4D_SendMessage(&d4d_msg);  
        }
        
        /* forced complete redraw of all objects or object wants to be completely redrawn*/
        if((tmpCompleteRedraw) || (pObj->flags->bits.bRedrawC))
        {
            d4d_msg.prm.draw.bits.bComplete = 1;
        }
        else
        {
            /* object does not need to redraw */
            if(!pObj->flags->bits.bRedraw)
                continue;
        }

        
       
        // draw special?
        if((pObj == pFocus) && (pObj->flags->bits.bTabStop))
        {
            d4d_msg.prm.draw.bits.bFocused = 1;

            if(pObj == d4d_pKeysCapturer)
                d4d_msg.prm.draw.bits.bCapturing = 1;
        }
        
        // will draw now            
        pObj->flags->bits.bRedraw = 0;
        pObj->flags->bits.bRedrawC = 0;       
        
        // send the DRAW message
        d4d_msg.nMsgId = D4D_MSG_DRAW;
        D4D_SendMessage(&d4d_msg);
        // send the DRAWDONE message
        d4d_msg.nMsgId = D4D_MSG_DRAWDONE;
        D4D_SendMessage(&d4d_msg);
        
        
        
    }
}


/**************************************************************//*!
*
* Convert Client point into the global screen point
*
******************************************************************/

D4D_POINT D4D_GetClientToScreenPoint(D4D_SCREEN* pScreen, D4D_POINT* nClientPoint)
{
  D4D_POINT tmp_point;
  
  if(pScreen == NULL)
  {
    return *nClientPoint;  
  }
  
  tmp_point.x = (D4D_COOR)(pScreen->position.x + nClientPoint->x);
  tmp_point.y = (D4D_COOR)(pScreen->position.y + nClientPoint->y);
  
  if(pScreen->flags.bits.bOutline)
  	tmp_point.x++;	
  
  tmp_point.y += D4D_GetScrHeaderSize(pScreen); 
  
  return tmp_point;
}

/**************************************************************//*!
*
* Convert global screen point into the client point
*
******************************************************************/

D4D_POINT D4D_GetScreenToClientPoint(D4D_SCREEN* pScreen, D4D_POINT* nScreenPoint)
{
  D4D_POINT tmp_point;
  
  if(pScreen == NULL)
  {
    return *nScreenPoint;  
  }
  
  tmp_point.x = (D4D_COOR)(nScreenPoint->x - pScreen->position.x);
  tmp_point.y = (D4D_COOR)(nScreenPoint->y - pScreen->position.y);
  
  if(pScreen->flags.bits.bOutline)
  	tmp_point.x--;
  
  tmp_point.y -= D4D_GetScrHeaderSize(pScreen); 
  
  return tmp_point;
}

/**************************************************************//*!
*
* Get real client screen size
*
******************************************************************/

D4D_SIZE D4D_GetClientScreenSize(D4D_SCREEN* pScreen)
{
  D4D_SIZE tmp_size = {0, 0};
  
  if(pScreen == NULL)
  {
    return tmp_size;  
  }
  
  tmp_size.cx = pScreen->size.cx;
  tmp_size.cy = pScreen->size.cy;
  
  if(pScreen->flags.bits.bOutline)
  {
    tmp_size.cx -= 2;
    tmp_size.cy -= 2;
  }
  
  tmp_size.cy -= D4D_GetScrHeaderSize(pScreen); 
  
  return tmp_size;
}


/**************************************************************//*!
*
* Check if the coordination are in range of exit button
*
******************************************************************/

D4D_BOOL D4D_ScrCheckExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* point)
{
  D4D_POINT tmp_pos;
  D4D_SIZE tmp_size;
  
  
  if(D4D_GetScrHeaderExitBtnCoor(pScreen, &tmp_pos, &tmp_size))
  {    
    if((point->x >= tmp_pos.x) && (point->y >= tmp_pos.y))
    { 
      if((point->x <= (tmp_pos.x + tmp_size.cx)) && (point->y <= (tmp_pos.y + tmp_size.cy)))
        return D4D_TRUE;
    }
  }
  return D4D_FALSE;
}

/**************************************************************//*!
*
* Check if the coordination are in range of screen
*
******************************************************************/

D4D_BOOL D4D_ScrCheckCoor(D4D_SCREEN* pScreen, D4D_POINT* point)
{
  if((point->x >= pScreen->position.x) && (point->y >= (pScreen->position.y + D4D_GetScrHeaderSize(pScreen))))
  { 
    if((point->x <= (pScreen->position.x + pScreen->size.cx)) && (point->y <= (pScreen->position.y + pScreen->size.cy)))
      return D4D_TRUE;
  }
  return D4D_FALSE;
}