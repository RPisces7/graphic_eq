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
* @file      d4dtchhw_mcf52259_adc.c
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Jun-29-2010
* 
* @brief     D4D driver - mcf52259_adc hardware touch screen driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCHHW_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtchhw_mcf52259_adc_ID 1


// copilation enable preprocessor condition
// the string d4dtch_mcf52259_adc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_mcf52259_adc_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dtchhw_mcf52259_adc.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DTCHHW_Init_MCF52259_adc(void);
  static unsigned char D4DTCHHW_DeInit_MCF52259_adc(void);
  static unsigned short D4DTCHHW_ReadTouchAxis_MCF52259_adc(D4DTCHHW_PINS pinId);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_MCF52259_adc(void);
  static unsigned char D4DTCHHW_PinCtl_MCF52259_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCHHW_FUNCTIONS d4dtchhw_mcf52259_adc =
  {
    D4DTCHHW_Init_MCF52259_adc,
    D4DTCHHW_ReadTouchAxis_MCF52259_adc,
    D4DTCHHW_GetRawLimits_MCF52259_adc,
    D4DTCHHW_PinCtl_MCF52259_adc,
    D4DTCHHW_DeInit_MCF52259_adc
  };
  
  const D4D_TOUCHSCREEN_LIMITS d4dtchhw_mcf52259_adc_12b_limits = 
  {
    D4DTCH_FULL_SCALE,
    D4DTCH_X_TOUCH_MIN,
    D4DTCH_Y_TOUCH_MIN, 
    D4DTCH_X_TOUCH_OFFMAX,
    D4DTCH_Y_TOUCH_OFFMAX     
  };

  
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

   
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_Init_MCF52259_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this hw low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCHHW_Init_MCF52259_adc(void)
  {
    D4DTCH_INIT_X_PLUS;
    D4DTCH_INIT_X_MINUS;
    D4DTCH_INIT_Y_PLUS;
    D4DTCH_INIT_Y_MINUS;
    
    return 1;    
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_DeInit_MCF52259_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this hw low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCHHW_DeInit_MCF52259_adc(void)
  {
    return 0;  
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_ReadTouchAxis_MCF52259_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used to read one RAW nonfiltered value of the 
  //              touch screen signal
  // PARAMETERS:  pinId - the signal to read
  //              
  // RETURNS:     readed value of the specified signal
  //               
  //-----------------------------------------------------------------------------  
  static unsigned short D4DTCHHW_ReadTouchAxis_MCF52259_adc(D4DTCHHW_PINS pinId)
  {
    short cnt = 0; 
      
    // Configure ADC for low power, 
    // clock divide = input clock, 
    // short sample time and 12-bits
  
  	if(MCF_ADC_ADSTAT & MCF_ADC_ADSTAT_CIP0)
  		return 0;
	  
  
	  MCF_ADC_CTRL1 =  0x0000; // set default setting of ADC
	  MCF_ADC_CTRL2 =  0x0007; // set the fastest speed on 80MHz core frequency
    MCF_ADC_ADSDIS = 0x00FE; // disable all samples without first one
    
    if(pinId == D4DTCH_X_PLUS_PIN)
      MCF_ADC_ADLST1 = D4DTCH_X_PLUS_ADCH;
    else if(pinId == D4DTCH_Y_PLUS_PIN)
      MCF_ADC_ADLST1 = D4DTCH_Y_PLUS_ADCH;
    else
      return 0;

    MCF_ADC_POWER &= ~0x0003;
    
    
    
    MCF_ADC_CTRL1 |=  MCF_ADC_CTRL1_START0; 
    
    while ((!(MCF_ADC_ADSTAT & MCF_ADC_ADSTAT_EOSI0)) && (++cnt))
    {
        ;  
    }
    
    MCF_ADC_ADSTAT |= MCF_ADC_ADSTAT_EOSI0;
    
    if(!cnt)
    	return 0;
    
    cnt = 1;
    while ((!(MCF_ADC_ADSTAT & MCF_ADC_ADSTAT_RDY0)) && (++cnt))
    {
        ;  
    }
    
    
    // if the measurement ends by timeout return 0 in other case result of ADC conversion
    if(cnt)
      return (unsigned short)(MCF_ADC_ADRSLT0 >> 3);    
    else
      return 0;
    
    #if 0
    short cnt = 1; 
    
    // Configure ADC for low power, 
    // clock divide = input clock, 
    // short sample time and 10-bits
    if(ADCSC2_ADACT)
      return 0;
    
      
    ADCCFG = 0x04; // set the ADC to 12 bit mode and short time measuring
    
    if(pinId == D4DTCH_X_PLUS_PIN)
      ADCSC1_ADCH = D4DTCH_X_PLUS_ADCH;
    else if(pinId == D4DTCH_Y_PLUS_PIN)
      ADCSC1_ADCH = D4DTCH_Y_PLUS_ADCH;
    else
      return 0;   
       
   
    // Wait for ADC conversion to complete 
    while ((!ADCSC1_COCO) && (++cnt))
    {
        ;  
    }
    ADCSC1_ADCH = 0b11111; //Turn off ADC 
    
    // if the measurement ends by timeout return 0 in other case result of ADC conversion
    if(cnt)
      return ADCR;    
    else
      return 0;  
    
    #endif
  }    
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_GetRawLimits_MCF52259_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits 
  //              structure. 
  // PARAMETERS:  none
  //              
  // RETURNS:     pointer on touch screen raw limit structure
  //               
  //----------------------------------------------------------------------------- 
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_MCF52259_adc(void)
  {
    return (D4D_TOUCHSCREEN_LIMITS*)&d4dtchhw_mcf52259_adc_12b_limits;  
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_PinCtl_MCF52259_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for touch screen purposes
  // 
  // PARAMETERS:  D4DTCHHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCHHW_PinCtl_MCF52259_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState)
  {       
    switch(pinId)
    {
      case D4DTCH_X_PLUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_X_PLUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_X_PLUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_X_PLUS
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_X_PLUS;
            break;
          case D4DHW_PIN_ADC_ON:
            D4DTCH_X_PLUS_ADCH_PIN_ENABLE;
            break;
          case D4DHW_PIN_ADC_OFF:
            D4DTCH_X_PLUS_ADCH_PIN_DISABLE;
            break;   
        }
        break;
      case D4DTCH_X_MINUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_X_MINUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_X_MINUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_X_MINUS
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_X_MINUS;
            break;         
        }
        break;
      case D4DTCH_Y_PLUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_Y_PLUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_Y_PLUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_Y_PLUS
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_Y_PLUS;
            break;
          case D4DHW_PIN_ADC_ON:
            D4DTCH_Y_PLUS_ADCH_PIN_ENABLE;
            break;
          case D4DHW_PIN_ADC_OFF:
            D4DTCH_Y_PLUS_ADCH_PIN_DISABLE;
            break;   
        }
        break;
      case D4DTCH_Y_MINUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_Y_MINUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_Y_MINUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_Y_MINUS
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_Y_MINUS;
            break;         
        }
        break;
        
    }
    return 0;  
  }

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtchhw_mcf52259_adc_ID)