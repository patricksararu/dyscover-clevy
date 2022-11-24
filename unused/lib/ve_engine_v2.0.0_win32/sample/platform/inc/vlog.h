#ifndef __VLOG_H__
#define __VLOG_H__

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
**  User Log service
** -----------------------------------------------------------------*/

/*-------------------------------------------------------------------
**  @func   Get the interface and an instance handle of the User Log
**          service.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_log_GetInterface(
    VE_INSTALL       * pInstall,   /* @parm [out] <nl>
                                      ** Vocalizer install info */
    VPLATFORM_RESOURCES * pResources  /* @parm [in] <nl>
                                      ** Platform resources*/
);

/*-------------------------------------------------------------------
**  @func   Release the handle of the User Log service.
**  @rdesc  NUAN_ERROR | Success or failure
**------------------------------------------------------------------*/
NUAN_ERROR vplatform_log_ReleaseInterface(
  void *  hLog    /* @parm [in] <nl>
                              ** Handle of concern */
);


#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VLOG_H__ */
