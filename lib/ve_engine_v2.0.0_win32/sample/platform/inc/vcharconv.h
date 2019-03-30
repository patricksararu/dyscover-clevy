#ifndef __VCHARCONV_H__
#define __VCHARCONV_H__

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
**  (c) Copyright 2013 Nuance Communications, Inc.
**  All rights reserved. Company confidential.
**  
** ******************************************************************/

/* ******************************************************************
**  HEADER (INCLUDE) SECTION
** ******************************************************************/

#include "vplatform.h"

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

/* ------------------------------------------------------------------
**   Character conversion
** -----------------------------------------------------------------*/

/*-------------------------------------------------------------------
**  @func   char to PLATFORM_TCHAR .
**  @comm   converts UTF-8 to PLATFORM_TCHAR type
**          source should be zero terminated
**          returned conversion is allocated on the heap
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_UTF8_to_PLATFORM_TCHAR(
    PLATFORM_TCHAR             **pDst,
    const char                  *pSrc,
    void *     hHeap
);

/*-------------------------------------------------------------------
**  @func   PLATFORM_TCHAR to char
**  @comm   converts PLATFORM_TCHAR to UTF-8
**          source should be zero terminated
**          returned conversion is allocated on the heap
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_PLATFORM_TCHAR_to_UTF8(
    char                       **pDst,
    const PLATFORM_TCHAR        *pSrc,
    void *     hHeap
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VCHARCONV_H__ */
