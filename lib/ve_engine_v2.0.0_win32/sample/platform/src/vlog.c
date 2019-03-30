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
#include "vlog.h"

#include <stdlib.h>

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/*lint -esym(715, hLog, pResources) 
**           not used in this implementation */


/* ******************************************************************
**  LOCAL FUNCTIONS
** ******************************************************************/

/* ------------------------------------------------------------------
**  User Log service
** -----------------------------------------------------------------*/

/* ******************************************************************
**  PUBLIC INTERFACE RETRIEVAL FUNCTIONS
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_log_GetInterface(
    VE_INSTALL       * pInstall,
    VPLATFORM_RESOURCES * pResources)
{
  pInstall->pILog = NULL;
  pInstall->hLog = NULL;
  return NUAN_OK;
} /* vplatform_log_GetInterface */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_log_ReleaseInterface(
  void *                 hLog)
{
  return NUAN_OK;
} /* vplatform_log_ReleaseInterface */

/* ******************************************************************
**  END
** ******************************************************************/
