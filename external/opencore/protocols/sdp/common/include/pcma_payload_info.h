

#ifndef PCMA_PAYLOAD_INFO_H
#define PCMA_PAYLOAD_INFO_H

#include "payload_info.h"

class PcmaPayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        PcmaPayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;
        };
};


#endif
