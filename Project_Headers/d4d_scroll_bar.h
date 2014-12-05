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
* @file      d4d_scroll_bar.h
*
* @author    b01119
* 
* @version   0.0.7.0
* 
* @date      Jan-27-2011
* 
* @brief     D4D Driver label object header file
*
*******************************************************************************/

#ifndef __D4D_SCROLL_BAR_H
#define __D4D_SCROLL_BAR_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_scroll_barSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#ifndef D4D_SCRLBAR_MIN_TAB_SIZE
  #define D4D_SCRLBAR_MIN_TAB_SIZE  ( 4 )
#endif  

#ifndef D4D_SCRLBR_F_DEFAULT
  #define D4D_SCRLBR_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TABSTOP | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FOCUSRECT)
#endif


typedef struct D4D_SCRLBAR_DATA_S
{
	D4D_INDEX position;								// index of last line 
	D4D_INDEX minimum;
	D4D_INDEX maximum;
	D4D_INDEX step;	               
	D4D_INDEX page;   
}D4D_SCRLBAR_DATA;

// label configuration (goes to ROM by default)

typedef struct
{
    D4D_POINT scrPos;        // position on the screen 
    D4D_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)    
    D4D_COOR  radius;        // corner radius
    void (*OnChange)(D4D_OBJECT* pThis, D4D_INDEX old_position, D4D_INDEX new_position);
    D4D_SCRLBAR_DATA* pData;		 // Run time data needed for ScrlBr 	
} D4D_SCROLL_BAR;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the SCROLL_BAR structure from general OBJECT
#define D4D_GET_SCROLL_BAR(pObj) ((D4D_SCROLL_BAR*)((pObj)->pParam))
#define D4D_GET_SCROLL_BAR_DATA(pObj) (D4D_GET_SCROLL_BAR(pObj)->pData)

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_SCROLL_BAR(type, name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg) \
    static D4D_SCRLBAR_DATA name##_data = { 0, 0, 100, 1, 0};\
    static type D4D_SCROLL_BAR name##_params = \
    { \
        { x, y },   /* scrPos */ \
        { cx, cy }, /* scrSize */ \
        radius,     /* radius */ \
        pOnChange, /* OnChange*/ \
        &name##_data, /* pData */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_scroll_barSysFunc, &(name##_params), flags, pUser, pScheme)

#define D4D_DECLARE_SCROLL_BAR(name, x, y, cx, cy, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_CONST, name, x, y, cx, cy, 0, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

#define D4D_DECLARE_STD_SCROLL_BAR(name, x, y, cx, cy, pOnChange)\
        D4D_DECLARE_SCROLL_BAR(name, x, y, cx, cy, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)

// Rounded scroll bar definition
#define D4D_DECLARE_RSCROLL_BAR(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_CONST, name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

#define D4D_DECLARE_STD_RSCROLL_BAR(name, x, y, cx, cy, radius, pOnChange)\
        D4D_DECLARE_RSCROLL_BAR(name, x, y, cx, cy, radius, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)


// IN RAM instantions macros
#define D4D_DECLARE_SCROLL_BAR_INRAM(name, x, y, cx, cy, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_NO_CONST, name, x, y, cx, cy, 0, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

#define D4D_DECLARE_STD_SCROLL_BAR_INRAM(name, x, y, cx, cy, pOnChange)\
        D4D_DECLARE_SCROLL_BAR_INRAM(name, x, y, cx, cy, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)
 

// Rounded scroll bar definition
#define D4D_DECLARE_RSCROLL_BAR_INRAM(name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg)\
        _D4D_DECLARE_SCROLL_BAR(D4D_NO_CONST, name, x, y, cx, cy, radius, flags, pScheme, pUser, pOnChange, pOnUsrMsg)

#define D4D_DECLARE_STD_RSCROLL_BAR_INRAM(name, x, y, cx, cy, radius, pOnChange)\
        D4D_DECLARE_RSCROLL_BAR_INRAM(name, x, y, cx, cy, radius, D4D_SCRLBR_F_DEFAULT, NULL, NULL, pOnChange, NULL)


/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/

void D4D_ScrlBrSetRange(D4D_OBJECT_PTR pObj, D4D_INDEX minimum, D4D_INDEX maximum); 
void D4D_ScrlBrSetStep(D4D_OBJECT_PTR pObj, D4D_INDEX page, D4D_INDEX step);
void D4D_ScrlBrSetPosition(D4D_OBJECT_PTR pObj, D4D_INDEX position);
D4D_INDEX D4D_ScrlBrGetPosition(D4D_OBJECT_PTR pObj);
void D4D_ScrlBrChangePosition(D4D_OBJECT_PTR pObj, D4D_INDEX_DELTA change);

#endif /* __D4D_SCROLL_BAR_H */