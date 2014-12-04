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
* @file      d4d_private.h
*
* @author    b01119
* 
* @version   0.0.21.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver private header file
*
*******************************************************************************/


#ifndef __D4D_PRIVATE_H
#define __D4D_PRIVATE_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Macros 
******************************************************************************/

#define D4D_MK_STR1(x) x##_ID
#define D4D_MK_STR(x) D4D_MK_STR1(x)


/******************************************************************************
* Global variables
******************************************************************************/

// screen activation hisotry array & index
extern D4D_SCREEN* d4d_screenHistory[D4D_SCREEN_HISTORY];
extern Byte d4d_screenHistoryIndex;

// if not NULL, keys are captured by this object
extern D4D_OBJECT* d4d_pKeysCapturer;

// one and only message object being used to route information to objects
extern D4D_MESSAGE d4d_msg;

// The D4D system flags
extern D4D_SYSTEM_FLAGS d4d_systemFlags;

// zero size structure for automatic function capability
extern const D4D_SIZE d4d_size_zero;

// zero point structure 
extern const D4D_POINT d4d_point_zero; 

#ifdef D4D_LLD_TCH
  extern D4D_OBJECT* d4d_LastTouchedObj;
#endif

#if (D4D_EXTSRC_TEXT_ENABLE == D4D_TRUE) || (D4D_EXTSRC_FILE_ENABLE == D4D_TRUE)
  extern Byte d4d_extsrcBuffer[D4D_EXTSRC_BUFF_SIZE];
#endif

/******************************************************************************
* Global functions
******************************************************************************/

// Internal API of base functions

void D4D_HandleKeys(void);
void D4D_HandleTouchScreen(D4D_SCREEN* pScreen);

void D4D_SendMessage(D4D_MESSAGE* pMsg);

void D4D_ObjOnMessage(D4D_MESSAGE* pMsg);
void D4D_ChangeText(D4D_STRING* pBuff, char* pNewText, char fillChar);

void D4D_FocusNextObject(D4D_SCREEN* pScreen, Byte bInitialSearch);
void D4D_FocusPrevObject(D4D_SCREEN* pScreen);

void D4D_ManageTouchScreenEvents(void);
D4D_BOOL D4D_GetTouchScreen(D4D_COOR *TouchPositionX, D4D_COOR *TouchPositionY);

void D4D_RedrawScreen(D4D_SCREEN* pScreen);
D4D_BOOL D4D_ScrCheckExitBtnCoor(D4D_SCREEN* pScreen, D4D_POINT* point);
D4D_BOOL D4D_ScrCheckCoor(D4D_SCREEN* pScreen, D4D_POINT* point);

// Font private function declaration


#endif /* __D4D_PRIVATE_H */
