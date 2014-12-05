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
* @file      d4dtchhw_s12_adc.c
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Jun-29-2010
* 
* @brief     D4D driver - low level driver of ADC and gpio for resistive touch screen 
*            source file 
*
******************************************************************************/
#include "d4d.h"
#include "d4d_lldapi.h"
#include "d4d_private.h"


// define the identification string of this low level driver
#define d4dtchhw_s12_adc_ID 1

#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_s12_adc_ID)

  #include "d4dtchhw_s12_adc.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  static unsigned char D4DTCHHW_Init_s12_adc(void);
  static unsigned char D4DTCHHW_DeInit_s12_adc(void);
  static unsigned short D4DTCHHW_ReadTouchAxis_s12_adc(D4DTCHHW_PINS pinId);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_s12_adc(void);
  static unsigned char D4DTCHHW_PinCtl_s12_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  const D4DTCHHW_FUNCTIONS d4dtchhw_s12_adc =
  {
    D4DTCHHW_Init_s12_adc,
    D4DTCHHW_ReadTouchAxis_s12_adc,
    D4DTCHHW_GetRawLimits_s12_adc,
    D4DTCHHW_PinCtl_s12_adc,
    D4DTCHHW_DeInit_s12_adc
  };
  
  const D4D_TOUCHSCREEN_LIMITS d4dtchhw_s12_adc_12b_limits = 
  {
    D4DTCH_FULL_SCALE,
    D4DTCH_X_TOUCH_MIN,
    D4DTCH_Y_TOUCH_MIN, 
    D4DTCH_X_TOUCH_OFFMAX,
    D4DTCH_Y_TOUCH_OFFMAX     
  };

  /**************************************************************//*!
  *
  * Initialize
  *
  ******************************************************************/

  static unsigned char D4DTCHHW_Init_s12_adc(void)
  {
    D4DTCH_INIT_X_PLUS;
    D4DTCH_INIT_X_MINUS;
    D4DTCH_INIT_Y_PLUS;
    D4DTCH_INIT_Y_MINUS;
    
    return 1;
  } 
  
    //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_DeInit_s12_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this hw low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCHHW_DeInit_s12_adc(void)
  {
    return 0;  
  }                  
  
  /**************************************************************//*!
  *
  * HCS12 family supporting new 12bit ATD module
  * Supported families: S12XE,XS,S12P,S12HY,S12HA,S12G
  *
  * Legacy HCS12 family (S12ATD10Bx module not supported)
  *
  ******************************************************************/
  static unsigned short D4DTCHHW_ReadTouchAxis_s12_adc(D4DTCHHW_PINS pinId)
  {
     short cnt = 1;
      unsigned short advalue=0;
      //unsigned short adres0,adres1,adres2,adres3;// Adresults
             
      // clock divide = max. 8.3MHz @50MHz BUS
      if(D4DTCH_ATDSTAT0_SCF) // b01800
        return 0;
           
      D4DTCH_ATDCTL1 = 0x40;// 12bit  
      //D4DTCH_ATDCTL2 = 0x40;// Fast flag clear (AFFC=1)
      D4DTCH_ATDCTL3 = 0xA3;// 4conv.(S4C=1),right justified (DJM=1), Freeze in debug
      D4DTCH_ATDCTL4 = 0x02;// Fatd = Fbus/6
      
      if(pinId == D4DTCH_X_PLUS_PIN)
        D4DTCH_ATDCTL5 = D4DTCH_X_PLUS_ADCH;
      
      else if(pinId == D4DTCH_Y_PLUS_PIN)
        D4DTCH_ATDCTL5 = D4DTCH_Y_PLUS_ADCH;
      else
        return 0;   
      
      //D4DTCH_ATDCTL5 = AdcChannel & 0x0F;// Set channel and run single sequence
              
      // Wait for ADC conversion to complete 
      while ((!D4DTCH_ATDSTAT0_SCF) && (++cnt)) // loop w timeout
      {
          ;  
      }     
      
      // if the measurement ends by timeout return 0 in other case result of ADC conversion
      if(cnt)
        {  //Average of four results
           advalue = (unsigned short)((D4DTCH_ATDDR0+D4DTCH_ATDDR1+D4DTCH_ATDDR2+D4DTCH_ATDDR3)/4);
           D4DTCH_ATDSTAT0 = 0x80; // clear Flag manually 
           return(advalue);
        }
      else
        return 0;   
  }
  
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_s12_adc(void)
  {
    return (D4D_TOUCHSCREEN_LIMITS*)&d4dtchhw_s12_adc_12b_limits;
  }
  
  static unsigned char D4DTCHHW_PinCtl_s12_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState)
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
  
#endif