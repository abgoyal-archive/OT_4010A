

#ifndef _h223_H
#define _h223_H

#include "oscl_types.h"
#include "pvt_common.h"

#ifdef __cplusplus
extern "C"
{
#endif
    uint H223GetMuxStuffingSz(TPVH223Level level);
    uint H223GetMaxStuffingSz(TPVH223Level level);
    uint H223MuxStuffing(TPVH223Level level, uint8* pPdu, uint bufSz);
    bool H223CheckSync(TPVH223Level level, uint8* buf, uint bufSz, uint tolerance);
#ifdef __cplusplus
}
#endif
#endif  /* _h223_H */
