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
* @file      d4dlcdhw_kinetis_spi.c
*
* @author    b01800
* 
* @version   0.0.5.0
* 
* @date      Mar-11-2011
* 
* @brief     D4D driver - kinetis_spi hardware lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_kinetis_spi_ID 1


// copilation enable preprocessor condition
// the string d4dtch_kinetis_spi_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_kinetis_spi_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dlcdhw_kinetis_spi.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/
  // Macros moved to header file
  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_Kinetis_Spi(void);
  static unsigned char D4DLCDHW_DeInit_Kinetis_Spi(void);
  static unsigned char D4DLCDHW_SendDataWord_Kinetis_Spi(unsigned short value);
  static unsigned char D4DLCDHW_SendCmdWord_Kinetis_Spi(unsigned short cmd);
  static unsigned short D4DLCDHW_ReadDataWord_Kinetis_Spi(void);
  static unsigned short D4DLCDHW_ReadCmdWord_Kinetis_Spi(void);
  static unsigned char D4DLCDHW_PinCtl_Kinetis_Spi(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState); 
  static void D4DLCD_FlushBuffer_Kinetis_Spi(void);
 
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHW_FUNCTIONS d4dlcdhw_kinetis_spi = 
  {
    D4DLCDHW_Init_Kinetis_Spi,    
    D4DLCDHW_SendDataWord_Kinetis_Spi,
    D4DLCDHW_SendCmdWord_Kinetis_Spi,
    D4DLCDHW_ReadDataWord_Kinetis_Spi,
    D4DLCDHW_ReadCmdWord_Kinetis_Spi,
    D4DLCDHW_PinCtl_Kinetis_Spi,
    D4DLCD_FlushBuffer_Kinetis_Spi,
    D4DLCDHW_DeInit_Kinetis_Spi
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
  // FUNCTION:    D4DLCDHW_Init_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_Init_Kinetis_Spi(void)
  { 
    
    // defined in spi_cfg.h
    #ifdef D4DLCD_DISPLAY_MCU_USER_INIT
      D4DLCD_DISPLAY_MCU_USER_INIT
    #endif  
    
      D4DLCD_SPI_PORT_INIT;
        
      D4DLCD_ASSERT_DC;
      D4DLCD_INIT_DC;
         
    // Enable and clear SPI
    D4DLCD_SPI_MCR &= (~ SPI_MCR_MDIS_MASK);
    D4DLCD_SPI_MCR = SPI_MCR_HALT_MASK | SPI_MCR_CLR_TXF_MASK | SPI_MCR_CLR_RXF_MASK;
  
    // 15+1 = 16-bit transfers, Fclk = Fsys/4
    D4DLCD_SPI_CTAR0 = SPI_CTAR_FMSZ(15) | SPI_CTAR_BR(0);
    
    #ifdef D4DLCD_SPI_DBL_BRATE
      D4DLCD_SPI_CTAR0 |= SPI_CTAR_DBR_MASK;   // Dual Baudrate enable in config file
    #endif
      
    // Set CS0-7 inactive high 
    D4DLCD_SPI_MCR |= (SPI_MCR_PCSIS(1 << (D4DLCD_SPI_PCS_ID))|SPI_MCR_MSTR_MASK);
    
    // Disable all IRQs
    D4DLCD_SPI_RSER = 0;
    // clear Flag
    D4DLCD_SPI_SR = SPI_SR_TFFF_MASK;
    D4DLCD_SPI_SR = SPI_SR_TCF_MASK;
    
    // Enable SPI
    D4DLCD_SPI_MCR &= (~ SPI_MCR_HALT_MASK);   
      return 1;  
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_DeInit_Kinetis_Spi(void)
  {
    return 1;  
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD  
  //
  // PARAMETERS:  unsigned short value    variable to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_SendDataWord_Kinetis_Spi(unsigned short value)
  {
    
    // wait write buffer not full flag        
    while (!(D4DLCD_SPI_SR & SPI_SR_TFFF_MASK)){}; 
    
    // Assert CS0, Use config 0
    D4DLCD_SPI_PUSHR = SPI_PUSHR_PCS(1 << (D4DLCD_SPI_PCS_ID)) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA((unsigned short)value);
    
    while (!(D4DLCD_SPI_SR & SPI_SR_TCF_MASK)){};// while shift-out complete
    D4DLCD_SPI_SR = SPI_SR_TCF_MASK;           // clear flag
    
    return 1;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendCmdWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit command into LCD  
  //
  // PARAMETERS:  unsigned short cmd    command to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_SendCmdWord_Kinetis_Spi(unsigned short cmd)
  {       
    D4DLCD_ASSERT_DC;                        // DataCmd := 1 
    (void)D4DLCDHW_SendDataWord_Kinetis_Spi(cmd);
    D4DLCD_DEASSERT_DC;                      // DataCmd := 0
    return 1;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadDataWord_Kinetis_Spi(void)
  {       
    return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadCmdWord_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadCmdWord_Kinetis_Spi(void)
  {       
    return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  
  static unsigned char D4DLCDHW_PinCtl_Kinetis_Spi(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
    switch(pinId)
    {
      case D4DLCD_RESET_PIN:
        switch(setState)
        {
          #ifdef D4DLCD_RESET
          
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
            
          #endif
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
  // FUNCTION:    D4DLCD_FlushBuffer_Kinetis_Spi
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_Kinetis_Spi(void)
  {       

  }

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_kinetis_Spi_ID)
