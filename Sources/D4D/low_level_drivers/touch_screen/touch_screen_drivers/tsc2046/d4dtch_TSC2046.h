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
* @file      d4dtch_TSC2046.h
*
* @author    b01119
* 
* @version   0.0.9.0
* 
* @date      May-7-2010
* 
* @brief     D4D driver - tsc2046 touch screen driver function header file 
*
*******************************************************************************/

#ifndef __D4DTCH_TSC2046_H
#define __D4DTCH_TSC2046_H

  #if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtch_tsc2046_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DTCH_TSC2046_H */











#if 0  // will be implemented ASAP, Petr Gargulak

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
* @file      d4dtch_TSC2046.h
*
* @author    b01119
* 
* @version   0.0.8.0
* 
* @date      Apr-30-2010
* 
* @brief     D4D driver - TSC2046 touch screen driver function header file 
*
*******************************************************************************/

/******************************************************************************
* Driver under development
******************************************************************************/


#ifndef __D4DTCH_TSC2046_H
#define __D4DTCH_TSC2046_H

#if (D4D_LLD_TCH == D4D_LLD_TCHSCR_TSC2046)

  /******************************************************************************
  * Includes
  ******************************************************************************/
  #include "d4dtch_TSC2046_cfg.h"
  /******************************************************************************
  * Constants
  ******************************************************************************/
  
  #define D4DTCH_AXIS_ORIENT_NORMAL 0
  #define D4DTCH_AXIS_ORIENT_INVERTED 1
  
  
  // define Channels
  #define TSC2046_CHNL_DIF_Y 1
  #define TSC2046_CHNL_DIF_X 5
  
  
  #define TSC_2046_CTRL_BYTE_DIF12_DEFAULT 0x80
  
    #if D4DTCH_SERIAL_OUTPUT_SPI_IX == 1 
      #define D4DLCD_SPIBR SPI1BR
      #define D4DLCD_SPIC1 SPI1C1
      #define D4DLCD_SPIC2 SPI1C2
      #define D4DLCD_SPID SPI1D
      #define D4DLCD_SPIS SPI1S
      #define D4DLCD_SPIS_SPTEF SPI1S_SPTEF
      #define D4DLCD_SPIS_SPRF  SPI1S_SPRF
    #elif D4DTCH_SERIAL_OUTPUT_SPI_IX == 2
      #define D4DLCD_SPIBR SPI2BR
      #define D4DLCD_SPIC1 SPI2C1
      #define D4DLCD_SPIC2 SPI2C2
      #define D4DLCD_SPID SPI2D
      #define D4DLCD_SPIS SPI2S      
      #define D4DLCD_SPIS_SPTEF SPI2S_SPTEF
      #define D4DLCD_SPIS_SPRF  SPI2S_SPRF
    #endif 

  /******************************************************************************
  * Types
  ******************************************************************************/
  typedef union{
    unsigned int all;
    struct{
      unsigned PD0 : 1;
      unsigned PD1 : 1;
      unsigned SER_DFR : 1;
      unsigned MODE : 1;
      unsigned A0 : 1;
      unsigned A1 : 1;
      unsigned A2 : 1;
      unsigned S : 1;
    }Bits;
    struct{
      unsigned PD : 2;
      unsigned SER_DFR : 1;
      unsigned MODE : 1;
      unsigned A : 3;
      unsigned S : 1;
    }MergedBits;  
  }TSC2046_CTRL_BYTE;
  

  /******************************************************************************
  * Macros 
  ******************************************************************************/


  /******************************************************************************
  * Global variables
  ******************************************************************************/

  /******************************************************************************
  * Global functions
  ******************************************************************************/

 
#endif
#endif /* __D4DTCH_TSC2046_H */

#endif








