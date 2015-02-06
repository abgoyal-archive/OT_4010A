
///////////////////////////////////////////////////////////////////////////////
//
// m4v_payload_parser_factory.cpp
//
// Implementation of MPEG-4 video payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#include "m4v_payload_parser_factory.h"
#include "m4v_payload_parser.h"

OSCL_EXPORT_REF IPayloadParser* M4VPayloadParserFactory::createPayloadParser()
{
    return OSCL_NEW(M4VPayloadParser, ());
}

OSCL_EXPORT_REF void M4VPayloadParserFactory::destroyPayloadParser(IPayloadParser* parser)
{
    OSCL_DELETE(parser);
}

