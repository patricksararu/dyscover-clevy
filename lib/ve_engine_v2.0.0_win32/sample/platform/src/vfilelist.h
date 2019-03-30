#ifndef __VFILELIST_H__
#define __VFILELIST_H__

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


/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/*-------------------------------------------------------------------
**  @func   Parses a single list file containing the relation between
**          broker string and the full path to the data files
**  @comm   Dynamically allocates the appropriate space for the list
**          of broker entries
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_ParseListFile(
        void *          hDataClass,
                                                /* @parm [in] <nl>
                                                ** Data class handle */
        void *      hHeap,    /* @parm [in] <nl>
                                                ** Instance handle of Heap service */
  const PLATFORM_TCHAR              * szListFile,
                                                /* @parm [in] <nl>
                                                ** list file describing data components */
        VPLATFORM_FILELIST         ** ppFileList
                                                /* @parm [out] <nl>
                                                ** List of broker strings & corresponding data files */
);

/*-------------------------------------------------------------------
**  @func   Read contents of the binary broker file
**  @rdesc  NUAN_ERROR
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_GetBinaryBrokerFile(
        void *          hDataClass,
                                                /* @parm [in] <nl>
                                                ** Data class handle */
        void *      hHeap,    /* @parm [in] <nl>
                                                ** Instance handle of Heap service */
  const PLATFORM_TCHAR              * szBinaryBrokerFile,
                                                /* @parm [in] <nl>
                                                ** Binary broker file name */
        char                       ** pszBrokerInfo
                                                /* @parm [out] <nl>
                                                ** Broker file contents */
                                                
);

/*-------------------------------------------------------------------
**  @func   Check if a file exists prior to reading it
**  @rdesc  NUAN_BOOL
**------------------------------------------------------------------*/
NUAN_BOOL vplatform_CheckFileExists(
        void *          hDataClass,
        void *      hHeap,
  const PLATFORM_TCHAR              * szFile
);


#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VFILELIST_H__ */
