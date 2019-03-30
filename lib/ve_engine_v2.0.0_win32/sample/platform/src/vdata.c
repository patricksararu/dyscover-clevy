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
#include "vstream.h"
#include "vmap.h"
#include "vfindfiles.h"
#include "vfilelist.h"
#include "vinit.h"

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/

/*lint -esym(715, hHeap, hDataClass)
**           not used in this implementation */

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/* Class structure for the data interface */
#include "vdatapriv.h"


/* ******************************************************************
**  LOCAL FUNCTIONS
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_FindFileEntry(
  const char                     * szName,
        void *   hHeap,
        VPLATFORM_FILELIST      ** ppFileList)
{
    VPLATFORM_FILELIST     * pFileList = *ppFileList;

    *ppFileList = NULL;

    /* find file (broker string) in the list */
    while (pFileList != NULL)
    {
      if (!strcmp(szName, pFileList->szBrokerString))
      {
        break;
      }
      pFileList = pFileList->pNext;
    }

    *ppFileList = pFileList;

    return (pFileList == NULL ? NUAN_E_NOTFOUND : NUAN_OK);

} /* vplatform_data_FindFileEntry */


/* ******************************************************************
**  GLOBAL FUNCTIONS
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetFullPathName(
  const char                   * szName,
        PLATFORM_TCHAR        ** pszFullPathName,
        void *     hDataClass)
{
  NUAN_ERROR               fRet = NUAN_OK;
  VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;
  VPLATFORM_FILELIST     * pFileList = pClass->pFileList;
  void *     hHeap = pClass->hHeap;

  *pszFullPathName = NULL;

  fRet = vplatform_data_FindFileEntry(szName, hHeap, &pFileList);

  if (fRet == NUAN_OK)
  {
    /* shallow copy */
    *pszFullPathName = pFileList->szFullPathName;
  }

  return fRet;

} /* vplatform_data_GetFullPathName */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_FreeFullPathName(
        PLATFORM_TCHAR         * szFullPath,
        void *     hDataClass)
{
  /* no action because of shallow copy in vplatform_GetFullPathName */
  (void)szFullPath;
  return NUAN_OK;
} /* vplatform_data_FreeFullPathName */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetEntry(
        void *        hDataClass,
  const char                      * szName,
        VPLATFORM_DATA_HENTRY     * phEntry,
        PLATFORM_TCHAR           ** pszFullPathName,
        VPLATFORM_DATA_HDATA      * phData)
{
  NUAN_ERROR               fRet = NUAN_OK;
  VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;
  VPLATFORM_FILELIST     * pFileList = pClass->pFileList;
  void *     hHeap = pClass->hHeap;

  fRet = vplatform_data_FindFileEntry(szName, hHeap, &pFileList);


  if (fRet == NUAN_OK)
  {
    *pszFullPathName = pFileList->szFullPathName; /* shallow copy */
    *phData = pFileList->hData;
    *phEntry = (VPLATFORM_DATA_HENTRY)pFileList;
  }

  return fRet;
} /* vplatform_data_GetEntry */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_StoreData(
        VPLATFORM_DATA_HENTRY  hEntry,
        VPLATFORM_DATA_HDATA   hData)
{
  VPLATFORM_FILELIST * pFileList = (VPLATFORM_FILELIST *)hEntry;

  pFileList->hData = hData;

  return NUAN_OK;

} /* vplatform_data_StoreData */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_RemoveData(
        VPLATFORM_DATA_HENTRY   hEntry)
{
  VPLATFORM_FILELIST * pFileList = (VPLATFORM_FILELIST *)hEntry;

  pFileList->hData = NULL;

  return NUAN_OK;

} /* vplatform_data_RemoveData */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetPlatformInfo(
    void *  hDataClass,
    void **pPlatformInfo)
{
  VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;

  *pPlatformInfo = pClass->pPlatformInfo;

  return NUAN_OK;
} /* vplatform_data_GetPlatformInfo */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_SetPlatformInfo(
    void *  hDataClass,
    void *pPlatformInfo)
{
  VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;

  pClass->pPlatformInfo = pPlatformInfo;

  return NUAN_OK;
} /* vplatform_data_SetPlatformInfo */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_AddTuningData(
    void *  hDataClass,
    VPLATFORM_RESOURCES * pResources)
{
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;

    if (pResources->u16NbrOfDataInstall != 0)
    {
        VPLATFORM_FILELIST ** ppFileList = &(pClass->pFileList);
        /* move ppFileList to the end of the list so that in case of an error,
         * only the additional found files will be removed again, and
         * not the original list in the data class
         * But only do this when there is no check on duplicates
         */
        if (pClass->bErrorCheck == NUAN_FALSE)
        {
            while (*ppFileList != NULL)
            {
                ppFileList = &((*ppFileList)->pNext);
            }
        }

        /* Find additional *.dat files in the provided installation paths
         * and add them to the data class
         */
        fRet = vplatform_FindFiles((void * )pClass, pClass->hHeap,
                                    pResources->u16NbrOfDataInstall,
                                    (const PLATFORM_TCHAR * const *)pResources->apDataInstall,
                                    NULL, /* do not add any new *.hdr files! */
                                    ppFileList,
                                    NUAN_FALSE /* do not search subdirectories */);
    }
    return fRet;
} /* vplatform_data_AddTuningData */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_RemoveTuningData(
    void *  hDataClass,
    VPLATFORM_RESOURCES * pResources)
{
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;

    if (pResources->u16NbrOfDataInstall != 0)
    {
        /* Find *.dat files in the provided installation paths
         * and remove corresponding entries from the data class
         */
        fRet = vplatform_RemoveFiles((void * )pClass, pClass->hHeap,
                                     pResources->u16NbrOfDataInstall,
                                     (const PLATFORM_TCHAR * const *)pResources->apDataInstall,
                                     &(pClass->pFileList),
                                     NUAN_FALSE);
    }
    return fRet;
} /* vplatform_data_RemoveTuningData */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_AddBrokeredData(
    void *  hDataClass,
    const PLATFORM_TCHAR  * szFile,
    const char            * szBrokerString)
{
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;
    PLATFORM_TCHAR *szFileCopy = NULL;
    char *szBrokerCopy = NULL;
    NUAN_U32 i = 0, len = 0;

    /* copy file and broker string so that application can free this */
    szBrokerCopy = vplatform_heap_Calloc(pClass->hHeap, (NUAN_U32)strlen(szBrokerString) + 1, sizeof(char));
    if (szBrokerCopy == NULL)
    {
        return NUAN_E_OUTOFMEMORY;
    }
    strcpy(szBrokerCopy, szBrokerString);
    while (szFile[len] != 0) len++;
    szFileCopy = vplatform_heap_Calloc(pClass->hHeap, (NUAN_U32)(len + 1), sizeof(PLATFORM_TCHAR));
    if (szFileCopy == NULL)
    {
        vplatform_heap_Free(pClass->hHeap, szBrokerCopy);
        return NUAN_E_OUTOFMEMORY;
    }
    while (i <= len)
    {
        szFileCopy[i] = szFile[i];
        i++;
    }

    /* Add to data class */
    fRet = vplatform_AddFileToFileList(hDataClass, szFileCopy, szBrokerCopy, &(pClass->pFileList));
    if (fRet == NUAN_W_ALREADYPRESENT)
    {
        vplatform_heap_Free(pClass->hHeap, szBrokerCopy);
        vplatform_heap_Free(pClass->hHeap, szFileCopy);
        fRet = NUAN_OK;
    }

    return fRet;
} /* vplatform_data_AddBrokeredData */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_RemoveBrokeredData(
    void *  hDataClass,
    const char          * szBrokerString)
{
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_DATA_CLASS_T * pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;

    fRet = vplatform_RemoveFileFromFileList(hDataClass, szBrokerString, &(pClass->pFileList));

    return fRet;
} /* vplatform_data_RemoveBrokeredData */

/*===================================================================
**  Definition of static interfaces
**==================================================================*/

/* ------------------------------------------------------------------
**  Data access services
** -----------------------------------------------------------------*/

static const VE_DATA_STREAM_INTERFACE     IDataStream = {
  vplatform_datastream_Open,
  vplatform_datastream_Close,
  vplatform_datastream_Read,
  vplatform_datastream_Seek,
  vplatform_datastream_GetSize,
  vplatform_datastream_Error,
  vplatform_datastream_Write               /* optional */
};

/* entire interface is optional */
#if defined(VPLATFORM_USE_DATA_MAPPING)
static const VE_DATA_MAPPING_INTERFACE     IDataMapping = {
  vplatform_datamapping_Open,
  vplatform_datamapping_Close,
  vplatform_datamapping_Map,
  vplatform_datamapping_Unmap,
  vplatform_datamapping_Freeze,
};
#endif /* VPLATFORM_USE_DATA_MAPPING */


/* ******************************************************************
**  PUBLIC INTERFACE RETRIEVAL FUNCTIONS
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetInterface(
    VE_INSTALL       * pInstall,
    VPLATFORM_RESOURCES * pResources)
{
  NUAN_ERROR fRet = NUAN_OK;
  VPLATFORM_FILELIST *pFileList = NULL;
  VPLATFORM_DATA_CLASS_T *pClass = NULL;
  void * hHeap = pInstall->hHeap;

  pInstall->hDataClass = NULL;

  pClass = (VPLATFORM_DATA_CLASS_T *)vplatform_heap_Calloc(hHeap, 1, sizeof(VPLATFORM_DATA_CLASS_T));
  if (! pClass)
  {
    return NUAN_E_OUTOFMEMORY;
  }
  pClass->hHeap = hHeap;

#ifdef _INIT_PLATFORM_
  /* initialize OS dependent data */
  fRet = vplatform_Init((void * )pClass, hHeap);
  if (fRet != NUAN_OK)
  {
      vplatform_heap_Free(hHeap, pClass);
      return fRet;
  }
#endif /* _INIT_PLATFORM_ */

  /* check for errors, such as identical filenames in separate directories */
  if (pResources->bFlags & F_ERROR_CHECK) pClass->bErrorCheck = NUAN_TRUE;

  if (pResources->u16NbrOfDataInstall != 0)
  {
    /* Initialize the engine by finding individual *.hdr files in the
    ** configured installation paths */
    fRet = vplatform_FindFiles((void * )pClass, hHeap,
                               pResources->u16NbrOfDataInstall,
                               (const PLATFORM_TCHAR * const *)pResources->apDataInstall,
                               &(pClass->szBrokerInfo),
                               &pFileList,
                               NUAN_TRUE);
    if (fRet != NUAN_OK) goto errorGetItf;

    /* Store file list and broker info to file (optional) */
    fRet = vplatform_data_StoreDataFiles(pClass->szBrokerInfo,
                                         pFileList,
                                         (void * )pClass,
                                         hHeap,
                                         pResources->szBinaryBrokerFile,
                                         pResources->szFileListFile);
    if (fRet != NUAN_OK) goto errorGetItf;
  }
  else if (pResources->szFileListFile != NULL)
  {
    /* Initialize the engine by reading a listfile with brokerstrings and related files,
    ** and a binary broker file */
    fRet = vplatform_ParseListFile((void * )pClass, hHeap,
                                   pResources->szFileListFile,
                                   &pFileList);
    if (fRet != NUAN_OK) goto errorGetItf;

    if (pResources->szBinaryBrokerFile != NULL)
    {
      fRet = vplatform_GetBinaryBrokerFile((void * )pClass, hHeap,
                                           pResources->szBinaryBrokerFile,
                                           &(pClass->szBrokerInfo));
    }
    if (fRet != NUAN_OK) goto errorGetItf;
  }

  /* only overwrite broker info if it wasn't specified before */
  if (pInstall->pBinBrokerInfo == NULL) pInstall->pBinBrokerInfo = pClass->szBrokerInfo;

  pClass->pFileList = pFileList;

  pInstall->pIDataStream = &IDataStream;
#if defined(VPLATFORM_USE_DATA_MAPPING)
  pInstall->pIDataMapping = &IDataMapping; /* optional interface */
#else
  pInstall->pIDataMapping = NULL;          /* optional interface */
#endif
  pInstall->hDataClass = (void * )pClass;

  return NUAN_OK;

errorGetItf:
  vplatform_data_ReleaseInterface((void * )pClass);
  return fRet;

} /* vplatform_data_GetInterface */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_ReleaseInterface(
  void *                          hDataClass)
{
  VPLATFORM_DATA_CLASS_T *pClass = (VPLATFORM_DATA_CLASS_T *)hDataClass;

  if (! hDataClass) return NUAN_E_NULLPOINTER;

#ifdef _INIT_PLATFORM_
    vplatform_Uninit((void * )pClass);
#endif
  vplatform_FreeFiles(pClass->hHeap, pClass->szBrokerInfo, pClass->pFileList);
  vplatform_heap_Free(pClass->hHeap, pClass);

  return NUAN_OK;
} /* vplatform_data_ReleaseInterface */

/* ******************************************************************
**  END
** ******************************************************************/
