
#ifndef BIT_UTIL_H
#define BIT_UTIL_H

#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

class BitUtil
{
    public:
        static uint32 bitValue(octet* buff, uint32 offset, uint32 len);
        static bool bitCopy(octet* src, uint32 src_offset, uint32 bitLen, octet** output, uint32& byteLenOut);

};



#endif //BIT_UTIL_H


