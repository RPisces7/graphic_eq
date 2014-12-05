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
* @file      d4dlcdhw_kinetis_spi.h
*
* @author    b01800
* 
* @version   0.0.5.0
* 
* @date      Feb-3-2011
* 
* @brief     D4D driver - kinetis_spi hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4DLCDHW_KINETIS_SPI_H
#define __D4DLCDHW_KINETIS_SPI_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_kinetis_spi_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_kinetis_spi_cfg.h"

    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/

#define SET(x)       (x##_PORT) |= (1 << (x))
 
#define RESET(x)     (x##_PORT) &= ~(1 << (x))

  #define OUTPUT(x)    (x##_DDR) |= (1 << (x)) ; (x##_PCR) = PORT_PCR_MUX(1);
  #define INPUT(x)     (x##_DDR) &= ~(1 << (x)); (x##_PCR) = PORT_PCR_MUX(1);
 
  // MISO pin PCR settings
  #ifndef D4DLCD_SPI_MISO_PCR_VAL
    #define D4DLCD_SPI_MISO_PCR_VAL (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK)
  #endif
  // MOSI pin PCR settings
  #ifndef D4DLCD_SPI_MOSI_PCR_VAL
    #define D4DLCD_SPI_MOSI_PCR_VAL (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK)
  #endif
  // CLK pin PCR settings
  #ifndef D4DLCD_SPI_CLK_PCR_VAL
    #define D4DLCD_SPI_CLK_PCR_VAL  (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK)
  #endif
  // CS pin PCR settings
  #ifndef D4DLCD_SPI_CS_PCR_VAL
    #define D4DLCD_SPI_CS_PCR_VAL   (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK)
  #endif


 #ifndef D4DLCD_SPI_PORT_INIT
    #define D4DLCD_SPI_PORT_INIT  D4DLCD_SPI_MISO_PCR = D4DLCD_SPI_MISO_PCR_VAL;\
                                  D4DLCD_SPI_MOSI_PCR = D4DLCD_SPI_MOSI_PCR_VAL;\
                                  D4DLCD_SPI_CLK_PCR  = D4DLCD_SPI_CLK_PCR_VAL;\
                                  D4DLCD_SPI_CS_PCR   = D4DLCD_SPI_CS_PCR_VAL;
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

  
  // BackLight
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
  
  // Reset
  #ifdef D4DLCD_RESET

    #ifndef D4DLCD_INIT_RESET
    	#define D4DLCD_INIT_RESET OUTPUT(D4DLCD_RESET);
    #endif
    
    #ifndef D4DLCD_ASSERT_RESET
    	#define D4DLCD_ASSERT_RESET RESET(D4DLCD_RESET);
    #endif  
    
    #ifndef D4DLCD_DEASSERT_RESET
      #define D4DLCD_DEASSERT_RESET SET(D4DLCD_RESET);      
    #endif
    
  #endif
  

  #ifndef D4DLCD_SPI_ID
    	#error "The D4DLCD_SPI_ID must be set in LCD low level driver configuration file."   
  #endif  


 #if D4DLCD_SPI_ID == 0
     
      #define D4DLCD_SPI_MCR     SPI0_MCR
      #define D4DLCD_SPI_CTAR0   SPI0_CTAR0
      #define D4DLCD_SPI_RSER    SPI0_RSER
      #define D4DLCD_SPI_SR      SPI0_SR
      #define D4DLCD_SPI_PUSHR   SPI0_PUSHR
           
    #elif D4DLCD_SPI_ID == 1

      #define D4DLCD_SPI_MCR     SPI1_MCR
      #define D4DLCD_SPI_CTAR0   SPI1_CTAR0
      #define D4DLCD_SPI_RSER    SPI1_RSER
      #define D4DLCD_SPI_SR      SPI1_SR
      #define D4DLCD_SPI_PUSHR   SPI1_PUSHR
            
    #elif D4DLCD_SPI_ID == 2
        
      #define D4DLCD_SPI_MCR     SPI2_MCR
      #define D4DLCD_SPI_CTAR0   SPI2_CTAR0
      #define D4DLCD_SPI_RSER    SPI2_RSER
      #define D4DLCD_SPI_SR      SPI2_SR
      #define D4DLCD_SPI_PUSHR   SPI2_PUSHR
 
 #else
    #error "The D4DLCD_SPI_ID is invalid" 
 #endif





    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DLCDHW_KINETIS_SPI_H */










