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
* @file      d4dlcdhw_flexbus_16b.h
*
* @author    b01119
* 
* @version   0.0.4.0
* 
* @date      Jun-28-2010
* 
* @brief     D4D driver - flexbus_16b hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4DLCDHW_FLEXBUS_16B_H
#define __D4DLCDHW_FLEXBUS_16B_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_flexbus_16b_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_flexbus_16b_cfg.h"
    
    
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
      
     
     #ifndef D4DLCD_FLEX_CSAR
     	#if D4DLCD_FLEX_CS == 0
     		#define D4DLCD_FLEX_CSAR CSAR0
     	#else
     		#define D4DLCD_FLEX_CSAR CSAR1
     	#endif
     #endif
     
     #ifndef D4DLCD_FLEX_CSMR
     	#if D4DLCD_FLEX_CS == 0
     		#define D4DLCD_FLEX_CSMR CSMR0
     	#else
     		#define D4DLCD_FLEX_CSMR CSMR1
     	#endif
     #endif
     
     #ifndef D4DLCD_FLEX_CSCR
     	#if D4DLCD_FLEX_CS == 0
     		#define D4DLCD_FLEX_CSCR CSCR0
     	#else
     		#define D4DLCD_FLEX_CSCR CSCR1
     	#endif
     #endif
     
        
		#ifndef D4DLCD_FLEX_CSMR_V_MASK
			#define D4DLCD_FLEX_CSMR_V_MASK CSMR0_V_MASK
		#endif
    
    #ifndef D4DLCD_FLEX_CSCR_MUX_MASK
			#define D4DLCD_FLEX_CSCR_MUX_MASK CSCR0_MUX_MASK
		#endif
    
    #ifndef D4DLCD_FLEX_CSCR_AA_MASK
			#define D4DLCD_FLEX_CSCR_AA_MASK CSCR0_AA_MASK
		#endif
    
    #ifndef D4DLCD_FLEX_CSCR_PS1_MASK
			#define D4DLCD_FLEX_CSCR_PS1_MASK CSCR0_PS1_MASK
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
#endif /* __D4DLCDHW_FLEXBUS_16B_H */










