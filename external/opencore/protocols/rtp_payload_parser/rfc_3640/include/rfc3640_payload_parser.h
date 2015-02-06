
///////////////////////////////////////////////////////////////////////////////
//
// RFC3640_payload_parser.h
//
// Payload parser for RFC3640 RTP format (mpeg4 for DVB and others).
//
// This implementation is intentionally incomplete.  It does not implement
// interleaved access units (AUs) nor does it handle missing "crucial" AUs.
// Neither of which are common for DVB.
//
// This implementation currently only supports AAC high-bit-rate (AAChbr).
// Other modes can be added by implementing mediaInfo parsing in Init.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef RFC3640_PAYLOAD_PARSER_H_INCLUDED
#define RFC3640_PAYLOAD_PARSER_H_INCLUDED

#ifndef PAYLOAD_PARSER_H_INCLUDED
#include "payload_parser.h"
#endif

class RFC3640PayloadParser : public IPayloadParser
{
    public:
        OSCL_IMPORT_REF          RFC3640PayloadParser(void);
        OSCL_IMPORT_REF virtual ~RFC3640PayloadParser();

        //Initializes the parser.
        //This is where the parser gets the SDP MIME types
        //that configure the RFC3640 header format.
        OSCL_IMPORT_REF bool Init(mediaInfo* config);

        //This parses the input payload packets, skips over the AU headers,
        //and returns the new pointers as fragments in the output payload
        //fragment vector.
        OSCL_IMPORT_REF PayloadParserStatus Parse(const Payload& inputPacket,
                Oscl_Vector<Payload, OsclMemAllocator>& vParsedPayloads);

        //Not supported or implemented.
        OSCL_IMPORT_REF void Reposition(const bool   adjustSequence = false,
                                        const uint32 stream = 0,
                                        const uint32 seqnum = 0);

        //Not supported or implemented.
        OSCL_IMPORT_REF uint32 GetMinCurrTimestamp(void);

    private:
        //These correspond to the MIME types specified in RFC3640.
        bool   headersPresent;
        uint32 headersLength;
        uint32 sizeLength;
        uint32 indexLength;
        uint32 indexDeltaLength;
        uint32 CTSDeltaLength;
        uint32 DTSDeltaLength;
        bool   randomAccessIndication;
        uint32 auxDataSizeLength;
};

#endif //RFC3640_PAYLOAD_PARSER_H_INCLUDED
