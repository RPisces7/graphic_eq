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
* @file      d4dtchhw_template.c
*
* @author    b01119
* 
* @version   0.0.4.0
* 
* @date      May-7-2010
* 
* @brief     D4D driver - template hardware touch screen driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCHHW_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtchhw_template_ID 1


// copilation enable preprocessor condition
// the string d4dtch_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_template_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dtchhw_template.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DTCHHW_Init_Template(void);
  static unsigned char D4DTCHHW_DeInit_Template(void);
  static unsigned short D4DTCHHW_ReadTouchAxis_Template(D4DTCHHW_PINS pinId);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_Template(void);
  static unsigned char D4DTCHHW_PinCtl_Template(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCHHW_FUNCTIONS d4dtchhw_template =
  {
    D4DTCHHW_Init_Template,
    D4DTCHHW_ReadTouchAxis_Template,
    D4DTCHHW_GetRawLimits_Template,
    D4DTCHHW_PinCtl_Template,
    D4DTCHHW_DeInit_Template
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
  // FUNCTION:    D4DTCHHW_Init_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this hw low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCHHW_Init_Template(void)
  {
    
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_DeInit_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this hw low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCHHW_DeInit_Template(void)
  {
    
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_ReadTouchAxis_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used to read one RAW nonfiltered value of the 
  //              touch screen signal
  // PARAMETERS:  pinId - the signal to read
  //              
  // RETURNS:     readed value of the specified signal
  //               
  //-----------------------------------------------------------------------------  
  static unsigned short D4DTCHHW_ReadTouchAxis_Template(D4DTCHHW_PINS pinId)
  {
    
  }    
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_GetRawLimits_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits 
  //              structure. 
  // PARAMETERS:  none
  //              
  // RETURNS:     pointer on touch screen raw limit structure
  //               
  //----------------------------------------------------------------------------- 
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_Template(void)
  {
    
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_PinCtl_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for touch screen purposes
  // 
  // PARAMETERS:  D4DTCHHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCHHW_PinCtl_Template(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState)
  {       

  }

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtchhw_template_ID)