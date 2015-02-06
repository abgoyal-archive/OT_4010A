

#ifndef IMDCT_FXP_H
#define IMDCT_FXP_H

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

#define     LONG_WINDOW_TYPE  2048
#define     SHORT_WINDOW_TYPE  256

#define     ALL_ZEROS_BUFFER       31

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    extern const Int32 exp_rotation_N_256[64];
    extern const Int32 exp_rotation_N_2048[512];
    /*
    extern const Int exp_rotation_N_256[128];
    extern const Int exp_rotation_N_2048[1024];
    */

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
    Int imdct_fxp(
        Int32   data_quant[],
        Int32   freq_2_time_buffer[],
        const   Int     n,
        Int     Q_format,
        Int32   max
    );


#ifdef __cplusplus
}
#endif

#endif  /* IMDCT_FXP_H */
