
#ifndef PAYLOADPARSER_FACTORY_BASE_H_INCLUDED
#define PAYLOADPARSER_FACTORY_BASE_H_INCLUDED

#include "oscl_base_alloc.h"

class IPayloadParser;

class IPayloadParserFactory
{
    public:
        OSCL_IMPORT_REF virtual IPayloadParser* createPayloadParser() = 0;
        OSCL_IMPORT_REF virtual void destroyPayloadParser(IPayloadParser* parser) = 0;
        virtual ~IPayloadParserFactory()
        {
            ;
        }
};

#endif // PAYLOADPARSER_FACTORY_BASE_H_INCLUDED
