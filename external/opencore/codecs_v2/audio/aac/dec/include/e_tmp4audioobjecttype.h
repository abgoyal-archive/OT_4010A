

#ifndef E_TMP4AUDIOOBJECTTYPE_H
#define E_TMP4AUDIOOBJECTTYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; INCLUDES
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    typedef enum eMP4AudioObjectType
    {
        MP4AUDIO_NULL            =  0, /*                                       */
        MP4AUDIO_AAC_MAIN        =  1, /*                                       */
        MP4AUDIO_AAC_LC          =  2, /* LC = Low Complexity                   */
        MP4AUDIO_AAC_SSR         =  3, /* SSR = Scalable Sampling Rate          */
        MP4AUDIO_LTP             =  4, /* LTP = Long Term Prediction            */
        MP4AUDIO_SBR             =  5, /* SBR = Spectral Band Replication       */
        MP4AUDIO_AAC_SCALABLE    =  6, /* scales both bitrate and sampling rate */
        MP4AUDIO_TWINVQ          =  7, /* low bit rate                          */
        MP4AUDIO_CELP            =  8,
        MP4AUDIO_HVXC            =  9,
        /* 10 is reserved                        */
        /* 11 is reserved                        */
        MP4AUDIO_TTSI            = 12,
        /* 13-16 are synthesis and MIDI types    */
        MP4AUDIO_ER_AAC_LC       = 17, /*                                       */
        /* 18 is reserved                        */
        MP4AUDIO_ER_AAC_LTP      = 19, /*                                       */
        MP4AUDIO_ER_AAC_SCALABLE = 20, /*                                       */
        MP4AUDIO_ER_TWINVQ       = 21, /*                                       */
        MP4AUDIO_ER_BSAC         = 22, /*                                       */
        MP4AUDIO_ER_AAC_LD       = 23, /*                                       */
        MP4AUDIO_ER_CELP         = 24, /*                                       */
        MP4AUDIO_ER_HVXC         = 25, /*                                       */
        MP4AUDIO_ER_HILN         = 26, /*                                       */
        MP4AUDIO_PARAMETRIC      = 27, /*                                       */
        MP4AUDIO_PS              = 29  /*  Explicit Parametric Stereo           */

    } tMP4AudioObjectType;

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    /* Should not be any function declarations in this file */

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* E_TMP4AUDIOOBJECTTYPE_H */


