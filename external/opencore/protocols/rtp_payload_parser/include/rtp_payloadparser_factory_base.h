
#ifndef RTP_PAYLOADPARSER_FACTORY_BASE_H
#define RTP_PAYLOADPARSER_FACTORY_BASE_H

#include "oscl_base_alloc.h"

class RTPPayloadParser;

class RTPPayloadParserFactory
{
    public:
        OSCL_IMPORT_REF virtual RTPPayloadParser* createRTPPayloadParserInstance() = 0;
        OSCL_IMPORT_REF virtual void destroyRTPPayloadParserInstance(RTPPayloadParser* parser) = 0;
};

#endif // RTP_PAYLOADPARSER_FACTORY_BASE_H

