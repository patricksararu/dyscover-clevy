#ifndef __VFILE_H__
#define __VFILE_H__

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

#if defined( __cplusplus )
extern "C"
{
#endif

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

typedef void * VPLATFORM_FILE_H;

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/* ------------------------------------------------------------------
**  File I/O functions
** -----------------------------------------------------------------*/


/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Open(
  void *         hDataClass,
  void *     hHeap,
  const PLATFORM_TCHAR       * szName,
  const PLATFORM_TCHAR       * szMode,
  VPLATFORM_FILE_H           * phFile
);

#ifdef PLATFORM_UNICODE
NUAN_ERROR vplatform_file_OpenA(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  const char                 * szMode,
  VPLATFORM_FILE_H           * phFile
);
#else
 #define vplatform_file_OpenA vplatform_file_Open
#endif

/*------------------------------------------------------------------*/
/* Used by the stream implementation to open files directly specified
 * by the Vocalizer engine, e.g. files referenced with <esc>\audio=...\
 * in the input text or diagnostic logging. 
 * Those file names are in UTF-8.
 */
NUAN_ERROR vplatform_file_OpenUTF8(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  const char                 * szMode,
  VPLATFORM_FILE_H           * phFile
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Close(
  VPLATFORM_FILE_H  hFile
);

/*------------------------------------------------------------------*/
size_t vplatform_file_Read(
  void                * pBuffer,
  size_t                cElementBytes,
  size_t                cElements,
  VPLATFORM_FILE_H      hFile
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Seek(
  VPLATFORM_FILE_H                hFile,
  size_t                          cOffset,
  VE_STREAM_ORIGIN             eOrigin,
  VE_STREAM_DIRECTION          eDirection
);

/*------------------------------------------------------------------*/
size_t vplatform_file_GetSize(
  VPLATFORM_FILE_H  hFile
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Error(
  VPLATFORM_FILE_H  hFile
);

/*------------------------------------------------------------------*/
size_t vplatform_file_Write(
  const void          * pBuffer,
  size_t                cElementBytes,
  size_t                cElements,
  VPLATFORM_FILE_H      hFile
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_Flush(
  VPLATFORM_FILE_H  hFile
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_DeleteFile(
  void *         hDataClass,
  const PLATFORM_TCHAR       * pszFile
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_CreateDirectory(
  void *         hDataClass,
  const PLATFORM_TCHAR       * pszDir
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_IsDirectory(
  void *      hDataClass,
  const PLATFORM_TCHAR    * pszPath,
  NUAN_BOOL               * pbIsDir
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_file_SetUnbufferedOutput(
    void
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VFILE_H__ */
