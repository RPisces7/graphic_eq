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
* @file      d4dtch_resistive.c
*
* @author    b01119
* 
* @version   0.0.16.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D driver - resistive touch screen driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_resistive_ID 1


// copilation enable preprocessor condition
// the string d4dtch_resistive_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_resistive_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dtch_resistive.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DTCH_Init_Resistive(void);
  static unsigned char D4DTCH_DeInit_Resistive(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Resistive(void);
  static unsigned char D4DTCH_GetPositionRaw_Resistive(unsigned short *TouchPositionX, 
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_resistive = 
  {
     D4DTCH_Init_Resistive,
     D4DTCH_GetPositionRaw_Resistive,
     D4DTCH_GetRawLimits_Resistive,
     D4DTCH_DeInit_Resistive
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
   static unsigned char TchScr_GetFilteredAxis(D4DTCHHW_PINS pinId, unsigned short *Res)
  {
    unsigned short wSample[AVERAGE_BUFF_LEN];
    unsigned short wRes, wSum;
    unsigned short wADCReading;
    unsigned short wMaxLimit;
    unsigned char tmp, tmp_res;    
    int cnt;  
    unsigned char sample_margin;
    D4D_TOUCHSCREEN_LIMITS* p_limits = D4D_LLD_TCH_HW.D4DTCHHW_GetRawLimits();
    
    
    sample_margin = (unsigned char)(p_limits->rawDataScale / 256);
    wMaxLimit = (unsigned short)(p_limits->rawDataScale - sample_margin);  
    
    // clear all local variables and buffer 
    for(tmp = 0; tmp < AVERAGE_BUFF_LEN;tmp++)
      wSample[tmp] = 0;
    wSum = 0;
    cnt = 0;
    
    
    while (cnt < 0x50)
    {
      cnt++;
      // Read ADC value
      wADCReading = D4D_LLD_TCH_HW.D4DTCHHW_ReadTouchAxis(pinId);
      
      // check if input value is under minimal value
      if(wADCReading < p_limits->touchMinX)
        break;
      
      // check if input value is above maximal value
      if(wADCReading > wMaxLimit)
      {
      #if(D4D_MCU_TYPE != D4D_MK)
        asm (nop);
      #endif
        break;
      }
      // Compute the current output value
      wSum += wADCReading;
      wSum -= wSample[AVERAGE_BUFF_LEN - 1];
      
      // shift all values in input buffer
      for(tmp = (AVERAGE_BUFF_LEN - 1); tmp > 0 ;tmp--)
      {
        wSample[tmp] = wSample[tmp - 1]; 
      }
      
      // put new value into buffer
      wSample[0] = wADCReading;
      
      // compute current result from all bufer values
      wRes = (unsigned short)(wSum / AVERAGE_BUFF_LEN);
  
      // if buffer is full
      if(cnt > AVERAGE_BUFF_LEN) 
      {
        // Check all input samples if are in allowed range 
        tmp_res = 0;       
        for(tmp = 0; tmp < AVERAGE_BUFF_LEN ;tmp++)
        {
          if((wRes > (wSample[tmp] - sample_margin)) && (wRes < (wSample[tmp] + sample_margin)))
          {
            tmp_res++;            
          }
        }
        
        // If most of sample are in allowed range with output value, assert this sample as result
        if(tmp_res >= (AVERAGE_BUFF_LEN - 2)) {            
          *Res = wRes;
          return 1;
        }
      }
    }
    
    return 0;  
  }

   
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_Init_Resistive
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCH_Init_Resistive(void)
  {
    if(D4D_LLD_TCH_HW.D4DTCHHW_Init == NULL)
      return 0;
    if(D4D_LLD_TCH_HW.D4DTCHHW_ReadTouchAxis == NULL)
      return 0;
    if(D4D_LLD_TCH_HW.D4DTCHHW_GetRawLimits == NULL)
      return 0;
    if(D4D_LLD_TCH_HW.D4DTCHHW_PinCtl == NULL)
      return 0;
    if(D4D_LLD_TCH_HW.D4DTCHHW_DeInit == NULL)
      return 0;
        
    return D4D_LLD_TCH_HW.D4DTCHHW_Init();  
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_DeInit_Resistive
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCH_DeInit_Resistive(void)
  {
    return 0;  
  }    
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetRawLimits_Resistive
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits 
  //              structure. 
  // PARAMETERS:  none
  //              
  // RETURNS:     pointer on touch screen raw limit structure
  //               
  //----------------------------------------------------------------------------- 
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Resistive(void)
  {
    return D4D_LLD_TCH_HW.D4DTCHHW_GetRawLimits();  
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetPositionRaw_Resistive
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y 
  //              coordinates if screen touched 
  // PARAMETERS:  unsigned short *TouchPositionX    Pointer to X coordinate
  //              unsigned short *TouchPositionY    Pointer to Y ccordinate
  // RETURNS:     0   no screen touch
  //              1   screen touch, 
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_GetPositionRaw_Resistive (unsigned short *TouchPositionX, 
                                         unsigned short *TouchPositionY)
  {       
    // Declare and initialise local variables
    unsigned short tmpRes; 
    unsigned char tmpCnt; 
    unsigned char bSampleComplete = 0;


    // Switch on ADC channel on Y+ wire
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_PLUS_PIN, D4DHW_PIN_ADC_ON);
    tmpCnt = 0;

    // to check that the touch screen surface is without any voltage and prepared
    while((D4D_LLD_TCH_HW.D4DTCHHW_ReadTouchAxis(D4DTCH_Y_PLUS_PIN) > D4D_LLD_TCH_HW.D4DTCHHW_GetRawLimits()->touchMinY) && (++tmpCnt))
    {
      ;
    }
    // when the touch screen surface is prepared
    // switch on on X+ wire high level 
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_PLUS_PIN, D4DHW_PIN_SET_1);      
    // wait a moment
    #if(D4D_MCU_TYPE != D4D_MK)
      asm (nop);
    #endif

    // and Y- pin put into high Z mode to keep measurent non affected

    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_MINUS_PIN, D4DHW_PIN_IN);
    #if(D4D_MCU_TYPE != D4D_MK)
      asm (nop);
    #endif  

    // read value of Y axis and check if touch screen is touched
    if(TchScr_GetFilteredAxis(D4DTCH_Y_PLUS_PIN, &tmpRes))
    {
      // if it's touched save the result of Y measurement
      *TouchPositionY = tmpRes;
      
      // Disable ADC function on Y+
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_PLUS_PIN, D4DHW_PIN_ADC_OFF);      

      
      // Set pins to default values
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_PLUS_PIN, D4DHW_PIN_SET_0);      
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_MINUS_PIN, D4DHW_PIN_SET_0);      
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_PLUS_PIN, D4DHW_PIN_SET_0);      
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_MINUS_PIN, D4DHW_PIN_SET_0);      

      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_MINUS_PIN, D4DHW_PIN_OUT);      

      // wait a moment
      #if(D4D_MCU_TYPE != D4D_MK)
        asm (nop); 
      #endif

      // Switch on ADC channel on X+ wire
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_PLUS_PIN, D4DHW_PIN_ADC_ON);
       // to check that the touch screen surface is without any voltage and prepared
      tmpCnt = 0;
      while((D4D_LLD_TCH_HW.D4DTCHHW_ReadTouchAxis(D4DTCH_X_PLUS_PIN) > D4D_LLD_TCH_HW.D4DTCHHW_GetRawLimits()->touchMinX) && (++tmpCnt))
      {
        ;
      }
                   

      // when the touch screen surface is prepared
      // switch on on Y+ wire high level         
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_PLUS_PIN, D4DHW_PIN_SET_1);      
      
      // wait a moment
      #if(D4D_MCU_TYPE != D4D_MK)
        asm(nop);
      #endif
      
      // and X- pin put into high Z mode to keep measurent non affected
      (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_MINUS_PIN, D4DHW_PIN_IN);      

      // read value of X axis and check if touch screen is touched
      if(TchScr_GetFilteredAxis(D4DTCH_X_PLUS_PIN, &tmpRes))
      {
        // if it's touched save the result of X measurement
        *TouchPositionX = tmpRes;
        bSampleComplete =  1;
      }
    }
    // Disable ADC function on X+ and Y+
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_PLUS_PIN, D4DHW_PIN_ADC_OFF);      
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_MINUS_PIN, D4DHW_PIN_OUT);      
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_PLUS_PIN, D4DHW_PIN_ADC_OFF);      
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_MINUS_PIN, D4DHW_PIN_OUT);      

    // Put on touch screen pins into default mode 
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_PLUS_PIN, D4DHW_PIN_SET_0);      
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_X_MINUS_PIN, D4DHW_PIN_SET_0);      
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_PLUS_PIN, D4DHW_PIN_SET_0);      
    (void)D4D_LLD_TCH_HW.D4DTCHHW_PinCtl(D4DTCH_Y_MINUS_PIN, D4DHW_PIN_SET_0);      
     
    // return back result - if screen is touched or  not
    return bSampleComplete;
  }

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_resistive_ID)