

#ifndef AAC_MEDIAINFO_PARSER_H
#define AAC_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "aac_media_info.h"

class SDPAACMediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPAACMediaInfoParser() {};
        virtual ~SDPAACMediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
};

#endif //AAC_MEDIAINFO_PARSER_H
