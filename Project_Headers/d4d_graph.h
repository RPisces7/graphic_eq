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
* @file      d4d_graph.h
*
* @author    b01119
* 
* @version   0.0.19.0
* 
* @date      Feb-23-2011
* 
* @brief     D4D Driver graph object header file
*
*******************************************************************************/

#ifndef __D4D_GRAPH_H
#define __D4D_GRAPH_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_graphSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4D_GRAPH_F_DEFAULT
  #define D4D_GRAPH_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_FOCUSRECT | D4D_GRAPH_F_MODE_ROLLOVER | D4D_GRAPH_F_VALUE_X_BOTT | D4D_GRAPH_F_VALUE_Y_RIGHT  )
#endif

#ifndef D4D_GRAPH_TXT_PRTY_DEFAULT
  #define D4D_GRAPH_TXT_PRTY_DEFAULT  (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
#endif

#ifndef D4D_GRAPH_FNT_PRTY_DEFAULT
  #define D4D_GRAPH_FNT_PRTY_DEFAULT  ( 0 )
#endif

#ifndef D4D_GRAPH_LBL_FNT_PRTY_DEFAULT
  #define D4D_GRAPH_LBL_FNT_PRTY_DEFAULT  ( 0 )
#endif


// standard graph colors
#ifndef D4D_COLOR_GRAPH_GRID  
  #define D4D_COLOR_GRAPH_GRID        D4D_COLOR_LIGHT_GREY
#endif

#ifndef D4D_GRAPH_BORDER_OFF  
  #define D4D_GRAPH_BORDER_OFF        5
#endif

#ifndef D4D_GRAPH_VALUE_OFF  
  #define D4D_GRAPH_VALUE_OFF        2
#endif


#define D4D_GRAPH_F_MODE_MASK       0x0300
#define D4D_GRAPH_F_MODE_NORMAL     0x0000
#define D4D_GRAPH_F_MODE_ROLLOVER   0x0100

#define D4D_GRAPH_F_VALUE_X_BOTT    0x0400
#define D4D_GRAPH_F_VALUE_X_TOP     0x0800

#define D4D_GRAPH_F_VALUE_Y_LEFT    0x1000
#define D4D_GRAPH_F_VALUE_Y_RIGHT   0x2000


#define D4D_GRAPH_TRACE_TYPE_MASK       0x03
#define D4D_GRAPH_TRACE_TYPE_LINE       0x00
#define D4D_GRAPH_TRACE_TYPE_DOT        0x01
#define D4D_GRAPH_TRACE_TYPE_AREA       0x02
#define D4D_GRAPH_TRACE_TYPE_AREA_INV   0x03


/******************************************************************************
* Types
******************************************************************************/



typedef Byte D4D_GRAPH_VALUE;
typedef Word D4D_GRAPH_DATA_LEN;
typedef LWord D4D_GRAPH_SAMPLE_IX;

typedef struct
{
  D4D_GRAPH_VALUE *pData;
  D4D_COLOR color;
  D4D_LINETYPE line;
  Byte type;
} D4D_GRAPH_TRACE;

typedef struct
{
    D4D_GRAPH_SAMPLE_IX xPosCnt;      // position counter on X axis
    D4D_GRAPH_DATA_LEN lastShowPos;   // position in input data buffer of shown data
    D4D_GRAPH_DATA_LEN lastDataPos;   // position in input data buffer of last added data 
    D4D_GRAPH_DATA_LEN initData;
    D4D_GRAPH_DATA_LEN dataShowX; // count of showed data on axis X 
    D4D_COOR lblSizeY;       
    D4D_COOR columnDrawed;
    D4D_INDEX drigDrawedX;
} D4D_GRAPH_DATA;

typedef struct
{
  D4D_INDEX x_cnt;
  D4D_INDEX y_cnt;    
}D4D_GRAPH_GRID;

// GRAPH configuration (in ROM by default)
  
typedef struct
{
    D4D_STRING textBuff;       // GRAPH main text
    D4D_POINT scrPos;           // position on the screen
    D4D_SIZE  scrSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4D_COOR radius;            // radius
    D4D_GRAPH_GRID grid;        // graph grid information   
    const D4D_GRAPH_TRACE* traces;
    D4D_GRAPH_DATA_LEN dataLenght;
    D4D_FONT labelFont;
    char* (*OnNeedLabelText)(D4D_OBJECT_PTR pThis, D4D_BOOL axisX, D4D_INDEX gridIx, D4D_GRAPH_SAMPLE_IX sampleIx);
    void (*OnValueChanged)(D4D_OBJECT_PTR pThis);
    D4D_GRAPH_DATA* pData;    
} D4D_GRAPH;
  
/******************************************************************************
* Macros 
******************************************************************************/

// getting the GRAPH structure from general OBJECT
#define D4D_GET_GRAPH(pObj) ((D4D_GRAPH*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define _D4D_DECLARE_GRAPH_BEGIN(type, name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
    extern type D4D_GRAPH_TRACE  name##_traces[];\
    static D4D_GRAPH_DATA name##_data = { 0, 0, 0, 0, 0, 0, 0, 0}; /* reset data of graph*/ \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_GRAPH_FNT_PRTY_DEFAULT  << D4D_BITFIELD_SHIFT, D4D_GRAPH_TXT_PRTY_DEFAULT << D4D_BITFIELD_SHIFT}; \
    static type D4D_GRAPH name##_params = \
    { \
        { text, sizeof(text), fontId, &name##_strPrties}, /* textBuff */ \
        { x, y },     /* scrPos     */ \
        { cx, cy },   /* scrSize    */ \
        radius,       /* radius */ \
        { gx, gy },   /* grid info  */ \
        &(name##_traces[0]),  /* pLines     */ \
        dataLen,      /* Length of input data buffers */ \
        lblFontId,    /* Font ID for axis values */ \
        pOnNeedLblTxt, /* On need the label text*/ \
        pOnValch,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(name, pOnUsrMsg, &d4d_graphSysFunc, &(name##_params), (flags), pUser, pScheme)\
    \
    type D4D_GRAPH_TRACE name##_traces[] = \
    {
    
#define D4D_DECLARE_GRAPH_TRACE(pData, clr, line, type) { pData, clr , line, type},

#define D4D_DECLARE_GRAPH_END() { NULL, D4D_COLOR_WHITE , D4D_LINE_THIN, 0}};


#define D4D_DECLARE_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_CONST, name, text, x, y, cx, cy, 0, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

#define D4D_DECLARE_STD_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, fontId, valFontId) \
  D4D_DECLARE_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, valFontId, NULL, NULL, NULL, NULL)

// Rounded graph definition
#define D4D_DECLARE_RGRAPH_BEGIN(name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_CONST, name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

#define D4D_DECLARE_STD_RGRAPH_BEGIN(name, text, x, y, cx, cy, radius, gx, gy, dataLen, fontId, valFontId) \
  D4D_DECLARE_RGRAPH_BEGIN(name, text, x, y, cx, cy, radius, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, valFontId, NULL, NULL, NULL, NULL)


// IN RAM instantions macros
#define D4D_DECLARE_GRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, 0, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

#define D4D_DECLARE_STD_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, fontId, valFontId) \
  D4D_DECLARE_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, valFontId, NULL, NULL, NULL, NULL)


// Rounded graph definition
#define D4D_DECLARE_RGRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

#define D4D_DECLARE_STD_RGRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, radius, gx, gy, dataLen, fontId, valFontId) \
  D4D_DECLARE_RGRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, radius, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, valFontId, NULL, NULL, NULL, NULL)

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/
D4D_BOOL D4D_GraphAddTraceData(D4D_OBJECT_PTR pObj, D4D_INDEX trace_ix,  D4D_GRAPH_VALUE value);
void D4D_GraphAddTracesData(D4D_OBJECT_PTR pObj, D4D_GRAPH_VALUE* pValues);
void D4D_GraphClearAll(D4D_OBJECT_PTR pObj);
D4D_GRAPH_SAMPLE_IX D4D_GraphGetSampleIndex(D4D_OBJECT_PTR pObj);
D4D_COOR D4D_GraphGetSizeX(D4D_OBJECT_PTR pObj);
D4D_BOOL D4D_GraphSetScaleX(D4D_OBJECT_PTR pObj, Byte mul, Byte div);
D4D_BOOL D4D_GraphSetDataWidth(D4D_OBJECT_PTR pObj, D4D_INDEX samples);


// Obsolete functions, replaced by any general
#define D4D_GraphSetText D4D_SetText



                                     
#endif /* __D4D_GRAPH_H */        


