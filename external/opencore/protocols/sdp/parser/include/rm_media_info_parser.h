
#ifndef RM_MEDIAINFO_PARSER_H
#define RM_MEDIAINFO_PARSER_H
#include "base_media_info_parser.h"
#include "m4v_media_info.h"

SDP_ERROR_CODE rmMediaInfoParser(const char *, const int , SDPInfo *);
class SDPMediaParserFactory;
class SDPRMMediaInfoParser : public SDPBaseMediaInfoParser
{
    public:
        SDPRMMediaInfoParser() {};
        virtual ~SDPRMMediaInfoParser() {};
        SDP_ERROR_CODE parseMediaInfo(const char *buff, const int index, SDPInfo *sdp,
                                      payloadVector payload_vec, bool isSipSdp, int alt_id = 0, bool alt_def_id = false);
        SDP_ERROR_CODE parseRMLength(const char* sptr1, const char *end_ptr_str, NptTimeFormat& length);
        const char *   parse_range_integer(const char *start, const char *end,
                                           int max_digits, char *sep, uint32& value);
};

#endif //RM_MEDIAINFO_PARSER_H
