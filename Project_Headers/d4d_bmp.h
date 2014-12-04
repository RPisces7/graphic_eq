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
* @file      d4d_bmp.h
*
* @author    b01119
* 
* @version   0.0.8.0
* 
* @date      Nov-1-2010
* 
* @brief     D4D driver bmp decoder header file 
*
******************************************************************************/

#ifndef __D4D_BMP_H
#define __D4D_BMP_H

/******************************************************************************
* Constants
******************************************************************************/

#define D4D_BMP_COMPRESS_MASK   0x0001

#define D4D_BMP_COMPRESSED      0x0001
#define D4D_BMP_UNCOMPRESSED    0x0000

#define D4D_BMP_TYPE_MASK       0x001E

#define D4D_BMP_65536NOPAL      0x0000
#define D4D_BMP_4096NOPAL       0x0002
#define D4D_BMP_256NOPAL        0x0004

#define D4D_BMP_PAL256_32       0x0006
#define D4D_BMP_PAL16_8         0x0008
#define D4D_BMP_PAL4            0x000A
#define D4D_BMP_PAL2            0x000C
#define D4D_BMP_MON_VER         0x0010

#define D4D_BMP_CLR_DEPTH_MASK  0x0700

#define D4D_BMP_CLR_DEPTH256    0x0000
#define D4D_BMP_CLR_DEPTH128    0x0100
#define D4D_BMP_CLR_DEPTH64     0x0200
#define D4D_BMP_CLR_DEPTH32     0x0300
#define D4D_BMP_CLR_DEPTH16     0x0400
#define D4D_BMP_CLR_DEPTH8      0x0500
#define D4D_BMP_CLR_DEPTH4      0x0600
#define D4D_BMP_CLR_DEPTH2      0x0700

#define D4D_BMP_ENDIAN_FLAG     0x8000

#define D4D_BMP_PAL_256         (D4D_BMP_PAL256_32 | D4D_BMP_CLR_DEPTH256)
#define D4D_BMP_PAL_16          (D4D_BMP_PAL16_8 | D4D_BMP_CLR_DEPTH16)
#define D4D_BMP_PAL_2           (D4D_BMP_PAL2 | D4D_BMP_CLR_DEPTH2)

#ifndef D4D_BMP_65536NOPAL_ENABLE
  #define D4D_BMP_65536NOPAL_ENABLE D4D_FALSE
#endif  

#ifndef D4D_BMP_256NOPAL_ENABLE
  #define D4D_BMP_256NOPAL_ENABLE D4D_FALSE
#endif  

#ifndef D4D_BMP_PAL_256_ENABLE
  #define D4D_BMP_PAL_256_ENABLE D4D_FALSE
#endif  

#ifndef D4D_BMP_PAL_16_ENABLE
  #define D4D_BMP_PAL_16_ENABLE D4D_FALSE
#endif  

#ifndef D4D_BMP_PAL_2_ENABLE
  #define D4D_BMP_PAL_2_ENABLE D4D_FALSE
#endif  


#ifndef D4D_BMPR_65536NOPAL_ENABLE
  #define D4D_BMPR_65536NOPAL_ENABLE D4D_FALSE
#endif  

#ifndef D4D_BMPR_256NOPAL_ENABLE
  #define D4D_BMPR_256NOPAL_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMPR_PAL_256_ENABLE
  #define D4D_BMPR_PAL_256_ENABLE D4D_FALSE
#endif

#ifndef D4D_BMPR_PAL_16_ENABLE
  #define D4D_BMPR_PAL_16_ENABLE D4D_FALSE
#endif  

#ifndef D4D_BMPR_PAL_2_ENABLE
  #define D4D_BMPR_PAL_2_ENABLE D4D_FALSE
#endif    

/******************************************************************************
* Global D4D bitmap drawing level functions
******************************************************************************/

D4D_BOOL D4D_BmpHeaderCheckEndianism(const D4D_BMP* pBmp);

void D4D_LCD_Bmp65kNoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Word Pbmp[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale);
void D4D_LCD_Bmp256NoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,
                        const Byte Pbmp[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale);
void D4D_LCD_Bmp256Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[],
                        D4D_BOOL Compressed, D4D_BOOL GreyScale);
void D4D_LCD_Bmp16Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale);
void D4D_LCD_Bmp2Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                        const Byte Pbmp[], const Word ColorTable[], 
                        D4D_BOOL Compressed, D4D_BOOL GreyScale);

#if D4D_ROUND_CORNER_ENABLE == D4D_TRUE
  void D4D_LCD_RBmp65kNoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                          const Word Pbmp[], 
                          D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius);

  void D4D_LCD_RBmp256NoPal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2,
                            const Byte Pbmp[], 
                            D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius);
  void D4D_LCD_RBmp256Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                          const Byte Pbmp[], const Word ColorTable[],
                          D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius);
  /*
  void D4D_LCD_RBmp16Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                          const Byte Pbmp[], const Word ColorTable[], 
                          D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius);
  void D4D_LCD_RBmp2Pal(D4D_COOR x1, D4D_COOR y1, D4D_COOR x2, D4D_COOR y2, 
                          const Byte Pbmp[], const Word ColorTable[], 
                          D4D_BOOL Compressed, D4D_BOOL GreyScale, D4D_COOR radius);
  */
  #endif
#endif /* __D4D_BMP_H */