

#ifndef ARMDECODE_H
#define ARMDECODE_H

#include    "typedef.h"
#include    "mode.h"
#include    "frame_type_3gpp.h"
#include    "pvamrnbdecoder_api.h"

/*--------------------------------------------------------------------------*/
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
#define NUM_AMRSID_RXMODE_BITS   3
#define AMRSID_RXMODE_BIT_OFFSET 36
#define AMRSID_RXTYPE_BIT_OFFSET 35

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/
    extern const Word16 WmfDecBytesPerFrame[];
    extern const Word16 If2DecBytesPerFrame[];

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    Word16 AMRDecode(
        void *state_data,
        enum Frame_Type_3GPP  frame_type,
        UWord8 *speech_bits_ptr,
        Word16 *raw_pcm_buffer,
        bitstream_format input_format
    );

#ifdef __cplusplus
}
#endif

#endif  /* _AMRDECODE_H_ */
