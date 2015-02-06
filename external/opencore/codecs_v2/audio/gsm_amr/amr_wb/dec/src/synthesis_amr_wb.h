

#ifndef SYNTHESIS_AMR_WB_H
#define SYNTHESIS_AMR_WB_H



#include "pv_amr_wb_type_defs.h"




#ifdef __cplusplus
extern "C"
{
#endif

    void synthesis_amr_wb(
        int16 Aq[],                          /* A(z)  : quantized Az               */
        int16 exc[],                         /* (i)   : excitation at 12kHz        */
        int16 Q_new,                         /* (i)   : scaling performed on exc   */
        int16 synth16k[],                    /* (o)   : 16kHz synthesis signal     */
        int16 prms,                          /* (i)   : parameter                  */
        int16 HfIsf[],
        int16 nb_bits,
        int16 newDTXState,
        Decoder_State * st,                   /* (i/o) : State structure            */
        int16 bfi,                           /* (i)   : bad frame indicator        */
        int16 * ScratchMemory
    );

#ifdef __cplusplus
}
#endif



#endif  /* PV_NORMALIZE_H */
