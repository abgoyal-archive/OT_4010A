
#ifndef H264_MEDIAINFO_PARSER_H
#define H264_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "h264_media_info.h"

SDP_ERROR_CODE h264MediaInfoParser(const char*, const int, SDPInfo*);

class SDPH264MediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPH264MediaInfoParser() {};
        virtual ~SDPH264MediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff,
                                      const int index,
                                      SDPInfo *sdp,
                                      payloadVector payload_vec,
                                      bool isSipSdp,
                                      int alt_id = 0,
                                      bool alt_def_id = false);
    private:
        SDP_ERROR_CODE parseParameterSets(const char* aParamSetBuf,
                                          int   aParamSetBufLen,
                                          h264_mediaInfo* aH264MediaInfo,
                                          uint32 aPayLoadNumber);
};

#endif //H264_MEDIAINFO_PARSER_H

