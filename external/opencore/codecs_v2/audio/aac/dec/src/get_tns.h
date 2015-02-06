

#ifndef GET_TNS_H
#define GET_TNS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; INCLUDES
    ----------------------------------------------------------------------------*/
#include "pv_audio_type_defs.h"
#include "s_frameinfo.h"
#include "s_mc_info.h"
#include "s_tns_frame_info.h"
#include "s_bits.h"
#include "e_window_sequence.h"

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

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    void get_tns(
        const Int               max_bands,
        BITS            * const pInputStream,
        const WINDOW_SEQUENCE   wnd_seq,
        const FrameInfo * const pFrameInfo,
        const MC_Info   * const pMC_Info,
        TNS_frame_info  * const pTnsFrameInfo,
        Int32                   scratchTnsDecCoefMem[]);


#ifdef __cplusplus
}
#endif

#endif /* GET_TNS_H */


