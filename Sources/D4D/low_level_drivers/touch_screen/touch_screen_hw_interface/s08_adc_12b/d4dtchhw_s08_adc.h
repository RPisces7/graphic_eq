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
* @file      d4dtchhw_s08_adc.h
*
* @author    b01119
* 
* @version   0.0.3.0
* 
* @date      Jul-27-2010
* 
* @brief     D4D driver - low level driver of ADC and gpio for resistive touch screen 
*            header file 
*
*******************************************************************************/

#ifndef __D4DTCHHW_S08_ADC_H
#define __D4DTCHHW_S08_ADC_H

  #if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_s08_adc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtchhw_s08_adc_cfg.h"
    
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
    
    
    #ifndef D4D_TCHHW_ADCH
      #define D4D_TCHHW_ADCH           ADCSC1_ADCH
    #endif 
    
    #ifndef D4D_TCHHW_ADCCFG
      #define D4D_TCHHW_ADCCFG         ADCCFG
    #endif 
    
    #ifndef D4D_TCHHW_ADCR
      #define D4D_TCHHW_ADCR           ADCR
    #endif 
    
    #ifndef D4D_TCHHW_ADCSC1_COCO
      #define D4D_TCHHW_ADCSC1_COCO           ADCSC1_COCO
    #endif 
    
    #ifndef D4D_TCHHW_ADCSC2_ADACT
      #define D4D_TCHHW_ADCSC2_ADACT           ADCSC2_ADACT
    #endif 
    
    #ifndef D4D_TCHHW_ADC_INIT
      #define D4D_TCHHW_ADC_INIT           D4D_TCHHW_ADCCFG = 0x04;
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
   
   
   
   
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DTCHHW_S08_ADC_H */










