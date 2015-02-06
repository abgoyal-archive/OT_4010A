

#ifndef H263_MEDIAINFO_PARSER_H
#define H263_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "h263_media_info.h"

SDP_ERROR_CODE h263MediaInfoParser(const char *, const int , SDPInfo *);

class SDPH263MediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPH263MediaInfoParser() {};
        virtual ~SDPH263MediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
};

#endif //H263_MEDIAINFO_PARSER_H

