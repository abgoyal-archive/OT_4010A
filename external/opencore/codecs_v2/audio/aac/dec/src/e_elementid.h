

#ifndef E_ELEMENTID_H
#define E_ELEMENTID_H






typedef enum
{
    /* sfb 40, coef 672, pred bw of 15.75 kHz at 48 kHz
     * this is also the highest number of bins used
     * by predictor for any sampling rate
     */
    MAX_PRED_SFB    = 40,   /* 48 kHz only, now obsolete */
    MAX_PRED_BINS   = 672,

    ID_SCE      = 0,
    ID_CPE,
    ID_CCE,
    ID_LFE,
    ID_DSE,
    ID_PCE,
    ID_FIL,
    ID_END
}
ElementId;



#endif
