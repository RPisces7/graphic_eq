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
* @file      d4dtch_mcf52277_asp.h
*
* @author    b01119
* 
* @version   0.0.2.0
* 
* @date      Sep-1-2010
* 
* @brief     D4D driver - mcf52277_asp touch screen driver function header file 
*
*******************************************************************************/

#ifndef __D4DTCH_MDF52277_ASP_H
#define __D4DTCH_MDF52277_ASP_H

  #if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_mcf52277_asp_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtch_mcf52277_asp_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
    
    #ifndef D4DTCH_MIRROVED_X
    	#define D4DTCH_MIRROVED_X 0
    #endif
    	
    #ifndef D4DTCH_MIRROVED_Y
    	#define D4DTCH_MIRROVED_Y 0
    #endif
    
    #ifndef D4DTCH_SWAP_AXES
    	#define D4DTCH_SWAP_AXES 0
    #endif
    
    
    #ifndef D4DTCH_ASYNCH_EVENTS
    	#define D4DTCH_ASYNCH_EVENTS 0
    #endif	
    
    #ifndef D4DTCHHW_VECTOR_RAM
      #define D4DTCHHW_VECTOR_RAM __VECTOR_RAM
    #endif  
    
     /****** Algorithm constants/macros *****/
    #ifndef D4DTCH_FULL_SCALE
      #define D4DTCH_FULL_SCALE           0x0FFF
    #endif 
                         
    #ifndef D4DTCH_X_TOUCH_MIN
      #define D4DTCH_X_TOUCH_MIN              (D4DTCH_FULL_SCALE / 10)
    #endif
   
    #ifndef D4DTCH_Y_TOUCH_MIN
      #define D4DTCH_Y_TOUCH_MIN              (D4DTCH_FULL_SCALE / 10)
    #endif
   
    #ifndef D4DTCH_X_TOUCH_OFFMAX
      #define D4DTCH_X_TOUCH_OFFMAX              (D4DTCH_X_TOUCH_MIN * 4)
    #endif
   
    #ifndef D4DTCH_Y_TOUCH_OFFMAX
      #define D4DTCH_Y_TOUCH_OFFMAX              (D4DTCH_Y_TOUCH_MIN * 4)
    #endif
   
    #ifndef D4DTCH_SAMPLE_MARGIN
      #define D4DTCH_SAMPLE_MARGIN            (D4DTCH_FULL_SCALE / 256)
    #endif
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DTCH_MDF52277_ASP_H */










