

#ifndef PVMP3_POLY_PHASE_SYNTHESIS_H
#define PVMP3_POLY_PHASE_SYNTHESIS_H


#include "pvmp3_audio_type_defs.h"
#include "s_tmp3dec_chan.h"
#include "pvmp3decoder_api.h"








#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_poly_phase_synthesis(tmp3dec_chan   *pChVars,
    int32          numChannels,
    e_equalization equalizerType,
    int16          *outPcm);

#ifdef __cplusplus
}
#endif

#endif

