

#ifndef QISF_NS_H
#define QISF_NS_H



#include "pv_amr_wb_type_defs.h"



#define ORDER   16            /* order of linear prediction filter */
#define ISF_GAP 128

#define SIZE_BK_NOISE1  64
#define SIZE_BK_NOISE2  64
#define SIZE_BK_NOISE3  64
#define SIZE_BK_NOISE4  32
#define SIZE_BK_NOISE5  32

#define NB_QUA_GAIN6B  64     /* Number of quantization level */
#define NB_QUA_GAIN7B  128    /* Number of quantization level */


extern const int16 mean_isf_noise[ORDER];
extern const int16 dico1_isf_noise[SIZE_BK_NOISE1*2];
extern const int16 dico2_isf_noise[SIZE_BK_NOISE2*3];
extern const int16 dico3_isf_noise[SIZE_BK_NOISE3*3];
extern const int16 dico4_isf_noise[SIZE_BK_NOISE4*4];
extern const int16 dico5_isf_noise[SIZE_BK_NOISE5*4];

extern const int16 t_qua_gain6b[NB_QUA_GAIN6B*2];
extern const int16 t_qua_gain7b[NB_QUA_GAIN7B*2];





#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif




#endif  /* QISF_NS_H */
