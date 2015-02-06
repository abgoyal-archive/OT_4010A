

#ifndef E_INFOINIT_CONST_H
#define E_INFOINIT_CONST_H

#include "chans.h"





typedef enum
{
    /* block switch windows for single channels or channel pairs */
    Winds   = Chans,

    /* average channel block length, bytes */
    Avjframe    = 341,

    TEXP    = 128,      /* size of exp cache table */
    MAX_IQ_TBL  = 128,      /* size of inv quant table */
    MAXFFT  = LN4

} infoinitConst;




#endif
