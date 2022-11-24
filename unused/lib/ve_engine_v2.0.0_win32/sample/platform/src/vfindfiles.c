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
#include "vfile.h"

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/* Class structure for the data interface */
#include "vdatapriv.h"

/* ******************************************************************
**  LOCAL FUNCTION PROTOTYPES
** ******************************************************************/

/*-------------------------------------------------------------------
**  @func   Compares to full path names
**  @comm   Ingores differences in directory separator
**  @rdesc  NUAN_BOOL | True or False
**------------------------------------------------------------------*/
static NUAN_BOOL vplatform_CmpFilePaths(
    PLATFORM_TCHAR *szFile1,
    PLATFORM_TCHAR *szFile2
);

/*-------------------------------------------------------------------
**  @func   Add contents of a pipeline header file to the
**          broker information, *<p pszBrokerInfo>
**  @comm   Dynamically reallocates the appropriate space for the
**          broker information *<p pszBrokerInfo> using <p hHeap>.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
static NUAN_ERROR vplatform_AddHdrToBrokerInfo(
        void *        hDataClass,
        void *    hHeap,
  const PLATFORM_TCHAR            * szFile,
        char                     ** pszBrokerInfo
);

/*-------------------------------------------------------------------
**  @func   Traverse the tree rooted at <p szRoot> looking for
**          pipeline headers and data files, concatenate the
**          pipeline headers into *<p pszBrokerInfo>, add or remove
**          (based on <p bAdd>) the data file information to
**           the file list *<p ppFileList>
**  @comm   Dynamically allocates the appropriate space for class data
**          *<p ppFileList> using <p hHeap>.
**          Optionally does not traverse subdirectories (<p bRecurse>)
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
static NUAN_ERROR vplatform_WalkDir(
  void *              hDataClass,
  void *          hHeap,
  const PLATFORM_TCHAR            * szRoot,
  char                           ** pszBrokerInfo,
  VPLATFORM_FILELIST             ** ppFileList,
  NUAN_BOOL                         bRecurse,
  NUAN_BOOL                         bAdd
);

/* ******************************************************************
**  LOCAL FUNCTION DEFINITIONS
** ******************************************************************/

/*-----------------------------------------------------------------*/
static NUAN_BOOL vplatform_CmpFilePaths(
    PLATFORM_TCHAR *szFile1,
    PLATFORM_TCHAR *szFile2)
{
    while ((*szFile1 != 0) && ( (*szFile1 == *szFile2) ||
                                ((*szFile1 == '\\') && (*szFile2 == '/')) ||
                                ((*szFile1 == '/') && (*szFile2 == '\\')) ) )
    {
        szFile1++;
        szFile2++;
    }
    if ((*szFile1 == 0) && (*szFile2 == 0)) return NUAN_TRUE;
    else return NUAN_FALSE;
} /* vplatform_CmpFilePaths */

/*-----------------------------------------------------------------*/
static NUAN_ERROR vplatform_AddHdrToBrokerInfo(
        void *        hDataClass,
        void *    hHeap,
  const PLATFORM_TCHAR            * szFile,
        char                     ** pszBrokerInfo)
{
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_FILE_H hFile = NULL;
    char * szBrokerInfo = NULL;
    char * szWritePos = NULL;
    size_t cSize = 0;

    /* open file handle */
    fRet = vplatform_file_Open(hDataClass, hHeap, szFile, _T("rb"), &hFile);
    if (fRet == NUAN_OK)
    {
        cSize = vplatform_file_GetSize(hFile);

        if (*pszBrokerInfo == NULL)
        {
            szBrokerInfo = (char *) vplatform_heap_Calloc(hHeap, cSize + 1, 1);
        }
        else
        {
            szBrokerInfo = (char *) vplatform_heap_Realloc(hHeap, *pszBrokerInfo, (NUAN_U32)(strlen(*pszBrokerInfo) + cSize + 1));
        }

        if (szBrokerInfo == NULL)
        {
            fRet = NUAN_E_OUTOFMEMORY;
        }
        else
        {
            *pszBrokerInfo = szBrokerInfo;
            szWritePos = szBrokerInfo + strlen(szBrokerInfo);

            cSize = vplatform_file_Read(szWritePos, 1, cSize, hFile);
            szWritePos[cSize] = 0; /* NULL terminate */
        }
        vplatform_file_Close(hFile);
    }
    return fRet;
} /* vplatform_AddHdrToBrokerInfo */

/*------------------------------------------------------------------*/
static NUAN_ERROR vplatform_WalkDir(
        void *        hDataClass,
        void *    hHeap,
  const PLATFORM_TCHAR            * szRoot,
  char                           ** pszBrokerInfo,
  VPLATFORM_FILELIST             ** ppFileList,
  NUAN_BOOL                         bRecurse,
  NUAN_BOOL                         bAdd)
{
  NUAN_ERROR fRet = NUAN_OK, fRet2 = NUAN_OK;
  VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;
  VPLATFORM_FIND_TYPE eType;
  PLATFORM_TCHAR * szElem = NULL;
  PLATFORM_TCHAR * szFullPath = NULL;
  VPLATFORM_FIND_H hFind = NULL;

  /* Validate args */

  if (szRoot == NULL)
  {
    return NUAN_E_INVALIDARG;
  }

  /* Recursively walk a directory tree, adding paths to data
  ** components to pszDataPaths */

  fRet = vplatform_find_Open(hDataClass, hHeap, szRoot, &szElem, &eType, &hFind);
  /* vplatform_find_Open() and vplatform_find_Next() return
  ** NUAN_E_NOTFOUND if no elements are found under szRoot */
  if (fRet == NUAN_OK)
  {
    do
    {
      if ((eType == VPLATFORM_FIND_DIR) && (bRecurse == NUAN_TRUE))
      {
        /* Walk the subdirectory, vplatform_Find[...]( ) ensures we don't
        ** get the "." or ".." directories here */
        szFullPath = vplatform_CombinePath(hHeap, szRoot, szElem);
        if (szFullPath == NULL)
        {
          fRet = NUAN_E_OUTOFMEMORY;
        }
        else
        {
          fRet = vplatform_WalkDir(hDataClass, hHeap, szFullPath, pszBrokerInfo, ppFileList, bRecurse, bAdd);
          vplatform_heap_Free(hHeap, szFullPath);
        }
      }
      else if ((eType == VPLATFORM_FIND_HDR) && (pszBrokerInfo != NULL) && (bAdd == NUAN_TRUE))
      {
        /* Found a *.hdr file to load */
        szFullPath = vplatform_CombinePath(hHeap, szRoot, szElem);
        if (szFullPath == NULL)
        {
          fRet = NUAN_E_OUTOFMEMORY;
        }
        else
        {
          fRet = vplatform_AddHdrToBrokerInfo(hDataClass, hHeap, szFullPath, pszBrokerInfo);
          vplatform_heap_Free(hHeap, szFullPath);
        }
      }
      else if (eType == VPLATFORM_FIND_DATA)
      {
        /* Found a *.dat file to store/remove */
        char *szBrokerString = NULL;
        /* construct the broker string */
        szBrokerString = vplatform_CopyToChar(hHeap, szElem);
        if (szBrokerString == NULL)
        {
          fRet = NUAN_E_OUTOFMEMORY;
        }
        else
        {
          vplatform_data_BuildBrokerIdFromFilename(szBrokerString,szBrokerString);
          if (bAdd == NUAN_TRUE)
          {
            /* add to data class */
            szFullPath = vplatform_CombinePath(hHeap, szRoot, szElem);
            if (szFullPath == NULL)
            {
              fRet = NUAN_E_OUTOFMEMORY;
              vplatform_heap_Free(hHeap, szBrokerString);
            }
            else
            {
              fRet = vplatform_AddFileToFileList(hDataClass, szFullPath, szBrokerString, ppFileList);
              if (fRet == NUAN_W_ALREADYPRESENT)
              {
                vplatform_heap_Free(hHeap, szBrokerString);
                vplatform_heap_Free(hHeap, szFullPath);
                fRet = NUAN_OK;
              }
              if ((fRet == NUAN_OK) && (pClass->bErrorCheck == NUAN_FALSE)) ppFileList = &((*ppFileList)->pNext); /* small optimization if no error checking */
            }
          }
          else
          {
            /* remove entry corresponding to this broker string from the data class */
            fRet = vplatform_RemoveFileFromFileList(hDataClass, szBrokerString, ppFileList);
            vplatform_heap_Free(hHeap, szBrokerString);
          }
        }
      }
      vplatform_heap_Free(hHeap, szElem);
    }
    while ((fRet == NUAN_OK) &&
           (fRet = vplatform_find_Next(hFind, &szElem, &eType)) == NUAN_OK);

    if (fRet == NUAN_E_NOTFOUND) fRet = NUAN_OK;

    fRet2 = vplatform_find_Close(hFind);
    if ((fRet2 != NUAN_OK) && (fRet == NUAN_OK)) fRet = fRet2;
  }

  return (fRet == NUAN_E_NOTFOUND ? NUAN_OK : fRet);
} /* vplatform_WalkDir */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_BuildBrokerIdFromFullPath(
    void *   hHeap,
    const PLATFORM_TCHAR      *szFullPath,
    char                     **pszBrokerString,
    PLATFORM_TCHAR           **pszFullPathCopy)
{
    const PLATFORM_TCHAR *szName = szFullPath, *p = szFullPath;
    NUAN_U32 size = 0;

    *pszBrokerString = NULL;
    if (pszFullPathCopy != NULL) *pszFullPathCopy = NULL;

    /* find filename without path, and size of the full path */
    while (*p != 0)
    {
        if ((*p == '/') || (*p == '\\'))
        {
            p++;
            szName = p;
        }
        else
        {
            p++;
        }
        size++;
    }
    *pszBrokerString = vplatform_CopyToChar(hHeap, szName);
    if (*pszBrokerString != NULL)
    {
        vplatform_data_BuildBrokerIdFromFilename(*pszBrokerString,*pszBrokerString);
    }
    else
    {
        return NUAN_E_OUTOFMEMORY;
    }
    if (pszFullPathCopy != NULL)
    {
        /* copy the path, so it can be freed in the application */
        *pszFullPathCopy = vplatform_heap_Calloc(hHeap, size+1, sizeof(PLATFORM_TCHAR));
        if (*pszFullPathCopy == NULL)
        {
            vplatform_heap_Free(hHeap, *pszBrokerString);
            *pszBrokerString = NULL;
            return NUAN_E_OUTOFMEMORY;
        }
        else
        {
            PLATFORM_TCHAR *szCopy = *pszFullPathCopy;
            while (*szFullPath != 0) *szCopy++ = *szFullPath++;
        }
    }
    return NUAN_OK;
} /* vplatform_BuildBrokerIdFromFullPath */

/* ******************************************************************
**  GLOBAL FUNCTIONS (prototypes in header file)
** ******************************************************************/

/*-----------------------------------------------------------------*/
NUAN_ERROR vplatform_AddFileToFileList(
        void *        hDataClass,
        PLATFORM_TCHAR            * szFile,
        char                      * szBrokerString,
        VPLATFORM_FILELIST       ** ppFileList)
{
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;
    void *  hHeap = pClass->hHeap;

    /* move ppFileList to the end of the list */
    while (*ppFileList != NULL)
    {
        if (pClass->bErrorCheck)
        {
            /* check for duplicate broker strings */
            if (0 == strcmp(szBrokerString, (*ppFileList)->szBrokerString))
            {
                if (NUAN_TRUE == vplatform_CmpFilePaths(szFile, (*ppFileList)->szFullPathName))
                {
                    /* same data is added more than once */
                    (*ppFileList)->u32Cnt++;
                    /* return a warning, so that caller knows that szFile and szBrokerString are not used */
                    return NUAN_W_ALREADYPRESENT;
                }
                else
                {
                    /* same data identificatino (broker string) but different data locations */
                    return NUAN_E_ALREADYDEFINED;
                }
            }
        }
        ppFileList = &((*ppFileList)->pNext);
    }
    /* allocate a new element on the list */
    *ppFileList = (VPLATFORM_FILELIST *)vplatform_heap_Calloc(hHeap, 1, sizeof(VPLATFORM_FILELIST));
    if (*ppFileList == NULL)
    {
        fRet = NUAN_E_OUTOFMEMORY;
        vplatform_heap_Free(hHeap, szBrokerString);
    }
    else
    {
        (*ppFileList)->pNext = NULL;
        (*ppFileList)->szBrokerString = szBrokerString;
        (*ppFileList)->szFullPathName = szFile;
        (*ppFileList)->u32Cnt = 1;
    }
    return fRet;
} /* vplatform_AddFileToFileList */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_RemoveFileFromFileList(
        void *        hDataClass,
        const char                * szBrokerString,
        VPLATFORM_FILELIST       ** ppFileList)
{
    VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;
    void *  hHeap = pClass->hHeap;

    /* Note: *ppFileList should point to the beginning of the file list */
    VPLATFORM_FILELIST *pPrev = NULL, *pNext = *ppFileList;
    while ((pNext != NULL) && (0 != strcmp(pNext->szBrokerString, szBrokerString)))
    {
        pPrev = pNext;
        pNext = pNext->pNext;
    }
    if (pNext != NULL)
    {
        /* found */
        if ((pNext->hData != NULL) && (pNext->u32Cnt == 1))
        {
            /* entry should not be removed if its data handle is non-zero                */
            /* which means the resources corresponding to that handle were not freed yet */
            return NUAN_E_UNRELEASEDMODULES;
        }
        else if (pNext->u32Cnt > 1)
        {
            /* same data was added more than once */
            pNext->u32Cnt--;
        }
        else
        {
            if (pPrev == NULL)
            {
                /* this was the first element in the list */
                *ppFileList = pNext->pNext;
            }
            else
            {
                pPrev->pNext = pNext->pNext;
            }
            /* free removed element */
            if (pNext->szFullPathName != NULL) vplatform_heap_Free(hHeap, pNext->szFullPathName);
            if (pNext->szBrokerString != NULL) vplatform_heap_Free(hHeap, pNext->szBrokerString);
            vplatform_heap_Free(hHeap, pNext);
        }
    }
    return NUAN_OK;
} /* vplatform_RemoveFileFromFileList */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_FindFiles(
        void *        hDataClass,
        void *    hHeap,
        NUAN_U16                    u16NbrOfDataInstallPaths,
  const PLATFORM_TCHAR    * const * aszDataInstallPaths,
        char                     ** pszBrokerInfo,
        VPLATFORM_FILELIST       ** ppFileList,
        NUAN_BOOL                   bRecurse)
{
    NUAN_ERROR fRet = NUAN_OK;
    NUAN_U16 i;

    if ( (u16NbrOfDataInstallPaths == 0) || (ppFileList == NULL) )
        return NUAN_E_INVALIDARG;

    if (pszBrokerInfo != NULL) *pszBrokerInfo = NULL;

    /* Walk the installation directories to load the *.hdr files */
    for (i = 0; (fRet == NUAN_OK) && (i < u16NbrOfDataInstallPaths); i++)
    {
        NUAN_BOOL bIsDir = NUAN_TRUE;
        fRet = vplatform_file_IsDirectory(hDataClass, aszDataInstallPaths[i], &bIsDir);
        if (fRet == NUAN_E_NOTIMPLEMENTED)
        {
            bIsDir = NUAN_TRUE;
            fRet = NUAN_OK;
        }
        if (fRet == NUAN_OK)
        {
            if (bIsDir == NUAN_TRUE)
            {
                fRet = vplatform_WalkDir(hDataClass, hHeap, aszDataInstallPaths[i],
                                        pszBrokerInfo, ppFileList, bRecurse, NUAN_TRUE);
            }
            else
            {
                char * szBrokerString = NULL;
                PLATFORM_TCHAR *szFullPath = NULL;
                fRet = vplatform_BuildBrokerIdFromFullPath(hHeap, aszDataInstallPaths[i], &szBrokerString, &szFullPath);
                if (fRet == NUAN_OK)
                {
                    fRet = vplatform_AddFileToFileList(hDataClass, szFullPath, szBrokerString, ppFileList);
                    if (fRet == NUAN_W_ALREADYPRESENT)
                    {
                        vplatform_heap_Free(hHeap, szBrokerString);
                        vplatform_heap_Free(hHeap, szFullPath);
                        fRet = NUAN_OK;
                    }
                }
            }
        }
  }

  /* Clean up if this fails */
  if (fRet != NUAN_OK)
  {
    char * szBrokerInfo = (pszBrokerInfo != NULL) ? *pszBrokerInfo : NULL;
    VPLATFORM_FILELIST *pFileList = (ppFileList != NULL) ? *ppFileList : NULL;

    vplatform_FreeFiles(hHeap, szBrokerInfo, pFileList);
    *ppFileList = NULL;
    if (pszBrokerInfo != NULL) *pszBrokerInfo = NULL;
  }

  return fRet;
} /* vplatform_FindFiles */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_RemoveFiles(
        void *        hDataClass,
        void *    hHeap,
        NUAN_U16                    u16NbrOfDataInstallPaths,
  const PLATFORM_TCHAR    * const * aszDataInstallPaths,
        VPLATFORM_FILELIST       ** ppFileList,
        NUAN_BOOL                   bRecurse)
{
    NUAN_ERROR fRet = NUAN_OK;
    NUAN_U16 i;

    if ( (u16NbrOfDataInstallPaths == 0) || (ppFileList == NULL) )
        return NUAN_E_INVALIDARG;

    /* Walk the installation directories to find and all *.dat files */
    for (i = 0; (fRet == NUAN_OK) && (i < u16NbrOfDataInstallPaths); i++)
    {
        NUAN_BOOL bIsDir = NUAN_TRUE;
        fRet = vplatform_file_IsDirectory(hDataClass, aszDataInstallPaths[i], &bIsDir);
        if (fRet == NUAN_E_NOTIMPLEMENTED)
        {
            bIsDir = NUAN_TRUE;
            fRet = NUAN_OK;
        }
        if (fRet == NUAN_OK)
        {
            if (bIsDir == NUAN_TRUE)
            {
                fRet = vplatform_WalkDir(hDataClass, hHeap, aszDataInstallPaths[i],
                                         NULL, ppFileList, bRecurse, NUAN_FALSE);
            }
            else
            {
                char * szBrokerString = NULL;
                fRet = vplatform_BuildBrokerIdFromFullPath(hHeap, aszDataInstallPaths[i], &szBrokerString, NULL);
                if (fRet == NUAN_OK)
                {
                    fRet = vplatform_RemoveFileFromFileList(hDataClass, szBrokerString, ppFileList);
                    vplatform_heap_Free(hHeap, szBrokerString);
                }
            }
        }
    }

    return fRet;
} /* vplatform_RemoveFiles */

/*------------------------------------------------------------------*/
void vplatform_FreeFiles(
        void *    hHeap,
        char                      * szBrokerInfo,
        VPLATFORM_FILELIST        * pFileList)
{
  VPLATFORM_FILELIST *pNext;

  if (szBrokerInfo != NULL) vplatform_heap_Free(hHeap, szBrokerInfo);

  while (pFileList != NULL)
  {
     if (pFileList->szFullPathName != NULL) vplatform_heap_Free(hHeap, pFileList->szFullPathName);
     if (pFileList->szBrokerString != NULL) vplatform_heap_Free(hHeap, pFileList->szBrokerString);
     pNext = pFileList->pNext;
     vplatform_heap_Free(hHeap, pFileList);
     pFileList = pNext;
  }


} /* vplatform_FreeFiles */

/* ****************************************************************************
**   END                                                                     **
** ************************************************************************* */
