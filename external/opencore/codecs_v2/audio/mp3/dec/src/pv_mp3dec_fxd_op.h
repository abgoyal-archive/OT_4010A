

#ifndef PV_MP3DEC_FXD_OP_H
#define PV_MP3DEC_FXD_OP_H

#include "pvmp3_audio_type_defs.h"


#ifdef __cplusplus
extern "C"
{
#endif


#if (defined(PV_ARM_V5)||defined(PV_ARM_V4))

#include "pv_mp3dec_fxd_op_arm.h"

#elif (defined(PV_ARM_GCC_V5)||defined(PV_ARM_GCC_V4))

#include "pv_mp3dec_fxd_op_arm_gcc.h"

#elif (defined(PV_ARM_MSC_EVC_V5)||defined(PV_ARM_MSC_EVC_V4))

#include "pv_mp3dec_fxd_op_msc_evc.h"

#else

#ifndef C_EQUIVALENT
#define C_EQUIVALENT
#endif

#include "pv_mp3dec_fxd_op_c_equivalent.h"

#endif


#ifdef __cplusplus
}
#endif



#endif  /* PV_MP3DEC_FXD_OP_H */
