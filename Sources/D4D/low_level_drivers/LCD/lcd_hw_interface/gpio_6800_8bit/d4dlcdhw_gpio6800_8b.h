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
* @file      d4dlcdhw_gpio6800_8b.h
*
* @author    b01119
* 
* @version   0.0.2.0
* 
* @date      May-7-2010
* 
* @brief     D4D driver SSD1289 hardware level LCD header file 
*
*******************************************************************************/

#ifndef __D4DRVHW_GPIO6800_8B_H
#define __D4DRVHW_GPIO6800_8B_H

#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_gpio6800_8b_ID)

#include "d4dlcdhw_gpio6800_8b_cfg.h"

  /******************************************************************************
  * Includes
  ******************************************************************************/
  
  /******************************************************************************
  * Macros 
  ******************************************************************************/
  
    #define SET(x)           (x##_PORT) |= (1 << (x))
    #define RESET(x)         (x##_PORT) &= ~(1 << (x))
    #define SET_ALL(x)       (x##_PORT) = 0xFF
    #define RESET_ALL(x)     (x##_PORT) = 0 

    #define OUTPUT(x)        (x##_DDR) |= (1 << (x)) 
    #define INPUT(x)         (x##_DDR) &= ~(1 << (x))
    #define OUTPUT_ALL(x)    (x##_DDR) = 0xFF 
    #define INPUT_ALL(x)     (x##_DDR) = 0
  
  /******************************************************************************
  * Constants
  ******************************************************************************/

  #ifndef D4DLCD_INIT_CS
    #ifndef D4DLCD_CS
    	#error "The Macro D4DLCD_CS must be set in LCD low level driver configuration file." 
    #else
    	#define D4DLCD_INIT_CS OUTPUT(D4DLCD_CS);
    #endif	
  #endif
  
  #ifndef D4DLCD_ASSERT_CS
  	#ifndef D4DLCD_CS
    	#error "The Macro D4DLCD_CS must be set in LCD low level driver configuration file." 
    #else
    	#define D4DLCD_ASSERT_CS RESET(D4DLCD_CS);
    #endif	
  #endif  
  
  #ifndef D4DLCD_DEASSERT_CS
    #ifndef D4DLCD_CS
    	#error "The Macro D4DLCD_CS must be set in LCD low level driver configuration file." 
    #else
    	#define D4DLCD_DEASSERT_CS SET(D4DLCD_CS);
    #endif	
  #endif  
    
  #ifndef D4DLCD_INIT_DC
    #ifndef D4DLCD_DC
    	#error "The Macro D4DLCD_DC must be set in LCD low level driver configuration file." 
    #else        
    	#define D4DLCD_INIT_DC OUTPUT(D4DLCD_DC);
    #endif	
  #endif
  
  #ifndef D4DLCD_ASSERT_DC
    #ifndef D4DLCD_DC
    	#error "The Macro D4DLCD_DC must be set in LCD low level driver configuration file." 
    #else        
    	#define D4DLCD_ASSERT_DC RESET(D4DLCD_DC);
    #endif	
  #endif  
  
  #ifndef D4DLCD_DEASSERT_DC
    #ifndef D4DLCD_DC
    	#error "The Macro D4DLCD_DC must be set in LCD low level driver configuration file." 
    #else        
    	#define D4DLCD_DEASSERT_DC SET(D4DLCD_DC);
    #endif	
  #endif  

      
  #ifndef D4DLCD_READ_CMD_SUPPORT 
    #define D4DLCD_READ_CMD_SUPPORT D4D_TRUE
  #endif  
      
  #ifndef D4DLCD_INIT_E
    #ifndef D4DLCD_E
    	#error "The Macro D4DLCD_E must be set in LCD low level driver configuration file." 
    #else    
      #define D4DLCD_INIT_E OUTPUT(D4DLCD_E);
    #endif  
  #endif
  
  #ifndef D4DLCD_ASSERT_E
    #ifndef D4DLCD_E
    	#error "The Macro D4DLCD_E must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_ASSERT_E RESET(D4DLCD_E);
    #endif	
  #endif  

  #ifndef D4DLCD_DEASSERT_E
    #ifndef D4DLCD_E
    	#error "The Macro D4DLCD_E must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_DEASSERT_E SET(D4DLCD_E);
    #endif	
  #endif

  
  #ifndef D4DLCD_INIT_RW
    #ifndef D4DLCD_RW
    	#error "The Macro D4DLCD_RW must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_INIT_RW OUTPUT(D4DLCD_RW);
   #endif 	
  #endif

  #ifndef D4DLCD_ASSERT_RW
    #ifndef D4DLCD_RW
    	#error "The Macro D4DLCD_RW must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_ASSERT_RW RESET(D4DLCD_RW);
    #endif	
  #endif  

  #ifndef D4DLCD_DEASSERT_RW
    #ifndef D4DLCD_RW
    	#error "The Macro D4DLCD_RW must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_DEASSERT_RW SET(D4DLCD_RW);
    #endif	
  #endif

  
  #ifndef D4DLCD_INIT_DATA
    #ifndef D4DLCD_DATA
    	#error "The Macro D4DLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_INIT_DATA OUTPUT_ALL(D4DLCD_DATA);
    #endif	
  #endif
  
  #ifndef D4DLCD_DATA_RWITE
    #ifndef D4DLCD_DATA
    	#error "The Macro D4DLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4DLCD_DATA_RWITE(x) D4DLCD_DATA = x;
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
  * Types
  ******************************************************************************/

  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Global variables
  ******************************************************************************/
  
  /******************************************************************************
  * Global functions
  ******************************************************************************/

#endif
#endif /* __D4DRVHW_GPIO6800_8B_H */










