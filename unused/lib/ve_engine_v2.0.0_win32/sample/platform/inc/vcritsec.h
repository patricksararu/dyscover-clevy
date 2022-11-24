#ifndef __VCRITSEC_H__
#define __VCRITSEC_H__

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

#if defined( __cplusplus )
extern "C"
{
#endif

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/* ------------------------------------------------------------------
**  Critical Sections service
** -----------------------------------------------------------------*/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_ObjOpen(
  void *      hCCritSec,
  void *      hHeap,
  void *  * phCritSec
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_ObjClose(
  void *  hCritSec
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_Enter(
  void *  hCritSec
);

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_Leave(
  void *  hCritSec
);

/*-------------------------------------------------------------------
**  @func   Get the interface and a class handle of the Critical
**          Sections service.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_GetInterface(
    VE_INSTALL       * pInstall,   /* @parm [out] <nl>
                                      ** Vocalizer install info */
    VPLATFORM_RESOURCES * pResources  /* @parm [in] <nl>
                                      ** Platform resources*/
);

/*-------------------------------------------------------------------
**  @func   Release the handle of the Critical Sections service.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_ReleaseInterface(
  void                  *   hCSClass    /* @parm [in] <nl>
                                        ** Handle of concern */
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VCRITSEC_H__ */
