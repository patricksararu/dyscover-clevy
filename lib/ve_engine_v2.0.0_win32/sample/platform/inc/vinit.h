#ifndef __VINIT_H__
#define __VINIT_H__

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
**   Platform initialization
** -----------------------------------------------------------------*/

/*-------------------------------------------------------------------
**  @func   Initialize the platform.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_Init(
  void *         hDataClass,
  void *     hHeap
);

/*-------------------------------------------------------------------
**  @func   Uninitialize the platform.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_Uninit(
  void *         hDataClass
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VINIT_H__ */
