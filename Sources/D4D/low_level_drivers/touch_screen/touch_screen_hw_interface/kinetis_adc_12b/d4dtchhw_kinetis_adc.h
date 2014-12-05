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
* @file      d4dtchhw_kinetis_adc.h
*
* @author    b01800
* 
* @version   0.0.2.0
* 
* @date      Feb-3-2011
* 
* @brief     D4D driver - low level driver of ADC and gpio for resistive touch screen 
*            header file 
*
*******************************************************************************/

#ifndef __D4DTCHHW_KINETIS_ADC_H
#define __D4DTCHHW_KINETIS_ADC_H

  #if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_kinetis_adc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtchhw_kinetis_adc_cfg.h"
    
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
   
    // D4DTCH_X_PLUS_PCR pin PCR settings
    #ifndef D4DTCH_X_PLUS_PCR_VAL
      #define D4DTCH_X_PLUS_PCR_VAL   PORT_PCR_MUX(1)
    #endif
    // D4DTCH_X_MINUS_PCR pin PCR settings
    #ifndef D4DTCH_X_MINUS_PCR_VAL
      #define D4DTCH_X_MINUS_PCR_VAL  PORT_PCR_MUX(1)
    #endif
    // D4DTCH_Y_PLUS_PCR pin PCR settings
    #ifndef D4DTCH_Y_PLUS_PCR_VAL
      #define D4DTCH_Y_PLUS_PCR_VAL   PORT_PCR_MUX(1)
    #endif
    // D4DTCH_Y_MINUS_PCR pin PCR settings
    #ifndef D4DTCH_Y_MINUS_PCR_VAL
      #define D4DTCH_Y_MINUS_PCR_VAL  PORT_PCR_MUX(1)
    #endif
  
  
     #ifndef D4DTCH_INIT_TCH_PADS
        #define D4DTCH_INIT_TCH_PADS  D4DTCH_X_PLUS_PCR =  D4DTCH_X_PLUS_PCR_VAL;\
                                      D4DTCH_X_MINUS_PCR = D4DTCH_X_MINUS_PCR_VAL;\
                                      D4DTCH_Y_PLUS_PCR  = D4DTCH_Y_PLUS_PCR_VAL;\
                                      D4DTCH_Y_MINUS_PCR = D4DTCH_Y_MINUS_PCR_VAL;
     #endif
 
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
    
     /****** Switching pins between GPIO / ADC func *****/
    
    #ifndef D4DTCH_X_PLUS_ADCH_PIN_ENABLE
       #define D4DTCH_X_PLUS_ADCH_PIN_ENABLE  (D4DTCH_X_PLUS_PCR = PORT_PCR_MUX(0)); // Mux ADC  
    #endif

    #ifndef D4DTCH_X_PLUS_ADCH_PIN_DISABLE
       #define D4DTCH_X_PLUS_ADCH_PIN_DISABLE (D4DTCH_X_PLUS_PCR = PORT_PCR_MUX(1)); // Mux GPIO  
    #endif

    #ifndef D4DTCH_Y_PLUS_ADCH_PIN_ENABLE
       #define D4DTCH_Y_PLUS_ADCH_PIN_ENABLE  (D4DTCH_Y_PLUS_PCR = PORT_PCR_MUX(0)); // Mux ADC  
    #endif

    #ifndef D4DTCH_Y_PLUS_ADCH_PIN_DISABLE
       #define D4DTCH_Y_PLUS_ADCH_PIN_DISABLE (D4DTCH_Y_PLUS_PCR = PORT_PCR_MUX(1)); // Mux GPIO  
    #endif

    /****** ADC module selection *****/
    #ifndef D4DTCH_ADC_ID
      #error "The D4DTCH_ADC_ID must be set in Touch Screen low level driver configuration file." 
    #endif

    #if D4DTCH_ADC_ID == 0
    
      #define D4DTCH_ADC_SC1A                                	ADC0_SC1A                                
      #define D4DTCH_ADC_SC1B                               	ADC0_SC1B                               
      #define D4DTCH_ADC_CFG1                                	ADC0_CFG1                                
      #define D4DTCH_ADC_CFG2                                	ADC0_CFG2                                
      #define D4DTCH_ADC_RA                                  	ADC0_RA                                  
      #define D4DTCH_ADC_RB                                 	ADC0_RB                                 
      #define D4DTCH_ADC_CV1                                 	ADC0_CV1                                 
      #define D4DTCH_ADC_CV2                                	ADC0_CV2                                
      #define D4DTCH_ADC_SC2                                	ADC0_SC2                                
      #define D4DTCH_ADC_SC3                                 	ADC0_SC3                                 
      #define D4DTCH_ADC_OFS                                 	ADC0_OFS                                 
      #define D4DTCH_ADC_PG                                 	ADC0_PG                                 
      #define D4DTCH_ADC_MG                                  	ADC0_MG                                  
      #define D4DTCH_ADC_CLPD                                	ADC0_CLPD                                
      #define D4DTCH_ADC_CLPS                                	ADC0_CLPS                                
      #define D4DTCH_ADC_CLP4                                	ADC0_CLP4                                
      #define D4DTCH_ADC_CLP3                               	ADC0_CLP3                               
      #define D4DTCH_ADC_CLP2                                	ADC0_CLP2                                
      #define D4DTCH_ADC_CLP1                               	ADC0_CLP1                               
      #define D4DTCH_ADC_CLP0                                	ADC0_CLP0                                
      #define D4DTCH_ADC_PGA                                 	ADC0_PGA                                 
      #define D4DTCH_ADC_CLMD                               	ADC0_CLMD                               
      #define D4DTCH_ADC_CLMS                               	ADC0_CLMS                               
      #define D4DTCH_ADC_CLM4                                	ADC0_CLM4                                
      #define D4DTCH_ADC_CLM3                                	ADC0_CLM3                                
      #define D4DTCH_ADC_CLM2                               	ADC0_CLM2                               
      #define D4DTCH_ADC_CLM1                               	ADC0_CLM1                               
      #define D4DTCH_ADC_CLM0  	                                ADC0_CLM0     
 
       
    #elif D4DTCH_ADC_ID == 1
      
      #define D4DTCH_ADC_SC1A                                	ADC1_SC1A                                
      #define D4DTCH_ADC_SC1B                               	ADC1_SC1B                               
      #define D4DTCH_ADC_CFG1                                	ADC1_CFG1                                
      #define D4DTCH_ADC_CFG2                                	ADC1_CFG2                                
      #define D4DTCH_ADC_RA                                  	ADC1_RA                                  
      #define D4DTCH_ADC_RB                                 	ADC1_RB                                 
      #define D4DTCH_ADC_CV1                                 	ADC1_CV1                                 
      #define D4DTCH_ADC_CV2                                	ADC1_CV2                                
      #define D4DTCH_ADC_SC2                                	ADC1_SC2                                
      #define D4DTCH_ADC_SC3                                 	ADC1_SC3                                 
      #define D4DTCH_ADC_OFS                                 	ADC1_OFS                                 
      #define D4DTCH_ADC_PG                                 	ADC1_PG                                 
      #define D4DTCH_ADC_MG                                  	ADC1_MG                                  
      #define D4DTCH_ADC_CLPD                                	ADC1_CLPD                                
      #define D4DTCH_ADC_CLPS                                	ADC1_CLPS                                
      #define D4DTCH_ADC_CLP4                                	ADC1_CLP4                                
      #define D4DTCH_ADC_CLP3                               	ADC1_CLP3                               
      #define D4DTCH_ADC_CLP2                                	ADC1_CLP2                                
      #define D4DTCH_ADC_CLP1                               	ADC1_CLP1                               
      #define D4DTCH_ADC_CLP0                                	ADC1_CLP0                                
      #define D4DTCH_ADC_PGA                                 	ADC1_PGA                                 
      #define D4DTCH_ADC_CLMD                               	ADC1_CLMD                               
      #define D4DTCH_ADC_CLMS                               	ADC1_CLMS                               
      #define D4DTCH_ADC_CLM4                                	ADC1_CLM4                                
      #define D4DTCH_ADC_CLM3                                	ADC1_CLM3                                
      #define D4DTCH_ADC_CLM2                               	ADC1_CLM2                               
      #define D4DTCH_ADC_CLM1                               	ADC1_CLM1                               
      #define D4DTCH_ADC_CLM0  	                                ADC1_CLM0  
                 
    #else
      #error "The D4DLCD_SPI_ID is invalid"
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
      #define D4DTCH_X_TOUCH_OFFMAX           (D4DTCH_X_TOUCH_MIN * 4 / 2)
    #endif
   
    #ifndef D4DTCH_Y_TOUCH_OFFMAX
      #define D4DTCH_Y_TOUCH_OFFMAX           (D4DTCH_Y_TOUCH_MIN * 4 / 2)
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
#endif /* __D4DTCHHW_KINETIS_ADC_H */










