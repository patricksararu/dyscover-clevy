#ifndef __VEPLATFORM_H__
#define __VEPLATFORM_H__
/* -------------------------------------------------------------------------+
 |                      Nuance Communications, Inc (TM)                     |
 + -------------------------------------------------------------------------*/

/* --------------------------- COPYRIGHT INFORMATION -----------------------+
 | This program contains proprietary information that is a trade secret of  |
 | Nuance Communications, Inc. and also is protected as an unpublished work |
 | under applicable Copyright laws. Recipient is to retain this program in  |
 | confidence and is not permitted to use or make copies thereof other than |
 | as permitted in a prior written agreement with Nuance Communications,    |
 | Inc. or its affiliates.                                                  |
 | Copyright (c) 2003-2007 Nuance Communications, Inc.                      |
 | All Rights Reserved. Company confidential.                               |
 + -------------------------------------------------------------------------*/

/* -------------------------------------------------------------------------+
 |   HEADER (INCLUDE) SECTION                                               |
 + -------------------------------------------------------------------------*/

/* header files */
#include "ve_ttsapi.h"

#if defined( __cplusplus )
extern "C"
{
#endif

/* -------------------------------------------------------------------------+
 |   TYPE DEFINITIONS                                                       |
 + -------------------------------------------------------------------------*/

struct VE_HEAP_INTERFACE_S {
  /* Called when a component needs memory. Should return an address
   * which starts a memory block of at least \p cBytes bytes.  The
   * memory should be aligned such that any C-type can be stored at
   * the returned address.  \p hHeap can be used to identify the
   * heap that is used.
   * \return The address at which the memory is allocated. If no
   * memory is available return \p NULL. */
  void* (*pfMalloc)(void * hHeap,
                    size_t cBytes);

  /* Called when a component needs memory that is initialized with
   * zeroes.  Should return an address which starts a memory block of
   * at least <tt>cElements * cElementBytes</tt> bytes. The memory
   * should be aligned such that any C-type can be stored at the
   * returned address. The returned memory must be filled with zeroes.
   * \p hHeap can be used to identify the heap that is used.
   * \return The address at which the memory is allocated. If no
   * memory is available return \p NULL. */
  void* (*pfCalloc)(void *      hHeap,
                    size_t      cElements,
                    size_t      cElementBytes);

  /* Called when a component want to extend or shrink a memory block.
   * Should return an address which starts a memory block of at least
   * \p cBytes bytes.  The memory should be aligned such that any
   * C-type can be stored at the returned address.  Data starting at
   * the address \p pData should be copied into the new memory block.
   * The data may be truncated if \p cBytes is less than the length of
   * the memory block starting at address \p pData.  \p hHeap can
   * be used to identify the heap that is used.
   * \return The address at which the memory is allocated. If no
   * memory is available return \p NULL and leave the memory at
   * address \p pData allocated and unchanged. */
  void* (*pfRealloc)(void                   * hHeap,
                     void                   * pData,
                     size_t                  cBytes);

  /* Called when a component has memory that can be freed.  \p
   * hHeap can be used to identify the heap that is used.
   */
  void (*pfFree)(void * hHeap,
                 void * pData);
};


struct VE_CRITSEC_INTERFACE_S {
  NUAN_ERROR (*pfOpen)(void                    * hCritSecClass,
                       void                    * hHeap,
                       void *     * phCritSec);

  NUAN_ERROR (*pfClose)(void *    hCritSec);

  NUAN_ERROR (*pfEnter)(void *    hCritSec);

  NUAN_ERROR (*pfLeave)(void *    hCritSec);
};

/* Origin for seek operations */
typedef enum VE_STREAM_ORIGIN_E {
  VE_STREAM_SEEK_SET   = 0,
  VE_STREAM_SEEK_CUR   = 1,
  VE_STREAM_SEEK_END   = 2
} VE_STREAM_ORIGIN;

/* Direction for seek operations */
typedef enum VE_STREAM_DIRECTION_E {
  VE_STREAM_BACKWARD   = -1,
  VE_STREAM_FORWARD    =  1
} VE_STREAM_DIRECTION;

struct VE_DATA_STREAM_INTERFACE_S {
  NUAN_ERROR (*pfOpen)(void *            hDataClass,
                       void                   * hHeap,
                       const char             * szBrokerString,
                       const char             * szMode,
                       void *         * phStream);

  /* When called the engine is done reading this stream.  This
   * function will usually be mapped on some close function of the
   * user's stream.  \return Should return \p 0 if succesful, or a
   * non-zero value in case of failure. */
  NUAN_ERROR (*pfClose)(void *  hStream);

  /* When called the user needs to copy \p count elements of size \p
   * cBytes into \p buffer which must be sufficiently large.  \return
   * Should return the number of items actually read. If this number
   * is less than \p count then the end of the stream is reached. */
  size_t   (*pfRead)(void                * pBuffer,
                     size_t                cElementBytes,
                     size_t                cElements,
                     void *        hStream);

  /* When called the user needs to adjust the position in the
   * stream. \p u32Offset indicates the jump. \p eOrigin specificies
   * whether offset is relative to the beginning of the file, relative
   * to the current position or relative to the end of the file.  
   * \p eDirection specifies whether the offset goes forwards from the
   * origin or backwards from the origin. \return Should return \p 0
   * if succesful, or a non-zero value in case of failure. */
  NUAN_ERROR (*pfSeek)(void *            hStream,
                       size_t                    cOffset,
                       VE_STREAM_ORIGIN          eOrigin,
                       VE_STREAM_DIRECTION       eDirection);

  size_t     (*pfGetSize)(void *   hStream);

  /* Callback to check error state of the stream.  \return non-zero
   * value if the stream is in an error state, otherwise return \p
   * 0 */
  NUAN_ERROR (*pfError)(void *   hStream);
  
  size_t     (*pfWrite)(const void          * pBuffer,
                        size_t                cElementBytes,
                        size_t                cElements,
                        void *        hStream);
};

struct VE_DATA_MAPPING_INTERFACE_S {
  NUAN_ERROR (*pfOpen)(void *            hDataClass,
                       void                   * hHeap,
                       const char             * szBrokerString,
                       void *        * phMapping);

  NUAN_ERROR (*pfClose)(void *   hMapping);

  NUAN_ERROR (*pfMap)(void *        hMapping,
                      size_t                 cOffset,
                      size_t               * pcBytes,
                      const void          ** ppData);

  NUAN_ERROR (*pfUnmap)(void *        hMapping,
                        const void           * pData);

  NUAN_ERROR (*pfFreeze)(void *   hMapping);
};

struct VE_LOG_INTERFACE_S {
  void (*pfError)(void *   hLog,
                  NUAN_U32               u32ErrorID,
                  size_t                 cKeyValues,
                  const char          ** aszKeys,
                  const char          ** aszValues);

  void (*pfDiagnostic)(void *   hLog,
                       NUAN_S32               s32Level,
                       const char           * szMessage);
};

#if defined( __cplusplus )
}
#endif

/* -------------------------------------------------------------------------+
 |   END                                                                    |
 + -------------------------------------------------------------------------*/

#endif /* #ifndef __VEPLATFORM_H__ */

