

#ifndef PCMU_PAYLOAD_INFO_H
#define PCMU_PAYLOAD_INFO_H

#include "payload_info.h"

class PcmuPayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        PcmuPayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;
        };
};


#endif
