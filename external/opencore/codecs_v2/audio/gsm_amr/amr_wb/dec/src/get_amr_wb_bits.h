

#ifndef GET_AMR_WB_BITS_H
#define GET_AMR_WB_BITS_H


#include "pv_amr_wb_type_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif


    int16 Serial_parm(                        /* Return the parameter    */
        int16 no_of_bits,                    /* input : number of bits  */
        int16 ** prms
    );

    int16 Serial_parm_1bit(                        /* Return the parameter    */
        int16 ** prms
    );

#ifdef __cplusplus
}
#endif

#endif
