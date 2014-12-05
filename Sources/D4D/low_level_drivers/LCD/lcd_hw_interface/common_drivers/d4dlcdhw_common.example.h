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
* @file      d4dlcdhw_common.example.h
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Apr-26-2010
* 
* @brief     D4D low level driver SSD1289 hardware level LCD header configuration file 
*
*******************************************************************************/

#ifndef __D4DLCD_SSD1289_CFG_H
#define __D4DLCD_SSD1289_CFG_H

  /******************************************************************************
  * includes
  ******************************************************************************/
  
  #include "derivative.h"       /* include peripheral declarations and more for S08 and CV1 */
  //#include "support_common.h" /* include peripheral declarations and more for CV2 */


  /******************************************************************************
  * Constants
  ******************************************************************************/
 
  
  // Define the index of used communication interface
  #define D4DLCD_COMM_HW_ID 1
  

  
//#define D4DLCD_DISPLAY_MCU_USER_INIT;
  
  
 
  /*******************************************
  * Signals definition
  *******************************************/
  #define D4DLCD_CS              7         // CS   
  #define D4DLCD_CS_PORT         PTED      // CS 
  #define D4DLCD_CS_DDR          PTEDD     // CS    

  #define D4DLCD_DC              3         // DC 
  #define D4DLCD_DC_PORT         PTFD      // DC 
  #define D4DLCD_DC_DDR          PTFDD     // DC 
    

  /*********** Power macros - for modification uncoment and modify ***********/
  
  //#define D4DLCD_INIT_CS OUTPUT(D4DLCD_CS);
  //#define D4DLCD_ASSERT_CS RESET(D4DLCD_CS);
  //#define D4DLCD_DEASSERT_CS SET(D4DLCD_CS);
  
  
  //#define D4DLCD_INIT_DC OUTPUT(D4DLCD_DC);
  //#define D4DLCD_ASSERT_DC RESET(D4DLCD_DC);
  //#define D4DLCD_DEASSERT_DC SET(D4DLCD_DC);

#endif /* __D4DLCD_SSD1289_CFG_H */









