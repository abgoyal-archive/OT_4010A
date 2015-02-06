
///////////////////////////////////////////////////////////////////////////////
//
// RFC3640_payload_parser_factory.cpp
//
// Implementation of RFC3640 RTP payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#include "rfc3640_payload_parser_factory.h"
#include "rfc3640_payload_parser.h"

OSCL_EXPORT_REF IPayloadParser* RFC3640PayloadParserFactory::createPayloadParser()
{
    return OSCL_NEW(RFC3640PayloadParser, ());
}

OSCL_EXPORT_REF void RFC3640PayloadParserFactory::destroyPayloadParser(IPayloadParser* parser)
{
    OSCL_DELETE(parser);
    parser = NULL;
}

