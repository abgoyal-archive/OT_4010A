

#ifndef E_PV_AMRWBDEC_H
#define E_PV_AMRWBDEC_H


#include "pvamrwbdecoder_cnst.h"             /* coder constant parameters */
#include "dtx.h"








typedef struct
{
    int16 old_exc[PIT_MAX + L_INTERPOL];  /* old excitation vector */
    int16 ispold[M];                      /* old isp (immittance spectral pairs)*/
    int16 isfold[M];                      /* old isf (frequency domain) */
    int16 isf_buf[L_MEANBUF * M];         /* isf buffer(frequency domain) */
    int16 past_isfq[M];                   /* past isf quantizer */
    int16 tilt_code;                      /* tilt of code */
    int16 Q_old;                          /* old scaling factor */
    int16 Qsubfr[4];                      /* old maximum scaling factor */
    int32 L_gc_thres;                     /* threshold for noise enhancer */
    int16 mem_syn_hi[M];                  /* modified synthesis memory (MSB) */
    int16 mem_syn_lo[M];                  /* modified synthesis memory (LSB) */
    int16 mem_deemph;                     /* speech deemph filter memory */
    int16 mem_sig_out[6];                 /* hp50 filter memory for synthesis */
    int16 mem_oversamp[2 * L_FILT];       /* synthesis oversampled filter memory */
    int16 mem_syn_hf[M16k];               /* HF synthesis memory */
    int16 mem_hf[2 * L_FILT16k];          /* HF band-pass filter memory */
    int16 mem_hf2[2 * L_FILT16k];         /* HF band-pass filter memory */
    int16 mem_hf3[2 * L_FILT16k];         /* HF band-pass filter memory */
    int16 seed;                           /* random memory for frame erasure */
    int16 seed2;                          /* random memory for HF generation */
    int16 old_T0;                         /* old pitch lag */
    int16 old_T0_frac;                    /* old pitch fraction lag */
    int16 lag_hist[5];
    int16 dec_gain[23];                   /* gain decoder memory */
    int16 seed3;                          /* random memory for lag concealment */
    int16 disp_mem[8];                    /* phase dispersion memory */
    int16 mem_hp400[6];                   /* hp400 filter memory for synthesis */

    int16 prev_bfi;
    int16 state;
    int16 first_frame;
    dtx_decState dtx_decSt;
    int16 vad_hist;

} Decoder_State;

typedef struct
{
    Decoder_State state;
    int16 ScratchMem[L_SUBFR + L_SUBFR16k + ((L_SUBFR + M + M16k +1)<<1) + \
                     (2*L_FRAME + 1) + PIT_MAX + L_INTERPOL + NB_SUBFR*(M+1) \
                     + 3*(M+L_SUBFR) + M16k];
} PV_AmrWbDec;


#endif
