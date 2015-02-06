

#ifndef M4V_MEDIAINFO_PARSER_H
#define M4V_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "m4v_media_info.h"

SDP_ERROR_CODE m4vMediaInfoParser(const char *, const int , SDPInfo *);

class SDPMPEG4MediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPMPEG4MediaInfoParser() {};
        virtual ~SDPMPEG4MediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
};

#endif //M4V_MEDIAINFO_PARSER_H
