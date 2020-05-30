#ifndef __LIB_RS_TTS__
#define __LIB_RS_TTS__

/**
 * @file librstts.h
 * @brief C API for the rSpeak Text-to-Speech engine
 *
 * This header file defines the external C API for the rSpeak
 * text-to-speech engine. To use the API from a C/C++ program,
 * include this file and link with -lrstts (and any other
 * libraries that might be required) or similar (exact command
 * line syntax may vary depending on your compiler).
 *
 * The API as such is a pure C API. The library may use C++
 * exceptions internally but any internal exceptions are
 * always caught by the API layer and exceptions will thus
 * never be thrown past the API boundary.
 */

#include <stdio.h>
#include "librstts_error.h"

#if !defined(PUBLIC_API)
#if defined(__GNUC__) && defined(_WIN32)
#define PUBLIC_API __attribute__ ((dllimport))
#elif defined(_WIN32)
#define PUBLIC_API __declspec(dllimport)
#else
#define PUBLIC_API /*!< @brief (Marks functions that are part of the API) */
#endif
#endif


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
  * @brief Build time version (for conditional compilation using the C/C++ preprocessor)
  *
  * This preprocessor symbol evaluates to a number that identifies the
  * version of the rSpeak TTS library at build time, and which can
  * be used for conditional compilation using the C/C++ preprocessor.
  * Different buildtime version numbers do not necessarily indicate that
  * the API differ.
  *
  * Do not use it for other purposes! Programs that need to check the
  * current version during runtime should call rsttsGetRuntimeVersion()
  * instead, as that will give the version of the library that is
  * being in use at that point in time.
  *
  * The value is an integer where the lowest eight bits represent a
  * patch number, the next eight bits a minor version number, and
  * the next eight bits a major version number. For example,
  * the value 0x010203 represents version 1.2, patch level 3.
  *
  * To determine which version number to check against, see the
  * version numbers in the release notes, which show with which
  * releases certain functionality becomes available in the API.
  * Bug fix releases will normally not introduce new API functionality.
  * so the version number that you check against should normally end
  * with 00 (in hexadecimal).
  *
  * NEVER check against a specific version number, but always for
  * a specific version or later (using the >= operator)! This is
  * because newer versions of the SDK will increase this number
  * even when entirely API compatible.
  *
  * Note that the buildtime and runtime versions are not always
  * exactly the same. In particular, bug fix releases may update the
  * header files or the lib independently of each other, and an
  * app built against one version of the TTS SDK will normally
  * also work with later versions of the library (and often older
  * versions as well). A well-written application should consider
  * the buildtime and runtime version numbers as independent, and
  * not make assumptions based on how they relate to each other.
  */
#define RSTTS_BUILDTIME_VERSION 0x010207


/**
 * @brief TTS instance handle.
 *
 * This opaque pointer type is used as a handle to a TTS instance.
 * A TTS instance is created with rsttsInit() and destroyed with rsttsFree().
 */
typedef struct RSTTSInst_Record *RSTTSInst;

#ifndef RSTTSEVENTDATA_DEFINED
// Define this unless we already got it from librstts_event.h
typedef struct RSTTSEventData RSTTSEventData;  /*!< @brief TTS event data (the actual struct definition is in librstts_event.h) */
#define RSTTSEVENTDATA_DEFINED /*!< @brief Preprocessor symbol defined if the event RSTTSEventData typedef declaration has been included */
#endif


/**
 * @brief This type represents the state of a TTS instance.
 *
 * \li RSTTSInst_unset: Special value used to indicate unknown state or error. Returned as an error indication from rsttsGetState().
 * \li RSTTSInst_ready: The instance is not in use and is ready to be
 * used/reused (or freed with rsttsFree()).
 * \li RSTTSInst_paused: The instance is in use, and has been paused.
 * (It cannot be freed or reused in this state, and will hang around
 * indefinitely in this state until told to stop or resume playing,
 * which will let it eventually transition to another state.)
 * \li RSTTSInst_playing: The instance is actively used (processing
 * text and/or generating audio).
 * \li RSTTSInst_stopping: The instance has been requested to stop, but is
 * not yet ready to accept new work (it will need to finish any
 * ongoing callback, as well as release resources specific to its current
 * task before it can accept a new one)
 * It will transition automatically to RSTTSInst_ready when these
 * actions are completed and it becomes ready to accept new tasks.
 */
typedef enum { RSTTSInst_unset=0, RSTTSInst_ready=1, RSTTSInst_paused=2, RSTTSInst_playing=4, RSTTSInst_stopping=8 } RSTTSInst_State;

/**
 * @brief These values identify parameters that can be set via the
 * rsttsSetParameter() API function,
 * see this function for more information.
 */

  typedef enum {
    RSTTS_PARAM_NONE=0,
    RSTTS_PARAM_QUALITY_SETTING=1,
    RSTTS_PARAM_LICENSE_FILE=2,
    RSTTS_PARAM_LICENSE_BUFFER=3,
    RSTTS_PARAM_EVENT_MASK=4,
    RSTTS_PARAM_TEXT_POSITION_TRACKING=5,
    RSTTS_PARAM_VISEME_ALPHABET=6,
    RSTTS_PARAM_RESPONSIVENESS_SETTING=7,
    RSTTS_PARAM_PUNCTUATION_PAUSE_MS=8,
    RSTTS_PARAM_SENTENCE_PAUSE_MS=9,
    RSTTS_PARAM_X_WEAK_BREAK_PAUSE_MS=10,
    RSTTS_PARAM_WEAK_BREAK_PAUSE_MS=11,
    RSTTS_PARAM_MEDIUM_BREAK_PAUSE_MS=12,
    RSTTS_PARAM_STRONG_BREAK_PAUSE_MS=13,
    RSTTS_PARAM_X_STRONG_BREAK_PAUSE_MS=14,
    RSTTS_PARAM_MAX_BREAK_PAUSE_MS=15,
    RSTTS_PARAM_TEXT_POSITION_INCLUDES_MARKUP=16,
    RSTTS_PARAM_AUDIO_REALTIME_PCT=17,

    // Pitch parameters 100-109
    RSTTS_PARAM_PITCH_DEFAULT=100,
    RSTTS_PARAM_PITCH_X_LOW=101,
    RSTTS_PARAM_PITCH_LOW=102,
    RSTTS_PARAM_PITCH_MEDIUM=103,
    RSTTS_PARAM_PITCH_HIGH=104,
    RSTTS_PARAM_PITCH_X_HIGH=105,

    // Rate parameters 110-119
    RSTTS_PARAM_RATE_DEFAULT=110,
    RSTTS_PARAM_RATE_X_SLOW=111,
    RSTTS_PARAM_RATE_SLOW=112,
    RSTTS_PARAM_RATE_MEDIUM=113,
    RSTTS_PARAM_RATE_FAST=114,
    RSTTS_PARAM_RATE_X_FAST=115,

    // Volume parameters 120-129
    RSTTS_PARAM_VOLUME_DEFAULT=120,
    RSTTS_PARAM_VOLUME_SILENT=121,
    RSTTS_PARAM_VOLUME_X_SOFT=122,
    RSTTS_PARAM_VOLUME_SOFT=123,
    RSTTS_PARAM_VOLUME_MEDIUM=124,
    RSTTS_PARAM_VOLUME_LOUD=125,
    RSTTS_PARAM_VOLUME_X_LOUD=126,

    // Marked up emphasis level "reduced" 130-139
    RSTTS_PARAM_MARKUP_EMPHASIS_REDUCED_RATE=130,
    RSTTS_PARAM_MARKUP_EMPHASIS_REDUCED_PITCH=131,
    RSTTS_PARAM_MARKUP_EMPHASIS_REDUCED_VOLUME=132,

    // Marked up emphasis level "moderate" 140-149
    RSTTS_PARAM_MARKUP_EMPHASIS_MODERATE_RATE=140,
    RSTTS_PARAM_MARKUP_EMPHASIS_MODERATE_PITCH=141,
    RSTTS_PARAM_MARKUP_EMPHASIS_MODERATE_VOLUME=142,

    // Marked up emphasis level "strong" 150-159
    RSTTS_PARAM_MARKUP_EMPHASIS_STRONG_RATE=150,
    RSTTS_PARAM_MARKUP_EMPHASIS_STRONG_PITCH=151,
    RSTTS_PARAM_MARKUP_EMPHASIS_STRONG_VOLUME=152,

    // General URI handling settings 160-169
    RSTTS_PARAM_URI_BASE=160,

    // Built-in file: URI handler settings 170-179
    RSTTS_PARAM_URI_FILE_BUILTIN_ENABLED=170,
    RSTTS_PARAM_URI_FILE_BUILTIN_ALLOW_ABS=171,
    RSTTS_PARAM_URI_FILE_BUILTIN_ALLOW_REL=172,
    RSTTS_PARAM_URI_FILE_BUILTIN_SFX=173

  } RSTTS_PARAMS;

/**
 * @brief This indicates the type of a parameter value passed as the value argument to rsttsSetParameter().
 *
 * The interpretation of the value argument passed to rsttsSetParameter() depends on the indicated type as follows:
 *
 * \li RSTTS_TYPE_UNDEFINED: No value is given, the argument is not used.
 *
 * \li RSTTS_TYPE_UINT: The value is an unsigned integer. The argument shall be a (const) unsigned int* pointing to the value.
 *
 * \li RSTTS_TYPE_INT: The value is an integer. The argument shall be a (const) int* pointing to the value.
 * 
 * \li RSTTS_TYPE_CHAR: The value is a single unicode character. The argument shall be a (const) char* pointing to the first byte of the UTF-8 representation of the character.
 *
 * \li RSTTS_TYPE_STRING: The value is a null-terminated UTF-8 string. The argument shall be a (const) char* pointing to the first character of the string.
 *
 * \li RSTTS_TYPE_BOOL: The value is a boolean (true or false) represented by one of the constants RSTTS_PARAMVAL_TRUE and RSTTS_PARAMVAL_FALSE, respectively. (In this case, the value parameter is not dereferenced as a pointer.)
 */

typedef enum { RSTTS_TYPE_UNDEFINED=0, RSTTS_TYPE_INT=1, RSTTS_TYPE_UINT=2, RSTTS_TYPE_CHAR=3, RSTTS_TYPE_STRING=4, RSTTS_TYPE_BOOL=5 } RSTTS_TYPES;

/**
 * @brief This type is for specifying at what processing stage a lexicon
 * should be applied to text being read by the text-to-speech engine.
 * See rsttsLoadLexicon functions for more information.
 */

typedef enum {  RSTTS_PRE_MARKUP_PARSE=0, RSTTS_POST_MARKUP_PARSE=100, RSTTS_PRE_NORMALIZATION=200  } EngineStage;

#define RSTTS_PRE_SSML_PARSE RSTTS_PRE_MARKUP_PARSE /*!< @deprecated Use RSTTS_PRE_MARKUP_PARSE instead @brief Alias for RSTTS_PRE_MARKUP_PARSE */

#define RSTTS_PARAMVAL_FALSE NULL /*!< @brief Used with rsttsSetParameter() for setting a boolean parameter to false */
/* (arbitrary non-NULL void pointer value used to set a boolean TTS parameter to true) */
#define RSTTS_PARAMVAL_TRUE ((void*)1) /*!< @brief Used with rsttsSetParameter() for setting a boolean parameter to true */


/**
 * @brief TTS audio callback function pointer type
 *
 * A function pointer of this type is given to rsttsSetAudioCallback() as
 * the destination for coming audio callbacks, which will be generated by
 * the TTS when it has audio data to output (if a callback function has
 * been provided).
 *
 * The callback function has a void return type and is called with four
 * arguments: The TTS instance from which the callback is called, a void
 * pointer to audio data, a size_t containing the length (in chars) of
 * the audio data, and finally a void pointer to arbitrary user data (also
 * set up with rsttsSetAudioCallback()).
 */
typedef void (*rsttsAudioCallback)(RSTTSInst, const void*, size_t, void*);


/**
 * @brief TTS event callback function pointer type
 *
 * A function pointer of this type is given to rssttsSetEventCallback()
 * as the destination for coming event callbacks, which will be generated
 * by the tts during reading if the corresponding events have been
 * enabled (by calling rsttsSetParameter() with RSTTS_PARAM_EVENT_MASK,
 * RSTTS_TYPE_INT and a bitmask of RSTTSEventType values as defined in
 * librstts_event.h).
 *
 * The callback function has a void return type and is called with three
 * arguments: The TTS instance from which the callback is called, a pointer
 * to a constant of the type RSTTSEventData defined in librstts_event.h,
 * and finally a void pointer to arbitrary user data (also set up with
 * rsttsSetEventCallback()).
 */
typedef void (*rsttsEventCallback)(RSTTSInst, const RSTTSEventData*, void*); 


/**
 * @brief Returns the currently used version of the rSpeak TTS library
 *
 * This call returns an integer representing the currently used version of
 * the rSpeak TTS library. The returned value is a positive integer, of
 * which the least significant 8 bits represents the patch version, the
 * next 8 bits the minor version, and the next 8 bits the major version.
 * I.e. the integer 0x010203 represents version 1.2, patch level 3.
 *
 * See also RSTTS_BUILDTIME_VERSION, which is a preprocessor symbol on
 * the same format, that can be used for build-time version checks (such
 * as conditional inclusion of code using #if etc), but should not be
 * used for other purposes.
 *
 * Always successful. Can be called at any time.
 *
 * @returns A positive integer identifying the version, as described above.
 */

PUBLIC_API long rsttsGetRuntimeVersion(void);


/**
 * @brief Creates and initializes a TTS instance
 * 
 * This call creates and initializes a new TTS instance and returns a
 * handle to it, or NULL on failure.
 *
 * @param[in] basedir the file path to the base directory of the TTS
 * installation.
 * @returns NULL on failure, non-NULL on success.
 */

PUBLIC_API RSTTSInst rsttsInit(const char *basedir); /* initialise the tts instance */


/**
 * @brief Releases resources used by a TTS instance,
 * and/or thread-local resources used by the API
 *
 * This call frees resources used by a TTS instance created with rsttsInit().
 * It also frees thread-local resources allocated by the TTS API.
 *
 * @param[in] ttsinst TTS instance to operate on, or NULL
 *
 * @note This function will release thread-local resources allocated by
 * the TTS API for the current thread. Supply NULL as an argument to only
 * release thread-local resources without affecting any TTS instance.
 * (These thread-local resources are used for implementing the
 * rsttsGetErrorMessage(NULL) functionality, which reports the last
 * API error message of the current thread regardless of instance.)
 *
 * @note Never stores any thread-local error message on success (as
 * that would require the thread-local resources to be kept around
 * although we wanted to free them!) After a successful call with a
 * NULL argument, rsttsGetErrorMessage(NULL) will therefore only
 * return a generic message that does not describe the status of
 * any previous call.
 *
 * @note After a successful call with a non-NULL argument, all resources
 * used by the supplied TTS instance, if any, will have been released.
 * The supplied TTS instance must then not be used in any
 * subsequent TTS API call by this or any other thread, not even
 * rsttsGetErrorMessage().
 *
 * @note If the instance is in use during the duration of the call to
 * rsttsFree(), then this function will not have any
 * effect, but return the error code RSTTS_INSTANCE_BUSY.
 * It is then safe to continue using the TTS instance in further API calls,
 * or waiting some and eventually trying rsttsFree() again.
 * Attempting to free an instance from within a callback from that
 * instance will always harmlessly fail with RSTTS_INSTANCE_BUSY.
 *
 * @warning While the call is in progress, it is generally not known or
 * determined whether resources are freed or not. Other threads may not
 * initiate any new calls using the same TTS instance during this time.
 * 
 * @warning Note that if the TTS instance was recently stopped with
 * rsttsStop() or rsttsSetState(), it may not yet be possibly to free it,
 * as stopping the TTS instance is an asynchronous request and the TTS
 * instance may still need a little time to finish ongoing callbacks and
 * interrupt ongoing work before it is ready to be freed.
 * See rsttsWaitState() for information
 * on how to wait until the instance is ready to be freed.
 * 
 */

PUBLIC_API int rsttsFree(RSTTSInst ttsinst); /* release the tts instance */


/**
 * @brief Gets an error message (as a string) from the TTS API. This
 * function will often be able to give a more detailed description of
 * an error than can be represented by merely a status code.
 *
 * This function retrieves an error message, in the form of a NULL-terminated
 * UTF-8 string, from a previous TTS API call. If the parameter is NULL, then
 * the last error message for the current thread will be retrieved.
 * Alternatively, if a TTS instance is supplied, then the last error
 * message generated for that TTS instance (by any thread) will be retrieved.
 *
 * @param[in] inst TTS instance to retrieve the last error for, or NULL
 *
 * @returns A pointer to a message in the form of null-terminated UTF-8
 * string (which may be temporarily allocated). Never NULL.
 *
 * @note The returned value is valid until the next API call by the current
 * thread (regardless of other API calls made to the same TTS instance in
 * between), but no longer (unless it is expressly documented that an API
 * function does not affect the thread-local or per-instance last error
 * message). Do not attempt to free/delete the returned value.
 *
 * @note The thread-local buffer used for storing the error message will be
 * freed when the current thread exits. It can also be freed by calling
 * rsttsFree(), optionally with a NULL argument to only free thread-local
 * resources without releasing any TTS instance.
 *
 * @note The returned message is intended only for human users. Messages from
 * this function can otherwise not be assumed to be in any specific form, but
 * might vary depending on TTS version, locale, platform and other factors.
 * Do not try to automatically parse or compare strings from this function.
 */

PUBLIC_API const char* rsttsGetErrorMessage(RSTTSInst inst);


/**
 * @brief Gets a string corresponding to an error code from the TTS API.
 *
 * This function returns a NULL-terminated string corresponding to an
 * error/status code returned by the API. It always succeeds (but may
 * return a string indicating that an error code represents an unknown
 * error, if that is the case).
 *
 * @param[in] err error/result code returned by an API call, and/or one
 * of the result codes defined in librstts_error.h.
 *
 * @returns A pointer to a NULL-terminated, UTF-8 string constant
 * describing the meaning of the result code in human-readable form
 * is returned. The returned pointer is valid until the next API call
 * (not including rsttsGetErrorMessage(), which is guaranteed to not
 * affect messages from rsttsGetErrorCodeString()).
 *
 * @note The returned string is intended only for human users. Strings
 * from this function can otherwise not be assumed to be in any
 * specific form, but might vary depending on TTS version, locale,
 * platform and other factors. Do not try to automatically parse
 * or compare strings from this function.
 *
 * @note This function always succeeds, and calling it does not affect
 * thread-local or per-instance last error messages (as retrieved by
 * rsttsGetErrorMessage()).
 * The returned message will often be less detailed than that from
 * rsttsGetErrorMessage(), as it does not have access to any error
 * information except what is represented by the error code itself.
 */

PUBLIC_API const char* rsttsGetErrorCodeString(int err);


/**
 * @brief Sets the value of one of various parameters
 *
 * This functions sets the value of the selected parameter.
 * The following parameters can be set:
 *
 * parameter name | parameter type identifier | usage, value, result notes
 * -----------------|---------------------|-------
 * <b>Event reporting settings</b> |||
 * RSTTS_PARAM_EVENT_MASK       | RSTTS_TYPE_INT    | Selects for which events callbacks will be generated. The value should be a bit mask of (or:ed together) values of RSTTS_EventType. The predefined masks RSTTS_EVENT_MASK_NONE, RSTTS_EVENT_MASK_DEFAULT, RSTTS_EVENT_MASK_ALL may also be used. (The event mask may be set during ongoing generation, but changes may not take effect immediately.)
 * RSTTS_PARAM_TEXT_POSITION_INCLUDES_MARKUP | RSTTS_TYPE_BOOL | Set to let markup (SSML tags/entities etc) count as part of the text in reported positions in events.
 * RSTTS_PARAM_TEXT_POSITION_TRACKING | RSTTS_TYPE_BOOL | Set to RSTTS_PARAMVAL_FALSE to disable text position tracking, RSTTS_PARAMVAL_TRUE to enable. Text position tracking is disabled per default, and should be enabled for applications relying on events being reported together with their positions in the input text. (May incur a slight extra cost in CPU time and memory usage; applications not needing this can leave it disabled.)
 * RSTTS_PARAM_VISEME_ALPHABET  | RSTTS_TYPE_STRING | Sets the viseme alphabet to use when reporting viseme events (when enabled by the event mask). Most voices support the "sapi" viseme alphabet. (Success merely indicates that the parameter was set, it does not indicate that the selected viseme alphabet is available for any voice.)
 * <b>License settings</b> |||
 * RSTTS_PARAM_LICENSE_BUFFER   | RSTTS_TYPE_STRING | Loads a license from memory. Provide license file contents as a null-terminated string (const char*). Success indicates that the license was successfully loaded and validated.
 * RSTTS_PARAM_LICENSE_FILE     | RSTTS_TYPE_STRING | Loads a license from a file. Provide a path as a null-terminated UTF-8 string (const char*). Success indicates that the license was successfully loaded and validated.
 * <b>Speed/quality tradeoff settings</b> |||
 * RSTTS_PARAM_AUDIO_REALTIME_PCT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Limits audio generation speed, in percentage of realtime speed. Set to a percentage of realtime speed (i.e. 200 for 2x realtime), or 0 (default) for no speed limitation (license and system limitations still apply).
 * RSTTS_PARAM_QUALITY_SETTING | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Sets a general voice quality/speed tradeoff level. Use the constants RSTTS_QUALITY_NORMAL or RSTTS_QUALITY_LOW or some value in between.
 * RSTTS_PARAM_RESPONSIVENESS_SETTING | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Controls tradeoffs that affect time to first audio, including adjusting how much voice data to preload on voice selection. Setting the value RSTTS_RESPONSIVENESS_FAST may decrease the time from initiating a call to rsttsSynthesize() or rsttsSynthesizeAsync() to the first emitted audio from that call, but may instead slow down voice selection. Also, see note below. Use the constants RSTTS_RESPONSIVENESS_NORMAL or RSTTS_RESPONSIVENESS_FAST or some value in between.
 * <b>Pause settings</b> |||
 * RSTTS_PARAM_MAX_BREAK_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Sets the maximum pause length (in ms) that can be specified in markup; longer pauses will be shortened to this pause length. |
 * RSTTS_PARAM_MEDIUM_BREAK_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) for breaks specified with prosodic strength <i>medium</i>, and for SSML breaks without a specified prosodic strength |
 * RSTTS_PARAM_PUNCTUATION_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) for sentence-internal punctuation (such as commas) |
 * RSTTS_PARAM_SENTENCE_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) between sentences |
 * RSTTS_PARAM_STRONG_BREAK_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) for breaks specified with prosodic strength <i>strong</i> |
 * RSTTS_PARAM_WEAK_BREAK_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) for breaks specified with prosodic strength <i>weak</i> |
 * RSTTS_PARAM_X_STRONG_BREAK_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) for breaks specified with prosodic strength <i>x-strong</i> |
 * RSTTS_PARAM_X_WEAK_BREAK_PAUSE_MS | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Default pause length (in ms) for breaks specified with prosodic strength <i>x-weak</i> |
 * <b>Pitch/rate/volume settings</b> |||
 * RSTTS_PARAM_PITCH_DEFAULT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Basic pitch level (percentage, linear scale) for text without pitch markup, or explicitly marked up as having default pitch. (Cannot be modified while the TTS instance is in use. The real-time master pitch setting set by rsttsSetPitch() is applied on top of this setting during processing, to achieve the final pitch.) |
 * RSTTS_PARAM_PITCH_X_LOW | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_PITCH_DEFAULT, but instead applies when markup specifies the <i>x-low</i> pitch level. |
 * RSTTS_PARAM_PITCH_LOW | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_PITCH_DEFAULT, but instead applies when markup specifies the <i>low</i> pitch level. |
 * RSTTS_PARAM_PITCH_MEDIUM | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_PITCH_DEFAULT, but instead applies when markup specifies the <i>medium</i> pitch level. |
 * RSTTS_PARAM_PITCH_HIGH | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_PITCH_DEFAULT, but instead applies when markup specifies the <i>high</i> pitch level. |
 * RSTTS_PARAM_PITCH_X_HIGH | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_PITCH_DEFAULT, but instead applies when markup specifies the <i>x-high</i> pitch level. |
 * RSTTS_PARAM_RATE_DEFAULT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Basic rate/speed level (percentage, linear scale) for text without rate markup, or explicitly marked up as having default rate. (Cannot be modified while the TTS instance is in use. The real-time master rate setting set by rsttsSetRate() is applied on top of this setting during processing, to achieve the final rate.) |
 * RSTTS_PARAM_RATE_X_SLOW | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_RATE_DEFAULT, but instead applies when markup specifies the <i>x-slow</i> rate level. |
 * RSTTS_PARAM_RATE_SLOW | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_RATE_DEFAULT, but instead applies when markup specifies the <i>slow</i> rate level. |
 * RSTTS_PARAM_RATE_MEDIUM | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_RATE_DEFAULT, but instead applies when markup specifies the <i>medium</i> rate level. |
 * RSTTS_PARAM_RATE_FAST | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_RATE_DEFAULT, but instead applies when markup specifies the <i>fast</i> rate level. |
 * RSTTS_PARAM_RATE_X_FAST | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_RATE_DEFAULT, but instead applies when markup specifies the <i>x-fast</i> rate level. |
 * RSTTS_PARAM_VOLUME_DEFAULT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Basic volume level (percentage of default, linear scale) for text without volume markup, or explicitly marked up as having default volume. (Cannot be modified while the TTS instance is in use. The real-time master volume setting set by rsttsSetVolume() is applied on top of this setting during processing, to achieve the final pitch.) The setting may be zero, but not negative. Settings larger than RSTTS_VOLUME_MAX are truncated to that value. |
 * RSTTS_PARAM_VOLUME_SILENT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_VOLUME_DEFAULT, but instead applies when markup specifies the <i>silent</i> volume level. Should normally be left at zero. |
 * RSTTS_PARAM_VOLUME_X_SOFT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_VOLUME_DEFAULT, but instead applies when markup specifies the <i>x-soft</i> volume level. |
 * RSTTS_PARAM_VOLUME_SOFT | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_VOLUME_DEFAULT, but instead applies when markup specifies the <i>soft</i> volume level. |
 * RSTTS_PARAM_VOLUME_MEDIUM | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_VOLUME_DEFAULT, but instead applies when markup specifies the <i>medium</i> volume level. |
 * RSTTS_PARAM_VOLUME_LOUD | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_VOLUME_DEFAULT, but instead applies when markup specifies the <i>loud</i> volume level. |
 * RSTTS_PARAM_VOLUME_X_LOUD | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | As RSTTS_PARAM_VOLUME_DEFAULT, but instead applies when markup specifies the <i>x-loud</i> volume level. |
 * <b>Settings for modifying pitch/rate/volume based on emphasis markup</b> |||
 * RSTTS_PARAM_MARKUP_EMPHASIS_REDUCED_RATE | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of rate/speed to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>reduced</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_REDUCED_PITCH | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of pitch to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>reduced</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_REDUCED_VOLUME | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of volume to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>reduced</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_MODERATE_RATE | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of rate/speed to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>moderate</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_MODERATE_PITCH | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of pitch to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>moderate</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_MODERATE_VOLUME | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of volume to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>moderate</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_STRONG_RATE | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of rate/speed to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>strong</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_STRONG_PITCH | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of pitch to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>strong</i>. |
 * RSTTS_PARAM_MARKUP_EMPHASIS_STRONG_VOLUME | RSTTS_TYPE_INT or RSTTS_TYPE_UINT | Amount of volume to apply (as percentage of prosody settings) when reading text marked up with prosody level <i>strong</i>. |
 * <b>URI handling</b> |||
 * RSTTS_PARAM_URI_BASE | RSTTS_TYPE_STRING | Base URI to use for resolving URI:s in document. Only "file:" URIs are supported at this time. Using a file URI with an absolute path is recommended. |
 * RSTTS_PARAM_URI_FILE_BUILTIN_ENABLED | RSTTS_TYPE_BOOL | If set, enables the built-in handling of file URIs. Setting this means that documents will be able to reference files (in for example the SSML <i>audio</i> element). This might have security implications if processing untrusted documents. Note that some additional parameters limit which paths may be used, to mitigate this. |
 * RSTTS_PARAM_URI_FILE_BUILTIN_ALLOW_ABS | RSTTS_TYPE_BOOL | If set, allows URIs in the document to refer to files outside the base URI directory using absolute paths. (For platforms with a current device concept like MS Windows, a path absolute to the current device, but not specifying the device, is considered as absolute for this purpose.) This may have security implications if reading untrusted content. |
 * RSTTS_PARAM_URI_FILE_BUILTIN_ALLOW_REL | RSTTS_TYPE_BOOL | If set, allows URIs in the document to refer to files outside the base URI directory using relative paths. This may have security implications if reading untrusted content. |
 * RSTTS_PARAM_URI_FILE_BUILTIN_SFX | RSTTS_TYPE_BOOL | If set, allows the document use the audio tag (with a file: method and no file suffix) to refer to sound effects and special recorded sounds included in the TTS installation. Enabled per default. |
 * 
 * @param[in] inst TTS instance to set the parameter for
 * @param[in] parameterName which parameter you want to set, specified in enum RSTTS_PARAMS
 * @param[in] type The type of the value parameter, specified in enum RSTTS_TYPES
 * @param[in] value Value to set for the parameter. See the description of enum RSTTS_TYPES. Note that except for RSTTS_TYPE_BOOL, all parameter values are given as pointers; for non-pointer parameter types (RSTTS_TYPE_INT, RSTTS_TYPE_UINT) the passed value should actually be a pointer to the new parameter value.
 *
 * @returns Result code indicating whether the parameter was successfully set; check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note Parameters for a TTS instance can in some cases be changed regardless
 * of whether the instance is in use or not.
 * The change may then not take effect until the next
 * new sentence or a similar event.
 * Setting the license will unload any previously set license for the TTS
 * instance, unless the instance is busy, in which case it will fail
 * with RSTTS_INSTANCE_BUSY.
 * The quality and responsiveness settings may affect 
 * what components of the voice/language data are pre-loaded into memory on language/voice selection,
 * so in order for such settings to to take full effect, these settings
 * should if possible be set before selecting language and voice. 
 * (Setting them at a later point is permitted, but will not affect already
 * loaded voices to the full extent.)
 */

PUBLIC_API int rsttsSetParameter(RSTTSInst inst, const RSTTS_PARAMS parameterName, const RSTTS_TYPES type, const void* value );


/**
 * @brief Sets directory for dictionary data to use by a TTS instance
 *
 * This call tells a TTS instance where to find its dictionary
 * databases. This can be changed at any time, but changes will normally
 * not apply to a voice currently loaded or in use by the instance.
 *
 * @param[in] inst TTS instance
 * @param[in] dir directory with dictionary databases
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note The default directory for this is the "data" subdirectory of the base
 * directory given to rsttsInit().
 *
 * @note This call does not verify that the directory actually exists
 * and contains usable data. It simply stores a path that is used as
 * an implicit parameter for subsequent language/voice selection.

 */

PUBLIC_API int rsttsSetDictsdir(RSTTSInst inst ,const char *dir);


/**
 * @brief Sets directory for voice audio data for a TTS instance
 *
 * This call tells a TTS instance where to find its voice audio data.
 * This can be changed at any time, but changes will normally
 * not apply to a voice currently loaded or in use by the instance.
 *
 * @param[in] inst TTS instance
 * @param[in] dir directory with audio data
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note The default directory for this is the "data" subdirectory of the base
 * directory given to rsttsInit().
 *
 * @note This call does not verify that the directory actually exists
 * and contains usable data. It simply stores a path that is used as
 * an implicit parameter for subsequent language/voice selection.
 */

PUBLIC_API int rsttsSetAudiodir(RSTTSInst inst,const char *dir);


/**
 * @brief Sets the (default) language for a TTS instance
 *
 * This call sets the language that a TTS instance will use in cases where
 * the language is not specified by text markup.
 * This can be changed at any time, and will affect future operations but
 * changes will not affect any operation already in progress.
 *
 * The language is to be specified as a dash or underscore separated
 * BCP47 language tag (for example "en-AU"), which is matched in a
 * non-case-significant manner against the installed languages.
 * It is also permitted to only specify the first part of the language
 * code (for example "en").
 *
 * The TTS engine requires the primary language subtag to match,
 * and will also try to match any region subtag if possible (but
 * does not require it to match). I.e. "en-AU" will give
 * Australian English if installed, but will otherwise try to fall
 * back to some other installed variant of English.
 *
 * @param[in] inst TTS instance
 * @param[in] lang Language code, for example "de-DE" or just "de"
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note Selecting a language undoes any previous voice selection; also,
 * the language setting determines which voices can be selected. Therefore,
 * language should always be selected before voice.
 *
 * @note When using the rSpeak TTS with input text containing language
 * markup (for example SSML, where this is mandatory according to the
 * specifications),
 * explicitly setting language+voice beforehand via the API is not
 * required. Doing so may still be meaningful for checking beforehand that a
 * language+voice is available, for responsiveness purposes (allowing
 * voice data to be pre-loaded), and because the voice selection is then
 * treated as the default voice for that language.
 *
 */

PUBLIC_API int rsttsSetLanguage(RSTTSInst inst,const char *lang);


/**
 * @brief Selects a voice by name
 *
 * This call selects the voice to use by the TTS instance.
 *
 * Make sure that the language is set before setting the voice.
 *
 * Setting the voice is not mandatory, the TTS engine will otherwise
 * choose an installed and licensed voice automatically as long as the
 * language is set.  Also, voice markup will override API voice
 * settings.
 *
 * @param[in] inst TTS instance
 * @param[in] name Voice name as an UTF-8 string, for example "Maja"
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note There is currently no way to determine via the API which voices
 * the TTS engine provides.
 *
 */

PUBLIC_API int rsttsSetVoiceByName(RSTTSInst inst,const char *name); /* Select voice by name to use */


/**
 * @brief Selects a voice by gender
 *
 * This call selects which voice to use by the TTS instance, selecting
 * by gender rather than by name.  This may be used instead of than
 * rsttsSetVoiceByName() when a voice of a certain gender is desired,
 * but it not important which specific voice of that gender is used.
 *
 * Make sure that the language is set before setting the voice.
 *
 * Setting the voice is not mandatory, the TTS engine will otherwise
 * choose an installed and licensed voice automatically as long as the
 * language is set.  Also, voice markup will override API voice
 * settings.
 *
 * @param[in] inst TTS instance
 * @param[in] gender Voice gender; "male", "female" or "neutral"
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note There is currently no way to determine via the API which voices
 * the TTS engine provides.
 */

PUBLIC_API int rsttsSetVoiceByGender(RSTTSInst inst,const char *gender); /* Select voice by gender to use */


/**
 * @brief Sets/modifies pitch for speech generation
 *
 * This call sets or modifies the audio pitch for any ongoing and future
 * speech by a TTS instance. The pitch is specified on a linear scale
 * where higher values mean higher pitch, and RSTTS_PITCH_DEFAULT is the
 * default value (normal pitch.)
 *
 * Extreme values are allowed but may only be partially applied
 * due to (generous) limitations within the TTS engine.
 *
 * @param[in] inst TTS instance
 * @param[in] pitch New pitch value
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note This master pitch setting is cumulative with any pitch setting
 * specified in markup or the RSTTS_PARAM_PITCH_DEFAULT setting of
 * rsttsSetParameter().
 * I.e. if a call specifies half pitch and the SSML specifies
 * triple pitch, then the resulting speech will be at 3/2 pitch.
 * Internal TTS engine limitations for minimal/maximum pitch are applied
 * to the resulting cumulative value.
 *
 * @note Pitch can be changed during ongoing audio generation. Changes
 * will take effect within approximately 0.1 seconds (from the perspective
 * of the TTS engine, i.e. not taking into account any additional delays
 * caused by buffering outside of the TTS).
 *
 * @note Pitch settings only affect speech, not audio inserted into
 * the audio stream using the SSML audio tag or similar.
 */

PUBLIC_API int rsttsSetPitch(RSTTSInst inst,float pitch); /* Select pitch to use */


/**
 * @brief Gets pitch setting for the specified TTS instance
 *
 * This call returns the audio pitch for TTS instance inst, returning either
 * the default value or the value set by a previous call to rsttsSetPitch()
 *
 * @param[in] inst TTS instance
 * @param[out] pval pointer to memory location used to store the current pitch setting
 *
 * @returns success, or error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note Pitch settings internal to a document, such as from SSML document markup, are not reported by this call.
 */

PUBLIC_API int rsttsGetPitch(RSTTSInst inst,float* pval); /* Get pitch from TTS instance inst, stored in memory pointed to by pval */


/**
 * @brief Sets/modifies speed (rate) for speech generation
 *
 * This call sets or modifies the speed for any ongoing and future
 * speech by a TTS instance. The speed is specified on a linear scale
 * where higher values mean higher speed, and RSTTS_SPEED_DEFAULT is the
 * default value (normal speed.)
 *
 * Extreme values are allowed but may only be partially applied
 * due to (generous) limitations within the TTS engine.
 *
 * @param[in] inst TTS instance
 * @param[in] speed New speed value
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note This master speed setting is cumulative with any rate setting
 * specified in text markup or the RSTTS_PARAM_RATE_DEFAULT setting of
 * rsttsSetParameter().
 * I.e. if a call specifies half speed and the SSML text specifies
 * triple speed (rate), then the resulting speech will be at 3/2 speed.
 * Internal TTS engine limitations for minimal/maximum speed are applied
 * to the resulting cumulative value.
 *
 * @note Speed can be changed during ongoing audio generation. Changes
 * will take effect within approximately 0.1 seconds (from the perspective
 * of the TTS engine, i.e. not taking into account any additional delays
 * caused by buffering outside of the TTS).
 *
 * @note It is not possible to make the TTS engine speak backwards
 * by supplying a negative value. Zero or negative values will simply
 * yield the lowest supported speed. 
 *
 * @note Speed settings only affect speech, not audio inserted into
 * the audio stream using the SSML audio tag or similar.
 */

PUBLIC_API int rsttsSetSpeed(RSTTSInst inst,float speed); /* Select speed to use */


/**
 * @brief Gets speed/rate setting for the specified TTS instance
 *
 * This call returns the speech speed/rate for TTS instance inst, returning either
 * the default value or the value set by a previous call to rsttsSetSpeed()
 *
 * @param[in] inst TTS instance
 * @param[out] pval pointer to memory location used to store the current speed/rate setting
 *
 * @returns success, or error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note Speed/rate settings internal to a document, such as from SSML document markup, are not reported by this call.
 */

  PUBLIC_API int rsttsGetSpeed(RSTTSInst inst,float* pval); /* Get speed/rate from TTS instance inst, stored in memory pointed to by pval */


/**
 * @brief Sets/modifies volume level for a TTS instance
 *
 * This call sets or modifies the volume level for any ongoing and future
 * audio from a TTS instance. The volume is specified on a linear scale
 * where higher values mean louder volume, and RSTTS_VOLUME_DEFAULT is the
 * default value (normal volume.) Additional useful constants to use with
 * this call are RSTTS_VOLUME_SILENT, meaning completely quiet output,
 * and RSTTS_VOLUME_MAX, which is the loudest volume at which the TTS engine
 * can generate audio without clipping.
 *
 * Extreme volume values can be set and will be applied as far as possible,
 * except the TTS will never generate audio at a volume more quiet than
 * completely silent, or so loud that clipping is introduced (also taking
 * into account volume changes introduced by SSML markup and such).
 *
 * @param[in] inst TTS instance
 * @param[in] volume New volume value
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note This master volume setting is cumulative with any volume setting
 * specified in text markup or the RSTTS_PARAM_VOLUME_DEFAULT setting
 * of rsttsSetParameter().
 * I.e. if a call specifies half volume and the SSML text specifies
 * triple volume, then the resulting speech will be at 3/2 volume.
 * Internal TTS engine limitations for minimal/maximum volume are applied
 * to the resulting cumulative value.
 *
 * @note Volume can be changed during ongoing audio generation. Changes
 * will take effect within approximately 0.1 seconds (from the perspective
 * of the TTS engine, i.e. not taking into account any additional delays
 * caused by buffering outside of the TTS).
 *
 * @note Volume settings apply to speech as well as to other audio inserted
 * into the audio stream using the SSML audio tag or similar. In this respect,
 * the volume setting differs from the speed and pitch settings.
 */

PUBLIC_API int rsttsSetVolume(RSTTSInst inst, float volume); /* Select volume to use */


/**
 * @brief Gets volume for specified TTS instance
 *
 * This call returns the speech volume for TTS instance inst, returning either
 * the default value or the value set by a previous call to rsttsSetVolume().
 *
 * @param[in] inst TTS instance
 * @param[out] pval pointer to memory location used to store current volume setting.
 *
 * @returns Status/error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note Volume settings internal to a document, such as from SSML document markup, are not reported by this call.
 */

  PUBLIC_API int rsttsGetVolume(RSTTSInst inst,float* pval); /* Get volume from TTS instance inst, stored in memory pointed to by pval */


/**
 * @brief Sets the sample rate to use for output generated by a TTS instance
 *
 * This call sets the desired sample rate for audio output, in Hz (for example
 * 20500), and should always be called before audio generation is started.
 * 
 * @param[in] inst TTS instance
 * @param[in] rate New sample rate (samples per second)
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note This function can be called during ongoing audio generation,
 * although changes might not take effect until the ongoing audio
 * generation is complete.
 *
 * @note Setting extremely high or low sample rates might not work. No
 * sanity checks are applied (as for volume, for example).
 */

PUBLIC_API int rsttsSetSampleRate(RSTTSInst inst, int rate); /* Select sample rate to use for generated audio */


/**
 * @brief Gets sample rate for specified TTS instance
 *
 * This call fetches the sample rate (in Hz) used for generating audio with
 * the TTS instance inst, returning either the default value or the value
 * set by a previous call to rsttsSetSampleRate().
 *
 * @param[in] inst TTS instance
 * @param[out] pval pointer to memory location used to store the sample rate in Hz.
 *
 * @returns success, or error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros. On success, *pval, will hold the current sample rate in Hz.
 *
 */

  PUBLIC_API int rsttsGetSampleRate(RSTTSInst inst,float* pval); /* Get sample rate from TTS instance inst, stored in memory pointed to by pval */


/**
 * @brief Sets callback function for audio generated by a TTS instance
 *
 * This call sets up a callback function that a TTS instance will call when
 * it has audio to emit, and a user data pointer to give to this callback
 * function. (Typically a rsttsSynthesize() or rsttsSynthesizeAsync() call
 * will generate multiple callbacks with audio and event information before
 * returning.)
 *
 * @param[in] inst TTS instance
 * @param[in] cb Callback function for audio. Will, if non-NULL, be called
 * repeatedly with a pointer to generated audio data (samples) and a length
 * (in chars) for that audio data, and finally the user-provided data value
 * provided when the callback was set up.
 * NULL may be provided instead of a function pointer,
 * to disable audio callbacks.
 * @param[in] userdata pointer to arbitrary user data associated with audio callbacks
 * from this TTS instance. This value is not used by the TTS API or engine,
 * except that it will be passed as an argument to all callback calls
 * from the instance. NULL should be used if there is no such data.
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note This operation will fail and return RSTTS_INSTANCE_BUSY if the TTS instance is busy.
 */

PUBLIC_API int 
rsttsSetAudioCallback(RSTTSInst inst, rsttsAudioCallback cb, void *userdata);


/**
 * @brief Sets callback function for events generated by a TTS instance
 *
 * This call sets up (or disables) a callback function that a TTS instance
 * will call to give notification about events generated during audio
 * generation.
 *
 * @param[in] inst TTS instance
 * @param[in] cb Callback function for events. Depending on event settings, it will be called for certain events encountered during audio generation with the TTS instance, an event data struct pointer and a user-provided data value. May be NULL to disable event callbacks.
 * @param[in] userdata pointer to arbitrary user data associated with event callbacks
 * from this TTS instance. This value is not used by the TTS API or engine,
 * except that it will be passed as an argument to all callback calls
 * from the instance. NULL should be used if there is no such data.
 *
 * @note Not all types of events are reported per default. See
 * rsttsParameter() and its parameter RSTTS_PARAM_EVENT_MASK to select
 * which types of events will be reported.
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note This operation will fail and return RSTTS_INSTANCE_BUSY if the TTS instance is busy.
 */

PUBLIC_API int rsttsSetEventCallback(RSTTSInst inst, rsttsEventCallback cb, void *userdata);

/**
 * @brief Generates speech from text
 *
 * This call generates speech from text. It will not return until the text
 * is fully processed and all audio and event callbacks have been generated (except
 * on a critical error, or if requested to stop earlier).
 *
 * Input format must be supplied. Supported input formats are "text"
 * (utf-8 plain text), "ssml" (any version of SSML) and "autossml"
 * (handles incomplete SSML or UTF-8 plain text with mixed-in SSML fragments).
 * (Additional formats may be supported in the future.)
 *
 * It is mandatory that the language of the text to read is specified.
 * This is done within markup contained in the text itself (for input
 * formats that support this), or by a previous call to rsttsSetLanguage().
 *
 * Specifying a voice is not required; if no voice is explicitly selected
 * (via API calls or markup), then a voice supporting the language will
 * be selected automatically by the TTS engine (if there is at least one
 * such voice available).
 *
 * @param[in] inst TTS instance
 * @param[in] text input text (null-terminated UTF-8 string)
 * @param[in] format input format (for example "text" or "ssml")
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros. RSTTS_LANGUAGE_NOT_SET is returned if the input format requires the language to be set beforehand, but this has not been done.
 *
 * @note A result code indicating an error is only returned on a critical error that caused the whole operation to be prevented or aborted. Errors after which the operation may proceed are reported via Failure events (see rsttsSetEventCallback()). (The same result code is also indicated in the SynthesizeEnd event, if enabled; unless the instance was stopped during the call, which inhibits further callbacks from the ongoing operation.)
 */

PUBLIC_API int rsttsSynthesize(RSTTSInst inst, const char *text, const char *format); /* synthesize text into audio */


/**
 * @brief Generates speech from text in a background thread
 *
 * This call initiates a background generation of speech from text.
 * It will return instantly while speech generation proceeds in the background.
 *
 * Input format must be supplied. Supported input formats are "text"
 * (utf-8 plain text), "ssml" (any version of SSML) and "autossml"
 * (handles incomplete SSML or UTF-8 plain text with mixed-in SSML fragments).
 * (Additional formats may be supported in the future.)
 *
 * It is mandatory that the language of the text to read is specified.
 * This is done within markup contained in the text itself (for input
 * formats that support this), or by a previous call to rsttsSetLanguage().
 *
 * Specifying a voice is not required; if no voice is explicitly selected
 * (via API calls or markup), then a voice supporting the language will
 * be selected automatically by the TTS engine (if there is at least one
 * such voice available).
 *
 * @param[in] inst TTS instance
 * @param[in] text input text (null-terminated UTF-8 string)
 * @param[in] format input format (for example "text" or "ssml")
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros. RSTTS_LANGUAGE_NOT_SET is returned if the input format requires the language to be set beforehand, but this has not been done.
 * Success indicates that the operation is proceeding in a background thread.
 *
 * @note A successful return value merely indicates that the call was
 * successfully handed over to a background thread; it does not indicate
 * whether the operation as a whole will be successful, which is often
 * not yet known at that point.
 * A final result can however be reported in a SynthesizeEnd callback,
 * if enabled (see rsttsSetEventCallback() and rsttsSetParameter()).
 * Errors after which the operation may proceed are reported
 * via Failure events.
 *
 * @note The TTS instance will, on success, be busy for a while and can not
 * be freed with rsttsFree() while audio generation is still in progress.
 * To free the instance, optionally stop it first and then wait until it
 * is done.
 */

PUBLIC_API int rsttsSynthesizeAsync(RSTTSInst inst,const char *text,const char *format); /* syntesize in background */

/**
 * @brief Sets/changes the state of a TTS instance (stop/pause etc)
 *
 * This call sets the state of a TTS instance to any of the values
 * RSTTSInst_paused, RSTTSInst_playing or RSTTSInst_stopping.
 * It can be used to stop or to pause and resume speech generation.
 *
 * See the descriptions of RSTTSInst_State for more information about
 * the states of a TTS instance.
 *
 * Arbitrary transitions between RSTTSInst_paused and RSTTSInst_playing are
 * permitted, as well as transitions to RSTTSInst_stopping from one of
 * those two states.
 * Other transitions will fail.
 *
 * You may want to use the convenience macros rsttsStop(), rsttsPause()
 * and rsttsResume() rather than calling this function directly.
 *
 * @param[in] inst TTS instance
 * @param[in] state New instance state
 *
 * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 * If neither an error nor a state change occurred (i.e., the state requested is equal to
 * the current state at the time of the call), then the value RSTTS_OK is returned.
 * If the call caused a state change, then the new state value is returned as an RSTTS_State
 * casted to an int. (RSTTS_SUCCESS() is always true for such a value.)
 *
 * @note An instance in the RSTTSInst_ready state does not keep track of
 * what it was doing previously.
 * Audio generation can not be resumed or restarted, except by a new call to
 * rsttsSynthesize() or rsttsSynthesizeAsync().
 *
 * @note Even though the state change will be immediately visible to rsttsGetState(),
 * the state change may have asynchronous side effects (such as memory management,
 * debug output, ongoing callbacks and such) that still have not completed
 * when the call returns. (It is nevertheless safe to try to reuse or free a TTS instance at any time as long at it has not been freed. Such operations might return RSTTS_INSTANCE_BUSY if the instance is not ready.)
 */

PUBLIC_API int rsttsSetState(RSTTSInst inst, RSTTSInst_State state);

/**
  * @brief Retrieves the state of a TTS instance
  *
  * This call returns the current state of a TTS instance, as a
  * RSTTSInst_State value.

  *
  * @param[in] inst TTS instance
  *
  * @returns On success, one of the defined values of RSTTSInst_State.
  * On an error, an error code casted to RSTTSInst_State. In both cases,
  * the macros RSTTS_SUCCESS(), RSTTS_ERROR() can be used to check the
  * result. In practice, this function will always succeed as long as
  * the passed TTS instance is valid.
  */

PUBLIC_API RSTTSInst_State rsttsGetState(RSTTSInst inst);


/**
  * @brief Waits for a TTS instance to transition to some state(s)
  *
  * This call blocks until a TTS instance has transitioned to a state
  * contained in a provided set of states, with an optional timeout.
  *
  * The call will watch for TTS instance state transitions and return
  * when the TTS is in one of the desired states, or until the given
  * timeout has passed, whichever happens first.
  *
  * @param[in] inst TTS instance
  * @param[in] state_mask Set of states to wait for (RSTTSInst_State values OR:ed together)
  * @param[in] timeout_ms Timeout in milliseconds
  *
  * @returns Error code, check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
  *
  * @note This can be used to wait for background audio generation (such as
  * initiated with rsttsSynthesizeAsync() or by another thread) to complete.
  * Though, note that it is not safe to wait on an instance that may, at
  * any time during the wait, be freed by another thread using rsttsFree().
  */

PUBLIC_API int rsttsWaitState(RSTTSInst inst, int state_mask, int timeout_ms);


/**
 * @brief Loads a lexicon to apply to text during future audio generation
 *
 * This call gives the TTS engine a file to use as lexicon.
 *
 * @param[in] inst TTS instance
 * @param[in] lexfile Path to lexicon file
 * @param[in] where Determines at which stage to apply the lexicon. Use one of the constants RSTTS_PRE_MARKUP_PARSE, RSTTS_POST_MARKUP_PARSE.
 *
 * On success, (i.e. when the RSTTS_SUCCESS() macro evaluates the returned value to true), the returned value is a lexicon ID which can later be passed to rsttsUnloadLexicon() to unload the lexicon.
 * Trying to load a lexicon on a busy instance will return RSTTS_INSTANCE_BUSY and have no effect.
 *
 * A lexicon applied at the RSTTS_PRE_MARKUP_PARSE stage may introduce new markup, which is then interpreted according to the current text format (SSML markup if the text is in SSML, for example). This may make a lexicon specific to the used text format, and may also be more prone to errors (for example, a rule may inadvertently transform valid SSML into invalid SSML).
 *
 * A lexicon run at the RSTTS_POST_MARKUP_PARSE stage may not modify or add markup (SSML tags etc); or rather, any characters introduced at that point (even characters which would otherwise have special meanings in the current text format) will simply be considered as text and not as markup. This is because the markup processing has already been completed before the lexicon runs.
 *
 * Additional stages may be supported in the future.
 * For backwards compatibility, the name RSTTS_PRE_SSML_PARSE is available as an alias for RSTTS_PRE_MARKUP_PARSE.
 *
 * @warning Only one lexicon per stage can be loaded in the current version. Future versions may support having multiple lexicons loaded at the same stage at the same time.
 * @returns An arbitrary lexicon identifier number on success, otherwise an error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 */

PUBLIC_API int rsttsLoadLexiconFromFile(RSTTSInst inst, const char *lexfile,EngineStage where); /* load a lexicon to TTS instance inst, from file named lexfile to apply to text at engine stage where */


/**
 * @brief Unloads a lexicon from a TTS instance.
 *
 * This call removes a lexicon from TTS instance, causing it to no
 * longer be applied.  The passed lexicon id must be a a value
 * number returned by a call that loaded a lexicon to the same
 * instance (see rsttsLoadLexiconFromFile()). The call to
 * rsttsUnloadLexicon() will then unload the specific lexicon
 * identified by that lexicon id.
 *
 * @param[in] inst TTS instance
 * @param[in] lexid An integer used as a lexicon identifier
 * @returns Error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros.
 *
 * @note Trying to unload a lexicon from a busy instance will return RSTTS_INSTANCE_BUSY and have no effect.
 */

PUBLIC_API int rsttsUnloadLexicon(RSTTSInst inst, int lexid); /* unload a lexicon identified by lexid from TTS instance inst */


/**
 * @brief Retrieves language/voice version information for a voice
 *
 * This call saves sets the values pointed to by its pointer
 * parameters (if non-NULL) to numbers representing the
 * versions of the language and voice data used for a voice.
 *
 * @param[in] inst TTS instance
 * @param[in] lang Language code (NULL for current language, if any)
 * @param[in] voice Voice name (NULL for current voice, if any)
 * @param[out] langv Used to output version number for language data (if non-NULL)
 * @param[out] voicev Used to output version number for voice data (if non-NULL)
 * @returns Error code. Check for error/success with RSTTS_ERROR(), RSTTS_SUCCESS() macros. On success, the version information will be written to the locations pointed to by the pointer arguments.
 *
 * If the lang and voice parameters are both NULL, then information
 * about the voice currently in use by the instance (if any) is
 * requested. (When a voice is selected via an API call, then that
 * voice remains the current voice for this purpose until a new
 * voice/language selection, or until the voice currently in use
 * changes because of text markup.)
 *
 * If language and voice is specified, then the information for the
 * installed voice (if any) that matches those language/voice names is
 * used. If only voice is specified, then the language last selected
 * via API call is assumed (a language must have been selected in that
 * case). If only language is specified, then the preferred voice for
 * that language (as determined by the TTS engine) is assumed.
 *
 * The number consists of two fields, which can be masked out with the
 * RSTTS_LANGVOICEVER_MAJOR() and RSTTS_LANGVOICEVER_MINOR() macros.
 * The recommended way to represent these numbers in text is to print them
 * with a separating period ('.').
 *
 * The MAJOR part may be increased on a major revision of that
 * particular voice or language data. Apart from that, these numbers
 * do not hold any particular meaning except that they will increase
 * between consecutive revisions of the voice/audio data files, so
 * that newer files will always get higher numbers. The fields are
 * represented in such a way that the full integers can be compared
 * directly without, needing to mask out the fields first, so that a
 * more recent version will always compare as a higher number.
 *
 * @note Will fail and return RSTTS_OPERATION_NOT_ALLOWED if
 * information about the current voice is requested when the instance
 * has no current voice set (either via API call or via voice/language
 * markup).
 *
 * @note Can be called from within a RSTTSEvent_Voice event callback,
 * with lang and voice both set to NULL, to retrieve the version of
 * the voice just switched to.
 *
 * If information for the current voice (i.e. with both the lang and
 * voice parameters passed as NULL) is requested from a separate
 * thread on an instance that is currently reading a text using
 * multiple voices or languages, a call might temporarily fail or
 * return version information for a voice in use at that approximate
 * time.
 */

PUBLIC_API int rsttsGetLanguageVoiceVersion(RSTTSInst inst, const char *lang, const char *voice, unsigned long *langv, unsigned long *voicev);

#define RSTTS_LANGVOICEVER_MAJOR(v) ((v)>>24) /*!< @brief Extract major part of voice/lang version retrieved with rsttsGetLanguageVoiceVersion() */
#define RSTTS_LANGVOICEVER_MINOR(v) ((v)&0xfffffful) /*!< @brief Extract minor part of voice/lang version retrieved with rsttsGetLanguageVoiceVersion() */


#define rsttsStop(ttsinst) rsttsSetState(ttsinst, RSTTSInst_stopping);  /*!< @brief Stops a playing or paused TTS instance. (Convenience macro.) */
#define rsttsPause(ttsinst) rsttsSetState(ttsinst, RSTTSInst_paused);  /*!< @brief Pauses a playing TTS instance. (Convenience macro.) */
#define rsttsResume(ttsinst) rsttsSetState(ttsinst, RSTTSInst_playing);  /*!<@brief Starts a paused tts instance again. (Convenience macro.) */


// Settings constants exposed by API
// (useful values for rsttsSetVolume() etc)
#define RSTTS_PITCH_DEFAULT 100.0  /*!< @brief Default pitch setting (see rsttsSetPitch()) */
#define RSTTS_SPEED_DEFAULT 100.0  /*!< @brief Default speed setting (see rsttsSetSpeed()) */
#define RSTTS_VOLUME_DEFAULT 100.0 /*!< @brief Default volume setting (see rsttsSetVolume()) */
#define RSTTS_VOLUME_SILENT 0.0    /*!< @brief Lower limit of volume scale (see rsttsSetVolume()) */
#define RSTTS_VOLUME_MAX 250.0     /*!< @brief Upper limit of volume scale (see rsttsSetVolume()) */
#define RSTTS_QUALITY_FAST 0	   /*!< @deprecated Use RSTTS_QUALITY_LOW instead @brief Lower limit of quality scale (see rsttsSetParameter())*/
#define RSTTS_QUALITY_FULL 100	   /*!< @deprecated Use RSTTS_QUALITY_NORMAL instead @brief Upper limit of quality scale (see rsttsSetParameter())*/
#define RSTTS_QUALITY_LOW 0	   /*!< @brief Lower limit of accuracy scale (see rsttsSetParameter())*/
#define RSTTS_QUALITY_NORMAL 100	   /*!< @brief Upper limit of accuracy scale (see rsttsSetParameter())*/
#define RSTTS_RESPONSIVENESS_NORMAL 0	   /*!< @brief Lower limit of responsiveness scale (see rsttsSetParameter())*/
#define RSTTS_RESPONSIVENESS_FAST 100	   /*!< @brief Upper limit of responsiveness scale (see rsttsSetParameter())*/

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
