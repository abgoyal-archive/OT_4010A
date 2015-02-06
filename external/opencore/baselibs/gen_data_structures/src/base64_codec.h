
#ifndef BASE64_CODEC_H_INCLUDED
#define BASE64_CODEC_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

#ifndef OSCL_UTF8CONV_H
#include "oscl_utf8conv.h"
#endif

#ifndef OSCL_ERROR_CODES_H_INCLUDED
#include "oscl_error_codes.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H_INCLUDED
#include "oscl_mem_basic_functions.h"
#endif

class PVBase64Codec
{
    public:
        OSCL_IMPORT_REF PVBase64Codec();

        OSCL_IMPORT_REF bool Encode(uint8* aInBuf, uint32 aInBufLen,
                                    uint8* aOutBuf, uint32& aOutBufLen, uint32 aMaxOutBufLen);
        bool Decode(uint8* aInBuf, uint32 aInBufLen,
                    uint8* aOutBuf, uint32& aOutBufLen, uint32 aMaxOutBufLen);
    private:
        uint8 etable[64], dtable[256];

};

#endif

