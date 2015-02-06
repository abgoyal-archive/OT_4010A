

#ifndef S_SBR_DEC_H
#define S_SBR_DEC_H


#include    "s_sbr_frame_data.h"
#include    "pv_audio_type_defs.h"
#include    "s_patch.h"
#include    "e_blockswitching.h"






typedef struct
{
    Int32 outSampleRate;
    Int32 LC_aacP_DecoderFlag;  /* Low Complexity decoder flag  */

    Int32 startIndexCodecQmf;
    Int32 lowBandAddSamples;
    Int32 noCols;
    Int32 qmfBufLen;
    Int32 bufWriteOffs;
    Int32 bufReadOffs;

    Int32 sbStopCodec;
    Int   lowSubband;
    Int   prevLowSubband;
    Int32 highSubband;
    Int32 noSubbands;

    Int   FreqBandTable[2][MAX_FREQ_COEFFS + 1];
    Int32 FreqBandTableNoise[MAX_NOISE_COEFFS + 1];
    Int32 V_k_master[MAX_FREQ_COEFFS + 1];         /* Master BandTable which freqBandTable is derived from*/
    Int32 NSfb[2];
    Int32 NoNoiseBands;                            /* Number of noisebands */
    Int32 Num_Master;                              /* Number of bands in V_k_master*/

    struct PATCH Patch;                         /* Used by sbr_generate_high_freq */
    /* Used by calc_sbr_envelope */
    Int32 gateMode[4];
    Int32 limSbc[4][12 + 1];                            /* Limiting bands */

    Int32 sqrt_cache[8][4];                     /* cache memory for repeated sqrt() calculations */

} SBR_DEC;



#endif


