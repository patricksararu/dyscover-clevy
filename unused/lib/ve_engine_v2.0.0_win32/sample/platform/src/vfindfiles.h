#ifndef __VFINDFILES_H__
#define __VFINDFILES_H__

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

#include "vdata.h"

#if defined( __cplusplus )
extern "C"
{
#endif


/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/
/* linked list of all *.dat files in the install directories */
typedef struct VPLATFORM_FILELIST_S
{
  PLATFORM_TCHAR              * szFullPathName; /* full path name to the file */
  char                        * szBrokerString; /* short file name converted to broker string */
  VPLATFORM_DATA_HDATA          hData;          /* data handle */
  NUAN_U32                      u32Cnt;         /* reference count */
  struct VPLATFORM_FILELIST_S * pNext;
} VPLATFORM_FILELIST;

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/*-------------------------------------------------------------------
**  @func   Searches for *.hdr and *.dat files
**          under the <p u16NbrOfDataInstallPaths> directories in
**          <p aszDataInstallPaths>[].
**          Compiles the broker info string for ttsInitialize by
**          concatenating all found *.hdr files
**          Returns a list of all found *.dat files
**  @comm   Dynamically allocates the appropriate space for the broker
**          info string and list of data files using <p hHeap>.
**  @rdesc  error code.
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_FindFiles(
        void *        hDataClass, /* @parm [in] <nl>
                                                ** data class handle */
        void *    hHeap,    /* @parm [in] <nl>
                                              ** Instance handle of Heap service */
        NUAN_U16                    u16NbrOfDataInstallPaths,
                                              /* @parm [in] <nl>
                                              ** Number of entries in aszDataInstallPaths[] */
  const PLATFORM_TCHAR    * const * aszDataInstallPaths,
                                              /* @parm [in] <nl>
                                              ** Array of install paths to data
                                              ** components */
        char                     ** pszBrokerInfo,
                                              /* @parm [out] <nl>
                                              ** Concatenated pipeline headers */
        VPLATFORM_FILELIST       ** pFileList,
                                              /* @parm [out] <nl>
                                              ** List of found data files */
        NUAN_BOOL                   bRecurse
                                              /* @parm [in] <nl>
                                              ** if true, also search subdirectories */

);

/*-------------------------------------------------------------------
**  @func   Searches for *.dat files
**          under the <p u16NbrOfDataInstallPaths> directories in
**          <p aszDataInstallPaths>[].
**          Removes corresponding entries from pFileList
**          Returns a list of remaining *.dat files
**  @comm   Dynamically allocates the appropriate space for the broker
**          info string <p hHeap>.
**  @rdesc  error code.
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_RemoveFiles(
        void *        hDataClass, /* @parm [in] <nl>
                                                ** data class handle */
        void *    hHeap,    /* @parm [in] <nl>
                                              ** Instance handle of Heap service */
        NUAN_U16                    u16NbrOfDataInstallPaths,
                                              /* @parm [in] <nl>
                                              ** Number of entries in aszDataInstallPaths[] */
  const PLATFORM_TCHAR    * const * aszDataInstallPaths,
                                              /* @parm [in] <nl>
                                              ** Array of install paths to data
                                              ** components */
        VPLATFORM_FILELIST       ** pFileList,
                                              /* @parm [in/out] <nl>
                                              ** List of found data files */
        NUAN_BOOL                   bRecurse
                                              /* @parm [in] <nl>
                                              ** if true, also search subdirectories */
);

/*-------------------------------------------------------------------
**  @func   Frees the broker info string <p szBrokerInfo> and
**          list of data files <p pFileList> previously
**          returned by vplatform_FindFiles.
**  @comm   Frees the memory using <p hHeap>.
**  @rdesc  void
**------------------------------------------------------------------*/
void vplatform_FreeFiles(
        void *    hHeap,    /* @parm [in] <nl>
                                              ** Instance handle of Heap service */
        char                      * szBrokerInfo,
                                              /* @parm [in] <nl>
                                              ** Broker info string */
        VPLATFORM_FILELIST        * pFileList
                                              /* @parm [in] <nl>
                                              ** List of data files */
);

/*-------------------------------------------------------------------
**  @func   Store the binary broker data to file, and store the
**          file list to file.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_data_StoreDataFiles(
        char                              * szBrokerInfo,
                                              /* @parm [in] <nl>
                                              ** Broker information */
        VPLATFORM_FILELIST                * pFileList,
                                              /* @parm [in] <nl>
                                              ** list of data files */
        void *                hDataClass,
                                              /* @parm [in] <nl>
                                                ** data class handle */
        void *            hHeap,
                                              /* @parm [in] <nl>
                                              ** heap handle */
  const PLATFORM_TCHAR                    * szBinaryBrokerFile,
                                              /* @parm [in] <nl>
                                              ** String containing binary broker file location */
  const PLATFORM_TCHAR                    * szFileListFile
                                              /* @parm [in] <nl>
                                              ** String containing file list file location */
);

/*-------------------------------------------------------------------
**  @func   Add new file path <pszFile> and according broker string
**          <p szBrokerString> to the file list *<p ppFileList>
**  @comm   Dynamically allocates the appropriate space for the
**          path <p szFile> and broker string <p pszBrokerString>
**          Path and broker string are NOT duplicated and should only
**          be freed after this info was reomoved from the file list
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_AddFileToFileList(
        void *             hDataClass,
                                              /* @parm [in] <nl>
                                                ** data class handle */
        PLATFORM_TCHAR                 * szFile,   /* @parm [in] <nl>
                                              ** full path name */
        char                           * szBrokerString,
                                              /*@parm [in] <nl>
                                              ** broker string corresponding to path name */
        VPLATFORM_FILELIST            ** ppFileList
                                              /* @parm [in/out] <nl>
                                              ** List of found data files */
);

/*-------------------------------------------------------------------
**  @func   remove class information corresponding to broker string
**          <p szBrokerString from the file list *<p ppFileList>
**  @comm   *<p ppFileList> should point to the beginning of the list
**          Class information data should have been freed prior to
**          calling this function.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_RemoveFileFromFileList(
        void *                hDataClass,
                                              /* @parm [in] <nl>
                                                ** data class handle */
        const char                         * szBrokerString,
                                              /*@parm [in] <nl>
                                              ** broker string */
        VPLATFORM_FILELIST                ** ppFileList
                                              /* @parm [in/out] <nl>
                                              ** List of data files */
);


/*-------------------------------------------------------------------
**  @func   Build broker string *<p pszBrokerInfo> from a full path
**          specification <p szFullPath>.
**          Optionally copy the full path into *<pszFullPathCopy> so
**          that the caller can free that data.
**  @comm   Dynamically allocates the appropriate space for
**          *<p pszBrokerString> and *<p pszFullPathCopy> using <p hHeap>.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_BuildBrokerIdFromFullPath(
    void *   hHeap,
    const PLATFORM_TCHAR      *szFullPath,
    char                     **pszBrokerString,
    PLATFORM_TCHAR           **pszFullPathCopy
);



#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VFINDFILES_H__ */
