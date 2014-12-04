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
* @file      d4dlcd_LS020.c
*
* @author    b01119
* 
* @version   0.0.13.0
* 
* @date      Jun-28-2010
* 
* @brief     D4D driver - ls020 lcd driver source c file 
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_ls020_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_ls020_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ls020_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "d4dlcd_ls020.h"
  #include "d4dlcdhw_common.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4DLCD_Init_LS020(void);
  static unsigned char D4DLCD_SetWindow_LS020(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_LS020(D4DLCD_ORIENTATION new_orientation);
  static unsigned char D4DLCD_Send_PixelColor_LS020(unsigned short value) ;
  static unsigned short D4DLCD_Read_Word_LS020(void);
  static void D4DLCD_Flush_LS020(void);
  static unsigned char D4DLCD_DeInit_LS020(void);
  
  static void D4DLCD_Command_Data(const unsigned short data[], unsigned int count);
  
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_ls020 = 
  {
    D4DLCD_Init_LS020,    
    D4DLCD_SetWindow_LS020,
    D4DLCD_SetOrientation_LS020,
    D4DLCD_Send_PixelColor_LS020,
    D4DLCD_Read_Word_LS020,
    D4DLCD_Flush_LS020,
    D4DLCD_Delay_ms_Common,
    D4DLCD_DeInit_LS020,
  };

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/
  static D4DLCD_ORIENTATION d4dlcd_orientation = Portrait;
  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

  // sends Command/Data sequence word (16 bit) or array with words - first 1 command, then 1 x data
  static void D4DLCD_Command_Data(const unsigned short data[], unsigned int count)
  {
    int i;
    
    for (i=0; i<count; i++)
    {
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned short)data[i]);      
    }
  }
   
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Init_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4DLCD_Init_LS020(void)
  {
      static const unsigned short init_data[] =
    	{ 
    		0xFDFD, 0xFDFD, 
    		// pause 
    		0xEF00, 0xEE04, 0x1B04, 0xFEFE, 0xFEFE, 
    		0xEF90, 0x4A04, 0x7F3F, 0xEE04, 0x4306, 
    		// pause 
    		0xEF90, 0x0983, 0x0800, 0x0BAF, 0x0A00, 
    		0x0500, 0x0600, 0x0700, 0xEF00, 0xEE0C, 
    		0xEF90, 0x0080, 0xEFB0, 0x4902, 0xEF00, 
    		0x7F01, 0xE181, 0xE202, 0xE276, 0xE183, 
    		0x8001, 0xEF90, 0x0000 
    	};  
    
    if(D4D_LLD_LCD_HW.D4DLCDHW_Init == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_ReadCmdWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_PinCtl == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_DeInit == NULL)
      return 0;
    
    
    if(!D4D_LLD_LCD_HW.D4DLCDHW_Init())
      return 0;
    
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_BACKLIGHT_PIN, D4DHW_PIN_SET_1);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_BACKLIGHT_PIN, D4DHW_PIN_OUT);    

    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_OUT);            

    D4DLCD_Delay_ms_Common(300); 
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);   D4DLCD_Delay_ms_Common(75);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_1);   D4DLCD_Delay_ms_Common(500);      
    
    D4DLCD_Command_Data(&init_data[0], 2);    D4DLCD_Delay_ms_Common(75); 
    D4DLCD_Command_Data(&init_data[2], 10);   D4DLCD_Delay_ms_Common(75); 
    D4DLCD_Command_Data(&init_data[12], 23);
    
    return 1;  
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------  
  static unsigned char D4DLCD_DeInit_LS020(void)
  {
    return 0;   
  }    
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function sets the logic window in memory of LCD driver
  // 
  // PARAMETERS:  unsigned int x1 - left cordination of logic window
  //              unsigned int y1 - top cordination of logic window
  //              unsigned int x2 - right cordination of logic window
  //              unsigned int y2 - bottom cordination of logic window
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4DLCD_SetWindow_LS020(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
    unsigned char data[] = 
  	{ 
  		0xEF, 0x08,
  		0x18, 0x00,
  		0x12,0,
  		0x15,0,
  		0x13,0,
  		0x16,0 
  	};

    
  	switch (d4dlcd_orientation)
  	{
  		default:
  			// Invalid! Fall through to portrait mode
  		case Portrait:
    	  data[3] = 0;		// Mode
    		data[5]= (unsigned char) x1; 
        data[7]= (unsigned char) x2;
        data[9]= (unsigned char) y1;
        data[11]= (unsigned char) y2;			
  			break;
  		case Portrait180:
  			data[3] = 0x03;		// Mode
  			data[5] = (unsigned char) (D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - x1);
  			data[7] = (unsigned char) (D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - x2);
  			data[9] = (unsigned char) (D4D_SCREEN_SIZE_LONGER_SIDE - 1 - y1);
  			data[11] = (unsigned char) (D4D_SCREEN_SIZE_LONGER_SIDE - 1 - y2);
  			break;
  		case Landscape:
  			data[3] = 0x05;		// Mode
  			data[5] = (unsigned char) (D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y1);
  			data[7] = (unsigned char) (D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y2);
  			data[9] = (unsigned char) x1;
  			data[11] = (unsigned char) x2;
  			break;
  		case Landscape180:
  			// Information on mode Landscape180: At this mode the display seems to have a bug.
  			// Please read the programmers manual for more information or just do not use this mode with
  			// large pictures or large boxes >125 lines in Y-direction
  			data[3] = 0x06;		// Mode
  			data[5] = (unsigned char) y1;
  			data[7] = (unsigned char) y2;
  			data[9] = (unsigned char) (D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x1);
  			data[11] = (unsigned char) (D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x2);
  			break;
  	}
  	
  	D4DLCD_Command_Data((unsigned short*)data, 6);  
  	
  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD  
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed                
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_LS020(D4DLCD_ORIENTATION new_orientation)
  {       
    d4dlcd_orientation = new_orientation;
    return D4DLCD_SetWindow_LS020(0, 0, 1, 1);
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD  
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed                
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Send_PixelColor_LS020(unsigned short value)
  {       
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(value);
    return 1;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Read_Word_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCD_Read_Word_LS020(void)
  {       
    return D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_LS020
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_LS020(void)
  {       
    D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer();
  }


#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ls020_ID)
