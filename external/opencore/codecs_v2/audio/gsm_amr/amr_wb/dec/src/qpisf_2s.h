

#ifndef QPISF_2S_H
#define QPISF_2S_H



#include "qisf_ns.h"




#define N_SURV  4

#define SIZE_BK1  256
#define SIZE_BK2  256
#define SIZE_BK21 64
#define SIZE_BK22 128
#define SIZE_BK23 128
#define SIZE_BK24 32
#define SIZE_BK25 32

#define SIZE_BK21_36b 128
#define SIZE_BK22_36b 128
#define SIZE_BK23_36b 64


extern const int16 mean_isf[ORDER];
extern const int16 dico1_isf[SIZE_BK1*9];
extern const int16 dico2_isf[SIZE_BK2*7];
extern const int16 dico21_isf[SIZE_BK21*3];
extern const int16 dico22_isf[SIZE_BK22*3];
extern const int16 dico23_isf[SIZE_BK23*3];
extern const int16 dico24_isf[SIZE_BK24*3];
extern const int16 dico25_isf[SIZE_BK25*4];
extern const int16 dico21_isf_36b[SIZE_BK21_36b*5];
extern const int16 dico22_isf_36b[SIZE_BK22_36b*4];
extern const int16 dico23_isf_36b[SIZE_BK23_36b*7];




#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif




#endif  /* QPISF_2S_H */
