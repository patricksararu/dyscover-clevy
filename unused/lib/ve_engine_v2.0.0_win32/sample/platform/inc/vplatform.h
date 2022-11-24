#ifndef __VPLATFORM_H__
#define __VPLATFORM_H__

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

#ifdef PLATFORM_UNICODE
#ifdef __SYMBIAN32__
#include <stddef.h>
#else
#include <wchar.h>
#endif
#endif

#include "ve_platform.h"

#ifdef PLATFORM_UNICODE
typedef wchar_t            PLATFORM_TCHAR;
#else
#ifdef PLATFORM_UTF16
typedef short              PLATFORM_TCHAR;
#else
typedef char               PLATFORM_TCHAR;
#endif
#endif


#if defined( __cplusplus )
extern "C"
{
#endif

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

#define VPLATFORM_CURRENT_VERSION ((NUAN_U16) 0x0200)

/* @type VPLATFORM_VERSION | 16-bit version ID where the first 8 bits
** are the major version number and the second 8 bits the minor
** version number. */
typedef NUAN_U16 VPLATFORM_VERSION;

/* @struct VPLATFORM_MEMBLOCK | Memory buffer */
typedef struct {
  void      * pStart;   /* Pointer to start of data buffer */
  size_t      cByte;    /* Size of buffer, in bytes */
  NUAN_U32    cFlags;   /* flags to configure heap */
} VPLATFORM_MEMBLOCK;


/* @struct VPLATFORM_RESOURCES | Platform resources. */
typedef struct {
  VPLATFORM_VERSION     fmtVersion;               /* format version of this struct */
  NUAN_U16              u16NbrOfDataInstall;      /* Number of data install items */
  PLATFORM_TCHAR     ** apDataInstall;            /* Array of install items */
  VPLATFORM_MEMBLOCK    stHeap;                   /* heap memory block */
  void                * pDatPtr_Table;            /* Optional data pointers for static deployments */
  const PLATFORM_TCHAR  * szBinaryBrokerFile;     /* Optional file with broker information */
  const PLATFORM_TCHAR  * szFileListFile;         /* Optional list of data file locations */
  NUAN_U32              bFlags;                   /* Additional configuration flags (implementation dependent) */
  /* reserved for future use */
  NUAN_U32 rfu1;
} VPLATFORM_RESOURCES;

#define F_ERROR_CHECK     0x1                     /* bitfield specifier for error checking flag (implementation dependent) */   

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/* ------------------------------------------------------------------
**   All services
** -----------------------------------------------------------------*/

/*-------------------------------------------------------------------
**  @func   Get all interfaces and instance handles for the platform
**          services.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_GetInterfaces(
  VE_INSTALL            * pInstall,                 /* @parm [in/out] <nl>
                                                       ** Structure holding interfaces <nl>
                                                       ** and binary broker data */
  VPLATFORM_RESOURCES      * pResources          /* @parm [in] <nl>
                                                       ** Platform specific resources */
);

/*-------------------------------------------------------------------
**  @func   Release all interfaces and handles.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_ReleaseInterfaces(
  VE_INSTALL     * pInstall    /* @parm [in] <nl>
                              ** Structure holding interfaces */
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VPLATFORM_H__ */
