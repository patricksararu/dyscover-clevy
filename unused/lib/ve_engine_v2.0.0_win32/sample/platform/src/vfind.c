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

#include <stdlib.h>
#include <string.h>

#include "vplatform.h"
#include "vplatform_tchar.h"
#include "vfind.h"
#include "vheap.h"

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/

/* Windows or WinCE */
#if defined (WIN32) || defined (UNDER_CE)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>

#endif  

#define VPLATFORM_FIND_DIR_SEP _T("\\")

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/*lint -esym(715, hDataClass) 
**           not used in this implementation */

#ifdef WIN32
/* Structure to keep track of a directory search */
struct VPLATFORM_FIND_S {
  /***/ void *    hHeap;
  /***/ HANDLE                      hFind;
};
#endif

/* ******************************************************************
**  LOCAL FUNCTIONS
** ******************************************************************/

#ifdef WIN32

/*------------------------------------------------------------------*/
static NUAN_ERROR vplatform_find_GetInfo(
  const VPLATFORM_FIND_T  * pFind,
  const WIN32_FIND_DATA   * pwfd,
  PLATFORM_TCHAR         ** pszElem,
  VPLATFORM_FIND_TYPE     * peType)
{
  NUAN_ERROR Err = NUAN_OK;
  size_t ElemSize;
  
  /* user is responsible for freeing this memory */
  ElemSize = _tcslen(pwfd->cFileName) + 1;
  *pszElem = (PLATFORM_TCHAR *)vplatform_heap_Malloc(pFind->hHeap,
    (NUAN_U32)(ElemSize*sizeof(PLATFORM_TCHAR)));
  if (*pszElem == NULL)
  {
    return NUAN_E_OUTOFMEMORY;
  }

  _tcscpy(*pszElem, pwfd->cFileName);

  *peType = VPLATFORM_FIND_OTHER;  
  if (pwfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    *peType = VPLATFORM_FIND_DIR;
  }
  else if (_tcslen(*pszElem) > 4)
  {
    PLATFORM_TCHAR *pExt = &(*pszElem)[_tcslen(*pszElem) - 4];
    if (_tcsicmp(pExt, _T(".hdr")) == 0)
    {
      *peType = VPLATFORM_FIND_HDR;
    }
    else if (_tcsicmp(pExt, _T(".dat")) == 0)
    {
      *peType = VPLATFORM_FIND_DATA;
    }
  }
  return Err;
} /* vplatform_find_GetInfo */

#endif /* WIN32 */

/* ******************************************************************
**  GLOBAL FUNCTIONS (prototypes in header file)
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_find_Open(
        void *        hDataClass,
        void *    hHeap,
  const PLATFORM_TCHAR            * szDir,
        PLATFORM_TCHAR           ** pszElem,
        VPLATFORM_FIND_TYPE       * peType,
        VPLATFORM_FIND_H          * phFind)
{
  NUAN_ERROR Err = NUAN_OK;

#ifdef WIN32

  WIN32_FIND_DATA wfd;
  HANDLE hFind;
  VPLATFORM_FIND_T * pFind = NULL;

  /* Open a directory and start retrieving its contents */
  if ((szDir == NULL) || (szDir[0] == 0) || (phFind == NULL))
  {
    return NUAN_E_NULL_POINTER;
  }

  *pszElem = NULL;
  *phFind = NULL;

  /* Windows and WinCE implementation */
  {
    PLATFORM_TCHAR * pszFileSpec = NULL;
    size_t FileSpecSize;

    FileSpecSize = (_tcslen(szDir) + 3) * sizeof(PLATFORM_TCHAR);
    pszFileSpec = vplatform_heap_Malloc(hHeap, (NUAN_U32)FileSpecSize);
    if (NULL == pszFileSpec)
    {
      return NUAN_E_OUTOFMEMORY;
    }

    _tcscpy(pszFileSpec, szDir);
    _tcscat(pszFileSpec, VPLATFORM_FIND_DIR_SEP);
    _tcscat(pszFileSpec, _T("*"));
    hFind = FindFirstFile(pszFileSpec, &wfd);
    vplatform_heap_Free(hHeap, pszFileSpec);
  }

  if (hFind == INVALID_HANDLE_VALUE)
  {
    Err = NUAN_E_NOTFOUND;
  }
  else
  {
    pFind = (VPLATFORM_FIND_T *)vplatform_heap_Malloc(hHeap, sizeof(VPLATFORM_FIND_T));
    if (pFind == NULL)
    {
      FindClose(hFind);
      return NUAN_E_OUTOFMEMORY;
    }

    pFind->hHeap = hHeap;
    pFind->hFind = hFind;

    /* Skip the "." and ".." directories */
    if ((_tcscmp(wfd.cFileName, _T(".")) == 0) ||
        (_tcscmp(wfd.cFileName, _T("..")) == 0))
    {
      Err = vplatform_find_Next(pFind, pszElem, peType);
    }
    else
    {
      Err = vplatform_find_GetInfo(pFind, &wfd, pszElem, peType);
    }

    if (Err != NUAN_OK)
    {
      vplatform_find_Close(pFind);
    }
    else
    {
      *phFind = pFind;
    }
  }

#else

  /* Not Windows */
  Err = NUAN_E_NOTIMPLEMENTED;

#endif

  return Err;
} /* vplatform_find_Open */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_find_Next(
  const VPLATFORM_FIND_H       hFind,
        PLATFORM_TCHAR      ** pszElem,
        VPLATFORM_FIND_TYPE  * peType)
{
  NUAN_ERROR Err = NUAN_OK;

#ifdef WIN32

  VPLATFORM_FIND_T * pFind = NULL;
  WIN32_FIND_DATA wfd;

  /* Find the next file for a directory search */
  if (hFind == NULL)
  {
    return NUAN_E_NULL_POINTER;
  }

  pFind = (VPLATFORM_FIND_T *)hFind;
  *pszElem = NULL;

  /* Windows and WinCE implementation */

  /* Skip the "." and ".." directories */
  do
  {
    if (! FindNextFile(pFind->hFind, &wfd))
    {
      Err = NUAN_E_NOTFOUND;
    }
  }
  while ((Err == NUAN_OK) && ((_tcscmp(wfd.cFileName, _T(".")) == 0) ||
                              (_tcscmp(wfd.cFileName, _T("..")) == 0)));

  if (Err == NUAN_OK)
  {
    Err = vplatform_find_GetInfo(pFind, &wfd, pszElem, peType);
  }

#else

  /* Not Windows */
  Err = NUAN_E_NOTIMPLEMENTED;

#endif

  return Err;
} /* vplatform_find_Next */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_find_Close(
  VPLATFORM_FIND_H hFind)
{
  NUAN_ERROR Err = NUAN_OK;

#ifdef WIN32  

  VPLATFORM_FIND_T * pFind = NULL;

  /* Close a directory search */
  if (hFind == NULL)
  {
    return NUAN_E_NULL_POINTER;
  }

  pFind = (VPLATFORM_FIND_T *)hFind;

  /* Windows and WinCE implementation */
  if (0 == FindClose(pFind->hFind))
  {
    return NUAN_E_SYSTEM_ERROR;
  }

  vplatform_heap_Free(pFind->hHeap, pFind);

#endif

  return Err;
} /* vplatform_find_Close */

/*------------------------------------------------------------------*/
PLATFORM_TCHAR * vplatform_CombinePath(
  void *          hHeap,
  const PLATFORM_TCHAR            * szDir,
  const PLATFORM_TCHAR            * szFile)
{
  NUAN_U32 len = (NUAN_U32)_tcslen(szDir);
  PLATFORM_TCHAR *szPath = (PLATFORM_TCHAR *)vplatform_heap_Malloc(hHeap,
    (NUAN_U32)((len + _tcslen(szFile) + 2)*sizeof(PLATFORM_TCHAR)));
  if (szPath)
  {
    _tcscpy(szPath, szDir);
    if ((szPath[len-1] != (PLATFORM_TCHAR)'\\') && (szPath[len-1] != (PLATFORM_TCHAR)'/')) 
    {
        _tcscat(szPath, VPLATFORM_FIND_DIR_SEP);
    }
    _tcscat(szPath, szFile);
  }

  return szPath;
} /* vplatform_CombinePath */

#define VTOLOWER(x) ( (((x) >= 'A') && ((x) <= 'Z')) ? (x)+'a'-'A' : (x))

/*------------------------------------------------------------------*/
char * vplatform_CopyToChar(
  void *          hHeap,
  const PLATFORM_TCHAR            * szFile)
{
  char *szFileName = (char *)vplatform_heap_Malloc(hHeap, (NUAN_U32)(_tcslen(szFile) + 1));
  if (szFileName)
  {
    size_t i;
    for (i = 0; i <= _tcslen(szFile); i++)
    {
      szFileName[i] = VTOLOWER((char)szFile[i]);
    }
  }
  return szFileName;
} /* vplatform_CopyToChar */

/* ******************************************************************
**  END
** ******************************************************************/
