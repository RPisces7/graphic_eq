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
* @file      d4dlcdhw_template_fb.c
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Jun-29-2010
* 
* @brief     D4D driver - template_fb hardware lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_template_fb_ID 1


// copilation enable preprocessor condition
// the string d4dtch_template_fb_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_template_fb_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dlcdhw_template_fb.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_TemplateFb(void);
  static unsigned char D4DLCDHW_DeInit_TemplateFb(void);
  static unsigned char D4DLCDHW_WriteData_TemplateFb(unsigned long addr, unsigned short value);
  static unsigned short D4DLCDHW_ReadData_TemplateFb(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_TemplateFb(void);
  static unsigned char D4DLCDHW_PinCtl_TemplateFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_TemplateFb(void);
 
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHWFB_FUNCTIONS d4dlcdhw_template_fb = 
  {
    D4DLCDHW_Init_TemplateFb,    
    D4DLCDHW_WriteData_TemplateFb,
    D4DLCDHW_ReadData_TemplateFb,
    D4DLCDHW_PinCtl_TemplateFb,
    D4DLCD_FlushBuffer_TemplateFb,
    D4DLCDHW_DeInit_TemplateFb
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
  // FUNCTION:    D4DLCDHW_Init_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_Init_TemplateFb(void)
  {
    
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_DeInit_TemplateFb(void)
  {
    
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD  
  //
  // PARAMETERS:  unsigned long addr  		address to write data
  //							unsigned short value    variable to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_WriteData_TemplateFb(unsigned long addr, unsigned short value)
  {       
  
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)  
  //
  // PARAMETERS:  unsigned long addr  		address to read data
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadDataWord_TemplateFb(unsigned long addr)
  {       
  
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_GetFbDescriptor_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function return the pointer on filled frame buffer descriptor  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     D4DLCD_FRAMEBUFF_DESC* - pointer on frame buffer descriptor
  //              
  //-----------------------------------------------------------------------------
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_TemplateFb(void)
  {
  	
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_TemplateFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {       

  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_TemplateFb
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_TemplateFb(void)
  {       

  }

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_template_fb_ID)