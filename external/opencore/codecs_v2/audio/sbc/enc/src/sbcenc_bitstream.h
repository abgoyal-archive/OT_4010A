
#ifndef     __BITSTREAM__
#define     __BITSTREAM__

#ifdef __cplusplus
extern "C"
{
#endif


#include "oscl_types.h"
    Int pack_bitstream(UWord8 *data, enc_state_t *state, UInt len);

#ifdef __cplusplus
}
#endif


#endif /* __BITSTREAM__ */
