

#ifndef Q_PULSE_H
#define Q_PULSE_H



#include "pv_amr_wb_type_defs.h"


#ifdef __cplusplus
extern "C"
{
#endif


    void dec_1p_N1(int32 index, int16 N, int16 offset, int16 pos[]);
    void dec_2p_2N1(int32 index, int16 N, int16 offset, int16 pos[]);
    void dec_3p_3N1(int32 index, int16 N, int16 offset, int16 pos[]);
    void dec_4p_4N1(int32 index, int16 N, int16 offset, int16 pos[]);
    void dec_4p_4N(int32 index, int16 N, int16 offset, int16 pos[]);
    void dec_5p_5N(int32 index, int16 N, int16 offset, int16 pos[]);
    void dec_6p_6N_2(int32 index, int16 N, int16 offset, int16 pos[]);


#ifdef __cplusplus
}
#endif

#endif  /* Q_PULSE_H */
