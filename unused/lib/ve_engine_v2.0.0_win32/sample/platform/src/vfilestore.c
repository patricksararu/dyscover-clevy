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
#include "vheap.h"
#include "vdata.h"
#include "vfile.h"
#include "vfindfiles.h"

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/

/*lint -esym(715, hHeap, hDataClass) 
**           not used in this implementation */

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/
NUAN_ERROR vplatform_data_StoreDataFiles(
        char                              * szBrokerInfo,
        VPLATFORM_FILELIST                * pFileList,
        void *                hDataClass,
        void *            hHeap,
  const PLATFORM_TCHAR                    * szBinaryBrokerFile,
  const PLATFORM_TCHAR                    * szFileListFile
)
{
  VPLATFORM_FILE_H hFile = NULL;
  NUAN_ERROR fRet = NUAN_OK;
  size_t cWritten;
  const PLATFORM_TCHAR * szTab  = _T("\t");
  const PLATFORM_TCHAR * szCRLF = _T("\r\n");
#if defined(PLATFORM_UNICODE) || defined(PLATFORM_UTF16)
  const PLATFORM_TCHAR szBOM[2] = {(PLATFORM_TCHAR)0xFEFF,(PLATFORM_TCHAR)0x0000};
#endif
  if ((szBinaryBrokerFile == NULL) && (szFileListFile == NULL))
    return NUAN_OK;
  else if ((szBinaryBrokerFile == NULL) || (szFileListFile == NULL))
    return NUAN_E_INVALIDARG;
    
  fRet = vplatform_file_Open(hDataClass, hHeap, szBinaryBrokerFile, _T("wb"), &hFile);
  if (fRet == NUAN_OK)
  {
    NUAN_U32 cBrokerInfo = (NUAN_U32)strlen(szBrokerInfo);
  
    cWritten = vplatform_file_Write(szBrokerInfo, 1, cBrokerInfo, hFile);
    if (cWritten == cBrokerInfo)
    {
      fRet = vplatform_file_Close(hFile);
      if (fRet == NUAN_OK)
      {
        fRet = vplatform_file_Open(hDataClass, hHeap, szFileListFile, _T("wb"), &hFile);
        if (fRet == NUAN_OK)
        {
#if defined(PLATFORM_UNICODE) || defined(PLATFORM_UTF16)
          cWritten = vplatform_file_Write(szBOM, (NUAN_U32)(_tcslen(szBOM)*sizeof(PLATFORM_TCHAR)), 1, hFile);
#endif
          while (pFileList != NULL)
          {
            /* copy broker string to PLATFORM_TCHAR */
            PLATFORM_TCHAR *szCopiedString = (PLATFORM_TCHAR *)vplatform_heap_Malloc(hHeap, (NUAN_U32)(strlen(pFileList->szBrokerString) + 1) * sizeof(PLATFORM_TCHAR));
            if (szCopiedString)
            {
              size_t i;
              for (i = 0; i <= strlen(pFileList->szBrokerString); i++)
              {
                /*lint -e571 Warning -- Suspicious cast */
                szCopiedString[i] = (PLATFORM_TCHAR)pFileList->szBrokerString[i];
              }
            }
            else
            {
              vplatform_file_Close(hFile);
              return NUAN_E_OUTOFMEMORY;
            }
            cWritten = vplatform_file_Write(szCopiedString, (NUAN_U32)(_tcslen(szCopiedString)*sizeof(PLATFORM_TCHAR)), 1, hFile);
            vplatform_heap_Free(hHeap, szCopiedString);

            cWritten = vplatform_file_Write(szTab, (NUAN_U32)(_tcslen(szTab)*sizeof(PLATFORM_TCHAR)), 1, hFile);
            
            cWritten = vplatform_file_Write(pFileList->szFullPathName, (NUAN_U32)(_tcslen(pFileList->szFullPathName)*sizeof(PLATFORM_TCHAR)), 1, hFile);

            cWritten = vplatform_file_Write(szCRLF, (NUAN_U32)(_tcslen(szCRLF)*sizeof(PLATFORM_TCHAR)), 1, hFile);

            pFileList=pFileList->pNext;
          }
          fRet = vplatform_file_Close(hFile);
        }
      }
    }
    else
    {
      fRet = vplatform_file_Close(hFile);
      fRet = NUAN_E_FILEWRITEERROR;
    }
  }

  return fRet;
} /* vplatform_data_StoreDataFiles */

