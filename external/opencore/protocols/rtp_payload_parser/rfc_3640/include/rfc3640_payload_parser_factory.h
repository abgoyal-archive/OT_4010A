
///////////////////////////////////////////////////////////////////////////////
//
// RFC3640_payload_parser_factory.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef RFC3640_PAYLOAD_PARSER_FACTORY_H_INCLUDED
#define RFC3640_PAYLOAD_PARSER_FACTORY_H_INCLUDED

#ifndef PAYLOAD_PARSER_FACTORY_H_INCLUDED
#include "payload_parser_factory.h"
#endif

class RFC3640PayloadParserFactory : public IPayloadParserFactory
{
        OSCL_IMPORT_REF virtual IPayloadParser*  createPayloadParser(void);
        OSCL_IMPORT_REF virtual void            destroyPayloadParser(IPayloadParser* parser);
};

#endif // RFC3640_PAYLOAD_PARSER_FACTORY_H_INCLUDED
