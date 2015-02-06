
///////////////////////////////////////////////////////////////////////////////
//
// h263_payload_parser_factory.h
//
// H.263 payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef H263_PAYLOAD_PARSER_FACTORY_H_INCLUDED
#define H263_PAYLOAD_PARSER_FACTORY_H_INCLUDED

#ifndef PAYLOAD_PARSER_FACTORY_H_INCLUDED
#include "payload_parser_factory.h"
#endif

class H263PayloadParserFactory : public IPayloadParserFactory
{
    public:
        OSCL_IMPORT_REF virtual IPayloadParser* createPayloadParser();
        OSCL_IMPORT_REF virtual void destroyPayloadParser(IPayloadParser* parser);
};


#endif // H263_PAYLOAD_PARSER_FACTORY_H_INCLUDED
