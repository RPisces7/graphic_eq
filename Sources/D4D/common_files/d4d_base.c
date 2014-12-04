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
* @file      d4d_base.c
*
* @author    r50233
* 
* @version   0.0.93.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver core and base function c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

Byte d4d_scratchPad[D4D_SCRATCHPAD_SIZE];

/**************************************************************//*!
*
* Local variables
*
******************************************************************/

// screen activation hisotry array & index
D4D_SCREEN* d4d_screenHistory[D4D_SCREEN_HISTORY];
Byte d4d_screenHistoryIndex;

// if not NULL, keys are captured by this object
D4D_OBJECT* d4d_pKeysCapturer = NULL;

// one and only message object being used to route information to objects
D4D_MESSAGE d4d_msg;

const D4D_SIZE d4d_size_zero = {0, 0}; // zero size structure for automatic function capability
const D4D_POINT d4d_point_zero = {0, 0}; // zero point structure

// last point remembered for MoveTo, LineTo etc.
static D4D_POINT d4d_curXY;

// The buffer of input keys
static D4D_KEYS_BUFFER d4d_KeysBuff;

// The D4D system flags
D4D_SYSTEM_FLAGS d4d_systemFlags;

#ifdef D4D_LLD_TCH
  static D4D_TOUCHSCREEN_STATUS d4d_TouchScreen_Status;
  D4D_OBJECT* d4d_LastTouchedObj;
  static D4D_POINT d4d_TouchScreenCoor = {0, 0};  
  static D4D_TOUCHSCREEN_EVENT d4d_touchEvent = { 0, {0, 0}};
#endif


#ifdef __CWCC__
  #pragma mark -
  #pragma mark D4D System Functions
  #pragma mark -
#endif

/**************************************************************//*!
*
* Initialize
*
******************************************************************/

D4D_BOOL D4D_Init(D4D_SCREEN* pInitScreen)
{
  if(!D4D_LCD_Init()) // Init hardware of LCD
    return D4D_FALSE;
  
  d4d_screenHistoryIndex = 0;
  
  #ifdef D4D_LLD_TCH
    d4d_TouchScreen_Status.all = 0;
    d4d_LastTouchedObj = NULL;    
  #endif
  
  d4d_systemFlags.all = 0;
  
  D4D_ClearKeysBuffer();
  D4D_ActivateScreen(pInitScreen, D4D_FALSE);
    
  return D4D_TRUE;  
}

/**************************************************************//*!
*
* The main poll call
*
******************************************************************/

void D4D_Poll(void)
{
    // get active screen
    D4D_SCREEN* pScreen;
    
    // handle keys (may change active screen)
    D4D_HandleKeys();
   
  #ifdef D4D_LLD_TCH
    if(d4d_TouchScreen_Status.merged_bits.bEvents)
    {
      D4D_ManageTouchScreenEvents();
      d4d_TouchScreen_Status.merged_bits.bEvents = 0;
    }
    
    pScreen = D4D_GetActiveScreen();
    if(pScreen == NULL)
      return;
    D4D_HandleTouchScreen(pScreen);
  #endif
  
    pScreen = D4D_GetActiveScreen();
    if(pScreen == NULL)
      return;
    // call screen's main function
    if(pScreen->OnMain)
        pScreen->OnMain();    
        
    // redraw all objects on active screen
    pScreen = D4D_GetActiveScreen();
    D4D_RedrawScreen(pScreen);
}

/**************************************************************//*!
*
* Clear all rest information about pushed Keys in buffer
*
******************************************************************/
void D4D_ClearKeysBuffer(void)
{
  d4d_KeysBuff.readPos = 0;
  d4d_KeysBuff.writePos = 0;    
}

/**************************************************************//*!
*
* The function sets the orientation of display
*
******************************************************************/
void D4D_SetOrientation(D4D_ORIENTATION orient)
{
  // get active screen
  D4D_SCREEN* pScreen = D4D_GetActiveScreen();
    
  D4D_LCD_SetOrientation(orient);
  
  if(pScreen)
    D4D_InvalidateScreen(pScreen, D4D_TRUE); 
}


/**************************************************************//*!
*
* Put actual information about keys
*
******************************************************************/

void D4D_TimeTickPut(void)
{
  d4d_systemFlags.bits.bTimeTick = 1;  
}


/**************************************************************//*!
*
* Put actual information about keys
*
******************************************************************/

void D4D_KeysChanged(D4D_KEYS keys)
{
  static D4D_KEYS keysLastState = 0;
  
  
  
 if(keys != keysLastState)
 {
    D4D_KEYS kChange;
    D4D_KEY_SCANCODE tmp_scanCode;
    
    kChange = (Byte)(keys ^ keysLastState); 
    
    if(kChange & D4D_KEY_UP)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_UP;
      if((keys & D4D_KEY_UP) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;
      
      D4D_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4D_KEY_DOWN)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_DOWN;
      if((keys & D4D_KEY_DOWN) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;
      
      D4D_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4D_KEY_LEFT)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_LEFT;
      if((keys & D4D_KEY_LEFT) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;
      
      D4D_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4D_KEY_RIGHT)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_RIGHT;
      if((keys & D4D_KEY_RIGHT) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;
      
      D4D_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4D_KEY_ENTER)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_ENTER;
      if((keys & D4D_KEY_ENTER) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;
      
      D4D_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4D_KEY_ESC)
    {
      tmp_scanCode = D4D_KEY_SCANCODE_ESC;
      if((keys & D4D_KEY_ESC) == 0) // the key is released
        tmp_scanCode |= D4D_KEY_SCANCODE_RELEASEMASK;
      
      D4D_NewKeyEvent(tmp_scanCode);      
    }
    
    keysLastState = keys;           
 }   
}

/**************************************************************//*!
*
* Put new event from keys
*
******************************************************************/

void D4D_NewKeyEvent(D4D_KEY_SCANCODE scanCode)
{

  // Is there place for new record?
  if((d4d_KeysBuff.writePos + 1 == d4d_KeysBuff.readPos) || ((d4d_KeysBuff.readPos == 0) && (d4d_KeysBuff.writePos + 1 == D4D_KEYS_BUFF_LENGTH))) 
  {
    return;
  }
  
  // place new record to buffer
  d4d_KeysBuff.buff[d4d_KeysBuff.writePos++] = scanCode;
  if(d4d_KeysBuff.writePos >= D4D_KEYS_BUFF_LENGTH)
    d4d_KeysBuff.writePos = 0;  
  
  
  
  #ifdef D4D_INPUT_EVENT_CALLBACK
    D4D_INPUT_EVENT_CALLBACK();
  #endif
  
}


/**************************************************************//*!
*
* helper to invoke OBJECT's message handler 
*
* all information need to be already prepared in d4d_msg
*
******************************************************************/

void D4D_SendMessage(D4D_MESSAGE* pMsg)
{
    D4D_OBJECT* pObject = pMsg->pObject;
    
    // screen gets the first chance
    if(pMsg->pScreen->OnObjectMsg)
    {
        // screen may discard the message by returning TRUE
        if(pMsg->pScreen->OnObjectMsg(pMsg))
            return ;
    }
    
    if(pObject)
    {      
      // now invoke the object's handler        
      if(pObject->OnUsrMessage)
      {
        if(pObject->OnUsrMessage(pMsg))
          return;   
      }
      
      if(pObject->pObjFunc->OnSysMessage)
          pObject->pObjFunc->OnSysMessage(pMsg); 
    }
}

/**************************************************************//*!
*
* Get the last state of keys
*
******************************************************************/

D4D_KEYS D4D_GetKeys(void)
{
  return (D4D_KEYS)0;
}

/**************************************************************//*!
*
* Handle the keys (part of main poll call)
*
******************************************************************/
void D4D_HandleKeys(void)
{
    // get active screen
    D4D_SCREEN* pScreen = D4D_GetActiveScreen();
    D4D_OBJECT* pFocus;
    D4D_KEY_SCANCODE scanCode;
    D4D_BOOL tmp_release;
    
    // Check the buffer of input keys changes
    if(d4d_KeysBuff.writePos == d4d_KeysBuff.readPos)
      return;
    
    // Read the latest record in the buffer of input key event
    scanCode = d4d_KeysBuff.buff[d4d_KeysBuff.readPos++];
    
    // Check the overflow of the read pointer of readed input keys
    if(d4d_KeysBuff.readPos >= D4D_KEYS_BUFF_LENGTH)
        d4d_KeysBuff.readPos = 0;
    
    tmp_release = (D4D_BOOL)(scanCode & D4D_KEY_SCANCODE_RELEASEMASK);
    scanCode &= ~D4D_KEY_SCANCODE_RELEASEMASK;

    // do we handle the keys ourselves (to navigate across the screen) ?
    if(!d4d_pKeysCapturer)
    {
        if((scanCode & D4D_KEY_SCANCODE_RELEASEMASK) == 0)
        {                
          // escape the screen?
          if(scanCode == D4D_KEY_SCANCODE_ESC)
          {            
            if(tmp_release == 0)
              D4D_EscapeScreen();
            return;
          }
          // focus previous object on the screen?
          else if(scanCode ==  D4D_KEY_FUNC_FOCUS_PREV)          
          {
            if(tmp_release == 0)
              D4D_FocusPrevObject(pScreen);
            return;
          }          
          // focus next object on the screen?
          else if(scanCode ==  D4D_KEY_FUNC_FOCUS_NEXT)
          {
            if(tmp_release == 0)
              D4D_FocusNextObject(pScreen, D4D_FALSE);
            return;
          }
        }
    }
             
    // does the object get the key events?    
    // yes, invoke the key events to the active object
    pFocus = (D4D_OBJECT*) (d4d_pKeysCapturer ? d4d_pKeysCapturer : D4D_GetFocusedObject(pScreen));

    // prepare the message            
    d4d_msg.pScreen = pScreen;
    d4d_msg.pObject = pFocus;

    // if key was pressed down?    
    if(tmp_release == 0)
    {
      d4d_msg.nMsgId = D4D_MSG_KEYDOWN;
    }else
    // if key was released up?
    {
      d4d_msg.nMsgId = D4D_MSG_KEYUP;
    }
    d4d_msg.prm.key = (D4D_KEY_SCANCODE)(scanCode & D4D_KEY_SCANCODE_KEYMASK);
    D4D_SendMessage(&d4d_msg);
    
}

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Touch Screen Functions
  #pragma mark -
#endif

#ifdef D4D_LLD_TCH

/**************************************************************//*!
*
* get the last TouchScreen coordinates
*
******************************************************************/

D4D_POINT D4D_GetTouchScreenCoordinates(void)
{
  return D4D_GetScreenToClientPoint(D4D_GetActiveScreen(), &d4d_TouchScreenCoor);
}



/**************************************************************//*!
*
* Handle the TouchScreen (part of main poll call)
*
******************************************************************/

void D4D_HandleTouchScreen(D4D_SCREEN* pScreen)
{
  D4D_SCREEN_DATA* pData = pScreen->pData;
  
  if(d4d_TouchScreen_Status.bits.bNewTouch || d4d_TouchScreen_Status.bits.bAutorepeat)
  {
    
    
    D4D_OBJECT* pObj;
    Byte i;
    D4D_POINT tmpClientTouch = D4D_GetScreenToClientPoint(pScreen, &d4d_TouchScreenCoor);
    

    d4d_TouchScreen_Status.bits.bUnTouch = 1;

    if(d4d_TouchScreen_Status.bits.bNewTouch)
      d4d_msg.nMsgId = D4D_MSG_TOUCHED;
    else
      d4d_msg.nMsgId = D4D_MSG_TOUCH_AUTO;

    // prepare message
    d4d_msg.pScreen = pScreen;
    
    
    if(pScreen->flags.bits.bExit)
    {
      if(D4D_ScrCheckExitBtnCoor(pScreen, &d4d_TouchScreenCoor))
      {        
        D4D_EscapeScreen();      
        d4d_TouchScreen_Status.bits.bNewTouch = 0;
        d4d_TouchScreen_Status.bits.bAutorepeat = 0;
        return;
      }
    }
    
    if(pData->flags.bits.bTouchEnable)
    {  
      if(D4D_ScrCheckCoor(pScreen, &d4d_TouchScreenCoor))
      {        
        d4d_msg.pObject = NULL;
        d4d_msg.prm.draw.all = 0;

        // send the D4D_MSG_TOUCHED message
        D4D_SendMessage(&d4d_msg);  
      }
    }
    
    
    // redraw objects   
    for(i=0; pScreen->pObjects[i] != NULL; i++)
    {
      pObj = (D4D_OBJECT*) pScreen->pObjects[i];
      
      if(!(pObj->flags->bits.bVisible))
        continue;
      
      if(!(pObj->flags->bits.bTouchEnable))
        continue;        
      
      if(d4d_TouchScreen_Status.bits.bAutorepeat)
      {
        if(d4d_LastTouchedObj != pObj)
            continue;
      }
       
            
      if(pObj->pObjFunc->CheckCoordinates == NULL)
        continue;
      
      if(pObj->pObjFunc->CheckCoordinates(pObj, tmpClientTouch))
      {
        if(d4d_TouchScreen_Status.bits.bNewTouch)
          d4d_LastTouchedObj = pObj;
        
        d4d_msg.pObject = pObj;
        d4d_msg.prm.draw.all = 0;

        // send the D4D_MSG_TOUCHED message
        D4D_SendMessage(&d4d_msg);
      }
        
    }
    
    
    d4d_TouchScreen_Status.bits.bNewTouch = 0;
    d4d_TouchScreen_Status.bits.bAutorepeat = 0;
    
    
  }else
  { 
    if(!d4d_TouchScreen_Status.bits.bTouched)
      if(d4d_TouchScreen_Status.bits.bUnTouch)
      {
        d4d_TouchScreen_Status.bits.bUnTouch = 0;
        
        // prepare message
        d4d_msg.nMsgId = D4D_MSG_UNTOUCHED;
        d4d_msg.pScreen = pScreen;
        d4d_msg.prm.draw.all = 0;
        
        if(pData->flags.bits.bTouchEnable)
        {
          if(D4D_ScrCheckCoor(pScreen, &d4d_TouchScreenCoor))
          {
            d4d_msg.pObject = NULL;
            // send the D4D_MSG_UNTOUCHED message to screen
            D4D_SendMessage(&d4d_msg);  
          }
        }
        
        if(d4d_LastTouchedObj)
        {
          d4d_msg.pObject = d4d_LastTouchedObj;
          d4d_LastTouchedObj = NULL;
          // send the D4D_MSG_UNTOUCHED message to Object
          D4D_SendMessage(&d4d_msg);  
        }
        
      }
  }
  
}




/**************************************************************//*!
*
* Check  the TouchScreen - User function
*
******************************************************************/


void D4D_CheckTouchScreen(void)
{
  d4d_TouchScreen_Status.bits.bCheckTouchScreen = 1;
}

void D4D_PutTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y)
{
  if(d4d_TouchScreen_Status.merged_bits.bEvents == 1)
  	return;
  
	d4d_touchEvent.touched = touched;
	d4d_touchEvent.point.x = x;
	d4d_touchEvent.point.y = y;
	
	d4d_TouchScreen_Status.bits.bNewEvent = 1; 
  
}

void D4D_PutRawTouchScreen(D4D_BOOL touched, D4D_COOR x, D4D_COOR y)
{
  if(d4d_TouchScreen_Status.merged_bits.bEvents == 1)
  	return;
  
	d4d_touchEvent.touched = touched;
	d4d_touchEvent.point.x = x;
	d4d_touchEvent.point.y = y;
	
	d4d_TouchScreen_Status.bits.bNewRawEvent = 1; 
  
}




//
//-----------------------------------------------------------------------------
// FUNCTION:    D4D_GetTouchScreenPosition
// SCOPE:       Global
// DESCRIPTION: Reads touch screen and returns X, Y coorinates if screen 
//              touched 
// PARAMETERS:  LWord *TouchPositionX    Pointer to X coordinate
//              LWord *TouchPositionY    Pointer to Y ccordinate
// RETURNS:     0   no screen touch
//              1   screen touch, 
//-----------------------------------------------------------------------------
//
D4D_BOOL D4D_GetTouchScreen(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY)
{
    // Declare and initialise local variables
    D4D_BOOL ScreenTouch;

    
    if(d4d_TouchScreen_Status.bits.bCheckTouchScreen == 1)
    {    	
	    // Read raw touch position into *TouchPositionX and *TouchPositionY
	    ScreenTouch = D4D_TCH_GetRawPosition(TouchPositionX, TouchPositionY);
    }
    else
    {
    	ScreenTouch = d4d_touchEvent.touched;
    	*TouchPositionX = d4d_touchEvent.point.x;
    	*TouchPositionY = d4d_touchEvent.point.y;    	
    }
    
    
		if(ScreenTouch)
		{
			if(d4d_TouchScreen_Status.bits.bNewEvent == 0)	
				D4D_TCH_GetCalibratedPosition(TouchPositionX, TouchPositionY);
		}
    // Return flag to indicate if screen has been touched
    return ScreenTouch;

}

void D4D_ManageTouchScreenEvents(void)
{
  D4D_COOR x,y;
  D4D_BOOL tmp_res;
  
  if(D4D_TCH_GetCalibrationStatus() == 0)
  {
    D4D_CalibrateTouchScreen();
    return; 
  }
  
  if(d4d_TouchScreen_Status.bits.bNewTouch)    
    return;
  
  tmp_res = D4D_GetTouchScreen(&x, &y);
  
  if(((d4d_TouchScreen_Status.bits.bTouched) ^ (tmp_res)))
  {    
    d4d_TouchScreen_Status.bits.bTouched = tmp_res;
    
    if(tmp_res)
    {
      d4d_TouchScreen_Status.bits.bNewTouch = 1;
      d4d_TouchScreen_Status.bits.bAutorepeat = 0;     
    }
    
    d4d_TouchScreen_Status.bits.bAutorepeat = 0;
    
  }else    
    d4d_TouchScreen_Status.bits.bAutorepeat = tmp_res;
  
  if(tmp_res)
  {
    d4d_TouchScreenCoor.x = (D4D_COOR)x;
    d4d_TouchScreenCoor.y = (D4D_COOR)y; 
    
    #ifdef D4D_INPUT_EVENT_CALLBACK
      D4D_INPUT_EVENT_CALLBACK();
    #endif   
  }
  
}



/**************************************************************//*!
*
* Calibrate the TouchScreen - User function
*
******************************************************************/


void D4D_CalibrateTouchScreen(void) 
{
  D4D_TCH_Calibrate(D4D_COLOR_SYSTEM_FORE, D4D_COLOR_SYSTEM_BCKG);
  D4D_InvalidateScreen(D4D_GetActiveScreen(), D4D_TRUE);  
}


D4D_TOUCHSCREEN_CALIB D4D_GetTouchScreenCalibration(void)
{
  return D4D_TCH_GetCalibration();  
}

void D4D_SetTouchScreenCalibration(D4D_TOUCHSCREEN_CALIB newCalib)
{
  D4D_TCH_SetCalibration(newCalib);  
}




#else


#ifdef __CWCC__
  #pragma mark -
  #pragma mark Dummy Touch Screen Functions
  #pragma mark -
#endif

  D4D_POINT D4D_GetTouchScreenCoordinates(void)
  {
    D4D_POINT tmp = {0, 0};
    return tmp;
  } 
    
  void D4D_HandleTouchScreen(D4D_SCREEN* pScreen)
  {
    D4D_UNUSED(pScreen);
  }
  
  void D4D_CheckTouchScreen(void)
  {
  }
  
  D4D_TOUCHSCREEN_CALIB D4D_GetTouchScreenCalibration(void)
  {
    D4D_TOUCHSCREEN_CALIB tmp = { 0, 0, 0, 0, 0 };
    return tmp;  
  }
  
  void D4D_SetTouchScreenCalibration(D4D_TOUCHSCREEN_CALIB newCalib)
  {
    D4D_UNUSED(newCalib);
  }
  
  void D4D_CalibrateTouchScreen(void) 
  {
  
  }
  
#endif

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Help Strings & Fonts Functions
  #pragma mark -
#endif

/**************************************************************//*!
*
* convert decimal number to string
*
******************************************************************/

Byte D4D_SprintDecU32(LWord val, char *pText, char fill) 
{
  LWord value;
  LWord divider;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  Byte tmp_cnt;
  value = val;
  divider =  1000000000;
  
  for(tmp_cnt = 0; tmp_cnt < 9; tmp_cnt++)
  {
    if((value > divider - 1) || (cnt > 1))
    {    
      if(pText != NULL)
        *pText++ = (char)((value/divider)+'0');
      cnt++;
    } else if((pText != NULL) && (fill)) 
    {
      fill_cnt++;
      *pText++ = fill;
    }
    
    value %= divider;			
    divider /= 10;
  }
  
	if(pText != NULL)
    *pText++ = (char)((value)+'0');
  
  return (Byte)(cnt + fill_cnt);
}

/**************************************************************//*!
*
* convert signed decimal number to string
*
******************************************************************/

Byte D4D_SprintDecS32(sLWord val, char *pText, char fill) 
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4D_SprintDecU32((LWord)val, pText, fill));
}

/**************************************************************//*!
*
* convert decimal number to string
*
******************************************************************/

Byte D4D_SprintDecU16(Word val, char *pText, char fill) 
{
  Word value;
  Word divider;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  Byte tmp_cnt;
  value = val;
  divider =  10000;
  
  for(tmp_cnt = 0; tmp_cnt < 4; tmp_cnt++)
  {
    if((value > divider - 1) || (cnt > 1))
    {    
      if(pText != NULL)
        *pText++ = (char)((value/divider)+'0');
      cnt++;
    }else if((pText != NULL) && (fill)) 
    {
      fill_cnt++;
      *pText++ = fill;
    }
        
    value %= divider;			
    divider /= 10;
  }

	if(pText != NULL)
    *pText++ = (char)((value)+'0');
  
  return (Byte)(cnt + fill_cnt);
}

/**************************************************************//*!
*
* convert signed decimal number to string
*
******************************************************************/

Byte D4D_SprintDecS16(sWord val, char *pText, char fill) 
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4D_SprintDecU16((Word)val, pText, fill));
}

/**************************************************************//*!
*
* convert decimal number to string
*
******************************************************************/

Byte D4D_SprintDecU8(Byte val, char *pText, char fill) 
{
  Byte value;
  Byte cnt = 1;
  Byte fill_cnt = 0;  
  value = val;
  
  if(value > 99)
  {    
    cnt++;
    if(pText != NULL)
      *pText++ = (char)((value/100)+'0');
    value %= 100;
    
  }else if((pText != NULL) && (fill))
  {
    fill_cnt++;
    *pText++ = fill;
  }
  
  if((value > 9) || (cnt > 1))
  {    
    cnt++;
    if(pText != NULL)
      *pText++ = (char)((value/10)+'0');
    value %= 10;
    
  }else if((pText != NULL) && (fill))
  {
    fill_cnt++;
    *pText++ = fill;
  }
      
  			
	
	if(pText != NULL)
    *pText++ = (char)((value)+'0');
  
  return (Byte)(cnt + fill_cnt);
}

/**************************************************************//*!
*
* convert signed decimal number to string
*
******************************************************************/

Byte D4D_SprintDecS8(sByte val, char *pText, char fill) 
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4D_SprintDecU8((Byte)val, pText, fill));
}

/**************************************************************//*!
*
* Get curent text length
*
******************************************************************/

D4D_INDEX D4D_GetTextLength(D4D_CHAR* pText) 
{
  D4D_INDEX cnt = 0;
  
  if(pText == NULL)
    return 0;
  
#if D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE  
  
  if(*pText == d4d_extsrcLocale.strTranslatePrefix)
    cnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_GetTranslateStrLength(pText);
  else
  {
    while(*(pText++) != 0)
      cnt++;  
  }

#else
  
  while(*(pText++) != 0)
    cnt++;
#endif

  return cnt;
}


/**************************************************************//*!
*
* Get curent text length
*
******************************************************************/
D4D_COOR D4D_GetTextWidth(D4D_FONT ix, D4D_CHAR* pText){
  
  D4D_COOR width = 0;

  if(pText == NULL)
    return 0;
  
#if D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE    

  if(*pText == d4d_extsrcLocale.strTranslatePrefix)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = 0;
    D4D_INDEX charCntTmp; 
    
    do
    {
      charOff += charCnt;
      
      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pText, (char*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);
    
      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
        width += D4D_GetCharWidth(ix, (D4D_CHAR)d4d_extsrcBuffer[charCntTmp]);  
        
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
    
  
  }else
  {
    while(*(pText) != 0)
      width += D4D_GetCharWidth(ix, *pText++); 
  }


#else  

  while(*(pText) != 0)
    width += D4D_GetCharWidth(ix, *pText++);

#endif

  return width;  
}

/**************************************************************//*!
*
* Get curent text length
*
******************************************************************/
D4D_COOR D4D_GetTextBuffWidthTabAdj(D4D_STRING* text_buffer, D4D_INDEX offset, D4D_TAB* pTab){
  
  D4D_COOR width = 0;
  D4D_COOR tab;
  D4D_INDEX char_ix = 0;
  D4D_CHAR* pText = text_buffer->pText; 

  if(pText == NULL)
    return 0;
  
#if D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE    

  if(*pText == d4d_extsrcLocale.strTranslatePrefix)
  {
    D4D_INDEX charCnt = 0;
    D4D_INDEX charOff = offset;
    D4D_INDEX charCntTmp; 
    
    do
    {
      charOff += charCnt;
      
      charCnt = (D4D_INDEX)d4d_extsrcLocale.D4DLOCALE_TranslateStr(pText, (char*)d4d_extsrcBuffer, charOff, D4D_EXTSRC_BUFF_SIZE);
    
      for(charCntTmp = 0; charCntTmp < charCnt; charCntTmp++)
      {
        if(text_buffer->buffSize)
        {
          if(++char_ix >= text_buffer->buffSize)  
            break;
        }
        if((D4D_CHAR)d4d_extsrcBuffer[charCntTmp] == '\t')
        {
          tab = D4D_GetNextTab(pTab, width);  
          
          if(tab)
          {
            width = (D4D_COOR)(tab - (pTab->tabOffset));
            continue;  
          }
        } 
        
        width += D4D_GetCharWidth(text_buffer->fontId, (D4D_CHAR)d4d_extsrcBuffer[charCntTmp]);
      }        
    }while(charCnt == D4D_EXTSRC_BUFF_SIZE);
    
  
  }else

#endif  
  
  {
    pText += offset;
    
    while(*pText != 0)
    {    
      if(text_buffer->buffSize)
      {
        if(++char_ix >= text_buffer->buffSize)  
          break;
      }
      if(*pText == '\t')
      {
        tab = D4D_GetNextTab(pTab, width);  
        
        if(tab)
        {
          width = (D4D_COOR)(tab - (pTab->tabOffset));
          pText++;
          continue;  
        }
      } 
      
      width += D4D_GetCharWidth(text_buffer->fontId, *pText);
      pText++;
    }
  }
  
  
  return width;  
}


/**************************************************************//*!
*
* change text in the buffer
*
******************************************************************/

void D4D_ChangeText(D4D_STRING* pBuff, char* pNewText, char fillChar)
{
    Word n = pBuff->buffSize;
    char* pDest = pBuff->pText; 
    
    // preserve one byte in the buffer
    if(pDest != pNewText)
      while(n > 1 && *pNewText)
      {
        *pDest = *pNewText;
        pNewText++;
        pDest++;
        n--;        
      } else
      {
        Word i;
        i = D4D_GetTextLength(pDest);
        n -= i;
        pDest += i;
      }
    
    while(n > 1 && fillChar)
    {
      *(pDest++) = fillChar;
      n--;        
    }
    
    // terminate string always
    *pDest = 0;
}

/**************************************************************//*!
*
* Set Object text 
*
******************************************************************/

void D4D_SetText(D4D_OBJECT_PTR pObject, char* pText) 
{
  D4D_STRING* p_TextBuff = NULL;
  
  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4D_OBJECT*)pObject);
  
  if(p_TextBuff)
  {
    D4D_ChangeText(p_TextBuff, pText, 0);  
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }

}

/**************************************************************//*!
*
* Set Object text font properties
*
******************************************************************/

void D4D_SetFontProperties(D4D_OBJECT_PTR pObject, D4D_FONT_PROPERTIES property) 
{
  
  D4D_STRING* p_TextBuff = NULL;
  
  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4D_OBJECT*)pObject);
 
  if(p_TextBuff)
  {
    if(p_TextBuff->str_properties->font_properties.all == property.all)
      return;     // There is no change needed
    
    p_TextBuff->str_properties->font_properties.all = property.all;
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }

}

/**************************************************************//*!
*
* Set Object text properties
*
******************************************************************/

void D4D_SetTextProperties(D4D_OBJECT_PTR pObject, D4D_TEXT_PROPERTIES property) 
{
  
  D4D_STRING* p_TextBuff = NULL;
  
  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4D_OBJECT*)pObject);
 
  if(p_TextBuff)
  {
    if(p_TextBuff->str_properties->text_properties.all == property.all)
      return;     // There is no change needed
    
    p_TextBuff->str_properties->text_properties.all = property.all;
    D4D_InvalidateObject(pObject, D4D_FALSE);
  }

}

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Help Bitmap Functions
  #pragma mark -
#endif

/**************************************************************//*!
*
* Get bitmap Size
*
******************************************************************/

D4D_SIZE D4D_GetBmpSize(const D4D_BMP* pBmp)
{
  
  Word* pHead;  
  D4D_SIZE tmpSize = { 0, 0};
  
  #if D4D_ENDIAN == D4D_ENDIAN_BIG
    if(pBmp != NULL)
    {
      pHead = (Word*)pBmp->pData;
      tmpSize.cx = (D4D_COOR) D4D_MODIFY_WORD(pHead[0]);
      tmpSize.cy = (D4D_COOR) D4D_MODIFY_WORD(pHead[1]);    
    }
  #else
    if(pBmp != NULL)
    {
      D4D_BOOL endian = D4D_BmpHeaderCheckEndianism(pBmp);
      pHead = (Word*)pBmp->pData;
      tmpSize.cx = (D4D_COOR)(endian)? D4D_SWAP_WORD(pHead[0]) : pHead[0];
      tmpSize.cy = (D4D_COOR)(endian)? D4D_SWAP_WORD(pHead[1]) : pHead[1];    
    }    
  #endif
  
  return tmpSize;
}

/**************************************************************//*!
*
* Get bitmap width
*
******************************************************************/

D4D_COOR D4D_GetBmpWidth(const D4D_BMP* pBmp)
{
  Word* pHead;

  if(pBmp == NULL)
    return 0;
  
  pHead = (Word*)pBmp->pData;

  #if D4D_ENDIAN == D4D_ENDIAN_BIG
    return (D4D_COOR) pHead[0];
  #else
    return (D4D_COOR) (D4D_BmpHeaderCheckEndianism(pBmp))? D4D_SWAP_WORD(pHead[0]) : pHead[0];
  #endif
}

/**************************************************************//*!
*
* Get bitmap height
*
******************************************************************/

D4D_COOR D4D_GetBmpHeight(const D4D_BMP* pBmp)
{
  Word* pHead;

  if(pBmp == NULL)
    return 0;
  
  pHead = (Word*)pBmp->pData;
  
  #if D4D_ENDIAN == D4D_ENDIAN_BIG
    return (D4D_COOR) pHead[1];
  #else
    return (D4D_COOR) (D4D_BmpHeaderCheckEndianism(pBmp))? D4D_SWAP_WORD(pHead[1]) : pHead[1];
  #endif  
}

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Drawing Functions
  #pragma mark -
#endif

/**************************************************************//*!
*
* Draw bitmap 
*
******************************************************************/

void D4D_DrawBmp(D4D_POINT* ppt, const D4D_BMP* pBmp, D4D_BOOL greyScale)
{
  D4D_DrawBmpXY(ppt->x, ppt->y, pBmp, greyScale);
}


/**************************************************************//*!
*
* Draw bitmap 
*
******************************************************************/

void D4D_DrawBmpXY(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale)
{
  Word* pHead = (Word*)pBmp->pData;
  
 #if D4D_ENDIAN == D4D_ENDIAN_BIG 
  
  D4D_COOR width = (D4D_COOR)pHead[0];
  D4D_COOR height = (D4D_COOR)pHead[1];
  Word cmode = pHead[2];
  
  D4D_COOR x2 = (D4D_COOR)(x + width - 1);
  D4D_COOR y2 = (D4D_COOR)(y + height - 1);
  
  void* pData = pHead+3;
 #else
  D4D_BOOL endian = D4D_BmpHeaderCheckEndianism(pBmp);
  
  D4D_COOR width = (D4D_COOR)(endian)? D4D_SWAP_WORD(pHead[0]) : pHead[0];
  D4D_COOR height = (D4D_COOR)(endian)? D4D_SWAP_WORD(pHead[1]) : pHead[1];
  Word cmode = (endian)? D4D_SWAP_WORD(pHead[2]) : pHead[2];
  
  D4D_COOR x2 = (D4D_COOR)(x + width - 1);
  D4D_COOR y2 = (D4D_COOR)(y + height - 1);
  
  void* pData = pHead+3;
 #endif
    switch(cmode & (D4D_BMP_TYPE_MASK | D4D_BMP_CLR_DEPTH_MASK))
    {

#if D4D_BMP_65536NOPAL_ENABLE != D4D_FALSE    
    case D4D_BMP_65536NOPAL:                                  
        D4D_LCD_Bmp65kNoPal(x, y, x2, y2, (Word*)pData,
          (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale);       
        break;
#endif

#if D4D_BMP_256NOPAL_ENABLE != D4D_FALSE        
    case D4D_BMP_256NOPAL:                           
        D4D_LCD_Bmp256NoPal(x, y, x2, y2, (Byte*)pData,
          (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale); 
        break;
#endif

#if D4D_BMP_PAL_256_ENABLE != D4D_FALSE        
    case D4D_BMP_PAL_256:                           
         D4D_LCD_Bmp256Pal(x, y, x2, y2, (Byte*)pData, (unsigned short*)pBmp->pParam, 
          (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale);        
        break;
#endif

#if D4D_BMP_PAL_16_ENABLE != D4D_FALSE        
    case D4D_BMP_PAL_16:                    
        D4D_LCD_Bmp16Pal(x, y, x2, y2, (Byte*)pData, (unsigned short*)pBmp->pParam, 
          (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale);        
        break; 
#endif

#if D4D_BMP_PAL_2_ENABLE != D4D_FALSE    
    case D4D_BMP_PAL_2:  
        D4D_LCD_Bmp2Pal(x, y, x2, y2, (Byte*)pData, (unsigned short*)pBmp->pParam, 
          (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale);        
        break;
#endif
    
    default:
        // there are all non supported bitmaps 
        D4D_RectXY(x, y, x2, y2, D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore);   

        break;
    }
}

/**************************************************************//*!
*
* Basic drawing
*
******************************************************************/

void D4D_PutPixel(D4D_POINT* ppt, D4D_LINETYPE type, D4D_COLOR color) 
{
  D4D_LCD_PutPixel(ppt->x, ppt->y, type, color);
  d4d_curXY = *ppt;
}

void D4D_PutPixelTo(D4D_LINETYPE type, D4D_COLOR color) 
{
  D4D_LCD_PutPixel(d4d_curXY.x, d4d_curXY.y, type, color);  
}

void D4D_PutPixelXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE type, D4D_COLOR color) 
{
  D4D_LCD_PutPixel(x, y, type, color);
  d4d_curXY.x = x;
  d4d_curXY.y = y;
}

D4D_POINT D4D_SwapCoor(D4D_POINT point)
{
  D4D_POINT tmp_point;
  
  tmp_point.x = point.y;
  tmp_point.y = point.x;
  
  return tmp_point;
}

void D4D_MoveTo(D4D_POINT* ppt)
{
    d4d_curXY = *ppt;    
}

void D4D_MoveToXY(D4D_COOR x, D4D_COOR y)
{
  d4d_curXY.x = x;
  d4d_curXY.y = y;  
}

void D4D_ClearScreen(D4D_COLOR color) {
  D4D_LCD_Box(0, 0, D4D_DisplayWidth(), D4D_DisplayHeight(), color);
}

void D4D_LineTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Line(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, ltype, color);
    
    d4d_curXY = *ppt;    
}

void D4D_LineToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color)
{
   D4D_LCD_Line(d4d_curXY.x, d4d_curXY.y, x, y, ltype, color);
   
   d4d_curXY.x = x;    
   d4d_curXY.y = y;
}

void D4D_Rect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color)
{
    if(psz->cx && psz->cy)
      D4D_LCD_Rect((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), (D4D_LINETYPE)ltype, (D4D_COLOR)color);
    
    d4d_curXY = *ppt;    
}

void D4D_RectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Rect(x1, y1, x2, y2, ltype, color);
    
    d4d_curXY.x = x1;    
    d4d_curXY.y = y1;    
}

void D4D_RectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Rect(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, ltype, color);
    
    d4d_curXY = *ppt;    
}


void D4D_RectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Rect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, color);
    
    d4d_curXY.x = x;    
    d4d_curXY.y = y;    
}


void D4D_FillRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color)
{
    if(psz->cx && psz->cy)
      D4D_LCD_Box((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), (D4D_COLOR)color);
    
    d4d_curXY = *ppt;
}

void D4D_FillRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color)
{
    D4D_LCD_Box(x1, y1, x2, y2, color);
    
    d4d_curXY.x = x1;    
    d4d_curXY.y = y1;
}

void D4D_FillRectTo(D4D_POINT* ppt, D4D_COLOR color)
{
    D4D_LCD_Box(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, (D4D_COLOR)color);
    
    d4d_curXY = *ppt;
}

void D4D_FillRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color)
{
    D4D_LCD_Box(d4d_curXY.x, d4d_curXY.y, x, y, color);
    
    d4d_curXY.x = x;    
    d4d_curXY.y = y;
}

void D4D_Box(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{    
  if(psz->cx && psz->cy)
    D4D_BoxXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), ltype, colorLine, colorFill);
}

void D4D_BoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
    // fill background
    D4D_LCD_Box(x1, y1, x2, y2, colorFill);
    
    // draw rectangle around
    D4D_LCD_Rect(x1, y1, x2, y2, ltype, colorLine);
    
    d4d_curXY.x = x1;
    d4d_curXY.y = y1;   
}

void D4D_BoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
    D4D_BoxToXY(ppt->x, ppt->y, ltype, colorLine, colorFill);
}

void D4D_BoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill)
{
    // fill background
    D4D_LCD_Box(d4d_curXY.x, d4d_curXY.y, x, y, colorFill);
    
    // draw rectangle around
    D4D_LCD_Rect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, colorLine);
    
    d4d_curXY.x = x;
    d4d_curXY.y = y;   
}


void D4D_FillCircle(D4D_POINT* pCenter, D4D_COOR r, D4D_COLOR color)
{
    D4D_LCD_Circle(pCenter->x, pCenter->y, r, 1, D4D_LINE_THIN, color); 
}

void D4D_FillCircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_COLOR color)
{
    D4D_LCD_Circle(x, y, r, 1, D4D_LINE_THIN, color); 
}

void D4D_Circle(D4D_POINT* pCenter, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Circle(pCenter->x, pCenter->y, r, 0, ltype, color); 
}

void D4D_CircleXY(D4D_COOR x, D4D_COOR y, D4D_COOR r, D4D_LINETYPE ltype, D4D_COLOR color)
{
    D4D_LCD_Circle(x, y, r, 0, ltype, color); 
}


void D4D_FillRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  if(psz->cx && psz->cy)
    D4D_FillRectColorScaleXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), startColor, endColor, direction);
}

void D4D_FillRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  D4D_COOR range, i;
  D4D_COLOR color = startColor;
  
  d4d_curXY.x = x1;
  d4d_curXY.y = y1;
  
  switch(direction)
  {
    case D4D_DIR_UP:
      // switch X and Y coodinates and change direction of drawing
      startColor = endColor;
      endColor = color;
    case D4D_DIR_DOWN:
      // just switch X and Y coodinates
      range = (D4D_COOR)(y2 - y1);
      
      i = 0;
      for( ; y1 <= y2; y1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));
        i++;
        
        D4D_LCD_Box(x1, y1, x2, y1, color);    
      }
      break;
    
    case D4D_DIR_LEFT:
      // switch colors
      startColor = endColor;
      endColor = color;
    case D4D_DIR_RIGHT:
      // just keep all fine
      range = (D4D_COOR)(x2 - x1);
      i = 0;
      for( ; x1 <= x2; x1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));
        i++;
        
        D4D_LCD_Box(x1, y1, x1, y2, color);    
      }
      
      break;  
  }
  
  
  
  
}

void D4D_FillRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  D4D_FillRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, startColor, endColor, direction);
}

void D4D_FillRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction)
{
  D4D_FillRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, x, y, startColor, endColor, direction);
}


/**************************************************************//*!
*
* Draw text on XY coordination with TAB function
*
******************************************************************/
//void D4D_DrawTextTab(D4D_POINT* ppt, D4D_STRING* buffText, D4D_COOR* pTabTable, D4D_COLOR colorText, D4D_COLOR colorBkgd)
//{
//  D4D_DrawTextTabXY(ppt->x, ppt->y, buffText, pTabTable, colorText, colorBkgd); 
//}

/**************************************************************//*!
*
* Draw text on XY coordination with TAB function
*
******************************************************************/
void D4D_DrawTextTabXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_PRINT_STRING tmp_strDes;
    
  tmp_strDes.x = x;
  tmp_strDes.y = y;
  tmp_strDes.pText = buffText->pText;
  tmp_strDes.pFontType = D4D_GetFont(buffText->fontId);
  tmp_strDes.pTab = pTab;
  tmp_strDes.colorText = colorText;
  tmp_strDes.colorBack = colorBkgd;
  tmp_strDes.properties.all = buffText->str_properties->font_properties.all;
  tmp_strDes.textLength = (D4D_INDEX)(buffText->buffSize - 1);
  tmp_strDes.textOffset = 0;
  D4D_LCD_PrintStr(&tmp_strDes);  
}


/**************************************************************//*!
*
* Draw text on XY coordination
*
******************************************************************/
//void D4D_DrawText(D4D_POINT* ppt, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd)
//{
//  D4D_DrawTextTab(ppt, buffText, NULL, colorText, colorBkgd);
//}

/**************************************************************//*!
*
* Draw text on XY coordination
*
******************************************************************/
//void D4D_DrawTextXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_COLOR colorText, D4D_COLOR colorBkgd)
//{
//  D4D_DrawTextTabXY(x, y, buffText, NULL, colorText, colorBkgd);
//}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
/*
void D4D_DrawTextRectTab(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_PRINT_STRING tmp_strDes;
  D4D_SIZE tmp_size = *psz;
  D4D_TEXT_PROPERTIES tmp_txtProp;
  D4D_COOR tmp_txtWidth = D4D_GetTextBuffWidthTab(buffText, pTab);
  D4D_COOR tmp_txtHeight = D4D_GetFontHeight(buffText->fontId);
  
  tmp_strDes.pText = buffText->pText;
  tmp_strDes.pFontType = D4D_GetFont(buffText->fontId);
  tmp_strDes.pTab = pTab;
  tmp_strDes.colorText = colorText;
  tmp_strDes.colorBack = colorBkgd;
  tmp_strDes.properties.all = buffText->str_properties->font_properties.all;
  tmp_strDes.textLength = (D4D_INDEX)(buffText->buffSize - 1);
  
  if(!tmp_size.cx)
    tmp_size.cx = (D4D_COOR)(tmp_txtWidth + 1);
  
  if(!tmp_size.cy)
    tmp_size.cy = (D4D_COOR)(tmp_txtHeight + 1);
  
  tmp_txtProp.all = buffText->str_properties->text_properties.all;
  
   
  // Compute the text position in rectangle in Y axis
  switch(tmp_txtProp.bits.bAlignVertic)
  {
    case D4D_TXT_PRTY_ALIGN_V_BOTTOM:
      tmp_strDes.y = (D4D_COOR)(ppt->y + (tmp_size.cy - tmp_txtHeight));
      break;
    
    case D4D_TXT_PRTY_ALIGN_V_CENTER:
      tmp_strDes.y = (D4D_COOR)(ppt->y + ((tmp_size.cy - tmp_txtHeight) / 2));
      break;
    
    default:
    case D4D_TXT_PRTY_ALIGN_V_TOP:
      tmp_strDes.y = ppt->y;
      break;        
  }
  
  // Compute the text position in rectangle in X axis
  switch(tmp_txtProp.bits.bAlignHoriz)
  {
    case D4D_TXT_PRTY_ALIGN_H_RIGHT:
      tmp_strDes.x = (D4D_COOR)(ppt->x + (tmp_size.cx - tmp_txtWidth));
      break;
    
    case D4D_TXT_PRTY_ALIGN_H_CENTER:
      tmp_strDes.x = (D4D_COOR)(ppt->x + ((tmp_size.cx - tmp_txtWidth) / 2));
      break;
    
    default:
    case D4D_TXT_PRTY_ALIGN_H_LEFT:
      tmp_strDes.x = ppt->x;
      break;        
  }
  
  // Redraw the rest of rectangle where isn't text for normal (non trasparent) texts
  if(!tmp_strDes.properties.bits.bTransparent)
  {
    
    if(tmp_txtProp.bits.bAlignHoriz != D4D_TXT_PRTY_ALIGN_H_LEFT)
    {
      // Fill the part of rect in front of the text in full height of rect
      //if(tmp_strDes.x - ppt->x)
        D4D_FillRectXY(ppt->x, ppt->y, tmp_strDes.x, (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
    }
    
    if(tmp_txtProp.bits.bAlignHoriz != D4D_TXT_PRTY_ALIGN_H_RIGHT)
    {
      // Fill the part of rect behind the text in full height of rect
      //if((D4D_COOR)(ppt->x + tmp_size.cx - 1) - (D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1))
        D4D_FillRectXY((D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), ppt->y, (D4D_COOR)(ppt->x + tmp_size.cx - 1), (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
    }
    

      
    if(tmp_txtProp.bits.bAlignVertic != D4D_TXT_PRTY_ALIGN_V_TOP)
    {
      // Fill the part of rect above the text in lenght of text
      D4D_FillRectXY(tmp_strDes.x, ppt->y, (D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), tmp_strDes.y, colorBkgd);
    }
    
    if(tmp_txtProp.bits.bAlignVertic != D4D_TXT_PRTY_ALIGN_V_BOTTOM)
    {
      // Fill the part of rect under the text in lenght of text
      D4D_FillRectXY(tmp_strDes.x, (D4D_COOR)(tmp_strDes.y + tmp_txtHeight - 1), (D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
    }

  }
  
  // Print the string itself
  D4D_LCD_PrintStr(&tmp_strDes);
}
  */
/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
/*
void D4D_DrawTextRectTabTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_SIZE tmp_size;
  
  tmp_size.cx = (D4D_COOR)(ppt->x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(ppt->y - d4d_curXY.y);
  
  D4D_DrawTextRectTab( &d4d_curXY, &tmp_size, buffText, pTab, colorText, colorBkgd);
}
*/
/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
/*
void D4D_DrawTextRectTabXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  
  tmp_point.x = x1;
  tmp_point.y = y1;
  
  tmp_size.cx = (D4D_COOR)(x2 - x1);
  tmp_size.cy = (D4D_COOR)(y2 - y1);
  
  
  D4D_DrawTextRectTab( &tmp_point, &tmp_size, buffText, pTab, colorText, colorBkgd);
}
*/
/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
/*
void D4D_DrawTextRectTabToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_SIZE tmp_size;
  
  tmp_size.cx = (D4D_COOR)(x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(y - d4d_curXY.y);
  
  
  D4D_DrawTextRectTab( &d4d_curXY, &tmp_size, buffText, pTab, colorText, colorBkgd);
}
*/

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4D_DrawTextRectTabAdv(D4D_POINT* ppt, D4D_SIZE* psz, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_PRINT_STRING tmp_strDes;
  D4D_SIZE tmp_size = *psz;
  D4D_TEXT_PROPERTIES tmp_txtProp;
  D4D_COOR tmp_txtWidth = D4D_GetTextBuffWidthTabAdj(buffText, txtOff, pTab);
  D4D_COOR tmp_txtHeight = D4D_GetFontHeight(buffText->fontId);
  
  tmp_strDes.pText = buffText->pText;
  tmp_strDes.pFontType = D4D_GetFont(buffText->fontId);
  tmp_strDes.pTab = pTab;
  tmp_strDes.colorText = colorText;
  tmp_strDes.colorBack = colorBkgd;
  tmp_strDes.properties.all = buffText->str_properties->font_properties.all;
  tmp_strDes.textLength = (D4D_INDEX)(buffText->buffSize - 1);
  tmp_strDes.textOffset = txtOff;
  if(!tmp_size.cx)
    tmp_size.cx = (D4D_COOR)(tmp_txtWidth + 1);
  
  if(!tmp_size.cy)
    tmp_size.cy = (D4D_COOR)(tmp_txtHeight + 1);
  
  tmp_txtProp.all = buffText->str_properties->text_properties.all;
  
   
  // Compute the text position in rectangle in Y axis
  switch(tmp_txtProp.bits.bAlignVertic)
  {
    case D4D_TXT_PRTY_ALIGN_V_BOTTOM:
      tmp_strDes.y = (D4D_COOR)(ppt->y + (tmp_size.cy - tmp_txtHeight));
      break;
    
    case D4D_TXT_PRTY_ALIGN_V_CENTER:
      tmp_strDes.y = (D4D_COOR)(ppt->y + ((tmp_size.cy - tmp_txtHeight) / 2));
      break;
    
    default:
    case D4D_TXT_PRTY_ALIGN_V_TOP:
      tmp_strDes.y = ppt->y;
      break;        
  }
  
  // Compute the text position in rectangle in X axis
  switch(tmp_txtProp.bits.bAlignHoriz)
  {
    case D4D_TXT_PRTY_ALIGN_H_RIGHT:
      tmp_strDes.x = (D4D_COOR)(ppt->x + (tmp_size.cx - tmp_txtWidth));
      break;
    
    case D4D_TXT_PRTY_ALIGN_H_CENTER:
      tmp_strDes.x = (D4D_COOR)(ppt->x + ((tmp_size.cx - tmp_txtWidth) / 2));
      break;
    
    default:
    case D4D_TXT_PRTY_ALIGN_H_LEFT:
      tmp_strDes.x = ppt->x;
      break;        
  }
  
  // Redraw the rest of rectangle where isn't text for normal (non trasparent) texts
  if(!tmp_strDes.properties.bits.bTransparent)
  {
    
    if(tmp_txtProp.bits.bAlignHoriz != D4D_TXT_PRTY_ALIGN_H_LEFT)
    {
      // Fill the part of rect in front of the text in full height of rect
      //if(tmp_strDes.x - ppt->x)
        D4D_FillRectXY(ppt->x, ppt->y, tmp_strDes.x, (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
    }
    
    if(tmp_txtProp.bits.bAlignHoriz != D4D_TXT_PRTY_ALIGN_H_RIGHT)
    {
      // Fill the part of rect behind the text in full height of rect
      //if((D4D_COOR)(ppt->x + tmp_size.cx - 1) - (D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1))
        D4D_FillRectXY((D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), ppt->y, (D4D_COOR)(ppt->x + tmp_size.cx - 1), (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
    }
    

      
    if(tmp_txtProp.bits.bAlignVertic != D4D_TXT_PRTY_ALIGN_V_TOP)
    {
      // Fill the part of rect above the text in lenght of text
      D4D_FillRectXY(tmp_strDes.x, ppt->y, (D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), tmp_strDes.y, colorBkgd);
    }
    
    if(tmp_txtProp.bits.bAlignVertic != D4D_TXT_PRTY_ALIGN_V_BOTTOM)
    {
      // Fill the part of rect under the text in lenght of text
      D4D_FillRectXY(tmp_strDes.x, (D4D_COOR)(tmp_strDes.y + tmp_txtHeight - 1), (D4D_COOR)(tmp_strDes.x + tmp_txtWidth - 1), (D4D_COOR)(ppt->y + tmp_size.cy - 1), colorBkgd);
    }

  }
  
  // Print the string itself
  D4D_LCD_PrintStr(&tmp_strDes);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4D_DrawTextRectTabAdvTo(D4D_POINT* ppt, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_SIZE tmp_size;
  
  tmp_size.cx = (D4D_COOR)(ppt->x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(ppt->y - d4d_curXY.y);
  
  D4D_DrawTextRectTabAdv( &d4d_curXY, &tmp_size, buffText, txtOff, pTab, colorText, colorBkgd);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4D_DrawTextRectTabAdvXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_POINT tmp_point;
  D4D_SIZE tmp_size;
  
  tmp_point.x = x1;
  tmp_point.y = y1;
  
  tmp_size.cx = (D4D_COOR)(x2 - x1);
  tmp_size.cy = (D4D_COOR)(y2 - y1);
  
  
  D4D_DrawTextRectTabAdv( &tmp_point, &tmp_size, buffText, txtOff, pTab, colorText, colorBkgd);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4D_DrawTextRectTabAdvToXY(D4D_COOR x, D4D_COOR y, D4D_STRING* buffText, D4D_INDEX txtOff, D4D_TAB* pTab, D4D_COLOR colorText, D4D_COLOR colorBkgd)
{
  D4D_SIZE tmp_size;
  
  tmp_size.cx = (D4D_COOR)(x - d4d_curXY.x);
  tmp_size.cy = (D4D_COOR)(y - d4d_curXY.y);
  
  
  D4D_DrawTextRectTabAdv( &d4d_curXY, &tmp_size, buffText, txtOff, pTab, colorText, colorBkgd);
}


#ifdef __CWCC__
  #pragma mark -
  #pragma mark Drawing Round Corner Functions
  #pragma mark -
#endif

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE

void D4D_RRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
  if(psz->cx && psz->cy)
    D4D_LCD_RRect((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), ltype, color, radius);
    
    d4d_curXY = *ppt;    
}

void D4D_RRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RRect(x1, y1, x2, y2, ltype, color,radius);
    
    d4d_curXY.x = x1;    
    d4d_curXY.y = y1;    
}

void D4D_RRectTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RRect(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, ltype, color, radius);
    
    d4d_curXY = *ppt;    
}


void D4D_RRectToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RRect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, color, radius);
    
    d4d_curXY.x = x;    
    d4d_curXY.y = y;    
}


void D4D_FillRRect(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR color, D4D_COOR radius)
{
  if(psz->cx && psz->cy)  
    D4D_LCD_RBox((D4D_COOR)ppt->x, (D4D_COOR)ppt->y, (D4D_COOR)(ppt->x + psz->cx - 1), (D4D_COOR)(ppt->y + psz->cy - 1), color, radius);
    d4d_curXY = *ppt;
}

void D4D_FillRRectXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RBox(x1, y1, x2, y2, color, radius);
    
    d4d_curXY.x = x1;    
    d4d_curXY.y = y1;
}

void D4D_FillRRectTo(D4D_POINT* ppt, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RBox(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, (D4D_COLOR)color, radius);
    
    d4d_curXY = *ppt;
}

void D4D_FillRRectToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR color, D4D_COOR radius)
{
    D4D_LCD_RBox(d4d_curXY.x, d4d_curXY.y, x, y, color, radius);
    
    d4d_curXY.x = x;    
    d4d_curXY.y = y;
}

void D4D_RBox(D4D_POINT* ppt, D4D_SIZE* psz, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{    
  if(psz->cx && psz->cy)  
    D4D_RBoxXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x + psz->cx - 1), (D4D_COOR)(ppt->y + psz->cy - 1), ltype, colorLine, colorFill, radius);
}

void D4D_RBoxXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
    // fill background
    D4D_LCD_RBox(x1, y1, x2, y2, colorFill, radius);
    
    // draw rectangle around
    D4D_LCD_RRect(x1, y1, x2, y2, ltype, colorLine, radius);
    
    d4d_curXY.x = x1;
    d4d_curXY.y = y1;   
}

void D4D_RBoxTo(D4D_POINT* ppt, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
    D4D_RBoxToXY(ppt->x, ppt->y, ltype, colorLine, colorFill, radius);
}

void D4D_RBoxToXY(D4D_COOR x, D4D_COOR y, D4D_LINETYPE ltype, D4D_COLOR colorLine, D4D_COLOR colorFill, D4D_COOR radius)
{
    // fill background
    D4D_LCD_RBox(d4d_curXY.x, d4d_curXY.y, x, y, colorFill, radius);
    
    // draw rectangle around
    D4D_LCD_RRect(d4d_curXY.x, d4d_curXY.y, x, y, ltype, colorLine, radius);
    
    d4d_curXY.x = x;
    d4d_curXY.y = y;   
}


void D4D_FillQuadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(pCenter->x, pCenter->y, radius, D4D_TRUE, D4D_LINE_THIN, color, quadrant); 
}

void D4D_FillQuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(x, y, radius, D4D_TRUE, D4D_LINE_THIN, color, quadrant); 
}

void D4D_Quadrant(D4D_POINT* pCenter, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(pCenter->x, pCenter->y, radius, D4D_FALSE, ltype, color, quadrant); 
}

void D4D_QuadrantXY(D4D_COOR x, D4D_COOR y, D4D_COOR radius, D4D_LINETYPE ltype, D4D_COLOR color, D4D_QUADRANT quadrant)
{
    D4D_LCD_QCircle(x, y, radius, D4D_FALSE, ltype, color, quadrant); 
}

void D4D_FillRRectColorScale(D4D_POINT* ppt, D4D_SIZE* psz, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  if(psz->cx && psz->cy)
    D4D_FillRRectColorScaleXY(ppt->x, ppt->y, (D4D_COOR)(ppt->x+psz->cx - 1), (D4D_COOR)(ppt->y+psz->cy - 1), startColor, endColor, direction, radius);
}

void D4D_FillRRectColorScaleXY(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  D4D_COOR range, i, non_print_len;
  D4D_COLOR color = startColor;
  
  if(radius)
  {
    
    d4d_curXY.x = x1;
    d4d_curXY.y = y1;

    switch(direction)
    {
    case D4D_DIR_UP:
      // switch X and Y coodinates and change direction of drawing
      startColor = endColor;
      endColor = color;
    case D4D_DIR_DOWN:
      // just switch X and Y coodinates
      range = (D4D_COOR)(y2 - y1);
      
      i = 0;
      for( ; y1 <= y2; y1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));
        

        if(i < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, i); // get len of non printed pixels
        else if((range - i) < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(range - i)); // get len of non printed pixels
        else
          non_print_len = 0;
      
        i++;
        D4D_LCD_Box((D4D_COOR)(x1 + non_print_len), y1, (D4D_COOR)(x2 - non_print_len), y1, color);    
      }
      break;

    case D4D_DIR_LEFT:
      // switch colors
      startColor = endColor;
      endColor = color;
    case D4D_DIR_RIGHT:
      // just keep all fine
      range = (D4D_COOR)(x2 - x1);
      
      i = 0;
      for( ; x1 <= x2; x1++)
      {
        if(range)
          color = D4D_GetCrossColor(startColor, endColor, (Byte)D4D_MulDivUU(255, i, range));
        

        if(i < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, i); // get len of non printed pixels
        else if((range - i) < radius)
          non_print_len = D4D_RndCornerGetNonPrintedPxl(radius, (D4D_COOR)(range - i)); // get len of non printed pixels
        else
          non_print_len = 0;
      
        i++;
        D4D_LCD_Box(x1, (D4D_COOR)(y1 + non_print_len), x1, (D4D_COOR)(y2 - non_print_len), color);    
      }
      
      break;  
    }
  }else
  {
    D4D_FillRectColorScaleXY(x1, y1, x2, y2, startColor, endColor, direction);  
  }
  
}

void D4D_FillRRectColorScaleTo(D4D_POINT* ppt, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  D4D_FillRRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, ppt->x, ppt->y, startColor, endColor, direction, radius);
}

void D4D_FillRRectColorScaleToXY(D4D_COOR x, D4D_COOR y, D4D_COLOR startColor, D4D_COLOR endColor, D4D_DIRECTION direction, D4D_COOR radius)
{
  D4D_FillRRectColorScaleXY(d4d_curXY.x, d4d_curXY.y, x, y, startColor, endColor, direction, radius);
}



/**************************************************************//*!
*
* Draw bitmap 
*
******************************************************************/

void D4D_DrawRBmp(D4D_POINT* ppt, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius)
{
  if(radius)
    D4D_DrawRBmpXY(ppt->x, ppt->y, pBmp, greyScale, radius);
  else
    D4D_DrawBmpXY(ppt->x, ppt->y, pBmp, greyScale);
}


/**************************************************************//*!
*
* Draw bitmap 
*
******************************************************************/

void D4D_DrawRBmpXY(D4D_COOR x, D4D_COOR y, const D4D_BMP* pBmp, D4D_BOOL greyScale, D4D_COOR radius)
{
    Word* pHead = (Word*)pBmp->pData;
  
   #if D4D_ENDIAN == D4D_ENDIAN_BIG 
    
    D4D_COOR width = (D4D_COOR)pHead[0];
    D4D_COOR height = (D4D_COOR)pHead[1];
    Word cmode = pHead[2];
    
    D4D_COOR x2 = (D4D_COOR)(x + width - 1);
    D4D_COOR y2 = (D4D_COOR)(y + height - 1);
    
    void* pData = pHead+3;
   #else
    D4D_BOOL endian = D4D_BmpHeaderCheckEndianism(pBmp);
    
    D4D_COOR width = (D4D_COOR)(endian)? D4D_SWAP_WORD(pHead[0]) : pHead[0];
    D4D_COOR height = (D4D_COOR)(endian)? D4D_SWAP_WORD(pHead[1]) : pHead[1];
    Word cmode = (endian)? D4D_SWAP_WORD(pHead[2]) : pHead[2];
    
    D4D_COOR x2 = (D4D_COOR)(x + width - 1);
    D4D_COOR y2 = (D4D_COOR)(y + height - 1);
    void* pData = pHead+3;
   #endif 
    
    
    if(radius == 0)
      D4D_DrawBmpXY(x, y, pBmp, greyScale);
    else
    {
      
    
      switch(cmode & (D4D_BMP_TYPE_MASK | D4D_BMP_CLR_DEPTH_MASK))
      {

#if D4D_BMPR_65536NOPAL_ENABLE != D4D_FALSE    
      case D4D_BMP_65536NOPAL:                                  
          D4D_LCD_RBmp65kNoPal(x, y, x2, y2, (unsigned short*)pData,
            (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale, radius);       
          break;
#endif
  
#if D4D_BMPR_256NOPAL_ENABLE != D4D_FALSE        
      case D4D_BMP_256NOPAL:                           
          D4D_LCD_RBmp256NoPal(x, y, x2, y2, (Byte*)pData,
            (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale, radius); 
          break;
#endif

#if D4D_BMPR_PAL_256_ENABLE != D4D_FALSE        
      case D4D_BMP_PAL_256:                           
           D4D_LCD_RBmp256Pal(x, y, x2, y2, (Byte*)pData, (unsigned short*)pBmp->pParam, 
            (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale, radius);        
          break;
#endif
/*
#if D4D_BMPR_PAL_16_ENABLE != D4D_FALSE        
      case D4D_BMP_PAL_16:                    
          D4D_LCD_RBmp16Pal(x, y, x2, y2, (Byte*)pData, (unsigned short*)pBmp->pParam, 
            (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale, radius);        
          break; 
#endif

#if D4D_BMPR_PAL_2_ENABLE != D4D_FALSE    
      case D4D_BMP_PAL_2:  
          D4D_LCD_RBmp2Pal(x, y, x2, y2, (Byte*)pData, (unsigned short*)pBmp->pParam, 
            (Byte)(cmode & D4D_BMP_COMPRESS_MASK), (Byte)greyScale, radius);        
          break;
#endif
*/    
      default:
          // there are all non supported bitmaps 
          (void)greyScale;
          D4D_RBoxXY(x, y, x2, y2, D4D_LINE_THIN, D4D_ScreenGetScheme(D4D_GetActiveScreen())->fore, D4D_ScreenGetScheme(D4D_GetActiveScreen())->bckg, radius);   

          break;
      }
    }
}

D4D_COOR D4D_RndCornerGetNonPrintedPxl(D4D_COOR radius, D4D_COOR line)
{
  D4D_INDEX_DELTA error =  (D4D_INDEX_DELTA)(-1 * (D4D_INDEX_DELTA)radius);
  D4D_COOR x = radius;
  D4D_COOR y = 0;
  D4D_COOR x1_bf,y1_bf;
  D4D_BOOL match = D4D_FALSE;    
 
  line = (D4D_COOR)(radius - line);
 
  if(line == 0) return radius;
  
  while (x >= y)
  {    
    if ((x != 0) && (y != 0)) //Valid point
    {
    	if(y == line) return (D4D_COOR)(radius - x);
    	
    	if(x == line)
    	{
    		match = D4D_TRUE;
    		x1_bf = (Byte)x;
    		y1_bf = (Byte)y;
    	}
      
      if((x != x1_bf) && match) 
        return (D4D_COOR)(radius - y1_bf);    	
    }
    
    error += y;
    ++y;
    error += y;

    if (error >= 0)
    {
      if((radius - x) >= line)
        return (D4D_COOR)(radius - y);
      
      x--;
      error -= x;
      error -= x;
    }
  }
  
  return (D4D_COOR)(radius - x);
}

#endif

#ifdef __CWCC__
  #pragma mark -
  #pragma mark Help Functions
  #pragma mark -
#endif

/**************************************************************//*!
*
* Compute cross color between two basic color in 256 steps  
*
******************************************************************/
D4D_COLOR D4D_GetCrossColor(D4D_COLOR startColor, D4D_COLOR endColor, Byte value)
{
  Byte r, g, b;
  Byte c_s, c_e;
  
  //Red color computing
  c_s = D4D_COLOR_GET_R(startColor);
  c_e = D4D_COLOR_GET_R(endColor);
  
  if(c_s < c_e)
    r = (Byte)(c_s + D4D_MulDivUU8(value, (Byte)(c_e - c_s), 255));
  else
    r = (Byte)(c_s - D4D_MulDivUU8(value, (Byte)(c_s - c_e), 255));
  
  //Green color computing
  c_s = D4D_COLOR_GET_G(startColor);
  c_e = D4D_COLOR_GET_G(endColor);
  
  if(c_s < c_e)
    g = (Byte)(c_s + D4D_MulDivUU8(value, (Byte)(c_e - c_s), 255));
  else
    g = (Byte)(c_s - D4D_MulDivUU8(value, (Byte)(c_s - c_e), 255));
  
  //Blue color computing
  c_s = D4D_COLOR_GET_B(startColor);
  c_e = D4D_COLOR_GET_B(endColor);
  
  if(c_s < c_e)
    b = (Byte)(c_s + D4D_MulDivUU8(value, (Byte)(c_e - c_s), 255));
  else
    b = (Byte)(c_s - D4D_MulDivUU8(value, (Byte)(c_s - c_e), 255));
  
  return D4D_COLOR_RGB(r, g, b);
  
}


/**************************************************************//*!
*
* Compute the grayscale color  
*
******************************************************************/
D4D_COLOR D4D_GetGreyScale(D4D_COLOR color)
{
  return D4D_LCD_GetGreyScale(color);
}