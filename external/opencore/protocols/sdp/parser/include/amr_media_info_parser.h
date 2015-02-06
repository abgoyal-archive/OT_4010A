

#ifndef AMR_MEDIAINFO_PARSER_H
#define AMR_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "amr_media_info.h"

SDP_ERROR_CODE amrMediaInfoParser(const char *buff, const int index, SDPInfo *sdp);

class SDPAMRMediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPAMRMediaInfoParser() {};
        virtual ~SDPAMRMediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
};

#endif //AMR_MEDIAINFO_PARSER_H
