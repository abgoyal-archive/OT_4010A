
///////////////////////////////////////////////////////////////////////////////
//
// amr_payload_parser_factory.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AMR_PAYLOAD_PARSER_FACTORY_H_INCLUDED
#define AMR_PAYLOAD_PARSER_FACTORY_H_INCLUDED

#ifndef PAYLOAD_PARSER_FACTORY_H_INCLUDED
#include "payload_parser_factory.h"
#endif

class AmrPayloadParserFactory : public IPayloadParserFactory
{
        OSCL_IMPORT_REF virtual IPayloadParser* createPayloadParser();
        OSCL_IMPORT_REF virtual void destroyPayloadParser(IPayloadParser* parser);
        virtual ~AmrPayloadParserFactory()
        {
            ;
        }
};

#endif //AMR_PAYLOAD_PARSER_FACTORY_H_INCLUDED
