

#ifndef _PVAMRWBDECODER_API_H
#define _PVAMRWBDECODER_API_H

#include    "oscl_base.h"    /* Basic data types used within the lib */


#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

#define AMR_WB_PCM_FRAME   320             /* Frame size at 16kHz     */


#define NBBITS_7k     132                  /* 6.60k  */
#define NBBITS_9k     177                  /* 8.85k  */
#define NBBITS_12k    253                  /* 12.65k */
#define NBBITS_14k    285                  /* 14.25k */
#define NBBITS_16k    317                  /* 15.85k */
#define NBBITS_18k    365                  /* 18.25k */
#define NBBITS_20k    397                  /* 19.85k */
#define NBBITS_23k    461                  /* 23.05k */
#define NBBITS_24k    477                  /* 23.85k */

#define NBBITS_SID    35

#define KAMRWB_NB_BITS_MAX   NBBITS_24k
#define KAMRWB_NB_BYTES_MAX  ((KAMRWB_NB_BITS_MAX>>3)+1)

#define NUM_OF_MODES  10


    const int16 AMR_WB_COMPRESSED[NUM_OF_MODES] =
    {
        NBBITS_7k,
        NBBITS_9k,
        NBBITS_12k,
        NBBITS_14k,
        NBBITS_16k,
        NBBITS_18k,
        NBBITS_20k,
        NBBITS_23k,
        NBBITS_24k,
        NBBITS_SID
    };

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


    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


#endif  /* PVMP4AUDIODECODER_API_H */


