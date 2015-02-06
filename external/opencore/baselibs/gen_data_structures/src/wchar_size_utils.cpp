

#ifndef WCHAR_SIZE_UTILS_H_INCLUDED
#include "wchar_size_utils.h"
#endif

OSCL_EXPORT_REF void PackWChar4BytesTo2Bytes(uint8* dest, oscl_wchar* src, int nChars)
{
    int count;
    uint8* destPtr = dest;
    oscl_wchar* srcPtr = src;
    for (count = 0; count < (nChars - 1); count++)
    {
        oscl_memcpy(destPtr, srcPtr, 2);
        destPtr += 2;
        srcPtr++;
    }
    *destPtr = 0;
    *(++destPtr) = 0;
}

OSCL_EXPORT_REF void ExpandWChar2BytesTo4Bytes(oscl_wchar * dest, uint16* src, int nChars)
{
    int count;
    for (count = 0; count < (nChars - 1); count++)
    {
        dest[count] = src[count];
    }
    dest[count] = 0;
}
