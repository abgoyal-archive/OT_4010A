
///////////////////////////////////////////////////////////////////////////////
//
// m4v_audio_payload_parser.h
//
// Payload parser for mp4 audio format.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef M4V_AUDIO_PAYLOAD_PARSER_H_INCLUDED
#define M4V_AUDIO_PAYLOAD_PARSER_H_INCLUDED

#include "payload_parser.h"

class M4VAudioPayloadParser : public IPayloadParser
{
    public:
        OSCL_IMPORT_REF M4VAudioPayloadParser();
        OSCL_IMPORT_REF virtual ~M4VAudioPayloadParser();

        OSCL_IMPORT_REF bool Init(mediaInfo* config);
        OSCL_IMPORT_REF PayloadParserStatus Parse(const Payload& inputPacket,
                Oscl_Vector<Payload, OsclMemAllocator>& vParsedPayloads);

        OSCL_IMPORT_REF void Reposition(const bool adjustSequence = false, const uint32 stream = 0, const uint32 seqnum = 0);
        OSCL_IMPORT_REF uint32 GetMinCurrTimestamp();
};

#endif //M4V_AUDIO_PAYLOAD_PARSER_H
