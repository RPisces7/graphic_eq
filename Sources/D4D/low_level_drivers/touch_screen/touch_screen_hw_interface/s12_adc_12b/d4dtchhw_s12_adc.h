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
* @file      d4dtchhw_s12_adc.h
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Jun-29-2010
* 
* @brief     D4D driver - low level driver of ADC and gpio for resistive touch screen 
*            header file 
*
*******************************************************************************/

#ifndef __D4DTCHHW_S12_ADC_H
#define __D4DTCHHW_S12_ADC_H

  #if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_s12_adc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtchhw_s12_adc_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
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
    
    /****** X PLUS signal macros ******/
    
    #ifndef D4DTCH_INIT_X_PLUS
      #ifndef D4DTCH_X_PLUS
      	#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_INIT_X_PLUS OUTPUT(D4DTCH_X_PLUS); RESET(D4DTCH_X_PLUS); 
      #endif	
    #endif
    
    #ifndef D4DTCH_RESET_X_PLUS
    	#ifndef D4DTCH_X_PLUS
      	#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_RESET_X_PLUS RESET(D4DTCH_X_PLUS);
      #endif	
    #endif  
    
    #ifndef D4DTCH_SET_X_PLUS
      #ifndef D4DTCH_X_PLUS
      	#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_SET_X_PLUS SET(D4DTCH_X_PLUS);
      #endif	
    #endif
    
    /****** X MINUS signal macros ******/
    
    #ifndef D4DTCH_INIT_X_MINUS
      #ifndef D4DTCH_X_MINUS
      	#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_INIT_X_MINUS OUTPUT(D4DTCH_X_MINUS); RESET(D4DTCH_X_MINUS); 
      #endif	
    #endif
    
    #ifndef D4DTCH_RESET_X_MINUS
    	#ifndef D4DTCH_X_MINUS
      	#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_RESET_X_MINUS RESET(D4DTCH_X_MINUS);
      #endif	
    #endif  
    
    #ifndef D4DTCH_SET_X_MINUS
      #ifndef D4DTCH_X_MINUS
      	#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_SET_X_MINUS SET(D4DTCH_X_MINUS);
      #endif	
    #endif
    
    #ifndef D4DTCH_X_MINUS_HIGH_Z_ENABLE
      #ifndef D4DTCH_X_MINUS
      	#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_X_MINUS_HIGH_Z_ENABLE INPUT(D4DTCH_X_MINUS);
      #endif	
    #endif
    
    #ifndef D4DTCH_X_MINUS_HIGH_Z_DISABLE
      #ifndef D4DTCH_X_MINUS
      	#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_X_MINUS_HIGH_Z_DISABLE OUTPUT(D4DTCH_X_MINUS);
      #endif	
    #endif
    
    /****** Y PLUS signal macros ******/
    
    #ifndef D4DTCH_INIT_Y_PLUS
      #ifndef D4DTCH_Y_PLUS
      	#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_INIT_Y_PLUS OUTPUT(D4DTCH_Y_PLUS); RESET(D4DTCH_Y_PLUS); 
      #endif	
    #endif
    
    #ifndef D4DTCH_RESET_Y_PLUS
    	#ifndef D4DTCH_Y_PLUS
      	#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_RESET_Y_PLUS RESET(D4DTCH_Y_PLUS);
      #endif	
    #endif  
    
    #ifndef D4DTCH_SET_Y_PLUS
      #ifndef D4DTCH_Y_PLUS
      	#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_SET_Y_PLUS SET(D4DTCH_Y_PLUS);
      #endif	
    #endif
    
    /****** Y MINUS signal macros ******/
    
    #ifndef D4DTCH_INIT_Y_MINUS
      #ifndef D4DTCH_Y_MINUS
      	#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_INIT_Y_MINUS OUTPUT(D4DTCH_Y_MINUS); RESET(D4DTCH_Y_MINUS); 
      #endif	
    #endif
    
    #ifndef D4DTCH_RESET_Y_MINUS
    	#ifndef D4DTCH_Y_MINUS
      	#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_RESET_Y_MINUS RESET(D4DTCH_Y_MINUS);
      #endif	
    #endif  
    
    #ifndef D4DTCH_SET_Y_MINUS
      #ifndef D4DTCH_Y_MINUS
      	#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_SET_Y_MINUS SET(D4DTCH_Y_MINUS);
      #endif	
    #endif
    
    #ifndef D4DTCH_Y_MINUS_HIGH_Z_ENABLE
      #ifndef D4DTCH_Y_MINUS
      	#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_Y_MINUS_HIGH_Z_ENABLE INPUT(D4DTCH_Y_MINUS);
      #endif	
    #endif
    
    #ifndef D4DTCH_Y_MINUS_HIGH_Z_DISABLE
      #ifndef D4DTCH_Y_MINUS
      	#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file." 
      #else
      	#define D4DTCH_Y_MINUS_HIGH_Z_DISABLE OUTPUT(D4DTCH_Y_MINUS);
      #endif	
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
      #define D4DTCH_X_TOUCH_OFFMAX              (D4DTCH_X_TOUCH_MIN * 4 / 2)
    #endif
   
    #ifndef D4DTCH_Y_TOUCH_OFFMAX
      #define D4DTCH_Y_TOUCH_OFFMAX              (D4DTCH_Y_TOUCH_MIN * 4 / 2)
    #endif
   
    #ifndef D4DTCH_SAMPLE_MARGIN
      #define D4DTCH_SAMPLE_MARGIN            (D4DTCH_FULL_SCALE / 256)
    #endif
   
    // b01800
    #define D4DTCH_ADC_HW_ID 1 // USE ATD0 module
    
    #if D4DTCH_ADC_HW_ID == 0
    
      #define D4DTCH_ATDCTL0  ATD0CTL0
      #define D4DTCH_ATDCTL1  ATD0CTL1
      #define D4DTCH_ATDCTL2  ATD0CTL2
      #define D4DTCH_ATDCTL3  ATD0CTL3
      #define D4DTCH_ATDCTL4  ATD0CTL4
      #define D4DTCH_ATDCTL5  ATD0CTL5
      #define D4DTCH_ATDSTAT0  ATD0STAT0
      #define D4DTCH_ATDDR0  ATD0DR0
      #define D4DTCH_ATDDR1  ATD0DR1
      #define D4DTCH_ATDDR2  ATD0DR2
      #define D4DTCH_ATDDR3  ATD0DR3
      #define D4DTCH_ATDSTAT0_SCF ATD0STAT0_SCF
            
    #elif D4DTCH_ADC_HW_ID == 1 // USE ATD1 module
    
      #define D4DTCH_ATDCTL0  ATD1CTL0
      #define D4DTCH_ATDCTL1  ATD1CTL1
      #define D4DTCH_ATDCTL2  ATD1CTL2
      #define D4DTCH_ATDCTL3  ATD1CTL3
      #define D4DTCH_ATDCTL4  ATD1CTL4
      #define D4DTCH_ATDCTL5  ATD1CTL5
      #define D4DTCH_ATDSTAT0  ATD1STAT0
      #define D4DTCH_ATDDR0  ATD1DR0
      #define D4DTCH_ATDDR1  ATD1DR1
      #define D4DTCH_ATDDR2  ATD1DR2
      #define D4DTCH_ATDDR3  ATD1DR3
      #define D4DTCH_ATDSTAT0_SCF ATD1STAT0_SCF
    
    #endif

   
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DTCHHW_S12_ADC_H */










