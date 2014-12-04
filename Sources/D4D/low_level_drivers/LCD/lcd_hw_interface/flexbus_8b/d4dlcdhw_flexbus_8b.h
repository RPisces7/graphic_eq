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
* @file      d4dlcdhw_flexbus_8b.h
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      May-5-2010
* 
* @brief     D4D driver - flexbus_8b hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4DLCDHW_FLEXBUS_8B_H
#define __D4DLCDHW_FLEXBUS_8B_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_flexbus_8b_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_flexbus_8b_cfg.h"
    
    
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
    
      #define SET(x)           (x##_PORT) |= (1 << (x))
      #define RESET(x)         (x##_PORT) &= ~(1 << (x))

      #define OUTPUT(x)        (x##_DDR) |= (1 << (x)) 
      #define INPUT(x)         (x##_DDR) &= ~(1 << (x))
    
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    
      #ifndef D4DLCD_FLEX_BASE_ADDRESS 
        #define D4DLCD_FLEX_BASE_ADDRESS  0x00410000
      #endif
      
      #ifndef D4DLCD_FLEX_DC_ADDRESS
        #define D4DLCD_FLEX_DC_ADDRESS    0x00400000
      #endif
      
      #ifndef D4DLCD_FLEX_ADRESS_MASK
        #define D4DLCD_FLEX_ADRESS_MASK    0x00010000
      #endif
      
      
      #ifndef D4DLCD_FLEX_CS
        #define D4DLCD_FLEX_CS 0
      #else
        #if (D4DLCD_FLEX_CS < 0) || (D4DLCD_FLEX_CS > 1)
          #error The D4DLCD_FLEX_CS value must be set to 0 or 1
        #endif
      #endif  
      
      #ifdef D4DLCD_BACKLIGHT

        #ifndef D4DLCD_INIT_BACKLIGHT
        	#define D4DLCD_INIT_BACKLIGHT OUTPUT(D4DLCD_BACKLIGHT);
        #endif

        #ifndef D4DLCD_ASSERT_BACKLIGHT
        	#define D4DLCD_ASSERT_BACKLIGHT RESET(D4DLCD_BACKLIGHT);
        #endif  

        #ifndef D4DLCD_DEASSERT_BACKLIGHT
          #define D4DLCD_DEASSERT_BACKLIGHT SET(D4DLCD_BACKLIGHT);      
        #endif

      #endif

      #ifndef D4DLCD_INIT_RESET
        #ifndef D4DLCD_RESET
        	#error "The Macro D4DLCD_RESET must be set in LCD low level driver configuration file." 
        #else
        	#define D4DLCD_INIT_RESET OUTPUT(D4DLCD_RESET);
        #endif 	
      #endif

      #ifndef D4DLCD_ASSERT_RESET
        #ifndef D4DLCD_RESET
        	#error "The Macro D4DLCD_RESET must be set in LCD low level driver configuration file." 
        #else    
        	#define D4DLCD_ASSERT_RESET RESET(D4DLCD_RESET);
        #endif	
      #endif  

      #ifndef D4DLCD_DEASSERT_RESET
        #ifndef D4DLCD_RESET
        	#error "The Macro D4DLCD_RESET must be set in LCD low level driver configuration file." 
        #else    
        	#define D4DLCD_DEASSERT_RESET SET(D4DLCD_RESET);
        #endif	
      #endif
        
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DLCDHW_FLEXBUS_8B_H */










