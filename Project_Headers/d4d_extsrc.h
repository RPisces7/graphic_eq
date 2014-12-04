/******************************************************************************
* 
* Copyright (c) 2011 Freescale Semiconductor;
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
* @file      d4d_extsrc.h
*
* @author    b01119
* 
* @version   0.0.1.0
* 
* @date      Feb-8-2011
* 
* @brief     D4D Driver extscr helper functions header file
*
*******************************************************************************/

#ifndef __D4D_EXTSRC_H
#define __D4D_EXTSRC_H

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/

typedef void* D4D_FILEPTR; 

typedef struct D4DFILE_FUNCTIONS_S           
{
  D4D_FILEPTR (*D4DFILE_fopen)(char* filename, char* mode); // THE MODE WILL BE ALWAYS READ ONLY 'r'
  int (*D4DFILE_fclose)(D4D_FILEPTR p_file);  
  int (*D4DFILE_read)(D4D_FILEPTR p_file, void* buffer, int size);
  int (*D4DFILE_write)(D4D_FILEPTR p_file, const void* buffer, int size);
  int (*D4DFILE_fseek)(D4D_FILEPTR p_file, long offset, int origin);  
}D4DFILE_FUNCTIONS;

typedef struct D4DLOCALE_FUNCTIONS_S           
{
  char strTranslatePrefix;
  int (*D4DLOCALE_TranslateStr)(const char* originTxt, char* newTxt, int offset, int size);
  int (*D4DLOCALE_GetTranslateStrLength)(const char* originTxt);
}D4DLOCALE_FUNCTIONS;


/******************************************************************************
* Macros 
******************************************************************************/

#define D4D_DECLARE_LOCALE_FUNCTIONS(strTranslatePrefix, D4DLOCALE_TranslateStr, D4DLOCALE_GetTranslateStrLength) \
  const D4DLOCALE_FUNCTIONS d4d_extsrcLocale = \
  { \
    strTranslatePrefix, /* Translate Char prefix */ \
    D4DLOCALE_TranslateStr, /* Translate locale function pointer */ \
    D4DLOCALE_GetTranslateStrLength  /* Translate locale get str Length function pointer */ \
  };
    
#define D4D_DECLARE_FILE_FUNCTIONS(D4DFILE_fopen, D4DFILE_fclose, D4DFILE_read, D4DFILE_write, D4DFILE_fseek) \
  const D4DFILE_FUNCTIONS d4d_extsrcFile = \
  { \
    D4DFILE_fopen, /* D4D File - fopen */ \
    D4DFILE_fclose, /* D4D File - fclose */ \
    D4DFILE_read, /* D4D File - fread */ \
    D4DFILE_write, /* D4D File - fwrite */ \
    D4DFILE_fseek /* D4D File - fseek */ \
  };


/******************************************************************************
* Global variables
******************************************************************************/

extern const D4DLOCALE_FUNCTIONS d4d_extsrcLocale;

/******************************************************************************
* Global functions
******************************************************************************/



#endif /* __D4D_EXTSRC_H */