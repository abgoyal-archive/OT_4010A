

#ifndef PCMA_MEDIAINFO_PARSER_H
#define PCMA_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "pcma_media_info.h"

SDP_ERROR_CODE pcmaMediaInfoParser(const char *buff, const int index, SDPInfo *sdp);

class SDPPCMAMediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPPCMAMediaInfoParser() {};
        virtual ~SDPPCMAMediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
};

#endif //PCMA_MEDIAINFO_PARSER_H
