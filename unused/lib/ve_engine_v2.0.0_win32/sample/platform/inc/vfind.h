#ifndef __VFIND_H__
#define __VFIND_H__

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

struct VPLATFORM_FIND_S;
typedef struct VPLATFORM_FIND_S VPLATFORM_FIND_T;
typedef VPLATFORM_FIND_T * VPLATFORM_FIND_H;

typedef enum {VPLATFORM_FIND_OTHER, VPLATFORM_FIND_DIR, VPLATFORM_FIND_HDR, VPLATFORM_FIND_DATA} VPLATFORM_FIND_TYPE;

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/*-------------------------------------------------------------------
**  @func   Creates a file/directory iterator on the contents of
**          directory <p szDir>, and returns it in *<p phFind>.
**          Returns in **<p pszElem> the first element found and its
**          type *<p peType>.
**  @comm   Excludes the directories "." or ".." from
**          the list of returned elements. <nl>
**          Dynamically allocates the appropriate space for the name
**          in *<p pszElem> using <p hHeap>.
**  @rdesc  NUAN_ERROR | Success or failure
**  @flag   NUAN_NUAN_E_NOK | No elements are found
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_find_Open(
        void *        hDataClass,/* @parm [in] <nl>
                                               ** Instance handle of data class */
        void *    hHeap,    /* @parm [in] <nl>
                                              ** Instance handle of Heap service */
  const PLATFORM_TCHAR            * szDir,    /* @parm [in] <nl>
                                              ** Pathname of directory of concern */
        PLATFORM_TCHAR           ** pszElem,  /* @parm [out] <nl>
                                              ** Location for filename of first
                                              ** element found */
        VPLATFORM_FIND_TYPE       * peType ,  /* @parm [out] <nl>
                                              ** Whether *pszElem is a directory, a *.hdr or a *.dat file */
        VPLATFORM_FIND_H          * phFind    /* @parm [out] <nl>
                                              ** Location for created iterator */
);

/*-------------------------------------------------------------------
**  @func   Gets the next element from the file/directory iterator
**          <p hFind>. Returns its filename in **<pszElem>, and
**          its type *<p peType>.
**  @comm   Excludes the directories "." or ".." "." or ".." from
**          the list of returned elements. <nl>
**          Dynamically allocates the appropriate space for the name
**          in *<p pszElem> using <p hHeap>.
**  @rdesc  NUAN_ERROR | Success or failure
**  @flag   NUAN_NUAN_E_NOK | No elements are found
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_find_Next(
  const VPLATFORM_FIND_H      hFind,   /* @parm [in] <nl>
                                       ** Iterator of concern */
        PLATFORM_TCHAR     ** pszElem, /* @parm [out] <nl>
                                       ** Location for filename of next
                                       ** element found */
        VPLATFORM_FIND_TYPE * peType   /* @parm [out] <nl>
                                       ** Whether *pszElem is a directory, a *.hdr or a *.dat file */
);

/*-------------------------------------------------------------------
**  @func   Releases the file/directory iterator *<p hFind>.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_find_Close(
  VPLATFORM_FIND_H hFind
);

/*-------------------------------------------------------------------
**  @func   Appends the filename <p szFile> to the path <p szDir>.
**  @comm   Dynamically allocates the appropriate space for the
**          pathname using <p hHeap>.
**          The caller is responsible for freeing this memory.
**  @rdesc  char * | The concatenated path
**------------------------------------------------------------------*/
PLATFORM_TCHAR * vplatform_CombinePath(
        void *    hHeap,
  const PLATFORM_TCHAR            * szDir,
  const PLATFORM_TCHAR            * szFile
);

/*-------------------------------------------------------------------
**  @func   Converts the TCHAR filename <p szFile> to char.
**  @comm   Dynamically allocates the appropriate space for the
**          pathname using <p hHeap>.
**          The caller is responsible for freeing this memory.
**  @rdesc  char * | The converted filename.
**------------------------------------------------------------------*/
char * vplatform_CopyToChar(
  void *          hHeap,
  const PLATFORM_TCHAR                * szFile
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VFIND_H__ */
