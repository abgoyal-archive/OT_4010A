
///////////////////////////////////////////////////////////////////////////////
//
// h264_payload_parser_factory.cpp
//
// Implementation of H.264 payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#include "h264_payload_parser_factory.h"
#include "h264_payload_parser.h"

OSCL_EXPORT_REF IPayloadParser* H264PayloadParserFactory::createPayloadParser()
{
    return OSCL_NEW(H264PayloadParser, ());
}

OSCL_EXPORT_REF void H264PayloadParserFactory::destroyPayloadParser(IPayloadParser* parser)
{
    OSCL_DELETE(parser);
}

