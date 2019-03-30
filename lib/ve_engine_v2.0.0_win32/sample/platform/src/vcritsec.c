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
#include "vcritsec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/

/* Enables a non-stub implementation of the critical section service */
#define VPLATFORM_CRITSEC

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/*lint -esym(715, hHeap, hCritSecClass, hCritSec) 
**           not used in this implementation */

#if defined(VPLATFORM_CRITSEC)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

typedef struct CRITSEC_S {
  CRITICAL_SECTION          CriticalSection;
  void *  hHeap;
} CRITSEC_T;

#endif /* VPLATFORM_CRITSEC */

/* ******************************************************************
**  FUNCTIONS
** ******************************************************************/

/* ------------------------------------------------------------------
**  Critical Sections service
** -----------------------------------------------------------------*/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_ObjOpen(
  void *      hCCritSec,
  void *      hHeap,
  void **     phCritSec)
{
#if !defined(VPLATFORM_CRITSEC)

  /* Stub implementation */

  /* Validate args */
  if (! phCritSec) {
    return NUAN_E_INVALIDARG;
  }

  /* Set output arg */
  *phCritSec = (void * ) 0x1234abcd;

#else

  CRITSEC_T * pCritSec = NULL;

  /* Dummy reference unused args */
  (void)hCCritSec;

  /* Validate args */
  if (! phCritSec) {
    return NUAN_E_INVALIDARG;
  }

  /* Init output arg */
  *phCritSec = NULL;

  /* Allocate the structure */
  pCritSec = (CRITSEC_T*)vplatform_heap_Calloc(hHeap, 1, sizeof(CRITSEC_T));
  if (! pCritSec)
  {
    return NUAN_E_OUTOFMEMORY;
  }
  pCritSec->hHeap = hHeap;

  /* use Win32 exception handling to catch predictable problems */
  __try {
    InitializeCriticalSection(&pCritSec->CriticalSection);
  }
  __except (GetExceptionCode() == STATUS_NO_MEMORY ? 
      EXCEPTION_EXECUTE_HANDLER : 
      EXCEPTION_CONTINUE_SEARCH ) {
    vplatform_heap_Free(hHeap, pCritSec);
    return NUAN_E_OUTOFMEMORY;
  }

  /* Set output arg */
  *phCritSec = pCritSec;

#endif /* VPLATFORM_CRITSEC */

  return NUAN_OK;
} /* vplatform_critsec_ObjOpen */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_ObjClose(
  void *  hCritSec)
{
#if defined(VPLATFORM_CRITSEC)

  CRITSEC_T * pCritSec = (CRITSEC_T *)hCritSec;

  /* No predictable problems, so no Win32 exception handling */
  DeleteCriticalSection(&pCritSec->CriticalSection);

  vplatform_heap_Free(pCritSec->hHeap, pCritSec);

#endif /* VPLATFORM_CRITSEC */

  return NUAN_OK;
} /* vplatform_critsec_ObjClose */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_Enter(
  void *  hCritSec)
{
#if defined(VPLATFORM_CRITSEC)

  CRITSEC_T * pCritSec = (CRITSEC_T *)hCritSec;

  /* Validate args */
  if (pCritSec == NULL) {
    return NUAN_OK;
  }

  __try {
    EnterCriticalSection(&pCritSec->CriticalSection);
  }
  __except (GetExceptionCode() == STATUS_INVALID_HANDLE ? 
      EXCEPTION_EXECUTE_HANDLER : 
      EXCEPTION_CONTINUE_SEARCH ) {
    /* Yes, Microsoft says STATUS_INVALID_HANDLE is returned on low memory */
    return NUAN_E_OUTOFMEMORY;
  }

#endif /* VPLATFORM_CRITSEC */

  return NUAN_OK;
} /* vplatform_critsec_Enter */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_Leave(
  void *  hCritSec)
{
#if defined(VPLATFORM_CRITSEC)

  CRITSEC_T * pCritSec = (CRITSEC_T *)hCritSec;

  if (pCritSec == NULL) {
    return NUAN_OK;
  }

  /* No predictable problems, so no Win32 exception handling */
  LeaveCriticalSection(&pCritSec->CriticalSection);

#endif /* VPLATFORM_CRITSEC */

  return NUAN_OK;
} /* vplatform_critsec_Leave */


/*===================================================================
**  Definition of static interfaces
**==================================================================*/

static const VE_CRITSEC_INTERFACE     ICritSec = {
  vplatform_critsec_ObjOpen,
  vplatform_critsec_ObjClose,
  vplatform_critsec_Enter,
  vplatform_critsec_Leave
};


/* ******************************************************************
**  PUBLIC INTERFACE RETRIEVAL FUNCTIONS
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_GetInterface(
  VE_INSTALL       * pInstall,
  VPLATFORM_RESOURCES * pResources)
{
  (void) pResources;
  pInstall->pICritSec = &ICritSec;
  pInstall->hCSClass = NULL;
  return NUAN_OK;
} /* vplatform_critsec_GetInterface */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_critsec_ReleaseInterface(void * hCSClass)
{
  (void) hCSClass;
  return NUAN_OK;
} /* vplatform_critsec_ReleaseInterface */


/* ******************************************************************
**  END
** ******************************************************************/
