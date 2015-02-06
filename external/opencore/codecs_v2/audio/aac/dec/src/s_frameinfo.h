

#ifndef S_FRAMEINFO_H
#define S_FRAMEINFO_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; INCLUDES
    ----------------------------------------------------------------------------*/
#include "pv_audio_type_defs.h"
#include "e_blockswitching.h"

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
    typedef struct
    {
        Int     islong;                 /* true if long block */
        Int     num_win;                /* sub-blocks (SB) per block */
        Int     coef_per_frame;         /* coef's per block */
        Int     sfb_per_frame;          /* sfb per block */
        Int     coef_per_win[MAX_WIN];  /* coef's per SB */
        Int     sfb_per_win[MAX_WIN];   /* sfb per SB */
        Int     sectbits[MAX_WIN];
        Int16   *win_sfb_top[MAX_WIN];  /* top coef per sfb per SB */
        Int     *sfb_width_128;         /* sfb width for short blocks */

        Int     frame_sfb_top[MAXBANDS];    /* Only used in calc_gsfb_table() -
                                      it is simply a cum version of
                                      the above information */
        Int     num_groups;
        Int     group_len[8];

    } FrameInfo;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* S_FRAMEINFO_H */
