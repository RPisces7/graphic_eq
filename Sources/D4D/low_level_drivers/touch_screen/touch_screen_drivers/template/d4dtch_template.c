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
* @file      d4dtch_template.c
*
* @author    b01119
* 
* @version   0.0.4.0
* 
* @date      May-11-2010
* 
* @brief     D4D driver - template touch screen driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_template_ID 1


// copilation enable preprocessor condition
// the string d4dtch_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_template_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dtch_template.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DTCH_Init_Template(void);
  static unsigned char D4DTCH_DeInit_Template(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Template(void);
  static unsigned char D4DTCH_GetPositionRaw_Template(unsigned short *TouchPositionX, 
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_template = 
  {
     D4DTCH_Init_Template,
     D4DTCH_GetPositionRaw_Template,
     D4DTCH_GetRawLimits_Template,
     D4DTCH_DeInit_Template
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
  // FUNCTION:    D4DTCH_Init_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCH_Init_Template(void)
  {
    
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_DeInit_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DTCH_DeInit_Template(void)
  {
    
  }    
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetRawLimits_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits 
  //              structure. 
  // PARAMETERS:  none
  //              
  // RETURNS:     pointer on touch screen raw limit structure
  //               
  //----------------------------------------------------------------------------- 
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Template(void)
  {
    
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetPositionRaw_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y 
  //              coordinates if screen touched 
  // PARAMETERS:  unsigned short *TouchPositionX    Pointer to X coordinate
  //              unsigned short *TouchPositionY    Pointer to Y ccordinate
  // RETURNS:     0   no screen touch
  //              1   screen touch, 
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_GetPositionRaw_Template (unsigned short *TouchPositionX, 
                                         unsigned short *TouchPositionY)
  {       
     (void)TouchPositionX;
     (void)TouchPositionY;
     
     
     return 0;
  }

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_template_ID)