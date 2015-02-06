

#ifndef STILL_IMAGE_MEDIAINFO_PARSER_H
#define STILL_IMAGE_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "still_image_media_info.h"

SDP_ERROR_CODE still_imageMediaInfoParser(const char *, const int , SDPInfo *);

class SDPStillImageMediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPStillImageMediaInfoParser() {};
        virtual ~SDPStillImageMediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id, bool alt_def_id);
};

#endif //STILL_IMAGE_MEDIAINFO_PARSER_H
