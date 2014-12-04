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
* @file      d4d_scheme.c
*
* @author    b01119
* 
* @version   0.0.9.0
* 
* @date      Jan-24-2011
* 
* @brief     D4D driver color scheme c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"


D4D_DECLARE_CLR_SCHEME(d4d_clr_scheme_default, \
        D4D_COLOR_SCR_DESKTOP, D4D_COLOR_SCR_OUTLINE, D4D_COLOR_SCR_TITLEBAR, D4D_COLOR_SCR_TILTLETEXT, D4D_COLOR_SCR_EXIT_BTN_FORE, D4D_COLOR_SCR_EXIT_BTN_BCKG,\
        D4D_COLOR_BCKG_NORM, D4D_COLOR_BCKG_DISABLED, D4D_COLOR_BCKG_FOCUS, D4D_COLOR_BCKG_CAPTURE,\
        D4D_COLOR_FORE_NORM, D4D_COLOR_FORE_DISABLED, D4D_COLOR_FORE_FOCUS, D4D_COLOR_FORE_CAPTURE,\
        D4D_COLOR_GAUG_HUB, D4D_COLOR_GAUG_POINTER,\
        D4D_COLOR_SLDR_BAR_BCKG, D4D_COLOR_SLDR_BAR_FORE, D4D_COLOR_SLDR_BAR_START, D4D_COLOR_SLDR_BAR_END,\
        D4D_COLOR_CHECKBOX_ICON_BCKG,\
        D4D_COLOR_GRAPH_GRID,\
        D4D_COLOR_PRGRS_BAR_BAR_BCKG, D4D_COLOR_PRGRS_BAR_BAR_FORE, D4D_COLOR_PRGRS_BAR_BAR_END\
        )

static D4D_CLR_SCHEME *pSchemeDefault = (D4D_CLR_SCHEME*)&d4d_clr_scheme_default; 
        

D4D_CLR_SCHEME* D4D_ObjectGetScheme(D4D_OBJECT * pObj)
{ 
  if(pObj == NULL)
    return pSchemeDefault;
  
  if(pObj->clrScheme != NULL)
    return pObj->clrScheme; 
  else
    return D4D_ScreenGetScheme(*pObj->pScreen);  
}
        
D4D_CLR_SCHEME* D4D_ScreenGetScheme(D4D_SCREEN * pScreen)
{ 
  if(pScreen == NULL)
    return pSchemeDefault;
  
  if(pScreen->clrScheme != NULL)
    return pScreen->clrScheme; 
  else
    return pSchemeDefault;    
}

D4D_CLR_SCHEME* D4D_GetDefaultScheme(void)
{
  return pSchemeDefault;  
}

void D4D_SetDefaultScheme(D4D_CLR_SCHEME* pScheme)
{
  if(pScheme == NULL)
    pSchemeDefault = (D4D_CLR_SCHEME*)&d4d_clr_scheme_default;
  else
    pSchemeDefault = pScheme;
}


 
        
D4D_COLOR D4D_ObjectGetForeColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);
  
  if(!pObj->flags->bits.bEnabled)
    return pScheme->foreDis;
  
  if(draw.bits.bCapturing)
    return pScheme->foreCapture;
  
  if(draw.bits.bFocused)
    return pScheme->foreFocus;
  
  return pScheme->fore;
}

D4D_COLOR D4D_ObjectGetBckgColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);
  
  if(!pObj->flags->bits.bEnabled)
    return pScheme->bckgDis;
  
  if(draw.bits.bCapturing)
    return pScheme->bckgCapture;
  
  if(draw.bits.bFocused)
    return pScheme->bckgFocus;
  
  return pScheme->bckg;
}

D4D_COLOR D4D_ObjectGetForeFillColor(D4D_OBJECT * pObj)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);
  
  if(!pObj->flags->bits.bEnabled)
    return pScheme->foreDis;
  
  return pScheme->fore;
}

D4D_COLOR D4D_ObjectGetBckgFillColor(D4D_OBJECT * pObj)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);
  
  if(!pObj->flags->bits.bEnabled)
    return pScheme->bckgDis;
  
  return pScheme->bckg;
}