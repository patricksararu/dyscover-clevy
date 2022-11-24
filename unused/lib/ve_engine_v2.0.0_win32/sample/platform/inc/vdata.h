#ifndef __VDATA_H__
#define __VDATA_H__

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
#include "vheap.h"

#if defined( __cplusplus )
extern "C"
{
#endif

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/* handle to data entry in the data class.
 * This entry contains the link between the broker string and the
 * file name or cached data (stream or mapping handle)
 */
typedef void * VPLATFORM_DATA_HENTRY;

/* handle to the data cached in a data entry (stream or mapping handle) */
typedef void * VPLATFORM_DATA_HDATA;

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/* ------------------------------------------------------------------
**  Data access services
** -----------------------------------------------------------------*/

/*-------------------------------------------------------------------
**  @func   Compiles in <p szFileName> the filename of the data
**          component identified by the broker string <szBrokerId>.
**  @comm   The Vocalizer SDK has a separate file for each data component,
**          where the filename is the broker string with '/' converted to
**          '_', [^a-z0-9] converted to '-' and ".dat" appended.
**          This function expects that the caller has appropriately
**          dimensioned <szFileName> (alloc size >= strlen(szBrokerId) + 5).
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_BuildFilenameFromBrokerId(
        char * szFileName,
  const char * szBrokerId);

/*-------------------------------------------------------------------
**  @func   Compiles in <p szBrokerId> the broker string of the data
**          component identified by the filename <szFileName>.
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_BuildBrokerIdFromFilename(
        char * szBrokerId,
  const char * szFileName);

/*-------------------------------------------------------------------
**  @func   Retrieves the full path <p pszFullPath> to <p szName>
**          from the Data Class.
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetFullPathName(
  const char                   * szName,
        PLATFORM_TCHAR        ** pszFullPathName,
        void *     hDataClass
);

/*-------------------------------------------------------------------
**  @func   Releases path name returned by vplatform_data_GetFullPathName
**          or vplatform_data_GetEntry
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_FreeFullPathName(
        PLATFORM_TCHAR         * szFullPathName,
        void *     hDataClass
);

/*-------------------------------------------------------------------
**  @func   Retrieves the entry in the data class with all the info
**          for <p szName>, including the data handle <p phData>
**          and the full path <p pszFullPath>.
**          Only used if the Data Class does cache data handles
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetEntry(
        void *    hDataClass,
  const char                  * szName,
        VPLATFORM_DATA_HENTRY * phEntry,
        PLATFORM_TCHAR       ** szFullPathName,
        VPLATFORM_DATA_HDATA  * phData
);

/*-------------------------------------------------------------------
**  @func   Stores the data handle in the data class entry previously
**          retrieved via vplatform_data_GetEntry
**          Only used if the Data Class does cache data handles
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_StoreData(
        VPLATFORM_DATA_HENTRY  hEntry,
        VPLATFORM_DATA_HDATA   hData
);

/*-------------------------------------------------------------------
**  @func   Removes the data handle in the data class entry previously
**          retrieved via vplatform_data_GetEntry
**          Only used if the Data Class does cache data handles
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_RemoveData(
        VPLATFORM_DATA_HENTRY  hEntry
);

/*-------------------------------------------------------------------
**  @func   Get platform information
**          Only used if the data class initializes the platform
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetPlatformInfo(
    void *  hDataClass,
    void **pPlatformInfo
);

/*-------------------------------------------------------------------
**  @func   Set platform information
**          Only used if the data class initializes the platform
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_SetPlatformInfo(
    void *  hDataClass,
    void *pPlatformInfo
);

/*-------------------------------------------------------------------
**  @func   Add tuning data
**          Add additional install items to the data class,
**          only for data, not for broker header files
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_AddTuningData(
    void *  hDataClass,
    VPLATFORM_RESOURCES * pResources
);

/*-------------------------------------------------------------------
**  @func   Remove tuning data
**          Remove install items from the data class,
**          only for data, not for broker header files
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_RemoveTuningData(
    void *  hDataClass,
    VPLATFORM_RESOURCES * pResources
);

/*-------------------------------------------------------------------
**  @func   Add brokered data
**          Add additional install items to the data class,
**          only for data, identified by a specific broker string
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_AddBrokeredData(
    void *  hDataClass,
    const PLATFORM_TCHAR  * szFile,
    const char            * szBrokerString
);

/*-------------------------------------------------------------------
**  @func   Remove brokered data
**          Remove install items from the data class,
**          only for data, identified by a specific broker string
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_RemoveBrokeredData(
    void *  hDataClass,
    const char          * szBrokerString
);

/*-------------------------------------------------------------------
**  @func   Get the interface and a class handle of the data access
**          services.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_GetInterface(
    VE_INSTALL       * pInstall,   /* @parm [out] <nl>
                                      ** Vocalizer install info */
    VPLATFORM_RESOURCES * pResources  /* @parm [in] <nl>
                                      ** Platform resources*/
);

/*-------------------------------------------------------------------
**  @func   Release the handle of the data access services.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_ReleaseInterface(
  void *      hDataClass  /* @parm [in] <nl>
                                        ** Handle of concern */
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VDATA_H__ */
