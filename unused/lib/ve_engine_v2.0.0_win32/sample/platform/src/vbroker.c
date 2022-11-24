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

#include <string.h>
#include "vplatform.h"

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/


/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/


/* ******************************************************************
**  GLOBAL FUNCTIONS
** ******************************************************************/

NUAN_ERROR vplatform_data_BuildFilenameFromBrokerId(
        char * szFileName,
  const char * szBrokerId)
{
  size_t i = 0;

  for (i = 0; i < strlen(szBrokerId); i++)
  {
    if ((('a' <= szBrokerId[i]) && (szBrokerId[i] <= 'z')) ||
        (('0' <= szBrokerId[i]) && (szBrokerId[i] <= '9')))
    {
      szFileName[i] = szBrokerId[i];
    }
    else if ('/' == szBrokerId[i])
    {
      szFileName[i] = '_';
    }
    else
    {
      szFileName[i] = '-';
    }
  }
  szFileName[i] = '\0';

  strcat(szFileName, ".dat");

  return NUAN_OK;

} /* vplatform_data_BuildFilenameFromBrokerId */

NUAN_ERROR vplatform_data_BuildBrokerIdFromFilename(
        char * szBrokerId,
  const char * szFileName)
{
  size_t i = 0;

  for (i = 0; i < strlen(szFileName); i++)
  {
    if ('_' == szFileName[i])
    {
      szBrokerId[i] = '/';
    }
    else if ('.' == szFileName[i])
    {
      szBrokerId[i] = 0;
      break;
    }
    else
    {
      szBrokerId[i] = szFileName[i];
    }
  }

  return NUAN_OK;

} /* vplatform_data_BuildBrokerIdFromFilename */
