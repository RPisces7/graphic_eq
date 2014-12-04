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
* @file      d4dlcd_template.c
*
* @author    b01119
* 
* @version   0.0.4.0
* 
* @date      May-11-2010
* 
* @brief     D4D driver - template lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_template_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_template_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dlcd_template.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DLCD_Init_Template(void);
  static unsigned char D4DLCD_SetWindow_Template(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_Template(D4DLCD_ORIENTATION new_orientation);
  static unsigned char D4DLCD_Send_PixelColor_Template(unsigned short value) ;
  static unsigned short D4DLCD_Read_Word_Template(void);
  static void D4DLCD_Flush_Template(void);
  static void D4DLCD_Delay_ms_Template(unsigned short period);
  static unsigned char D4DLCD_DeInit_Template(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_template = 
  {
    D4DLCD_Init_Template,    
    D4DLCD_SetWindow_Template,
    D4DLCD_SetOrientation_Template,
    D4DLCD_Send_PixelColor_Template,
    D4DLCD_Read_Word_Template,
    D4DLCD_Flush_Template,
    D4DLCD_Delay_ms_Template,
    D4DLCD_DeInit_Template,
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
  // FUNCTION:    D4DLCD_Init_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCD_Init_Template(void)
  {
    
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------  
  static unsigned char D4DLCD_DeInit_Template(void)
  {
    
  }    
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function sets the logic window in memory of LCD driver
  // 
  // PARAMETERS:  unsigned short x1 - left cordination of logic window
  //              unsigned short y1 - top cordination of logic window
  //              unsigned short x2 - right cordination of logic window
  //              unsigned short y2 - bottom cordination of logic window
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4DLCD_SetWindow_Template(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
    
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD  
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed                
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_Template(D4DLCD_ORIENTATION new_orientation)
  {       
  
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD  
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed                
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Send_PixelColor_Template(unsigned short value)
  {       
  
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Read_Word_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCD_Read_Word_Template(void)
  {       
  
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_Template(void)
  {       

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Delay_ms_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For do some small delays in ms
  //
  // PARAMETERS:  period - count of ms
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Delay_ms_Template(unsigned short period)
  {       

  }


#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_template_ID)