
///////////////////////////////////////////////////////////////////////////////
//
// m4v_audio_payload_parser_factory.cpp
//
// Implementation of MP4 audio payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#include "m4v_audio_payload_parser_factory.h"
#include "m4v_audio_payload_parser.h"

OSCL_EXPORT_REF IPayloadParser* M4VAudioPayloadParserFactory::createPayloadParser()
{
    return OSCL_NEW(M4VAudioPayloadParser, ());
}

OSCL_EXPORT_REF void M4VAudioPayloadParserFactory::destroyPayloadParser(IPayloadParser* parser)
{
    OSCL_DELETE(parser);
}

