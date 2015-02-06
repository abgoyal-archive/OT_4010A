

#ifndef CHANS_H
#define CHANS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; INCLUDES
    ----------------------------------------------------------------------------*/
#include "pv_audio_type_defs.h"

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/
    /* #define is required in order to use these args in #if () directive */
#define ICChans 0
#define DCChans 0
#define XCChans 0
#define CChans  0

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
    enum
    {
        /*
         * channels for 5.1 main profile configuration
         * (modify for any desired decoder configuration)
         */
        FChans  = 2,    /* front channels: left, center, right */
        FCenter = 0,    /* 1 if decoder has front center channel */
        SChans  = 0,    /* side channels: */
        BChans  = 0,    /* back channels: left surround, right surround */
        BCenter = 0,    /* 1 if decoder has back center channel */
        LChans  = 0,    /* LFE channels */
        XChans  = 0,    /* scratch space for parsing unused channels */

        Chans   = FChans + SChans + BChans + LChans + XChans
    };
    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CHANS_H */

