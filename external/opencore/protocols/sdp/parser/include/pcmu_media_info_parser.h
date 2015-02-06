


#ifndef PCMU_MEDIAINFO_PARSER_H
#define PCMU_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "pcmu_media_info.h"

SDP_ERROR_CODE pcmuMediaInfoParser(const char *buff, const int index, SDPInfo *sdp);

class SDPPCMUMediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPPCMUMediaInfoParser() {};
        virtual ~SDPPCMUMediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp, payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
};

#endif //PCMU_MEDIAINFO_PARSER_H
