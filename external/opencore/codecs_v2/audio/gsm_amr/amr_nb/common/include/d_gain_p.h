
#ifndef d_gain_p_h
#define d_gain_p_h "$Id $"

#include "typedef.h"
#include "mode.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    **************************************************************************
    *
    *  Function    : d_gain_pitch
    *  Purpose     : Decodes the pitch gain using the received index.
    *  Description : In case of no frame erasure, the gain is obtained
    *                from the quantization table at the given index;
    *                otherwise, a downscaled past gain is used.
    *  Returns     : Quantized pitch gain
    *
    **************************************************************************
    */
    Word16 d_gain_pitch(       /* return value: gain (Q14)                */
        enum Mode mode,        /* i : AMR mode                            */
        Word16 index           /* i   : index of quantization             */
    );

#ifdef __cplusplus
}
#endif

#endif
