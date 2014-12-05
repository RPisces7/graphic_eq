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
* @file      d4dlcdhw_mqx_mpc5125_diu.h
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Jul-23-2010
* 
* @brief     D4D driver - mqx_mpc5125_diu hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4DLCDHW_MQX_MPC5125_DIU_H
#define __D4DLCDHW_MQX_MPC5125_DIU_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_mpc5125_diu_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    
    #include "d4dlcdhw_mqx_mpc5125_diu_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
		
		#ifndef D4DLCDHWFB_X 
			#define D4DLCDHWFB_X 1024
		#endif	
		   
		#ifndef D4DLCDHWFB_Y 
			#define D4DLCDHWFB_Y 768
		#endif	
				
		#ifndef D4DLCDHWFB_REFRESH 
			#define D4DLCDHWFB_REFRESH 60
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
#endif /* __D4DLCDHW_MQX_MPC5125_DIU_H */










