#ifndef LIBRSTTS_ERROR
#define LIBRSTTS_ERROR

/**
 * @file librstts_error.h
 * @brief Error/result codes returned by rSpeak Text-to-Speech C API calls
 *
 * This header file defines error/result codes returned from the functions
 * declared in the C api defined in librstts.h (it is automatically
 * included by librstts.h and thus does not need to be included
 * explicitly). They will also be reported in failure events to provide
 * information about some errors (see librstts_event.h).
 *
 * Result values are represented as the int standard type. API users
 * should not make any assumptions regarding the meaning of these
 * values (unless clearly stated in the documentation of a specific
 * API call), except by testing them using the macros provided in
 * this file or comparing with result code constants returned in
 * this file. Future versions of the library may add additional
 * constants so an application using the API should not assume that
 * this file defines all error values that might occur.
 */

// Return value testers
#define RSTTS_SUCCESS(e)         ((e) >= 0)  /*!< @brief True if the result code indicates a success. */
#define RSTTS_ERROR(e)           ((e) < 0)   /*!< @brief True if the result code indicates an error. */
#define RSTTS_ERROR_PARAMETER(e) ((e) < 0 && (e) >= -100)     /*!< @brief True  if the result code indicates an error relating to the parameters provided to an API call. */
#define RSTTS_ERROR_OPERATION(e) ((e) < -100 && (e) >= -200)  /*!< @brief True if the result code indicates that an operation failed (for other reasons than related to the API parameters) */
#define RSTTS_ERROR_INTERNAL(e)  ((e) < -200 && (e) >= -300)  /*!< @brief True if an operation failed because of an error internal to the Text-to-Speech engine (should normally never happen unless the library or its data files have been damaged or is not compatible with the runtime environment) */


// Return values

/**
 * @brief Generic success value (do not check against this value, use RSTTS_SUCCESS() instead!)
 *
 * This value is often used to indicate success, where
 * no other useful information is to be returned from the API call.
 * Some operations may return other values that also indicate succcess,
 * so API users should generally not check against this value but use
 * the RSTTS_SUCCESS() macro instead.
 */

#define RSTTS_OK 0

// Parameter errors

/**
 * The API call failed because the
 * provided language value could not be interpreted as a language code.
 * 
 * @note This error code is not used to report of language codes
 * that are on the correct format, but which do not match an available
 * language; such errors will be reported with other error codes such
 * as RSTTS_LANGUAGE_NOT_INSTALLED and RSTTS_LANGUAGE_NOT_LICENSED.
 */
#define RSTTS_LANGUAGE_INVALID -1
/**
 * The language code provided in the API call does not match
 * any language supported by any installed voice.
 */
#define RSTTS_LANGUAGE_NOT_INSTALLED -2
/**
 * The language code provided in the API call is supported by some installed
 * voice, but none that is covered by the current license. (Please check
 * that you are using the correct license file, and that it covers a
 * voice that speaks the language that you are trying to use.)
 */
#define RSTTS_LANGUAGE_NOT_LICENSED -3
/**
 * An invalid value was provided as a phoneme alphabet name.
 */
#define RSTTS_PHONEME_ALPHABET_INVALID -4
/**
 * The provided phoneme alphabet name does not match any supported
 * phoneme alphabet.
 */
#define RSTTS_PHONEME_ALPHABET_NOT_SUPPORTED -5
/**
 * A numeric value was specified for a parameter that is not within the
 * valid range of values for that parameter.
 */
#define RSTTS_PROVIDED_VALUE_OUTSIDE_RANGE -6
/**
 * Input specified to be in SSML format could not be parsed as SSML.
 */
#define RSTTS_SSML_PARSE_ERROR -7
/**
 * The provided voice name string cannot be interpreted as a voice name
 * (for example, is empty or contains invalid characters).
 */
#define RSTTS_VOICE_INVALID -8
/**
 * The provided voice name does not match any installed voice; or, if
 * a language has been selected, the provided voice name does not match
 * any installed voice supporting the selected language.
 */
#define RSTTS_VOICE_NOT_INSTALLED -9
/**
 * The API call tried to select a voice that is not covered by the current
 * license.
 */
#define RSTTS_VOICE_NOT_LICENSED -10
/**
 * The license is past its expiry date and can no longer be used.
 * Please check that you are using a currently valid license file.
 */
#define RSTTS_LICENSE_EXPIRED -11
/**
 * The license does not cover the system where this software
 * is currently being used.
 */
#define RSTTS_DEVICE_NOT_LICENSED -12
/**
 * The provided license file is corrupt or damaged or incompatible with this
 * version of the software.
 */
#define RSTTS_LICENSE_INVALID -13
/**
 * The provided license file could not be opened or is not in the correct
 * format.
 */
#define RSTTS_COULD_NOT_SET_LICENSE -14
/**
 * The operation requires a license, but no valid license has been loaded.
 */
#define RSTTS_NO_LICENSE_SET -15
/**
 * The provided parameter type constant does not match a valid type for
 * parameter specified by the API call.
 */
#define RSTTS_INVALID_TYPE_FOR_PARAMETER -16
/**
 * The provided TTS instance value does not specify a valid TTS instance.
 */
#define RSTTS_INVALID_INSTANCE -17
/**
 * The provided text format is not supported.
 */
#define RSTTS_INVALID_TEXT_FORMAT -18
/**
 * An invalid argument was passed, for example a NULL was provided as a buffer.
 */
#define RSTTS_ARGUMENT_INVALID -19
/**
 * The lexicon could not be loaded. This can occur if a file path does not exist or is not readable,
 * if lexicon is of wrong format or not readable, or if enough memory cannot be allocated.
 */
#define RSTTS_LEXICON_LOAD_ERROR -21

// Operation errors

/**
 * The requested operation can not be performed at this time because the TTS
 * instance is busy servicing a rsttsSynthesize() or rsttsSynthesizeAsync()
 * call.
 */
#define RSTTS_INSTANCE_BUSY -101
/**
 * The TTS instance is in a state where the requested operation cannot
 * be performed. (For example, it is not possible to stop or pause an
 * TTS instance that is not currently servicing a rsttsSynthesize() or
 * rsttsSynthesizeAsync() call, or attempt to resume an operation that
 * has already finished.)
 */
#define RSTTS_OPERATION_NOT_ALLOWED -102
/**
 * The requested operation requires support for threading, but this version
 * of the text-to-speech engine was built without threads support.
 */
#define RSTTS_THREADING_NOT_SUPPORTED -103
/**
 * The requested operation could not complete within the allowed time.
 * Returned by rsttsWaitState() when the TTS instance still has not
 * reached a requested state within the given time.
 */
#define RSTTS_TIMEOUT -104
/**
 * The current license allows a limited number of simultaneous channels,
 * and there is no free channel to handle the current request.
 */
#define RSTTS_LICENSE_CHANNEL_ALL_BUSY -105
/**
 * The current license allows a limited number of simultaneous channels,
 * and no channel could be allocated to handle the request because the
 * channel locking resource was not found.
 */
#define RSTTS_LICENSE_CHANNEL_LOCK_NOT_FOUND -106
/**
 * The current license allows a limited number of simultaneous channels,
 * and no channel could be allocated to handle the request because the
 * software is not running with adequate permissions to access the
 * channel locking resource.
 */
#define RSTTS_LICENSE_CHANNEL_LOCK_ACCESS_DENIED -107
/**
 * The current license allows a limited number of simultaneous channels,
 * and no channel could be allocated to handle the request because the
 * channel locking resource is in a corrupt or damaged state.
 */
#define RSTTS_LICENSE_CHANNEL_LOCK_CORRUPT -108
/**
 * The current license allows a limited number of simultaneous channels,
 * and no channel could be allocated to handle the request because of
 * an unexpected error when trying to access the channel locking resource.
 */
#define RSTTS_LICENSE_CHANNEL_LOCK_IO_ERROR -109

// Internal errors
// Internal error in thread creation/destruction or locking/unlocking of data
// structures. Should never happen.

/**
 * Unexpected internal threading/synchronization error
 *
 * An unexpected error occurred within the text-to-speech engine during
 * creation, destruction or synchronization of threads or shared data
 * structures. This should not happen during normal operation. It may
 * indicate that the heap of the current process is corrupt, or that the
 * runtime environment is not fully compatible with the software.
 * This error indicates that the text-to-speech engine is in an abnormal
 * state where future operations may hang or fail unexpectedly.
 */
#define RSTTS_INTERNAL_THREADING_ERROR -201
/**
 * Unexpected internal error during voice access or synthesis
 *
 * An unexpected error occurred within the text-to-speech engine while
 * trying to use a voice. This error typically indicates that
 * the engine is not fully compatible with the installed data files,
 * or that data files belonging to the current voice are corrupt,
 * missing, or have incorrect permissions, or that the operation could
 * not be performed due to insufficient system resources such as
 * being out of free memory or file handles.
 */
#define RSTTS_INTERNAL_VOICE_ERROR -202

#endif
