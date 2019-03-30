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

#include <string.h>

#include "vplatform.h"
#include "vplatform_tchar.h"
#include "vfind.h"
#include "vfindfiles.h"
#include "vfilelist.h"
#include "vfile.h"

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/


/* ******************************************************************
**  LOCAL FUNCTION PROTOTYPES
** ******************************************************************/


/*------------------------------------------------------------------*/
size_t vplatform_list_CountEntries(PLATFORM_TCHAR *szContents, size_t cSize)
{
  size_t count = 0, i = 0;
  
  cSize /= sizeof(PLATFORM_TCHAR);

  /* check if the last line ends with a new line */
  i = cSize-1;
  while ((i > 0) && ((szContents[i] == (PLATFORM_TCHAR)' ') || (szContents[i] == (PLATFORM_TCHAR)0x9))) i--;
  if (szContents[i] != (PLATFORM_TCHAR)0xA) count = 1;
  
  i = 0;
  while (i < cSize)
  {
    if ((PLATFORM_TCHAR)0xA == szContents[i]) count++;
    i++;
  }
  return count;
}

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_list_GetEntry(
  void *           hHeap,
  PLATFORM_TCHAR              ** ppReadPtr, 
  VPLATFORM_FILELIST           * pFileList)
{
  NUAN_U32 i, size;
  PLATFORM_TCHAR *pCur = *ppReadPtr;
  PLATFORM_TCHAR *pTmp = *ppReadPtr;
  
  /* determine size of broker string */
  while ((*pTmp != (PLATFORM_TCHAR)' ') && (*pTmp != (PLATFORM_TCHAR)0x9) && (*pTmp != 0)) pTmp++;
  if (*pTmp == 0)
  {
    return NUAN_E_FILEREADERROR;
  }
  size = (NUAN_U32)(pTmp - pCur) + 1;
  
  pFileList->szBrokerString = (char *)vplatform_heap_Malloc(hHeap, size);
  if (pFileList->szBrokerString == NULL)
  {
    return NUAN_E_OUTOFMEMORY;
  }
  
  /* copy brokerstring, convert to char */
  i = 0;
  while ((*pCur != (PLATFORM_TCHAR)' ') && (*pCur != (PLATFORM_TCHAR)0x9) )
  {
    pFileList->szBrokerString[i++] = (char)*pCur++;
  }
  pFileList->szBrokerString[i] = 0;
  
  /* move read pointer to file name */
  while ( ((*pCur == (PLATFORM_TCHAR)' ') || (*pCur == (PLATFORM_TCHAR)0x9)) && (*pCur != 0) ) pCur++;
  
  if (*pCur == 0)
  {
    vplatform_heap_Free(hHeap, pFileList->szBrokerString);
    return NUAN_E_FILEREADERROR;
  }
  
  /* determine size of file path */
  pTmp = pCur;
  while ((*pTmp != (PLATFORM_TCHAR)0xA) && (*pTmp != 0)) pTmp++;
  /* remove trailing blanks */
  pTmp--;
  while( ((*pTmp == (PLATFORM_TCHAR)' ') || (*pTmp == (PLATFORM_TCHAR)0x9) || (*pTmp == (PLATFORM_TCHAR)0xD)) && (pTmp > pCur)) pTmp--;
  if (pTmp <= pCur)
  {
    vplatform_heap_Free(hHeap, pFileList->szBrokerString);
    return NUAN_E_FILEREADERROR;
  }
  pTmp++;
  size = (NUAN_U32)(pTmp - pCur + 1)*sizeof(PLATFORM_TCHAR);
  
  pFileList->szFullPathName = (PLATFORM_TCHAR *)vplatform_heap_Malloc(hHeap, size);
  if (pFileList->szFullPathName == NULL)
  {
    vplatform_heap_Free(hHeap, pFileList->szBrokerString);
    return NUAN_E_OUTOFMEMORY;
  }
  
  /* copy file path */
  i = 0;
  while (pCur != pTmp)
  {
    pFileList->szFullPathName[i++] = *pCur++;
  }
  pFileList->szFullPathName[i] = 0;
  
  pFileList->hData = NULL;
  
  /* skip blanks */
  while ((*pCur == (PLATFORM_TCHAR)' ') || (*pCur == (PLATFORM_TCHAR)0x9) || (*pCur == (PLATFORM_TCHAR)0xD) || (*pCur == (PLATFORM_TCHAR)0xA)) pCur++;
  
  *ppReadPtr = pCur;
  
  return NUAN_OK;
}


/* ******************************************************************
**  GLOBAL FUNCTIONS (prototypes in header file)
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_ParseListFile(
        void *          hDataClass,
        void *      hHeap,
  const PLATFORM_TCHAR              * szListFile,
        VPLATFORM_FILELIST         ** ppFileList)
{
  NUAN_ERROR fRet = NUAN_OK;
  VPLATFORM_FILE_H hFile = NULL;
  size_t cCount, i, cSize;
  PLATFORM_TCHAR *szContents = NULL, *pReadPtr = 0;
  
  fRet = vplatform_file_Open(hDataClass, hHeap, szListFile, _T("rb"), &hFile);
  if ((fRet != NUAN_OK) || (hFile == NULL)) return fRet;
  
  cSize = vplatform_file_GetSize(hFile);
  
  szContents = (PLATFORM_TCHAR *)vplatform_heap_Malloc(hHeap, cSize + sizeof(PLATFORM_TCHAR));
  if (szContents == NULL)
  {
    fRet = NUAN_E_OUTOFMEMORY;
    goto error;
  }
  if (cSize != vplatform_file_Read(szContents, 1, cSize, hFile))
  {
    fRet = NUAN_E_FILEREADERROR;
    goto error;
  }
  /* zero terminate */
  szContents[cSize] = 0;
  
  cCount = vplatform_list_CountEntries(szContents, cSize);
  
  pReadPtr = szContents;

#if defined(PLATFORM_UNICODE) || defined(PLATFORM_UTF16)
  /* Skip UTF-16 BOM */
  if (*pReadPtr == (PLATFORM_TCHAR)0xFEFF) pReadPtr++;
#else
  /* Skip UTF-8 BOM */
  if (((unsigned char)*pReadPtr == (unsigned char)0xEF) && 
      ((unsigned char)*(pReadPtr+1) == (unsigned char)0xBB) && 
      ((unsigned char)*(pReadPtr+2) == (unsigned char)0xBF)) pReadPtr += 3;
#endif

  i = 0;
  while ((i < cCount) && (fRet == NUAN_OK))
  {  
    /* allocate a new element on the list */
    *ppFileList = (VPLATFORM_FILELIST *)vplatform_heap_Calloc(hHeap, 1, sizeof(VPLATFORM_FILELIST));
    if (*ppFileList == NULL)
    {
      fRet = NUAN_E_OUTOFMEMORY;
    }
    else
    {
      fRet = vplatform_list_GetEntry(hHeap, &pReadPtr, *ppFileList);
      i++;
    
      ppFileList = &((*ppFileList)->pNext);
    }
  }

error:
  if (szContents != NULL) vplatform_heap_Free(hHeap, szContents);
  if (hFile != NULL) vplatform_file_Close(hFile);
  /* caller is responsible to clean up pFileList in case of error */
  
  return fRet;
}  /* vplatform_ParseListFile */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_GetBinaryBrokerFile(
        void *          hDataClass,
        void *      hHeap,
  const PLATFORM_TCHAR              * szBinaryBrokerFile,
        char                       ** pszBrokerInfo)
{
  NUAN_ERROR fRet = NUAN_OK;
  VPLATFORM_FILE_H hFile = NULL;
  size_t   cSize;
  char *szContents = NULL;
  
  *pszBrokerInfo = NULL;
  fRet = vplatform_file_Open(hDataClass, hHeap, szBinaryBrokerFile, _T("rb"), &hFile);
  if ((fRet != NUAN_OK) || (hFile == NULL)) return fRet;
  
  cSize = vplatform_file_GetSize(hFile);
  
  szContents = (char *)vplatform_heap_Malloc(hHeap, cSize + 1);
  if (szContents == NULL)
  {
    fRet = NUAN_E_OUTOFMEMORY;
    goto error;
  }
  if (cSize != vplatform_file_Read(szContents, 1, cSize, hFile))
  {
    fRet = NUAN_E_FILEREADERROR;
    goto error;
  }
  /* zero terminate */
  szContents[cSize] = 0;
  
  *pszBrokerInfo = szContents;

  vplatform_file_Close(hFile);

  return fRet;

error:
  if (szContents != NULL) vplatform_heap_Free(hHeap, szContents);
  if (hFile != NULL) vplatform_file_Close(hFile);
  
  return fRet;
} /* vplatform_GetBinaryBrokerFile */

/*------------------------------------------------------------------*/
NUAN_BOOL vplatform_CheckFileExists(
        void *          hDataClass,
        void *      hHeap,
  const PLATFORM_TCHAR              * szFile)
{
  NUAN_BOOL  bRet = NUAN_FALSE;
  VPLATFORM_FILE_H hFile;
  
  if (NUAN_OK != vplatform_file_Open(hDataClass, hHeap, szFile, _T("rb"), &hFile)) return NUAN_FALSE;
  
  if (0 != vplatform_file_GetSize(hFile))
    bRet = NUAN_TRUE;
  
  vplatform_file_Close(hFile);

  /* delete the file in case vplatform_file_Open did create a file, happens on some platforms */
  if (bRet == NUAN_FALSE)
    vplatform_file_DeleteFile(hDataClass,szFile);

  return bRet;
} /* vplatform_CheckFileExists */




/* ****************************************************************************
**   END                                                                     **
** ************************************************************************* */
