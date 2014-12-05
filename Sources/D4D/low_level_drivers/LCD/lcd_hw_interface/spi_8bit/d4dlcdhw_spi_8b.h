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
* @file      d4dlcdhw_spi_8b.h
*
* @author    b01119
* 
* @version   0.0.3.0
* 
* @date      Apr-11-2011
* 
* @brief     D4D driver SSD1289 hardware level LCD header file 
*
*******************************************************************************/

#ifndef __D4DRVHW_SPI_8B_H
#define __D4DRVHW_SPI_8B_H

#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_spi_8b_ID)


  /******************************************************************************
  * Includes
  ******************************************************************************/
  #include "d4dlcdhw_spi_8b_cfg.h"
  
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
    #define D4DLCD_READ_CMD_SUPPORT D4D_FALSE
  #endif  
      
  #if D4DLCD_COMM_HW_ID == 0 
    #define D4DLCD_SPIBR 				SPIBR
    #define D4DLCD_SPIC1 				SPIC1
    #define D4DLCD_SPIC2 				SPIC2
    #define D4DLCD_SPID 				SPID
    #define D4DLCD_SPIS 				SPIS
    #define D4DLCD_SPIS_SPTEF 	SPIS_SPTEF
    #define D4DLCD_SPIS_SPRF 	  SPIS_SPRF
  #elif D4DLCD_COMM_HW_ID == 1
    #define D4DLCD_SPIBR 				SPI1BR               
    #define D4DLCD_SPIC1 				SPI1C1
    #define D4DLCD_SPIC2 				SPI1C2
    #define D4DLCD_SPID 				SPI1D
    #define D4DLCD_SPIS 				SPI1S
    #define D4DLCD_SPIS_SPTEF 	SPI1S_SPTEF      
    #define D4DLCD_SPIS_SPRF 	  SPI1S_SPRF
  #elif D4DLCD_COMM_HW_ID == 2
    #define D4DLCD_SPIBR 				SPI2BR
    #define D4DLCD_SPIC1 				SPI2C1
    #define D4DLCD_SPIC2 				SPI2C2
    #define D4DLCD_SPID 				SPI2D
    #define D4DLCD_SPIS 				SPI2S
    #define D4DLCD_SPIS_SPTEF 	SPI2S_SPTEF
    #define D4DLCD_SPIS_SPRF 	  SPI2S_SPRF
  #elif D4DLCD_COMM_HW_ID == 3
    #define D4DLCD_SPIBR 				SPI3BR
    #define D4DLCD_SPIC1 				SPI3C1
    #define D4DLCD_SPIC2 				SPI3C2
    #define D4DLCD_SPID 				SPI3D
    #define D4DLCD_SPIS 				SPI3S
    #define D4DLCD_SPIS_SPTEF 	SPI3S_SPTEF
    #define D4DLCD_SPIS_SPRF 	  SPI3S_SPRF
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
#endif /* __D4DRVHW_SPI_8B_H */










