
///////////////////////////////////////////////////////////////////////////////
//
// h263_payload_parser.h
//
// H.263 payload parser.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef H263_PAYLOAD_PARSER_H_INCLUDED
#define H263_PAYLOAD_PARSER_H_INCLUDED


#include "payload_parser.h"

///////////////////////////////////////////////////////////////////////////////
//
// H.263 PayloadParser Class
//
///////////////////////////////////////////////////////////////////////////////

class H263PayloadParser : public IPayloadParser
{
    public:
        OSCL_IMPORT_REF H263PayloadParser();
        OSCL_IMPORT_REF virtual ~H263PayloadParser();

        OSCL_IMPORT_REF virtual bool Init(mediaInfo* config);
        OSCL_IMPORT_REF virtual PayloadParserStatus Parse(const Payload& inputPacket,
                Oscl_Vector<Payload, OsclMemAllocator>& vParsedPayloads);

        OSCL_IMPORT_REF virtual void Reposition(const bool adjustSequence = false, const uint32 stream = 0, const uint32 seqnum = 0);
        OSCL_IMPORT_REF virtual uint32 GetMinCurrTimestamp();

    private:
};

#endif //H263_PAYLOAD_PARSER_H_INCLUDED
