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
* @file      d4d_picture.h
*
* @author    r50233
* 
* @version   0.0.25.0
* 
* @date      Jan-19-2011
* 
* @brief     D4D Driver picture object header file
*
*******************************************************************************/

#ifndef __D4D_PICTURE_H
#define __D4D_PICTURE_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_pictSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_PIC_F_DEFAULT
  #define D4D_PIC_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED)
#endif

/******************************************************************************
* Types
******************************************************************************/

// picture configuration (goes to ROM by default)

typedef struct
{
    D4D_POINT scrPos;       // position on the screen (size is taken from bitmap)
    D4D_COOR radius;        // picture corners radius
    const D4D_BMP*  pBmp;
    
} D4D_PICTURE;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the PICTURE structure from general OBJECT
#define D4D_GET_PICTURE(pObj) ((D4D_PICTURE*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_PICTURE(type, name, x, y, radius, pBmp, flags, pUser, pOnUsrMsg) \
    static type D4D_PICTURE name##_params = \
    { \
        { x, y },   /* scrPos */ \
        radius,     /* corners radius */ \
        pBmp        /* pBmp */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_pictSysFunc, &(name##_params), flags, pUser, NULL)

#define D4D_DECLARE_PICTURE(name, x, y, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_CONST, name, x, y, 0, pBmp, flags, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_PICTURE(name, x, y, pBmp) \
  D4D_DECLARE_PICTURE(name, x, y, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)

// Rounded button definition
#define D4D_DECLARE_RPICTURE(name, x, y, radius, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_CONST, name, x, y, radius, pBmp, flags, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RPICTURE(name, x, y, pBmp) \
  D4D_DECLARE_RPICTURE(name, x, y, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)


//IN RAM instantions macros
#define D4D_DECLARE_PICTURE_INRAM(name, x, y, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_NO_CONST, name, x, y, pBmp, flags, pUser, pOnUsrMsg)
    
#define D4D_DECLARE_STD_PICTURE_INRAM(name, x, y, pBmp) \
  D4D_DECLARE_PICTURE_INRAM(name, x, y, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)
  
// Rounded button definition
#define D4D_DECLARE_RPICTURE_INRAM(name, x, y, radius, pBmp, flags, pUser, pOnUsrMsg) \
        _D4D_DECLARE_PICTURE(D4D_NO_CONST, name, x, y, radius, pBmp, flags, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_RPICTURE_INRAM(name, x, y, pBmp) \
  D4D_DECLARE_RPICTURE_INRAM(name, x, y, pBmp, (D4D_PIC_F_DEFAULT), NULL, NULL)  


/******************************************************************************
* Global variables
******************************************************************************/
   


/******************************************************************************
* Global functions
******************************************************************************/



#endif /* __D4D_PICTURE_H */