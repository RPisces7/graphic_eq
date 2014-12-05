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
* @file      d4dlcdhw_mqx_fb.c
*
* @author    b01119
* 
* @version   0.0.4.0
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
#define d4dlcdhw_mqx_fb_ID 1


// copilation enable preprocessor condition
// the string d4dtch_Mqx_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_fb_ID)
  
// include of low level driver heaser file
// it will be included into wole project only in case that this driver is selected in main D4D configuration file
#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <spi.h>

#ifndef BSP_EXTERNAL_LCD_DC
   #error Define BSP_EXTERNAL_LCD_DC in bsp.
#endif

#ifndef BSP_EXTERNAL_LCD_BASE
   #error Define BSP_EXTERNAL_LCD_BASE in bsp.
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
   
// TWR-K40X256 Address Latch patch
#if defined BSP_TWR_K40X256
   
  static GPIO_PIN_STRUCT k40_static_ale_pin[]= 
  {
    (GPIO_PORT_E | GPIO_PIN6),
    GPIO_LIST_END      
   };
  
#endif  // TWR-K40X256 
   

extern void _bsp_flexbus_lcd_setup (const uint_32 base_address);
/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Internal function prototypes 
******************************************************************************/

static unsigned char D4DLCDHW_Init_Mqx(void);
static unsigned char D4DLCDHW_SendDataWord_Mqx(unsigned short value);
static unsigned char D4DLCDHW_SendCmdWord_Mqx(unsigned short cmd);
static unsigned short D4DLCDHW_ReadDataWord_Mqx(void);
static unsigned short D4DLCDHW_ReadCmdWord_Mqx(void);
static unsigned char D4DLCDHW_PinCtl_Mqx(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
static void D4DLCD_FlushBuffer_Mqx(void);
static unsigned char D4DLCDHW_DeInit_Mqx(void);

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

// the main structure that contains low level driver api functions
// the name fo this structure is used for recognizing of configured low level driver of whole D4D
// so this name has to be used in main configuration header file of D4D driver to enable this driver
const D4DLCDHW_FUNCTIONS d4dlcdhw_mqx_fb = 
{
   D4DLCDHW_Init_Mqx,    
   D4DLCDHW_SendDataWord_Mqx,
   D4DLCDHW_SendCmdWord_Mqx,
   D4DLCDHW_ReadDataWord_Mqx,
   D4DLCDHW_ReadCmdWord_Mqx,
   D4DLCDHW_PinCtl_Mqx,
   D4DLCD_FlushBuffer_Mqx,
   D4DLCDHW_DeInit_Mqx
};
/**************************************************************//*!
*
* Local variables
*
******************************************************************/
static FILE_PTR res_backlght_pin_fd;
static FILE_PTR k40_static_ale_pin_fd;

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
// RETURNS:     1 ok; 2 gpio init failed
//               
//-----------------------------------------------------------------------------  
static unsigned char D4DLCDHW_Init_Mqx(void)
{
   unsigned char result = 1;
   /* init reset and backlight */
   if (NULL == (res_backlght_pin_fd = fopen("gpio:write", (char_ptr) &res_backlght_pin ))) 
   {
      /* gpio initalization failed */
      result = 2;
   } 
   
   _bsp_flexbus_lcd_setup((uint_32)BSP_EXTERNAL_LCD_BASE);
        
// TWR-K40X256 Address Latch patch   
#if defined BSP_TWR_K40X256       
   /* init reset and backlight */
   if (NULL == (k40_static_ale_pin_fd = fopen("gpio:write", (char_ptr) &k40_static_ale_pin ))) 
   {
      /* gpio initalization failed */
      result = 2;
   }
   ioctl(k40_static_ale_pin_fd, GPIO_IOCTL_WRITE_LOG1, &k40_static_ale_pin );
#endif // BSP_TWR_K40X256 
   
   return result;
}

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
static unsigned char D4DLCDHW_DeInit_Mqx(void)
{
   if( res_backlght_pin_fd )
   {
      fclose( res_backlght_pin_fd );
      res_backlght_pin_fd = NULL;
   }
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
   *((unsigned short*)BSP_EXTERNAL_LCD_DC) = value;
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
   *((unsigned short*)BSP_EXTERNAL_LCD_BASE) = cmd; 
   return 1;
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this 
//              function is supported)  
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//              
//-----------------------------------------------------------------------------
static unsigned short D4DLCDHW_ReadDataWord_Mqx(void)
{       
   return *((unsigned short*)BSP_EXTERNAL_LCD_BASE);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadCmdWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this 
//              function is supported)  
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//              
//-----------------------------------------------------------------------------
static unsigned short D4DLCDHW_ReadCmdWord_Mqx(void)
{       
   return *((unsigned short*)BSP_EXTERNAL_LCD_BASE);
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