
///////////////////////////////////////////////////////////////////////////////
//
// amr_payload_parser_factory.cpp
//
// Implementation of AMR payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#include "amr_payload_parser_factory.h"
#include "amr_payload_parser.h"

OSCL_EXPORT_REF IPayloadParser* AmrPayloadParserFactory::createPayloadParser()
{
    return OSCL_NEW(AMRPayloadParser, ());
}

OSCL_EXPORT_REF void AmrPayloadParserFactory::destroyPayloadParser(IPayloadParser* parser)
{
    OSCL_DELETE(parser);
}

