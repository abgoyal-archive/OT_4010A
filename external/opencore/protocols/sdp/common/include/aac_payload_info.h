

#ifndef AAC_PAYLOAD_INFO_H
#define AAC_PAYLOAD_INFO_H

#include "payload_info.h"

#define PVMF_SDP_DEFAULT_LATM_SAMPLE_RATE 90000

class AacPayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        AacPayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;

            aac_cpresent = false;
            aacplusSBRenabled = false;
            configSize = 0;
        };

        inline void setcpresent(bool arg)
        {
            aac_cpresent = arg;
        };

        inline void setAACplusSBRenabled(bool arg)
        {
            aacplusSBRenabled = arg;
        };

    private:
        bool    aac_cpresent;
        bool    aacplusSBRenabled;
        int     configSize;
};

#endif
