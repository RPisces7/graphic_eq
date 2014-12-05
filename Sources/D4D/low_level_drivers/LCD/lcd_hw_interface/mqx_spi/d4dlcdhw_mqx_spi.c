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
* @file      d4dlcdhw_mqx_spi.c
*
* @author    b01119
* 
* @version   0.0.6.0
* 
* @date      Apr-6-2011
* 
* @brief     D4D driver - Mqx hardware lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_mqx_spi_ID 1


// copilation enable preprocessor condition
// the string d4dtch_Mqx_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_spi_ID)
  
// include of low level driver heaser file
// it will be included into wole project only in case that this driver is selected in main D4D configuration file
#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <spi.h>

#if defined BSP_LCD_DC
   static GPIO_PIN_STRUCT dc_pin[] = {
	   /* when opening, explicitly set output to 0 */
   	BSP_LCD_DC | GPIO_PIN_STATUS_0,
   	GPIO_LIST_END
	};
#else
   #error define BSP_LCD_DC in bsp 	
#endif

static GPIO_PIN_STRUCT res_backlght_pin[]= 
{
   #if defined BSP_LCD_RESET
      BSP_LCD_RESET,
   #endif
   #if defined BSP_LCD_BACKLIGHT
      BSP_LCD_BACKLIGHT,
   #endif
   GPIO_LIST_END      
};

#if defined BSP_LCD_RESET
   static reset_pin[] = 
   {
      BSP_LCD_RESET,
      GPIO_LIST_END
   };
#endif   
 
#if defined BSP_LCD_BACKLIGHT
   static backlight_pin[] = 
   {
      BSP_LCD_BACKLIGHT,
      GPIO_LIST_END
   };
#endif 
 
#ifndef BSP_LCD_SPI_CHANNEL
   #error Define BSP_LCD_SPI_CHANNEL in bsp.
#endif   

#define INIT_OK            1
#define INIT_GPIO_FAILED   2
#define INIT_SPI_FAILED    3
#define INIT_SPI_SETTINGS_FAILED 4
/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Internal function prototypes 
******************************************************************************/

static unsigned char D4DLCDHW_Init_Mqx(void);
static unsigned char D4DLCDHW_DeInit_Mqx(void);
static unsigned char D4DLCDHW_SendDataWord_Mqx(unsigned short value);
static unsigned char D4DLCDHW_SendCmdWord_Mqx(unsigned short cmd);
static unsigned short D4DLCDHW_ReadDataWord_Mqx(void);
static unsigned short D4DLCDHW_ReadComWord_Mqx(void);
static unsigned char D4DLCDHW_PinCtl_Mqx(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
static void D4DLCD_FlushBuffer_Mqx(void);

/**************************************************************//*!
*
* Global variables
*
******************************************************************/
static FILE_PTR spi_fd;
static FILE_PTR dc_pin_fd;
static FILE_PTR res_backlght_pin_fd;

// the main structure that contains low level driver api functions
// the name fo this structure is used for recognizing of configured low level driver of whole D4D
// so this name has to be used in main configuration header file of D4D driver to enable this driver
const D4DLCDHW_FUNCTIONS d4dlcdhw_mqx_spi = 
{
   D4DLCDHW_Init_Mqx,    
   D4DLCDHW_SendDataWord_Mqx,
   D4DLCDHW_SendCmdWord_Mqx,   
   D4DLCDHW_ReadDataWord_Mqx,
   D4DLCDHW_ReadComWord_Mqx,
   D4DLCDHW_PinCtl_Mqx,
   D4DLCD_FlushBuffer_Mqx,
   D4DLCDHW_DeInit_Mqx,
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
// FUNCTION:    D4DLCDHW_Init_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver 
//              
// PARAMETERS:  none
//              
// RETURNS:     none
//               
//-----------------------------------------------------------------------------  
static unsigned char D4DLCDHW_Init_Mqx(void)
{
   uint_32  param;
   unsigned char result = INIT_OK;
   
   /* init DC */     
   if (NULL == (dc_pin_fd = fopen("gpio:write", (char_ptr) &dc_pin ))) 
   {
      result = INIT_GPIO_FAILED;
   }  
   
   /* DC to 0 */
   ioctl(dc_pin_fd, GPIO_IOCTL_WRITE_LOG0, NULL );
   
   /* init reset and backlight */
   if (NULL == (res_backlght_pin_fd = fopen("gpio:write", (char_ptr) &res_backlght_pin ))) 
   {
      result = INIT_GPIO_FAILED;
   }    
   
   /* Open the SPI controller */         
   spi_fd = fopen (BSP_LCD_SPI_CHANNEL, NULL);
   if (NULL == spi_fd) 
   {
      result = INIT_SPI_FAILED;
   } 
#if defined BSP_TWRMCF52259  || defined BSP_TWRMCF51CN 
   /* Set baud rate */
   param = 20000000; /* 20 MHz - max baudrate for M52259 */
   if (SPI_OK != ioctl (spi_fd, IO_IOCTL_SPI_SET_BAUD, &param)) 
   {
      result = INIT_SPI_SETTINGS_FAILED;
   }
#endif

   /* Set clock mode */
   param = SPI_CLK_POL_PHA_MODE0;
   if (SPI_OK != ioctl (spi_fd, IO_IOCTL_SPI_SET_MODE, &param)) 
   {
      result = INIT_SPI_SETTINGS_FAILED;
   }     

   /* Set big endian */
   param = SPI_DEVICE_BIG_ENDIAN;
   if (SPI_OK != ioctl (spi_fd, IO_IOCTL_SPI_SET_ENDIAN, &param)) 
   {
      result = INIT_SPI_SETTINGS_FAILED;
   }
   
   /* Set transfer mode */
   param = SPI_DEVICE_MASTER_MODE;
   if (SPI_OK != ioctl (spi_fd, IO_IOCTL_SPI_SET_TRANSFER_MODE, &param)) 
   {
      result = INIT_SPI_SETTINGS_FAILED;
   } 

   /* Set framesize == 16 */
#if defined BSP_TWRMCF52259  || defined BSP_TWRMCF51CN 
   param = 0;
#else   	   
   param = 16;
#endif   
   if (SPI_OK != ioctl (spi_fd, IO_IOCTL_SPI_SET_FRAMESIZE, &param)) 
   {
      result = INIT_SPI_SETTINGS_FAILED;
   }
   
// ----------------------------------------------------------------------------
// Temporary MQX patch for Kinetis MCU family
//-----------------------------------------------------------------------------
   
  // Enable PAD Drive strength control for SPI output pins
#if defined  BSP_TWR_K40X256 || defined BSP_TWR_K60N512 
   
   /* Set baud rate */
   param = 25000000; /* 25 MHz - max baudrate for Kinetis */
   if (SPI_OK != ioctl (spi_fd, IO_IOCTL_SPI_SET_BAUD, &param)) 
   {
      result = INIT_SPI_SETTINGS_FAILED;
   }
   
   
  #if defined BSP_TWR_K40X256

	    PORTD_PCR2|=PORT_PCR_DSE_MASK;\
            PORTD_PCR1|=PORT_PCR_DSE_MASK;\
            PORTD_PCR0|=PORT_PCR_DSE_MASK;\
            PORTA_PCR9|=PORT_PCR_DSE_MASK;
	     /*PORTC_PCR3|=PORT_PCR_DSE_MASK; */

  #elif defined BSP_TWR_K60N512

	    PORTD_PCR13|=PORT_PCR_DSE_MASK;\
            PORTD_PCR12|=PORT_PCR_DSE_MASK;\
            PORTD_PCR11|=PORT_PCR_DSE_MASK;\
            PORTB_PCR17|=PORT_PCR_DSE_MASK;\
	    /*PORTD_PCR15|=PORT_PCR_DSE_MASK; */
  #endif
#endif
//-----------------------------------------------------------------------------
   
   return result;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_DeInit_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for deinitialization of this low level driver 
//              
// PARAMETERS:  none
//              
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------  
static unsigned char D4DLCDHW_DeInit_Mqx(void)
{
   return 1; 
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_SendDataWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit variable into LCD  
//
// PARAMETERS:  unsigned short value    variable to send
//
// RETURNS:     none
//              
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_SendDataWord_Mqx(unsigned short value)
{       
#if defined BSP_TWRMCF52259  || defined BSP_TWRMCF51CN 
   fwrite ( &value, 2, 1, spi_fd);
#else
   /* need to switch byte order */
   unsigned short p_color = ((value & 0xFF00) >> 8) | (( value & 0xFF ) << 8) ;
   fwrite ( &p_color, 2, 1, spi_fd);
#endif
   return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_SendCmdWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit command into LCD  
//
// PARAMETERS:  unsigned short cmd    command to send
//
// RETURNS:     none
//              
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_SendCmdWord_Mqx(unsigned short cmd)
{       
   /* DC to 0 */
   ioctl(dc_pin_fd, GPIO_IOCTL_WRITE_LOG0, NULL );      
   D4DLCDHW_SendDataWord_Mqx( cmd );   
   /* DC to 1 */
   ioctl(dc_pin_fd, GPIO_IOCTL_WRITE_LOG1, NULL ); 
   return 1;    
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadDataWord_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)  
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//              
//-----------------------------------------------------------------------------
static unsigned short D4DLCDHW_ReadDataWord_Mqx(void)
{       
    return 1;
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadCmdWord_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)  
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//              
//-----------------------------------------------------------------------------
static unsigned short D4DLCDHW_ReadComWord_Mqx(void)
{       
   return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_PinCtl_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: allows control GPIO pins for LCD conrol purposes
//
// PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
//              D4DHW_PIN_STATE setState - Pin action
// RETURNS:     for Get action retuns the pin value
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_PinCtl_Mqx(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
{       

   /* is the file opened? */     
   if (NULL == (res_backlght_pin_fd )) 
   {
      return(-1);
   }
 
   switch( pinId )
   {      
      case D4DLCD_RESET_PIN:
         #if defined BSP_LCD_RESET                  
         switch( setState ) 
         {       
            case D4DHW_PIN_SET_1:
               ioctl(res_backlght_pin_fd, GPIO_IOCTL_WRITE_LOG0, &reset_pin );
               break;
               
            case D4DHW_PIN_SET_0:
               ioctl(res_backlght_pin_fd, GPIO_IOCTL_WRITE_LOG1, &reset_pin );
               break;
               
            default:
               break;                                                             
         }
         #endif         
         break;
        
      case D4DLCD_BACKLIGHT_PIN:
         #if defined BSP_LCD_BACKLIGHT                 
         switch( setState ) 
         {       
            case D4DHW_PIN_SET_1:
               ioctl(res_backlght_pin_fd, GPIO_IOCTL_WRITE_LOG1, &backlight_pin );               
               break;
               
            case D4DHW_PIN_SET_0:
               ioctl(res_backlght_pin_fd, GPIO_IOCTL_WRITE_LOG0, &backlight_pin );
               break;
               
            default:
               break;                                                             
         }
         #endif         
         break;         
        
      default:
         break;                
   }
   return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCD_FlushBuffer_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: For buffered low level interfaces is used to inform
//              driver the complete object is drawed and pending pixels should be flushed
//
// PARAMETERS:  none
//              
// RETURNS:     none
//-----------------------------------------------------------------------------
static void D4DLCD_FlushBuffer_Mqx(void)
{       

}

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_Mqx_ID)