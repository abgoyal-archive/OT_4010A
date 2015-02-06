
///////////////////////////////////////////////////////////////////////////////
//
// h264_payload_parser_factory.h
//
// H.264 payload parser factory.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef H264_PAYLOAD_PARSER_FACTORY_H_INCLUDED
#define H264_PAYLOAD_PARSER_FACTORY_H_INCLUDED

#ifndef PAYLOAD_PARSER_FACTORY_H_INCLUDED
#include "payload_parser_factory.h"
#endif

class H264PayloadParserFactory : public IPayloadParserFactory
{
    public:
        OSCL_IMPORT_REF virtual IPayloadParser* createPayloadParser();
        OSCL_IMPORT_REF virtual void destroyPayloadParser(IPayloadParser* parser);
};


#endif // H264_PAYLOAD_PARSER_FACTORY_H_INCLUDED
