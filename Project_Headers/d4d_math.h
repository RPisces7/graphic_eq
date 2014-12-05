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
* @file      d4d_math.h
*
* @author    r50233
* 
* @version   0.0.14.0
* 
* @date      Feb-2-2011
* 
* @brief     D4D Driver math helper functions header file
*
*******************************************************************************/

#ifndef __D4D_MATH_H
#define __D4D_MATH_H

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/

// convert deg angle of -180..180 to internal representation (-128..127)
#define D4D_ANGLE_DEG(deg) (((deg)==180)?0xff:((deg)*0x100/180))

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

sByte D4D_Sin(Byte x);
sByte D4D_Cos(Byte x);

Byte D4D_MulDivUU8(Byte u1, Byte u2, Byte d);
sByte D4D_MulDivSU8(sByte s, Byte u, Byte d);

Word D4D_MulDivUU16(Word u1, Word u2, Word d);
sWord D4D_MulDivSU16(sWord s, Word u, Word d);

Word D4D_Abs16(sWord num);

Word D4D_LimitU16(Word val, Word min, Word max);
sWord D4D_LimitS16(sWord val, sWord min, sWord max);

Byte D4D_LimitU8(Byte val, Byte min, Byte max);
sByte D4D_LimitS8(sByte val, sByte min, sByte max);

#ifdef D4D_COOR_TYPE
  
  #define D4D_MulDivUU  D4D_MulDivUU16
  #define D4D_MulDivSU  D4D_MulDivSU16
  #define D4D_MULDIVU_TYPE  Word
  #define D4D_MULDIVS_TYPE  sWord

  #define D4D_LimitU  D4D_LimitU16    
  #define D4D_LimitS  D4D_LimitS16
#else
  #define D4D_MulDivUU  D4D_MulDivUU8
  #define D4D_MulDivSU  D4D_MulDivSU8
  #define D4D_MULDIVU_TYPE  Byte
  #define D4D_MULDIVS_TYPE  sByte
  
  #define D4D_LimitU  D4D_LimitU8    
  #define D4D_LimitS  D4D_LimitS8
#endif


#define D4D_LIMITU_TYPE  D4D_MULDIVU_TYPE
#define D4D_LIMITS_TYPE  D4D_MULDIVS_TYPE

#endif /* __D4D_MATH_H */