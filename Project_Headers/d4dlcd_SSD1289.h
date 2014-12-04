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
* @file      d4dlcd_SSD1289.h
*
* @author    b01119
* 
* @version   0.0.21.0
* 
* @date      Jun-4-2010
* 
* @brief     D4D driver SSD1289 hardware level LCD header file 
*
*******************************************************************************/

#ifndef __D4DRV_SSD1289_H
#define __D4DRV_SSD1289_H

#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ssd1289_ID)

#include "d4dlcd_SSD1289_cfg.h"

  /******************************************************************************
  * Includes
  ******************************************************************************/

  /******************************************************************************
  * Constants
  ******************************************************************************/

  #define D4DLCD_PIXEL_COUNT (D4D_SCREEN_SIZE_SHORTER_SIDE * D4D_SCREEN_SIZE_LONGER_SIDE)
   
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
  	extern const D4DLCDHW_FUNCTIONS D4D_LLD_LCD_HW;
	#endif

  /******************************************************************************
  * Global functions
  ******************************************************************************/

#endif
#endif /* __D4DRV_SSD1289_H */










