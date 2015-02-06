
///////////////////////////////////////////////////////////////////////////////
//
// m4v_audio_payload_parser_factory.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef M4V_AUDIO_PAYLOAD_PARSER_FACTORY_H_INCLUDED
#define M4V_AUDIO_PAYLOAD_PARSER_FACTORY_H_INCLUDED

#ifndef PAYLOAD_PARSER_FACTORY_H_INCLUDED
#include "payload_parser_factory.h"
#endif

class M4VAudioPayloadParserFactory : public IPayloadParserFactory
{
        OSCL_IMPORT_REF virtual IPayloadParser* createPayloadParser();
        OSCL_IMPORT_REF virtual void destroyPayloadParser(IPayloadParser* parser);
        virtual ~M4VAudioPayloadParserFactory()
        {
            ;
        }
};

#endif // M4V_AUDIO_PAYLOAD_PARSER_FACTORY_H_INCLUDED

