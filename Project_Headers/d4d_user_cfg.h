/******************************************************************************
*
* Copyright (c) 2009 Freescale Semiconductor;
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
* @file      d4d_user_cfg.h
*
* @author    b01119
*
* @version   0.0.1.0
*
* @date      Mar-4-2011
*
* @brief     D4D Driver user configuration file for Kinetis TWR-K60N512
*
*******************************************************************************/

#ifndef __D4D_USER_CFG_H
#define __D4D_USER_CFG_H

/******************************************************************************
* Desciption : This file allows user depends modification of D4D Freescale
*   graphical driver.
*
*
******************************************************************************/

//#include "mqx.h"
//#include "bsp.h"

/**************************************************************//*!
*
* Low Level driver specification for LCD diplay and optionally for
* touch screen
*
******************************************************************/
// List of implemented low level LCD drivers

// d4dlcd_ssd1289 - driver for LCD displays with SSD1289 controller from Solomon Systech
// d4dlcd_fsa506 - driver for LCD displays with FSA506 controller
// d4dlcd_lgdp4531 - driver for LCD displays with LGDP4531 controller from LG Electronics
// d4dlcd_ls020 - driver for LCD displays LS020
// d4dlcd_frame_buffer - frame buffer driver for MCU/MPU with LCD peripheral


// Please define a used low LCD driver
#define D4D_LLD_LCD d4dlcd_ssd1289   // the name of low level driver descriptor structure

// List of implemented low level LCD hw interface drivers

// d4dlcdhw_flexbus_16b - low level hw interface driver for flexbus with 16 bit witdh
// d4dlcdhw_flexbus_8b - low level hw interface driver for flexbus with 8 bit witdh
// d4dlcdhw_gpio6800_8b - low level hw interface driver for gpio drived parallel 8b 6800 bus
// d4dlcdhw_gpio8080_8b - low level hw interface driver for gpio drived parallel 8b 8080 bus
// d4dlcdhw_gpio8080_byte_8b - low level hw interface driver for gpio drived parallel 8b 8080 bus
//                              byte version (sends only bytes instead of words as normal)
// d4dlcdhw_spi_8b - low level hw interface driver for hardware SPI with 8 bit witdh for S08 MCU
// d4dlcdhw_s12_spi_16b - low level hw interface driver for hardware SPI with 8 bit witdh for S12 MCU
// d4dlcdhw_swspi_16b - low level hw interface driver for software SPI with 16 bit witdh
// d4dlcdhw_spi_swc_8b - low level hw interface driver for hardware SPI with 8 bit witdh and software
//                          controlled control signals of LCD controller (for example FSA506)

// d4dlcdhw_mqx_spi - low level driver for SPI over MQX (version 3.6 or higher)
// d4dlcdhw_mqx_fb - low level driver for flexbus over MQX (version 3.6 or higher)
// d4dlcdhw_dragonfire_lcdc - low level driver for MCF52277 MPU LCDC peripherial
// d4dlcdhw_mqx_mpc5125_diu - low level driver for MPC5125 MPU DIU peripherial over MQX (version 3.6 or higher)


// Please (if it's needed) define a used LCD hw interface driver
//#define D4D_LLD_LCD_HW d4dlcdhw_flexbus_16b   // the name of LCD hw interface driver descriptor structure


#define D4D_LLD_LCD_HW d4dlcdhw_kinetis_spi
//#define D4D_LLD_LCD_HW d4dlcdhw_flexbus_16b

/**************************************************************//*!
*
* Touch screen low level driver section
*
******************************************************************/


// List of supported low level touch screen drivers

// d4dtch_resistive - driver for resistive touch screen with direct analog connection to MCU
// d4dtch_resistive_mqx - driver for resistive touch screen with direct analog connection to MCU over MQX (version 3.6 or higher)
// d4dtch_mcf52277_asp - driver for resistive touch screen over MCF52277 ASP peripheral

// Please define a used touch screen driver if touch screen is used in project
//#define D4D_LLD_TCH  d4dtch_resistive


// List of implemented low level Touch screen hw interface drivers

// d4dtchhw_s08_adc - low level hw interface driver for S08 ADC
// d4dtchhw_s12_adc - low level hw interface driver for S12 ADC
// d4dtchhw_mcf52259_adc - low level hw interface driver for MCF52277 ASP peripheral (interrupt driven)

// Please (if it's needed) define a used touch screen hw interface driver
//#define D4D_LLD_TCH_HW d4dtchhw_kinetis_adc

/******************************************************************************
*
*        ==================== D4Drv General ======================
*
*               User definition of general bahaviour of D4Dvr
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/

// The D4Drv contains own standard types for cases that in the whole project missing
// loaded standard types as Byte, Word, LWord etc.
// To disable using of D4Drv own standard types change the below define to D4D_TRUE

#define D4D_USE_STANDARD_TYPES D4D_FALSE

// Variable of coordinations of display
// in simple - if display has resolution bigger than 256, than you have to specify
// bigger type than unsigned char (recommended is unsigned short)
#define D4D_COOR_TYPE Word

// Bitfield modification section
// By this options can be modified driver to run with different bitfield representations

      // The first modification is alignment of bitfields sets in compilator right or left align
      //#define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_RIGHT

      // The second one allows adapt to 2/4 byte bitfileds for example in Coldfire V2 compilators
      //#define D4D_BITFIELD_SHIFT 0

      // Example for Codewarrior Coldfire V2 Compilator
      //#define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_LEFT
      // #define D4D_BITFIELD_SHIFT 24

      // Example for Codewarrior Coldfire V1/S08 Compilator
      // #define D4D_BITFIELD_LSB_ALIGNMENT D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
      // #define D4D_BITFIELD_SHIFT 0


/******************************************************************************
*
*   User definition of input KEYS format
*     To apply user definition uncomment the accurate line
*
******************************************************************************/

//#define D4D_KEYS_BUFF_LENGTH 8

// Here is place for any change of standard keys codes

//#define D4D_KEY_SCANCODE_UP     0x51
//#define D4D_KEY_SCANCODE_DOWN   0x50
//#define D4D_KEY_SCANCODE_LEFT   0x4B
//#define D4D_KEY_SCANCODE_RIGHT  0x4D
//#define D4D_KEY_SCANCODE_ENTER  0x1C
//#define D4D_KEY_SCANCODE_ESC    0x01

//#define D4D_KEY_FUNC_FOCUS_NEXT    D4D_KEY_SCANCODE_RIGHT
//#define D4D_KEY_FUNC_FOCUS_PREV    D4D_KEY_SCANCODE_LEFT

// Backward compatibility section
  //#define D4D_KEY_UP     0x01
  //#define D4D_KEY_DOWN   0x02
  //#define D4D_KEY_LEFT   0x04
  //#define D4D_KEY_RIGHT  0x08
  //#define D4D_KEY_ENTER  0x10
  //#define D4D_KEY_ESC    0x20

/******************************************************************************
* Constants
******************************************************************************/

//Select the MCU type that is used in this project  supported types:
//D4D_HC08
//D4D_HCS08
//D4D_HC12
//D4D_HCS12
//D4D_HCS12X
//D4D_MCF51
//D4D_MCF52
//D4D_MCF53
//D4D_MCF54
//D4D_MPC51
//D4D_MK

#define D4D_MCU_TYPE D4D_MK

#define D4D_ENABLE_AUTOSIZE  D4D_TRUE // This option disable autosize part of code in driver
                                        // So it save some place in Flash but all parameters has to be declared

#define D4D_FONT_TABLE_DISABLED D4D_FALSE   // this option enable /disable font table for whole driver
                                             // when this option is sets to true all strings are disabled

#define D4D_ROUND_CORNER_ENABLE D4D_TRUE   // this option disable/enable round corners support of D4D

//#define D4D_SCREEN_HISTORY 10         // This option define depth of history of screens

#define D4D_SCREEN_SIZE_LONGER_SIDE  320  // The maximum resolution of longer longer side of physical LCD

#define D4D_SCREEN_SIZE_SHORTER_SIDE  240  // The maximum resolution of  shorter longer side of physical LCD

#define D4D_MCU_BUS_CLOCK 48000000L  // Mcu bus clock in Hz mainly for few delay loops in low level

#define D4D_COLOR_SYSTEM_FORE D4D_COLOR_YELLOW // System fore color (for example for calibration screen)

#define D4D_COLOR_SYSTEM_BCKG D4D_COLOR_BLACK // System background color (for example for calibration screen)

#define D4D_FONT_SYSTEM_DEFAULT 4 // System default font (for example for calibration screen)

/******************************************************************************
* CallBack function definition
******************************************************************************/


//#define D4D_INPUT_EVENT_CALLBACK D4D_InputEventCB // This name of callback function
                                                  // that is called with all input events
                                                  // from keyboard or touchscreen.
                                                  // Prototype is void D4D_INPUT_EVENT_CALLBACK(void)
                                                  // Uncomment to used it.


/******************************************************************************
*
*        ==================== D4D_SCREEN ======================
*
*               User definition of defaults of D4D screen
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/

// Define a default screen outline color
#define D4D_COLOR_SCR_OUTLINE  D4D_COLOR_BLACK

// Define a default screen header color
#define D4D_COLOR_SCR_TITLEBAR  D4D_COLOR_WHITE

// Define a default screen header text color
#define D4D_COLOR_SCR_TILTLETEXT  D4D_COLOR_BLACK

// Define a default screen header exit button fore color
#define D4D_COLOR_SCR_EXIT_BTN_FORE  D4D_COLOR_YELLOW

// Define a default screen header exit button background color
#define D4D_COLOR_SCR_EXIT_BTN_BCKG  D4D_COLOR_BRIGHT_RED

// Define a default screen background color
#define D4D_COLOR_SCR_DESKTOP  D4D_COLOR_BLACK



// Define a default title offset in axis X (is used already for title icon offset)
//#define D4D_SCR_TITLE_OFF_X  30

// Define a default title offset in axis Y (is used already for title icon offset)
//#define D4D_SCR_TITLE_OFF_Y  1

// Define a minimum size of screen header in case that header is enabled
//#define D4D_SCR_HEADER_SIZE_MIN_SIZE  13

// Define a minimum size of EXIT button(cross) in screen header
//#define D4D_SCR_TITLE_EXITBTN_MIN_SIZE  6

// Define a default leasing of cross in EXIT button, opposite to size of button
//#define D4D_SCR_EXITBTN_CROSS_SIZE 2

// Define a default exit button offset
//#define D4D_SCR_TITLE_EXITBTN_OFFSET  4


// Define a default flags
#define D4D_SCR_F_DEFAULT        (D4D_SCR_F_BCKG)

/******************************************************************************
*
*        ==================== D4D BITMAPS ======================
*
*                 List of supported bitmap types
*     To enable bitmap decoding algorithm change D4D_FALSE option to D4D_TRUE
*
******************************************************************************/

// 65k colors bitmap without pallete
#define D4D_BMP_65536NOPAL_ENABLE D4D_FALSE

// 256 colors bitmap without pallete
#define D4D_BMP_256NOPAL_ENABLE D4D_TRUE

// 256 colors bitmap with pallete
#define D4D_BMP_PAL_256_ENABLE D4D_TRUE

// 16 colors bitmap with pallete
#define D4D_BMP_PAL_16_ENABLE D4D_FALSE

// 2 colors bitmap with pallete
#define D4D_BMP_PAL_2_ENABLE D4D_TRUE


/******************************************************************************
* Rounded corners bitmpas decoders
******************************************************************************/

// 65k colors bitmap without pallete
#define D4D_BMPR_65536NOPAL_ENABLE D4D_TRUE

// 256 colors bitmap without pallete
#define D4D_BMPR_256NOPAL_ENABLE D4D_TRUE

// 256 colors bitmap with pallete
#define D4D_BMPR_PAL_256_ENABLE D4D_TRUE

/******************************************************************************
*
*        ==================== D4D_OBJECT ======================
*
*          User definition of defaults  of individual objects
*          To apply user definition uncomment the accurate line
*
******************************************************************************/

/******************************************************************************
* General object colors
******************************************************************************/

// standard normal color fore
#define D4D_COLOR_FORE_NORM  D4D_COLOR_YELLOW
// standard normal color background
#define D4D_COLOR_BCKG_NORM  D4D_COLOR_LIGHT_GREY

// standard disabled color fore
#define D4D_COLOR_FORE_DISABLED D4D_COLOR_WHITE
// standard disabled color background
#define D4D_COLOR_BCKG_DISABLED D4D_COLOR_LIGHT_GREY

// standard focus color fore
#define D4D_COLOR_FORE_FOCUS D4D_COLOR_ORANGE
// standard focus color background
#define D4D_COLOR_BCKG_FOCUS D4D_COLOR_WHITE

// standard capturing color fore
#define D4D_COLOR_FORE_CAPTURE D4D_COLOR_BRIGHT_RED
// standard capturing color background
#define D4D_COLOR_BCKG_CAPTURE D4D_COLOR_WHITE


/******************************************************************************
* Button Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_BTN_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT)

//#define D4D_BTN_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_BTN_FNT_PRTY_DEFAULT  ( 0 )
/***********************
* Sizes constants
***********************/

//#define D4D_BTN_BORDER_OFFSET 3

/******************************************************************************
* Gauge Object
******************************************************************************/

/***********************
* Properties
***********************/

#define D4D_GAUGE_F_DEFAULT     (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED  | D4D_GAUGE_F_HUB | D4D_GAUGE_F_THICK_POINTER)

//#define D4D_GAUGE_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_GAUGE_FNT_PRTY_DEFAULT  ( 0 )

/***********************
* Sizes constants
***********************/

#define D4D_GAUGE_HUB_RADIUS 4

/***********************
* Colors
***********************/
// standard gauge colors
#define D4D_COLOR_GAUG_HUB        D4D_COLOR_YELLOW
#define D4D_COLOR_GAUG_POINTER    D4D_COLOR_BRIGHT_YELLOW

/******************************************************************************
* Slider Object
******************************************************************************/

/***********************
* Properties
***********************/


//#define D4D_SLDR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TRANSP_TEXT)

//#define D4D_SLDR_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_SLDR_FNT_PRTY_DEFAULT  ( D4D_FNT_PRTY_TRANSPARENT_YES_MASK )
/***********************
* Sizes constants
***********************/
//#define D4D_SLDR_BAR_OFF_LENGTH  2
//#define D4D_SLDR_BAR_OFF_WIDTH  4

/***********************
* Colors
***********************/
// standard slider colors
//#define D4D_COLOR_SLDR_BAR_FORE   D4D_COLOR_DARK_BLUE
//#define D4D_COLOR_SLDR_BAR_BCKG   D4D_COLOR_GREY
//#define D4D_COLOR_SLDR_BAR_START  D4D_COLOR_WHITE
//#define D4D_COLOR_SLDR_BAR_END    D4D_COLOR_BLACK

/******************************************************************************
* Icon Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_ICON_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)

//#define D4D_ICON_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_ICON_FNT_PRTY_DEFAULT  ( 0 )

/******************************************************************************
* Label Object
******************************************************************************/

/***********************
* Properties
***********************/

#define D4D_LBL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED )

#define D4D_LBL_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#define D4D_LBL_FNT_PRTY_DEFAULT  ( D4D_FNT_PRTY_TRANSPARENT_YES_MASK )

/******************************************************************************
* Menu Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_MENU_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT | D4D_MENU_F_INDEX | D4D_MENU_F_SIDEBAR)

//#define D4D_MENU_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_MENU_FNT_PRTY_DEFAULT  ( 0 )

//#define D4D_MENU_IX_TXT_PRTY_DEFAULT  ( D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK )
//#define D4D_MENU_IX_FNT_PRTY_DEFAULT  ( 0 )

//#define D4D_MENU_ITEM_TXT_PRTY_DEFAULT  ( D4D_TXT_PRTY_ALIGN_V_CENTER_MASK | D4D_TXT_PRTY_ALIGN_H_LEFT_MASK )
//#define D4D_MENU_ITEM_FNT_PRTY_DEFAULT  ( 0 )


/******************************************************************************
* CheckBox Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_CHECKBOX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE\
//                                  | D4D_OBJECT_F_FOCUSRECT | D4D_CHECKBOX_F_ICON_RECTANGLE )

// #define D4D_CHECKBOX_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
// #define D4D_CHECKBOX_FNT_PRTY_DEFAULT  ( 0 )
/***********************
* Sizes constants
***********************/

//#define D4D_CHECKBOX_BORDER_OFFSET 3
//#define D4D_CHECKBOX_TEXT_OFFSET 4


/***********************
* Colors
***********************/
// standard check box colors
//#define D4D_COLOR_CHECKBOX_ICON_BCKG  D4D_COLOR_WHITE


/******************************************************************************
* Graph Object
******************************************************************************/

/***********************
* Properties
***********************/

#define D4D_GRAPH_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_GRAPH_F_MODE_ROLLOVER | D4D_GRAPH_F_VALUE_X_BOTT)

//#define D4D_GRAPH_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_GRAPH_FNT_PRTY_DEFAULT  ( 0 )
//#define D4D_GRAPH_LBL_FNT_PRTY_DEFAULT  ( 0 )

/***********************
* Sizes constants
***********************/
//#define D4D_GRAPH_BORDER_OFF        5
//#define D4D_GRAPH_VALUE_OFF        2


/***********************
* Colors
***********************/
// standard graph grid color
#define D4D_COLOR_GRAPH_GRID  D4D_COLOR_BLACK


/******************************************************************************
* Scroll bar Object
******************************************************************************/

/***********************
* Properties
***********************/

//#define D4D_SCRLBR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT)

/***********************
* Sizes constants
***********************/
//#define #define D4D_SCRLBAR_TAB_SIZE  ( 4 )


/******************************************************************************
* Console Object
******************************************************************************/

/***********************
* Properties
***********************/
//Console default properties
//#define D4D_CNSL_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_OBJECT_F_TOUCHENABLE)

// Console scroll bars default properties
//#define D4D_CNSL_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)

//#define D4D_CNSL_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_CNSL_FNT_PRTY_DEFAULT  ( 0 )

/***********************
* Sizes constants
***********************/
//#define D4D_CNSL_TAB_SIZE  ( 8 )
//#define D4D_CNSL_SCRLBR_WIDTH  ( 16 )

/******************************************************************************
* Text Box Object
******************************************************************************/

/***********************
* Properties
***********************/
//Text Box default properties
#define D4D_TXTBX_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_TABSTOP)

// Text Box scroll bar default properties
//#define D4D_TXTBX_F_SCRLBRS_DEFAULT  (D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_FOCUSRECT)

//#define D4D_TXTBX_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
//#define D4D_TXTBX_FNT_PRTY_DEFAULT  ( 0 )

/***********************
* Sizes constants
***********************/
//#define D4D_TXTBX_SCRLBR_WIDTH  ( 16 )
//#define D4D_TXTBX_SCRLBR_STEP  ( 2 )


#endif /* __D4D_USER_CFG_H */
