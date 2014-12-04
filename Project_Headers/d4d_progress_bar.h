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
* @file      d4d_progress_bar.h
*
* @author    b01119
* 
* @version   0.0.2.0
* 
* @date      Jan-25-2011
* 
* @brief     D4D Driver progress_bar object header file
*
*******************************************************************************/

#ifndef __D4D_PROGRESS_BAR_H
#define __D4D_PROGRESS_BAR_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_progress_barSysFunc;
/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_PRGRS_BAR_BAR_OFF
  #define D4D_PRGRS_BAR_BAR_OFF  5
#endif


// standard progress_bar colors
#ifndef D4D_COLOR_PRGRS_BAR_BAR_FORE  
  #define D4D_COLOR_PRGRS_BAR_BAR_FORE    D4D_COLOR_DARK_GREEN
#endif

#ifndef D4D_COLOR_PRGRS_BAR_BAR_END
  #define D4D_COLOR_PRGRS_BAR_BAR_END  D4D_COLOR_YELLOW
#endif

#ifndef D4D_COLOR_PRGRS_BAR_BAR_BCKG  
  #define D4D_COLOR_PRGRS_BAR_BAR_BCKG   D4D_COLOR_BRIGHT_GREY
#endif




#ifndef D4D_COLOR_PRGRS_BAR_MAX_VAL
  #define D4D_COLOR_PRGRS_BAR_MAX_VAL  100
#endif


/******************************************************************************
* Types
******************************************************************************/

typedef Byte D4D_PROGRESS_BAR_VALUE;

#define D4D_PRGRS_BAR_F_BAR_AUTOCOLOR 0x0100
#define D4D_PRGRS_BAR_F_TEXT_AUTOCOLOR 0x0200
#define D4D_PRGRS_BAR_F_BAR_SCALECOLOR 0x0400

#define D4D_PRGRS_BAR_F_AUTOCOLOR 0x0300


#ifndef D4D_PRGRS_BAR_F_DEFAULT
  #define D4D_PRGRS_BAR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED  | D4D_PRGRS_BAR_F_BAR_AUTOCOLOR | D4D_PRGRS_BAR_F_BAR_SCALECOLOR)
#endif



// progress_bar runtime data (always in RAM)

typedef struct
{
    D4D_PROGRESS_BAR_VALUE value;       // current value
    D4D_BOOL drawActiveOnly;    // old Value
    D4D_COLOR colorBar;                 // Color v Bar - can be changed
} D4D_PROGRESS_BAR_DATA;

// progress_bar configuration (in ROM by default)

typedef struct
{
    D4D_POINT scrPos;           // position on the screen
    D4D_SIZE  scrSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4D_COOR radius;            // radius of extrinsic radius of rounded corners
    D4D_PROGRESS_BAR_DATA* pData;
} D4D_PROGRESS_BAR;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the PROGRESS_BAR structure from general OBJECT
#define D4D_GET_PROGRESS_BAR(pObj) ((D4D_PROGRESS_BAR*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_PROGRESS_BAR(type, name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg) \
    static D4D_PROGRESS_BAR_DATA name##_data = { 0, 0, D4D_COLOR_PRGRS_BAR_BAR_FORE}; \
    static type D4D_PROGRESS_BAR name##_params = \
    { \
        { x, y },    /* scrPos     */ \
        { cx, cy },  /* scrSize    */ \
        radius,      /* radius    */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_progress_barSysFunc, &(name##_params), flags, pUser, pScheme)


#define D4D_DECLARE_PROGRESS_BAR(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg) \
  _D4D_DECLARE_PROGRESS_BAR(D4D_CONST, name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_PROGRESS_BAR(name, x, y, cx, cy, radius) \
    D4D_DECLARE_PROGRESS_BAR(name, x, y, cx, cy, radius, (D4D_PRGRS_BAR_F_DEFAULT), NULL, NULL, NULL)


// IN RAM instantions macros
#define D4D_DECLARE_PROGRESS_BAR_INRAM(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg) \
  _D4D_DECLARE_PROGRESS_BAR( D4D_NO_CONST, name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnUsrMsg)

#define D4D_DECLARE_STD_PROGRESS_BAR_INRAM(name, x, y, cx, cy, radius) \
    D4D_DECLARE_PROGRESS_BAR_INRAM(name, text, x, y, cx, cy, radius, (D4D_PRGRS_BAR_F_DEFAULT), NULL, NULL, NULL)

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/

void D4D_PrgrsBarSetValue(D4D_OBJECT_PTR pThis, D4D_PROGRESS_BAR_VALUE value);
D4D_PROGRESS_BAR_VALUE D4D_PrgrsBarGetValue(D4D_OBJECT_PTR pThis);
void D4D_PrgrsBarChangeValue(D4D_OBJECT_PTR pThis, D4D_PROGRESS_BAR_VALUE incr);

void D4D_PrgrsBarSetBarColor(D4D_OBJECT_PTR pThis, D4D_COLOR color);
D4D_COLOR D4D_PrgrsBarGetBarColor(D4D_OBJECT_PTR pThis);



#endif /* __D4D_PROGRESS_BAR_H */











