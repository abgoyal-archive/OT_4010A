
#ifndef __SBC_ENCODER__
#define __SBC_ENCODER__

#include "pvsbcencoderinterface.h"
#ifdef __cplusplus
extern "C"
{
#endif

    bool        encoder_init(TPvSbcEncConfig *);
    bool        encoder_execute(TPvSbcEncConfig *, unsigned short *);
    bool        encoder_delete(TPvSbcEncConfig *);

#ifdef __cplusplus
}
#endif

#endif /* __SBC_ENCODER__ */
