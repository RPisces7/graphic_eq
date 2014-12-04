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
* @file      d4d_object.h
*
* @author    b01119
* 
* @version   0.0.11.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver object functions header file
*
*******************************************************************************/

#ifndef __D4D_OBJECT_H
#define __D4D_OBJECT_H

/******************************************************************************
* Includes
******************************************************************************/


typedef union 
{
    D4D_BIT_FIELD all;
    
    struct
    {
        unsigned bVisible     :1;     // visible at all 
        unsigned bEnabled     :1;     // Enabled / Disabled option 
        unsigned bTabStop     :1;     // can be focused 
        unsigned bTouchEnable :1;     // Object is enabled for touchscreen
        unsigned bFastTouch   :1;     // Only one touch to action
        unsigned bRedraw      :1;     // needs redraw
        unsigned bRedrawC     :1;     // use complete redraw
        unsigned bInitDone    :1;     // object initialization done         
    } bits;
} D4D_OBJECT_FLAGS;

typedef Word D4D_OBJECT_INITFLAGS;

#if D4D_BITFIELD_LSB_ALIGNMENT == D4D_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4D_OBJECT_F_VISIBLE       0x01
  #define D4D_OBJECT_F_ENABLED       0x02
  #define D4D_OBJECT_F_TABSTOP       0x04
  #define D4D_OBJECT_F_TOUCHENABLE   0x08
  #define D4D_OBJECT_F_FASTTOUCH     0x10
  #define D4D_OBJECT_F_TRANSP_TEXT   0x20
  #define D4D_OBJECT_F_FOCUSRECT     0x40
#else
  #define D4D_OBJECT_F_VISIBLE       0x80
  #define D4D_OBJECT_F_ENABLED       0x40
  #define D4D_OBJECT_F_TABSTOP       0x20
  #define D4D_OBJECT_F_TOUCHENABLE   0x10
  #define D4D_OBJECT_F_FASTTOUCH     0x08
  #define D4D_OBJECT_F_TRANSP_TEXT   0x04
  #define D4D_OBJECT_F_FOCUSRECT     0x02
#endif

  #define D4D_OBJECT_F_SYSTEM        0x00FF
  #define D4D_OBJECT_F_INIT          0xFF00 


typedef struct D4D_OBJECT_SYS_FUNCTION_S
{
  D4D_OBJECT_TYPE type;
  void (*OnSysMessage)(struct D4D_MESSAGE_S* pMsg);
  Byte (*CheckCoordinates)(struct D4D_OBJECT_S* pObj, D4D_POINT point);
  struct D4D_STRING_S* (*GetTextBuffer)(struct D4D_OBJECT_S* pObj);            
}D4D_OBJECT_SYS_FUNCTION;

struct D4D_SCREEN_S;
struct D4D_MESSAGE_S;
struct D4D_CLR_SCHEME_S;

// this is allocated in ROM always
typedef struct D4D_OBJECT_S
{
    void* pParam;
    D4D_OBJECT_SYS_FUNCTION* pObjFunc; 
    Byte (*OnUsrMessage)(struct D4D_MESSAGE_S* pMsg);    
    void *userPointer;
    D4D_OBJECT_INITFLAGS initFlags;
    D4D_OBJECT_FLAGS*  flags;
    struct D4D_CLR_SCHEME_S* clrScheme;
    struct D4D_SCREEN_S** pScreen;
} D4D_OBJECT;

typedef const D4D_OBJECT* D4D_OBJECT_PTR;

/******************************************************************************
* Macros
******************************************************************************/
    
#define D4D_EXTERN_OBJECT(name) \
    extern const D4D_OBJECT name

/*********************************************************
*
* static declaration macros
*
*********************************************************/
#define D4D_DECLARE_OBJECT(name, onusrmsg, sysFunc , param, initFlags, usrPointer, pScheme) \
    D4D_OBJECT_FLAGS name##_flags = { (((D4D_BIT_FIELD)(initFlags & D4D_OBJECT_F_SYSTEM)) << D4D_BITFIELD_SHIFT) }; \
    D4D_SCREEN *name##_pScreen; \
    const D4D_OBJECT name = \
    { \
        (void*)param, \
        (D4D_OBJECT_SYS_FUNCTION*)sysFunc, \
        onusrmsg, \
        usrPointer, /* User Pointer */ \
        initFlags, \
        &(name##_flags), \
        pScheme, \
        &(name##_pScreen) \
    };    
    
#endif  /* __D4D_OBJECT_H */