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
* @file      d4dlcd_frame_buffer.h
*
* @author    b01119
* 
* @version   0.0.2.0
* 
* @date      Aug-30-2010
* 
* @brief     D4D driver - frame_buffer lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4DLCD_FRAMEBUFFER_H
#define __D4DLCD_FRAMEBUFFER_H

  #if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_frame_buffer_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    //#include "d4dlcd_frame_buffer_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
   	#ifndef D4DLCDHWFB_DIRECT_MEM_ACCESS
   		#define D4DLCDHWFB_DIRECT_MEM_ACCESS 0
   	#endif	
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

		#ifdef D4D_LLD_LCD_HW
		  extern const D4DLCDHWFB_FUNCTIONS D4D_LLD_LCD_HW;
		#endif

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DLCD_FRAMEBUFFER_H */









