
///////////////////////////////////////////////////////////////////////////////
//
// h263_payload_parser_factory.cpp
//
// Implementation of H.263 payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#include "h263_payload_parser_factory.h"
#include "h263_payload_parser.h"

OSCL_EXPORT_REF IPayloadParser* H263PayloadParserFactory::createPayloadParser()
{
    return OSCL_NEW(H263PayloadParser, ());
}

OSCL_EXPORT_REF void H263PayloadParserFactory::destroyPayloadParser(IPayloadParser* parser)
{
    OSCL_DELETE(parser);
}

