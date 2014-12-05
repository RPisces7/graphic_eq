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
* @file      d4d_math.c
*
* @author    r50233
* 
* @version   0.0.23.0
* 
* @date      Feb-2-2011
* 
* @brief     D4D driver math functions c file 
*
******************************************************************************/

#include "d4d.h"
#include "d4d_private.h"

const sByte d4d_sinTbl[64] = 
{
    0x00,
    0x03,
    0x06,
    0x09,
    0x0D,
    0x10,
    0x13,
    0x16,
    0x19,
    0x1C,
    0x1F,
    0x22,
    0x25,
    0x28,
    0x2B,
    0x2E,
    0x31,
    0x34,
    0x37,
    0x3A,
    0x3D,
    0x40,
    0x42,
    0x45,
    0x48,
    0x4A,
    0x4D,
    0x4F,
    0x52,
    0x54,
    0x56,
    0x59,
    0x5B,
    0x5D,
    0x5F,
    0x61,
    0x63,
    0x65,
    0x67,
    0x69,
    0x6B,
    0x6C,
    0x6E,
    0x70,
    0x71,
    0x72,
    0x74,
    0x75,
    0x76,
    0x77,
    0x78,
    0x79,
    0x7A,
    0x7B,
    0x7C,
    0x7C,
    0x7D,
    0x7D,
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7F,
    0x7F
};

/***************************************************************************//*!
*
* @brief  Fast 16/8=8 divide operation with rounding and saturation
*
* @param num The 16-bit numerator to be divided
* @param div The 8-bit denominator
*
* This function divides a given 16-bit value by a 8-bit divisor and returns
* 8-bit quotient. Half of the divisor is added to divident before the division
* so the result is rounded.
*
* @return This function returns the 8-bit quotient. Returned value is 255 
*         when overflow occurs.
*
******************************************************************************/
/*
#pragma NO_ENTRY 
#pragma NO_EXIT 
#pragma NO_RETURN 
#pragma NO_FRAME 
*/
static Byte D4D_Divide8(Word num, Byte div)
{
    D4D_UNUSED(num);
    D4D_UNUSED(div);

#if (D4D_MCU_TYPE == D4D_HC08) || (D4D_MCU_TYPE == D4D_HCS08)
    asm
    {
        PSHA
        PSHX
        PSHH
        
        LSRA            /* div/2 */
        ADD   2,SP      /* add div/2 to num on the stack */
        STA   2,SP
        CLRA  
        ADC   1,SP
        STA   1,SP

        PULH            
        PULA            /* HA = num + div/2 */
        PULX            /* X = div */
        
        DIV             /* A=HA/X, H=HA%X */
        BCC ok          /* return A when okay */
        LDA #0xff       /* return 255 on overflow */
    ok: RTS 
    }

    /* ignored dead code (just for C syntax) */    
    return 0;
#elif (D4D_MCU_TYPE == D4D_HC12) || (D4D_MCU_TYPE == D4D_HCS12) || (D4D_MCU_TYPE == D4D_HCS12X)    
    return (Byte)(num/div);
#elif (D4D_MCU_TYPE == D4D_MCF51) || (D4D_MCU_TYPE == D4D_MCF52) || (D4D_MCU_TYPE == D4D_MPC51) || (D4D_MCU_TYPE == D4D_MK)
    return (Byte)(num/div);
#else 
  #error "Not supported MCU for D4D math divide!"
#endif

    // ignored dead code (just for C syntax)     

}

static Word D4D_Divide16(LWord num, Word div)
{
  return (Word)(num/div);

}

Byte D4D_MulDivUU8(Byte u1, Byte u2, Byte d)
{
    Word n = (Word)(((Word)u1 * (Word)u2) + (d / 2));
    return D4D_Divide8(n, d);
}

sByte D4D_MulDivSU8(sByte s, Byte u, Byte d)
{
    Word n;
    Byte r;
    
    if(s >= 0)
    {
        n = (Word)(((Byte)s * (Word)u) + (d / 2));
        r = D4D_Divide8(n, d);
        return (sByte)((r > (Byte)127)? 127 : r);
    }
    else
    {
        n = (Word)((((Byte)-s) * (Word)u) + (d / 2));
        r = D4D_Divide8(n, d);
        return (sByte)(r > 128 ? -128 : (sByte)(-1 * ((sByte)r)));
    }
}

Word D4D_MulDivUU16(Word u1, Word u2, Word d)
{
    LWord n = (LWord)((LWord)((LWord)u1 * (LWord)u2) + (d / 2));
    return D4D_Divide16(n, d);
}

sWord D4D_MulDivSU16(sWord s, Word u, Word d)
{
    LWord n;
    Word r;
    
    if(s >= 0)
    {
        n = (LWord)(((Word)s * (LWord)u) + (d / 2));
        r = D4D_Divide16(n, d);
        return (sWord)((r > (Word)0x7FFF)? 0x7FFF : r);
    }
    else
    {
        n = (LWord)((((Word)-s) * (LWord)u)) + (d / 2);
        r = D4D_Divide16(n, d);
        return (sWord)(r > 0x8000 ? -32768 : (sWord)(-1 * ((sWord)r)));
    }
}

Word D4D_Abs16(sWord num) {
  if(num < 0)
    num = (sWord)(num * -1);
  
  return (Word)num;
}

Word D4D_LimitU16(Word val, Word min, Word max)
{
  if(val < min)
    return min;
  
  if(val > max)
    return max;
  
  
  return val;
}

sWord D4D_LimitS16(sWord val, sWord min, sWord max)
{
  if(val < min)
    return min;
  
  if(val > max)
    return max;
  
  
  return val;
}

Byte D4D_LimitU8(Byte val, Byte min, Byte max)
{
  if(val < min)
    return min;
  
  if(val > max)
    return max;
  
  
  return val;
}

sByte D4D_LimitS8(sByte val, sByte min, sByte max)
{
  if(val < min)
    return min;
  
  if(val > max)
    return max;
  
  
  return val;
}

/***************************************************************************//*!
*
* @brief  Sine calculation using lookup table
*
* @param x as unsigned integer <0..0xFF> ~ <0..2PI)
*
* This function calculates the sine value of argument phase 
* 64 x 8 bit look-up table. 
*
* @return signed sine value <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> ~ <-1,+1>
*
*****************************************************************************/

sByte D4D_Sin(Byte x)
{
    if(x < 0x80)
    {
        if(x <= 0x3F)
            return d4d_sinTbl[x];
        else
            return d4d_sinTbl[0x3F-(x-0x40)];
    }
    else
    {
        if (x <= 0xBF)
            return (sByte)(-d4d_sinTbl[x-0x80]);
        else
            return (sByte)(-d4d_sinTbl[0x3F-(x-0xC0)]);
    }
}

sByte D4D_Cos(Byte x)
{
    if(x < 0x80)
    {
        if(x <= 0x3F)
            return d4d_sinTbl[0x3F-x];
        else
            return (sByte)(-d4d_sinTbl[x-0x40]);
    }
    else
    {
        if (x <= 0xBF)
            return (sByte)(-d4d_sinTbl[0x3F-(x-0x80)]);
        else
            return d4d_sinTbl[x-0xC0];
    }
}

