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
* @file      d4dlcdhw_mqx_mpc5125_diu.c
*
* @author    b01119
* 
* @version   0.0.3.0
* 
* @date      Jan-27-2011
* 
* @brief     D4D driver - mqx_mpc5125_diu hardware lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_mqx_mpc5125_diu_ID 1


// copilation enable preprocessor condition
// the string d4dtch_mqx_mpc5125_diu_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_mpc5125_diu_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dlcdhw_mqx_mpc5125_diu.h"
  #include "mqx.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_MqxMpc5125_DIU(void);
  static unsigned char D4DLCDHW_DeInit_MqxMpc5125_DIU(void);
  static unsigned char D4DLCDHW_WriteData_MqxMpc5125_DIU(unsigned long addr, unsigned short value);
  static unsigned short D4DLCDHW_ReadData_MqxMpc5125_DIU(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU(void);
  static unsigned char D4DLCDHW_PinCtl_MqxMpc5125_DIU(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_MqxMpc5125_DIU(void);

 
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHWFB_FUNCTIONS d4dlcdhw_mqx_mpc5125_diu = 
  {
    D4DLCDHW_Init_MqxMpc5125_DIU,    
    D4DLCDHW_WriteData_MqxMpc5125_DIU,
    D4DLCDHW_ReadData_MqxMpc5125_DIU,
    D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU,
    D4DLCDHW_PinCtl_MqxMpc5125_DIU,
    D4DLCD_FlushBuffer_MqxMpc5125_DIU,
    D4DLCDHW_DeInit_MqxMpc5125_DIU
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

	static D4DLCD_FRAMEBUFF_DESC d4dlcdhw_mqx_mpc5125_diu_desc;
	static DISPLAY_CONTEXT_STRUCT_PTR d4dlcdhw_gdc_ptr = NULL;
	static DISPLAY_PIXEL_BUFFER_PTR d4dlcdhw_pixbuff_ptr = NULL;
	static DISPLAY_VIEW_STRUCT_PTR  d4dlcdhw_view_ptr = NULL;
  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

   
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_Init_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_Init_MqxMpc5125_DIU(void)
  {
		char buffer[64];
		
		sprintf (buffer, "%dx%dx%d", D4DLCDHWFB_X, D4DLCDHWFB_Y, D4DLCDHWFB_REFRESH);
		d4dlcdhw_gdc_ptr = display_initialize(buffer);
		if(d4dlcdhw_gdc_ptr == NULL)
			return 0;
		
		
		
		
		d4dlcdhw_pixbuff_ptr = display_create_pixel_buffer(D4DLCDHWFB_X, D4DLCDHWFB_Y, RGB24, NULL);
		
		d4dlcdhw_mqx_mpc5125_diu_desc.bpp_byte = d4dlcdhw_pixbuff_ptr->BPP;
		d4dlcdhw_mqx_mpc5125_diu_desc.lcd_x_max = d4dlcdhw_pixbuff_ptr->X;
		d4dlcdhw_mqx_mpc5125_diu_desc.lcd_y_max = d4dlcdhw_pixbuff_ptr->Y;
		d4dlcdhw_mqx_mpc5125_diu_desc.fb_start_addr = (unsigned long) d4dlcdhw_pixbuff_ptr->FB;
  	
  	d4dlcdhw_view_ptr = display_create_view(d4dlcdhw_gdc_ptr, d4dlcdhw_pixbuff_ptr);
  	
  	if(d4dlcdhw_view_ptr == NULL)
  	{
  		D4DLCDHW_DeInit_MqxMpc5125_DIU();	
  		return 0;
  	}
  
  	display_center_view(d4dlcdhw_view_ptr);	
  	display_show_view(d4dlcdhw_view_ptr, DIU_PLANE_1);
  	
  	return 1;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCDHW_DeInit_MqxMpc5125_DIU(void)
  {
		display_destroy_view(d4dlcdhw_view_ptr);
		display_destroy_pixel_buffer(d4dlcdhw_pixbuff_ptr);
		display_deinit(d4dlcdhw_gdc_ptr);
  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD  
  //
  // PARAMETERS:  unsigned long addr  		address to write data
  //							unsigned short value    variable to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_WriteData_MqxMpc5125_DIU(unsigned long addr, unsigned short value)
  {       
  	
 	
 	#if 0 
 			unsigned char color;
 			
 			color = D4D_COLOR_GET_R(value);
 			*((unsigned char*)addr) = color;
 			
 			color = D4D_COLOR_GET_G(value);
 			*((unsigned char*)addr+1) = color;
 			
 			color = D4D_COLOR_GET_B(value);
 			*((unsigned char*)addr+2) = color;
 #else
 			unsigned long color;
 
 			color = *((unsigned long*)addr);
 			color &= 0x000000ff;
 			color |= (D4D_COLOR_GET_R(value)) << 24;
  		color |= (D4D_COLOR_GET_G(value)) << 16;
 	 		color |= (D4D_COLOR_GET_B(value)) << 8;
 			
			*((unsigned long*)addr) = color; 
 #endif			
 			 	
  //	*((unsigned short*)addr) = value;
  	return 1;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)  
  //
  // PARAMETERS:  unsigned long addr  		address to read data
  //
  // RETURNS:     unsigned short - the read value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadData_MqxMpc5125_DIU(unsigned long addr)
  {       
  	unsigned short ret;
  	unsigned long color;
  	
  	color = (*((unsigned long*)addr)) & 0xff000000;
  	
  	ret = D4D_COLOR_RGB(((color & 0xffffff00)>> 3), ((color & 0xffff00ff)>> (2 + 8)), ((color & 0xff00ffff)>> (3 + 16)));
  	
  	return ret;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function return the pointer on filled frame buffer descriptor  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     D4DLCD_FRAMEBUFF_DESC* - pointer on frame buffer descriptor
  //              
  //-----------------------------------------------------------------------------
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU(void)
  {
  	return (D4DLCD_FRAMEBUFF_DESC*) &d4dlcdhw_mqx_mpc5125_diu_desc;	
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_MqxMpc5125_DIU(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {       
		(void)pinId;
		(void)setState;
		return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_MqxMpc5125_DIU(void)
  {       
		//display_flush_pixel_buffer(d4dlcdhw_pixbuff_ptr);
  }
#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_mpc5125_diu_ID)