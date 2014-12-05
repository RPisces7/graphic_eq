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
* @file      d4dlcdhw_swspi_16b.c
*
* @author    b01119
* 
* @version   0.0.2.0
* 
* @date      May-7-2010
* 
* @brief     D4D driver - swspi_16b hardware lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_swspi_16b_ID 1


// copilation enable preprocessor condition
// the string d4dtch_swSpi_16b_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_swspi_16b_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dlcdhw_swspi_16b.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_SwSpi_16b(void);
  static unsigned char D4DLCDHW_DeInit_SwSpi_16b(void);
  static unsigned char D4DLCDHW_SendDataWord_SwSpi_16b(unsigned short value);
  static unsigned char D4DLCDHW_SendCmdWord_SwSpi_16b(unsigned short cmd);
  static unsigned short D4DLCDHW_ReadDataWord_SwSpi_16b(void);
  static unsigned short D4DLCDHW_ReadCmdWord_SwSpi_16b(void);
  static unsigned char D4DLCDHW_PinCtl_SwSpi_16b(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_SwSpi_16b(void);
 
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHW_FUNCTIONS d4dlcdhw_swspi_16b = 
  {
    D4DLCDHW_Init_SwSpi_16b,    
    D4DLCDHW_SendDataWord_SwSpi_16b,
    D4DLCDHW_SendCmdWord_SwSpi_16b,
    D4DLCDHW_ReadDataWord_SwSpi_16b,
    D4DLCDHW_ReadCmdWord_SwSpi_16b,
    D4DLCDHW_PinCtl_SwSpi_16b,
    D4DLCD_FlushBuffer_SwSpi_16b,
    D4DLCDHW_DeInit_SwSpi_16b
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
  // FUNCTION:    D4DLCDHW_Init_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_Init_SwSpi_16b(void)
  {
    #ifdef D4DLCD_DISPLAY_MCU_USER_INIT
      D4DLCD_DISPLAY_MCU_USER_INIT
    #endif

    D4DLCD_ASSERT_CS;
    D4DLCD_ASSERT_DC;

    D4DLCD_INIT_CS;
    D4DLCD_INIT_DC;    
    
 
    D4DLCD_INIT_SPICLK;  
    D4DLCD_INIT_SPIDATA;  
    
    return 1;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_DeInit_SwSpi_16b(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD  
  //
  // PARAMETERS:  unsigned short value    variable to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_SendDataWord_SwSpi_16b(unsigned short value)
  {
    int Bit;  

      D4DLCD_ASSERT_CS;
      	asm     // 2 nops are KO, 3 nops are OK for QE DEMO, we go for 6 RE-CHECK [PL]
      {
          nop
          nop
          nop
          nop        
      };  
  		for (Bit=15; Bit>=0; --Bit)	// 
  		{ 
  			D4DLCD_ASSERT_SPICLK					// Clock := 0 
  		
  		  if (value & (1 << Bit))		// Bit testen 
  				D4DLCD_DEASSERT_SPIDATA				// falls das Bit 1 ist 
  			else 
  				D4DLCD_ASSERT_SPIDATA			// falls das Bit 0 ist 
  		
  			D4DLCD_DEASSERT_SPICLK					// Clock := 1 
  		} 
  		
  		asm     // 2 nops are KO, 3 nops are OK for QE DEMO, we go for 6 RE-CHECK [PL]
      {
          nop
          nop
          nop
          nop         
      };  	     
  	  D4DLCD_DEASSERT_CS;
  	  
  	  return 1;         
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendCmdWord_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit command into LCD  
  //
  // PARAMETERS:  unsigned short cmd    command to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_SendCmdWord_SwSpi_16b(unsigned short cmd)
  {       
    D4DLCD_ASSERT_DC;                        // DataCmd := 0 
    D4DLCDHW_SendDataWord_SwSpi_16b(cmd);
    D4DLCD_DEASSERT_DC;                      // DataCmd := 1 
    
    return 1;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadDataWord_SwSpi_16b(void)
  {       
    return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadCmdWord_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadCmdWord_SwSpi_16b(void)
  {       
    return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_SwSpi_16b(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {       
    switch(pinId)
    {
      case D4DLCD_RESET_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DLCD_RESET);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DLCD_RESET);
            break;
          case D4DHW_PIN_SET_1:
            D4DLCD_DEASSERT_RESET
            break;
          case D4DHW_PIN_SET_0:
            D4DLCD_ASSERT_RESET;
            break;           
        }
        break;
      case D4DLCD_BACKLIGHT_PIN:
        switch(setState)
        {
          
          #ifdef D4DLCD_BACKLIGHT
          
          case D4DHW_PIN_OUT:
            OUTPUT(D4DLCD_BACKLIGHT);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DLCD_BACKLIGHT);
            break;
          case D4DHW_PIN_SET_1:
            D4DLCD_DEASSERT_BACKLIGHT
            break;
          case D4DHW_PIN_SET_0:
            D4DLCD_ASSERT_BACKLIGHT;
            break; 
         
         #endif   
                    
        }
        break;    
    }
    
    return 1;   
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_SwSpi_16b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_SwSpi_16b(void)
  {       
    
  }

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_swspi_16b_ID)