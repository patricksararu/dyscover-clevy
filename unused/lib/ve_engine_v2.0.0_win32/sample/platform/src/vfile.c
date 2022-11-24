/* ******************************************************************
**  Nuance Communications, Inc.
** ******************************************************************/

/* ******************************************************************
**
**  COPYRIGHT INFORMATION
**
**  This program contains proprietary information that is a trade secret
**  of Nuance Communications, Inc. and also is protected as an unpublished
**  work under applicable Copyright laws. Recipient is to retain this
**  program in confidence and is not permitted to use or make copies
**  thereof other than as permitted in a prior written agreement with
**  Nuance Communications, Inc. or its affiliates.
**
**  (c) Copyright 2008 Nuance Communications, Inc.
**  All rights reserved. Company confidential.
**  
** ******************************************************************/

/* ******************************************************************
**  HEADER (INCLUDE) SECTION
** ******************************************************************/

#include "vplatform.h"
#include "vplatform_tchar.h"
#include "vheap.h"
#include "vdata.h"
#include "vfile.h"
#include "vcharconv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#if defined(WIN32) || defined(UNDER_CE)
#include <windows.h>
#endif

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/


/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/*lint -esym(715, hHeap, hDataClass) 
**           not used in this implementation */

/* ******************************************************************
**  LOCAL FUNCTIONS
** ******************************************************************/

/* ------------------------------------------------------------------
**  FILE I/O
** -----------------------------------------------------------------*/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Open(
  void *         hDataClass,
  void *     hHeap,
  const PLATFORM_TCHAR       * szName,
  const PLATFORM_TCHAR       * szMode,
  VPLATFORM_FILE_H           * phFile)
{
    *phFile = (VPLATFORM_FILE_H *)_tfopen(szName, szMode);

    return (*phFile == NULL) ? NUAN_E_COULDNOTOPENFILE : NUAN_OK;
}

#if defined(PLATFORM_UNICODE) || defined(PLATFORM_UTF16)
/* ANSI code page file open function */
NUAN_ERROR vplatform_file_OpenA(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  const char                 * szMode,
  VPLATFORM_FILE_H           * phFile)
{
    *phFile = (VPLATFORM_FILE_H *)fopen(szName, szMode);

    return (*phFile == NULL) ? NUAN_E_COULDNOTOPENFILE : NUAN_OK;
}
#endif

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_OpenUTF8(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  const char                 * szMode,
  VPLATFORM_FILE_H           * phFile)
{
    /* Do not use fopen(), since the stream interface implementation will pass UTF-8 strings to this function */
    NUAN_ERROR fRet = NUAN_OK;
    PLATFORM_TCHAR *tszName = NULL;
    PLATFORM_TCHAR *tszMode = NULL;
  
    /* Convert to PLATFORM_TCHAR */
    fRet = vplatform_UTF8_to_PLATFORM_TCHAR(&tszName, szName, hHeap);
    if (fRet != NUAN_OK) return fRet;
  
    fRet = vplatform_UTF8_to_PLATFORM_TCHAR(&tszMode, szMode, hHeap);
    if (fRet != NUAN_OK) goto endOpenUTF8;
  
    /* Open file */
    fRet = vplatform_file_Open(hDataClass, hHeap, tszName, tszMode, phFile);

endOpenUTF8:
  if (tszName != NULL) vplatform_heap_Free(hHeap, tszName);
  if (tszMode != NULL) vplatform_heap_Free(hHeap, tszMode);
  return fRet;
} /* vplatform_file_OpenUTF8 */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Close(
  VPLATFORM_FILE_H  hFile)
{
  NUAN_ERROR     fRet = NUAN_OK;
  FILE          *fp = (FILE *) hFile;
  
  if (0 != fclose(fp))
  {
    fRet = NUAN_E_FILECLOSE;
  }

  return fRet;
} /* vplatform_file_Close */

/*------------------------------------------------------------------*/
size_t vplatform_file_Read(
  void                * pBuffer,
  size_t                cElementBytes,
  size_t                cElements,
  VPLATFORM_FILE_H      hFile)
{
  FILE *fp = (FILE *) hFile;
  NUAN_U32 u32Read = (NUAN_U32)fread(pBuffer, cElementBytes, cElements, fp);
  return ((u32Read == 0) && (ferror(fp)) ? 0xffffffff : u32Read);
} /* vplatform_file_Read */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Seek(
  VPLATFORM_FILE_H            hFile,
  size_t                      cOffset,
  VE_STREAM_ORIGIN         eOrigin,
  VE_STREAM_DIRECTION      eDirection)
{
  NUAN_ERROR fRet = NUAN_OK;
  FILE *fp = (FILE *) hFile;
  int iRet;
  NUAN_S32 s32Origin;
  
  switch (eOrigin)
  {
  case VE_STREAM_SEEK_SET    : s32Origin = SEEK_SET; break;
  case VE_STREAM_SEEK_CUR    : s32Origin = SEEK_CUR; break;
  case VE_STREAM_SEEK_END    : s32Origin = SEEK_END; break;
  default:
    return NUAN_E_INVALIDARG;
    break;
  }

#if defined (_MSC_VER) && (_MSC_VER >= 1400) && !defined(UNDER_CE)
  iRet = _fseeki64(fp, eDirection * (__int64) cOffset, s32Origin);
  if (iRet != 0)
  {
    fRet = NUAN_E_FILESEEK;
  }
#else
  /* Not safe for files larger then 2 GB, but portable for
  ** demonstration purposes. For production use we recommend a seek
  ** method that supports files up to UINT_MAX, such as _fseeki64( )
  ** for Windows OSes and fseeko( ) for many Unix variants. */
  if (cOffset > (NUAN_U32) INT_MAX)
  {
    fRet = NUAN_E_FILESEEK;
  }
  else
  {
    iRet = fseek(fp, eDirection * (int) cOffset, s32Origin);
    if (iRet != 0)
    {
      fRet = NUAN_E_FILESEEK;
    }
  }
#endif
  
  return fRet;
} /* vplatform_file_Seek */

/*------------------------------------------------------------------*/
size_t vplatform_file_GetSize(
  VPLATFORM_FILE_H  hFile)
{
  NUAN_U32        u32FileSize = 0;
  FILE           *fp = (FILE *) hFile;

#if defined (_MSC_VER) && (_MSC_VER >= 1400) && !defined(UNDER_CE)
  NUAN_U32        u32Cur;

  u32Cur = (NUAN_U32) _ftelli64(fp);
  fseek(fp, 0, SEEK_END);
  u32FileSize = (NUAN_U32) _ftelli64(fp);
  _fseeki64(fp, (__int64) u32Cur, SEEK_SET);
#else
  NUAN_S32        s32Cur;

  /* Not safe for files larger then 2 GB, but portable for
  ** demonstration purposes. For production use we recommend a file
  ** size method that supports files up to UINT_MAX, such as _filelengthi64( )
  ** for Windows OSes and fstat(fileno(fp), [...]) for many Unix variants. */
  s32Cur = ftell(fp);
  fseek(fp, 0, SEEK_END);
  u32FileSize = (NUAN_U32) ftell(fp);
  fseek(fp, s32Cur, SEEK_SET);
#endif

  return u32FileSize;
} /* vplatform_file_GetSize */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Error(
  VPLATFORM_FILE_H  hFile)
{
  FILE *fp = (FILE *) hFile;
  return (ferror(fp) ? NUAN_E_FILEREADERROR : NUAN_OK);
} /* vplatform_file_Error */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Flush(
  VPLATFORM_FILE_H  hFile)
{
  NUAN_ERROR     fRet = NUAN_OK;
  FILE *fp = (FILE *) hFile;
  
  if (0 != fflush(fp))
  {
    fRet = NUAN_E_FILEWRITEERROR;
  }
    
  return (fRet);
} /* vplatform_file_Flush */

/*------------------------------------------------------------------*/
size_t vplatform_file_Write(
  const void          * pBuffer,
  size_t                cElementBytes,
  size_t                cElements,
  VPLATFORM_FILE_H      hFile)
{
  FILE *fp = (FILE *) hFile;
  return (NUAN_U32)fwrite(pBuffer, cElementBytes, cElements, fp);
} /* vplatform_file_Write */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_DeleteFile(
  void *         hDataClass,
  const PLATFORM_TCHAR           * pszFile)
{
  NUAN_ERROR nRc = NUAN_OK;

#if defined(WIN32) || defined(UNDER_CE)
  if(!DeleteFile(pszFile))
  {
    if(GetLastError() != ERROR_FILE_NOT_FOUND)
    {
      nRc = NUAN_E_SYSTEM_ERROR;
    }
  }
#elif !defined(PLATFORM_UNICODE)
  /* ignore the error, return value is implementation defined */
  remove(pszFile);
#endif

  return nRc;
} /* vplatform_file_DeleteFile */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_CreateDirectory(
  void *         hDataClass,
  const PLATFORM_TCHAR       * pszDir)
{
    NUAN_ERROR nRc = NUAN_OK;
#if defined(WIN32) || defined(UNDER_CE)
    if (!CreateDirectory(pszDir, NULL))
    {
        if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            nRc = NUAN_E_SYSTEM_ERROR;
        }
    }
#else
    nRc = NUAN_E_NOTIMPLEMENTED;
#endif
    
    return nRc;
} /* vplatform_file_CreateDirectory */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_IsDirectory(
  void *      hDataClass,
  const PLATFORM_TCHAR    * pszPath,
  NUAN_BOOL               * pbIsDir)
{
    DWORD attr;
    
    *pbIsDir = NUAN_FALSE;
    attr = GetFileAttributes(pszPath);
    if (INVALID_FILE_ATTRIBUTES == attr)
    {
        return NUAN_E_SYSTEM_ERROR;
    }
    else
    {
        if (attr & FILE_ATTRIBUTE_DIRECTORY) *pbIsDir = NUAN_TRUE;
    }
    return NUAN_OK;
} /* vplatform_file_IsDirectory */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_SetUnbufferedOutput(void)
{
#if !defined(UNDER_CE)
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
#endif
    return NUAN_OK;
} /* vplatform_file_SetUnbufferedOutput */

/* ******************************************************************
**  END
** ******************************************************************/
