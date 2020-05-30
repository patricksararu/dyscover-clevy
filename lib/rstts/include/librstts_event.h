#ifndef __LIBRSTTS_EVENT_H__
#define __LIBRSTTS_EVENT_H__

/**
 * @file librstts_event.h
 * @brief Data structures used for event reporting
 *
 * This header file defines data structures used by the TTS API to
 * report events that occur during processing. It is not included
 * automatically by librstts.h, and should be included explicitly
 * if desired.
 */

#include <sys/types.h>

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif


/**
 * @brief This enumerates the various events that the TTS can generate.
 * The values are used to identify events, and to compose the event
 * bitmask which specifies which events an instance should report.
 *
 * \li RSTTSEvent_None: Can be used to represent "no event".
 *     The TTS will never report an event of this type.
 *
 * \li RSTTSEvent_Mark: Generated when the speech has reached a mark
 *     in the input (for SSML input: the 'mark' element).
 *     Note that lexicon rules, built-in normalization rules and other
 *     circumstances may cause the reading order to not
 *     strictly follow the word order of the input; the reported mark
 *     events will then follow the actual reading order rather than the
 *     order in the input. Therefore, it cannot be assumed that mark events
 *     will always occur in in exactly the same order as that of the mark
 *     elements in SSML input.
 *
 * \li RSTTSEvent_Paragraph: Generated when starting to read a paragraph,
 *     where explicit paragraph markup is present (SSML p element).
 *     If the parameter TEXT_POSITION_TRACKING is set, then the event
 *     will contain the text position for the paragraph boundary as
 *     the text start position of the event. (The text end position is
 *     currently not used for this event type.)
 *
 * \li RSTTSEvent_Sentence: Generated when starting to read a sentence.
 *     The event will contain text positions for the start and the end of
 *     the sentence (including final punctuation), depending on the
 *     setting of the parameter TEXT_POSITION_TRACKING. Note that
 *     sentence boundaries are controlled both by (SSML) markup and
 *     punctuation.
 *
 * \li RSTTSEvent_SynthesizeEnd: Generated at the end of a call to
 *     rsttsSynthesize() and rsttsSynthesizeAsync(), as the last thing
 *     that happens, regardless of error or success. (It is however not
 *     generated if the call was stopped, which inhibits further events
 *     resulting from the same call.)
 *     The event type specific event information contains a status
 *     value from the call, which, in the case of rsttsSynthesize() is
       the same as its return value
 *     (for rsttsSynthesizeAsync(), it will differ in cases where an error
 *     was detected after the call was handed over to a background thread.)
 *     Unlike other events, this event happens after the call is
 *     essentially finished, and the instance has transitioned back to
 *     the ready state (i.e. the instance is considered no longer in use
 *     for the call at this point).
 *     rsttsGetErrorMessage(NULL) can be called within the callback to
 *     extract an human-readable error message from the call.
 *
 * \li RSTTSEvent_TextStart: Generated when starting to read a text, before
 *     all audio and other events.
 *
 * \li RSTTSEvent_TextEnd: Generated when finished reading a text, after
 *     all audio and other events.
 *
 * \li RSTTSEvent_Word: Generated when starting to read a word. The event
 *     will contain text positions for the start and the end of the word,
 *     if TEXT_POSITION_TRACKING has been enabled.
 *     Note that the event indicates a word being read, and that
 *     normalization rules within the TTS might split or join words.
 *     The same positions may be reported in multiple events, one for
 *     each word actually read, when text normalization has expanded an
 *     abbreviation into several words.
 *
 * Audio/pronunciation events:
 *
 * \li RSTTSEvent_Audio: Generated when inserting (non-text) audio into
 *     the TTS audio output, such as by the SSML audio element.
 *
 * \li RSTTSEvent_Break: Generated when a prosodic break and/or a period of
 *     silence is inserted into the TTS audio output. Note that the event
 *     indicates the presence of a prosodic break or a pause, rather than
 *     than the presence of markup; such may be generated automatically
 *     by the TTS (for example at punctuation) as well as explicitly
 *     (by an SSML break element).
 *     A break event with length 0 indicates the presence of a prosodic
 *     break without an associated period of silence.
 *
 * \li RSTTSEvent_Emphasis: Generated to indicate a emphasis level.
 *     The reported emphasis level is represented with an enum value,
 *     and corresponds to the emphasis value actually used by the TTS,
 *     regardless of what was specified in SSML input.
 *
 * \li RSTTSEvent_Phoneme: NOT IMPLEMENTED in this version of the TTS.
 *     Reserved for the use of reporting pronounced phonemes.
 *
 * \li RSTTSEvent_Prosody: Generated to indicate a document-internal
 *     change in prosody parameters (pitch, rate, volume), which may
 *     for example be caused by SSML markup.
 *
 * \li RSTTSEvent_Viseme: Generated to indicate mouth movements to
 *     facilitate speech animation. The events will contain viseme
 *     symbols only if a viseme alphabet has been set (using
 *     rsttsSetParameter()) which is supported for the current voice
 *     and language. Note that any of the values can be NULL:
 *     viseme_symbol or viseme_stress_symbol may be NULL if the
 *     current pronunciation or stress cannot be represented
 *     in the selected viseme alphabet (if any), and viseme_alphabet can be
 *     NULL to indicate that the selected viseme alphabet (if any)
 *     isn't available for the current voice or language
 *     (the other fields should not be expected to contain
 *     meaningful values, not even valid pointers, in this case).
 * 
 * \li RSTTSEvent_Voice: Generated when the engine switches to a voice
 *     from another (or first starts reading with a voice).
 *     Note that these events do not
 *     necessarily correspond to SSML voice tags, but indicate actual voice
 *     changes (so a voice element that doesn't trig a voice change
 *     will not generate an event, and an automatic voice change will
 *     also generate an event).
 *     The event contains information both about what voice was requested,
 *     and what voice was actually chosen.
 *
 * End and failure events:
 *
 * \li RSTTSEvent_SynthesizeEnd: Generated when a rsttsSynthesize() or
 *     rsttsSynthesizeAsync() call has finished (note however, that if the
 *     call is stopped with rsttsStop() or rsttsSetState(), that
 *     will suppress any remaining events from the ongoing call, including
 *     this one). The event will occur immediately before the call returns,
 *     and, in case the instance was Ready at the time of the call, after
 *     the instance has returned to the Ready state. (For multithreaded
 *     applications, note that other threads doing API calls on the same
 *     instance might observe this transition before the event is generated.)
 *     For an rsttsSynthesizeAsync() call, this event can be used to get
 *     access to the status value that the call would have returned if
 *     synchronous. (Some error codes can not be returned directly by
 *     rsttsSynthesizeAsync(), because the error might not be detected until
 *     after the asynchronous job has been handed over to a background
 *     thread).
 *     rsttsGetErrorMessage(NULL) can be called from within the event
 *     callback for a human readable error description.
 *
 * \li RSTTSEvent_Failure: Used to report non-critical TTS runtime errors, including failed voice/language switch, errors in transcriptions and such.
 */
  
typedef enum {
  // No event
  RSTTSEvent_None = 0,

  // Position events.
  RSTTSEvent_Mark = 1<<0,       // 1, 0x1
  RSTTSEvent_Paragraph = 1<<1,  // 2, 0x2
  RSTTSEvent_Sentence = 1<<2,   // 4, 0x4
  RSTTSEvent_TextEnd = 1<<3,    // 8, 0x8
  RSTTSEvent_TextStart = 1<<4,  // 16, 0x10
  RSTTSEvent_Word = 1<<5,       // 32, 0x20

  // Audio/pronunciation events
  RSTTSEvent_Audio = 1<<7,          // 128, 0x80
  RSTTSEvent_Break = 1<<8,          // 256, 0x100
  RSTTSEvent_Emphasis = 1<<9,       // 512, 0x200
  RSTTSEvent_Phoneme = 1<<10,       // 1024, 0x400
  RSTTSEvent_Prosody = 1<<11,       // 2048, 0x800
  RSTTSEvent_Viseme = 1<<12,        // 4096, 0x1000
  RSTTSEvent_Voice = 1<<13,         // 8192, 0x2000
  
  // End/failure events
  RSTTSEvent_SynthesizeEnd = 1<<14, // 16384, 0x4000
  RSTTSEvent_Failure = 1<<15        // 32768, 0x8000
  
} RSTTSEventType;


// Useful constants for event bitmasks.
#define RSTTS_EVENT_MASK_NONE ((int)RSTTSEvent_None)  //!< @brief Event bitmask for disabling all events
#define RSTTS_EVENT_MASK_DEFAULT ((int)(RSTTSEvent_Mark | RSTTSEvent_Failure))  //!< Event bitmask enabling only the set of events that an instance will generate per default (if no event bitmask has been explicitly set). The default set might change in future versions of the SDK; users interested in particular events should enable them explicitly even if in this default set.
#define RSTTS_EVENT_MASK_ALL ((int)0xffff)  //!< Event bitmask indicating all events enabled


/**
 * @brief Voice description for inclusion in events
 *
 * This type contains information about a voice. Values may be
 * unset/undefined, which is indicated with NULL for pointers,
 * -1 for ints.
 * These fields correspond to SSML voice attributes, and have
 * similar values.
 */
typedef struct {
  const char *lang;     //!< Language code
  const char *name;     //!< Voice name
  const char *gender;   //!< Voice gender
  int age;              //!< Voice age, if available, otherwise -1
  int variant;          //!< Voice variant no, if available, otherwise -1
} RSTTSEventVoiceDesc;

/**
 * @brief Emphasis level for inclusion in events
 *
 * This type specifies an emphasis level returned in an event
 * callback. (The Unset value is never returned by the API;
 * the default emphasis level is None.)
 */
typedef enum {
  RSTTSEvent_EmphasisLevel_Unset = 0,  // (never returned by API)
  RSTTSEvent_EmphasisLevel_Reduced,
  RSTTSEvent_EmphasisLevel_None,
  RSTTSEvent_EmphasisLevel_Moderate,
  RSTTSEvent_EmphasisLevel_Strong
} RSTTSEvent_EmphasisLevel;


// Additional data for some position events.
// Strings will be NULL if unavailable.
// (Some types of events don't have any additional data in this API
// version. They are declared here containing a 'char unused' so
// that they are accepted by C compilers that do not accept empty
// structs. Do not attempt to use this field for any purpose, it
// might go away in future versions.)


/**
 * @brief Event type specific data for Mark events
 *
 * Contains the name of the mark trigging the event.
 */
typedef struct {
  const char *name;  //!< Name of the mark
} RSTTSEventData_Mark;

/**
 * @brief Event type specific data for Paragraph events
 *
 * id is currently always NULL. Reserved to possibly be used to report
 * paragraph id (xml:id attribute in SSML input) in future releases
 * of the text-to-speech engine.
 */
typedef struct {
  const char *id;    //!< Currently always NULL
} RSTTSEventData_Paragraph;

/**
 * @brief Event type specific data for Sentence events
 *
 * id is currently always NULL. Reserved to possibly be used to report
 * paragraph id (xml:id attribute in SSML input) in future releases
 * of the text-to-speech engine.
 */
typedef struct {
  const char *id;    //!< Currently always NULL
} RSTTSEventData_Sentence;

/**
 * @brief Event type specific data for TextEnd events
 *
 * Contains no data; the "unused" field is there because some C compilers
 * do not accept empty struct declarations. Do not access it.
 */
typedef struct {
  char unused;  //!< Unused, do not access
} RSTTSEventData_TextEnd;

/**
 * @brief Event type specific data for TextStart events
 *
 * Contains no data; the "unused" field is there because some C compilers
 * do not accept empty struct declarations. Do not access it.
 */
typedef struct {
  char unused;  //!< Unused, do not access
} RSTTSEventData_TextStart;

/**
 * @brief Event type specific data for Word events
 *
 * id is currently always NULL. Reserved to possibly be used to report
 * paragraph id (xml:id attribute in SSML input) in future releases
 * of the text-to-speech engine.
 */
typedef struct {
  const char *id;    //!< Currently always NULL
} RSTTSEventData_Word;


// Data for audio/pronunciation events

/**
 * @brief Event type specific data for Audio events
 *
 * Contains no data; the "unused" field is there because some C compilers
 * do not accept empty struct declarations. Do not access it. May be given
 * another definition in future releases.
 */
typedef struct {
  char unused;  //!< Unused, do not access
} RSTTSEventData_Audio;

/**
 * @brief Event type specific data for Break events
 *
 * strength may hold a value describing the prosodic strength of the break,
 * typically corresponding to the break strength names used in the SSML spec.
 * Where reported, strength values correspond to the prosodic break levels
 * actually used by the TTS engine, which may come from SSML input but
 * will also sometimes be automatically generated based on context,
 * punctuation and sentence/paragraph markup.
 */
typedef struct {
  const char *strength;  //!< Break prosodic strength if defined (or NULL)
  int duration_ms;       //!< Duration of the break, in milliseconds
} RSTTSEventData_Break;

/**
 * @brief Event type specific data for Emphasis events
 *
 * Emphasis events indicate the current emphasis level used by the
 * text-to-speech engine. (The levels are those used in the SSML spec,
 * though the emitted emphasis values are those actually used by the
 * text-to-speech engine for reading the text, and should not be assumed
 * to strictly follow emphasis information given in SSML input.)
 */
typedef struct {
  RSTTSEvent_EmphasisLevel level;  //!< Currently used emphasis level
} RSTTSEventData_Emphasis;

/**
 * @brief Event type specific data for Phoneme events
 *
 * This type of event is not implemented in the current version of the
 * text-to-speech engine, but is reserved to possibly report pronounced
 * phonemes (not to be confused with SSML phoneme elements!) in future
 * releases.
 *
 * Contains no data; the "unused" field is there because some C compilers
 * do not accept empty struct declarations. Do not access it. Will be
 * given another definition if/when this type of event is implemented.
 */
typedef struct {
  char unused;  //!< Unused, do not access
} RSTTSEventData_Phoneme;

/**
 * @brief Event type specific data for Prosody events
 * 
 * The values correspond to the current cumulative prosody
 * values as set by text markup. These values are not scaled by API
 * settings, but are reported on the same 100-based scale as the
 * corresponding settings (pitch, speed, volume) in the API.
 *
 * (Note that the actual pitch, speed and volume settings used by the
 * text-to-speech engine are affected by both values in markup and by
 * the API parameter settings. This event only reports the former.)
 */
typedef struct {
  float pitch;   //!< Pitch selected by text markup (normal: 100.0)
  float rate;    //!< Rate (speed) selected by text markup (normal: 100.0)
  float volume;  //!< Volume selected by text markup (normal: 100.0)
} RSTTSEventData_Prosody;

/**
 * @brief Event type specific data for Viseme events
 *
 * The strings contain information about the viseme starting at the position
 * of the event. Note that any of the values may be NULL if no viseme
 * alphabet has been selected, or if the selected viseme alphabet is not
 * supported by the current voice or cannot represent the viseme and/or its
 * associated stress information.
 *
 * Silence is also a viseme, and is reported according to its
 * representation in the selected viseme alphabet.
 */
typedef struct {
  const char *viseme_alphabet;      //!< Viseme alphabet (NULL if none)
  const char *viseme_symbol;        //!< Viseme symbol (NULL if none available)
  const char *viseme_stress_symbol; //!< Viseme stress symbol (NULL if none available)
} RSTTSEventData_Viseme;

/**
 * @brief Event type specific data for Voice events
 *
 * The "requested" and "actual" fields are used to indicate the values
 * requested by the user (such as via SSML) attributes, and the values actually
 * used by the text-to-speech engine, respectively. (These values may or
 * may not be the same; for example, after selecting a voice
 * only by gender and not by voice name, the
 * "requested" field will indicate gender but not name, while the
 * "actual" field will contain name as well as gender for the voice
 * actually selected by the text-to-speech engine.)
 */
typedef struct {
  RSTTSEventVoiceDesc requested;  //!< Values requested by API and/or markup
  RSTTSEventVoiceDesc actual;     //!< Actual values used by TTS engine
} RSTTSEventData_Voice;

/**
 * @brief Event type specific data for SynthesizeEnd events
 *
 * rstts_status is the final status code indicating success/error of a
 * call to rsttsSynthesize() or rsttsSynthesizeAsync().
 *
 */
typedef struct {
  int rstts_statuscode;   //!< Status code for the just finishing call
} RSTTSEventData_SynthesizeEnd;

/**
 * @brief Failure types, to be reported with a Failure event.
 *
 * These values indicate none-critical failures, i.e. errors where the
 * text-to-speech engine can continue
 * processing the text even after the error unless instructed to
 * stop. Such errors may cause parts of the text to be skipped or
 * read in an unexpected manner, depending on the type of the error,
 * and this type of event is used as a notification when this happens.
 * (There are generally no failure events for errors that cause the TTS call
 * to fail completely, for example SSML parse errors; such errors will
 * simply be reported via a result code, rather than by failure events.
 * Failure events are primarily for errors that don't prevent
 * the TTS from continuing to try to read the text.)
 *
 * (Not all of these failure types are be supported in the current version.)
 */
typedef enum {
  RSTTSEvent_AudioLoadFailure,    //!< Can't load audio (SSML audio element)
  RSTTSEvent_AudioPlayFailure,    //!< Can't play (loaded) audio (SSML audio element)
  RSTTSEvent_LangFailure,         //!< Unsupported language in lang switch
  RSTTSEvent_LexiconLoadFailure,  //!< Cannot load lexicon
  RSTTSEvent_LexiconUseFailure,   //!< Cannot use (loaded) lexicon
  RSTTSEvent_MarkupFailure,       //!< Misc markup/SSML related warnings
  RSTTSEvent_SayAsFailure,        //!< Unsupported use of say-as (SSML)
  RSTTSEvent_SentenceLengthFailure, //!< Sentence too long, broken up
  RSTTSEvent_TranscriptionAlphabetFailure,  //!< Unsupported phoneme alphabet
  RSTTSEvent_TranscriptionOtherFailure,     //!< (Other transcription error)
  RSTTSEvent_TranscriptionPhonemeFailure,   //!< Unsupported/illegal phoneme
  RSTTSEvent_VoiceFailure,        //!< Voice switch failed
  RSTTSEvent_WordLengthFailure    //!< Word too long (currently unused)
  // (FIXME: Do we want WordLengthFailure? When generate it?)
} RSTTSFailureEventType;

/**
 * @brief Event type specific data for Failure events
 *
 * This type of event will be used by the text-to-speech engine to inform
 * of non-critical errors and failures that do not require the ongoing
 * rsttsSynthesize() or rsttsSynthesizeAsync() call to immediately return
 * an error code.
 *
 * The rstts_statuscode field indicates the status/error code from
 * librstts_error.h that most closely corresponds to the semantics
 * of the failure event.
 *
 * The http_statuscode field contains a HTTP status code that
 * matches the failure event, or -1 if none applies.
 *
 * The description is a plain-text, UTF-8 human readable description of
 * the failure event (currently always in English), for logging, error
 * reporting and debugging purposes. An application should not rely on
 * these messages being in any specific form. The precise contents of
 * these descriptions are not considered as a stable part of the
 * API and may vary depending on settings and platform, or change
 * arbitrarily between versions.
 *
 * This structure may be extended in future versions of the API.
 */
typedef struct {
  RSTTSFailureEventType failure_type; //!< Event subtype (type of failure)
  int rstts_statuscode;      //!< status code from librstts_error.h
  int http_statuscode;       //!< HTTP status code (-1 if none applicable)
  const char *description;   //!< Human readable description, for logging/printing
  RSTTSEventVoiceDesc *voice;  //!< Requested voice and/or lang that couldn't be satisfied (for VoiceFailure/LangFailure); NULL if not applicable
} RSTTSEventData_Failure;


/**
 * @brief Union of pointers to structs with event type specific information
 *
 * This is a union of pointers to each type of event specific data.
 * Only the member corresponding to the event type of the current event
 * may be accessed.
 *
 * (Each member is a pointer to a struct with event type specific
 * information for that type of event. These structs may be extended
 * with further information in future versions of this API. For symmetry
 * and extensibility purposes, such structs are defined for all event
 * types, including those that do not have any event type specific
 * information in the current release.)
 */

typedef union {
  // Extra data for position events
  RSTTSEventData_Mark *Mark;           //!< for eventtype: RSTTSEvent_Mark
  RSTTSEventData_Paragraph *Paragraph; //!< for eventtype: RSTTSEvent_Paragraph
  RSTTSEventData_Sentence *Sentence;   //!< for eventtype: RSTTSEvent_Sentence
  RSTTSEventData_TextEnd *TextEnd;     //!< for eventtype: RSTTSEvent_TextEnd
  RSTTSEventData_TextStart *TextStart; //!< for eventtype: RSTTSEvent_TextStart
  RSTTSEventData_Word *Word;           //!< for eventtype: RSTTSEvent_Word

  // Extra data for audio/pronunciation events
  RSTTSEventData_Audio *Audio;         //!< for eventtype: RSTTSEvent_Audio
  RSTTSEventData_Break *Break;         //!< for eventtype: RSTTSEvent_Break
  RSTTSEventData_Emphasis *Emphasis;   //!< for eventtype: RSTTSEvent_Emphasis
  RSTTSEventData_Phoneme *Phoneme;     //!< for eventtype: RSTTSEvent_Phoneme
  RSTTSEventData_Prosody *Prosody;     //!< for eventtype: RSTTSEvent_Prosody
  RSTTSEventData_Viseme *Viseme;       //!< for eventtype: RSTTSEvent_Viseme
  RSTTSEventData_Voice *Voice;         //!< for eventtype: RSTTSEvent_Voice

  // Extra data for end/failure events
  RSTTSEventData_SynthesizeEnd *SynthesizeEnd; //!< for eventtype: RSTTSEvent_SynthesizeEnd
  RSTTSEventData_Failure *Failure;     //!< for eventtype: RSTTSEvent_Failure

} RSTTSEventSpecificData;


/**
 * @brief Data associated with an event
 *
 * This data type contains data associated with an event. This data has a
 * lifetime limited to the duration of the event callback, and is allocated
 * and de-allocated by the text-to-speech engine itself.
 * An application needing to access any event data after the event callback
 * has completed will need to make its own deep copy of the data, as the
 * text-to-speech engine may reuse the memory holding for the callback data
 * for other purposes once the event callback function has returned.
 *
 * Unless otherwise specified: Strings (const char pointers) in event
 * data are always either in zero-terminated UTF-8 representation, or
 * may alternatively be NULL to indicate that no value is applicable
 * or available.
 */
struct RSTTSEventData {
  /**
   * A RSTTSEventType value indicating what kind of event this
   * RSTTSEventData record represents
   */
  RSTTSEventType eventtype;

  /**
   * Start position of the event in the input text, counted in bytes
   * of text data (excluding XML markup).
   * May be -1 to indicate that no text position can be
   * determined or is defined for this type of event.
   *
   * Note the RSTTS_PARAM_TEXT_POSITION_TRACKING parameter needs to be
   * enabled for text positions to be reported to the full possible extent.
   * (See rsttsSetParameter())
   */
  long text_pos_start;

  /**
   * End position of the event in the input text, counted in bytes of
   * text data (excluding XML markup).
   * The end position, if available, points directly past
   * the event. (Start and end position may be equal to indicate that
   * an event has a text length known to be zero.)
   * The special value -1 indicates that no text end position can be
   * determined or is defined for this type of event.
   * The value -1 is also used with
   * events that indicate a change until further notice, for example for
   * voice and prosody events.
   *
   * Note the RSTTS_PARAM_TEXT_POSITION_TRACKING parameter needs to be
   * enabled for text positions to be reported to the full possible extent.
   * (See rsttsSetParameter())
   */
  long text_pos_end;

  /**
   * Byte position for the event in the audio stream,
   * relative to the start of the current
   * rsttsSynthesize() or rsttsSynthesizeAsync() call.
   */
  long byte_pos;

  /**
   * Timing of the event in the audio stream,
   * measured in milliseconds and relative to the start of the current
   * rsttsSynthesize() or rsttsSynthesizeAsync() call.
   */
  long time_pos_ms;

  /**
   * This field contains any additional data specific to the type of
   * the event. (RSTTSEventSpecificData is a union type; it is only
   * allowed to access the member of evtspec which matches the
   * type of the event as indicated by eventtype.)
   */
  RSTTSEventSpecificData evtspec;
};

#ifndef RSTTSEVENTDATA_DEFINED
/**
 * @brief typedef for type RSTTSEventData
 *
 * Defined both here and in librstts.h; the preprocessor symbol
 * RSTTSEVENTDATA_DEFINED keeps track of whether we've seen the definition.
 */
typedef struct RSTTSEventData RSTTSEventData;
#define RSTTSEVENTDATA_DEFINED  /*!< @brief Preprocessor symbol defined if the event RSTTSEventData typedef declaration has been included */
#endif


#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
