


#ifndef RM_PAYLOAD_INFO_H
#define RM_PAYLOAD_INFO_H

#include "payload_info.h"

class RmPayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        RmPayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;
        };
};

#endif
