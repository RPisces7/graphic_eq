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
* @file      d4dtch_resistive.h
*
* @author    b01119
* 
* @version   0.0.9.0
* 
* @date      Apr-30-2010
* 
* @brief     D4D driver - resistive touch screen driver function header file 
*
*******************************************************************************/

#ifndef __D4DTCH_RESISTIVE_H
#define __D4DTCH_RESISTIVE_H

  #if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_resistive_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtch_resistive_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    /****** Algorithm constants *****/
    #ifndef AVERAGE_BUFF_LEN
      #define AVERAGE_BUFF_LEN 6
    #endif
   
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
#endif /* __D4DTCH_RESISTIVE_H */










