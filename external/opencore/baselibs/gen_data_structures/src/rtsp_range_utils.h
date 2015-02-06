


#ifndef RTSP_RANGE_UTILS_H
#define RTSP_RANGE_UTILS_H


#include "rtsp_time_formats.h"



OSCL_IMPORT_REF bool parseRtspRange(const char *rangeString, int length,
                                    RtspRangeType& range);


OSCL_IMPORT_REF int estimate_SDP_string_len(const RtspRangeType& range);
OSCL_IMPORT_REF int estimate_SDP_string_len(const NptTimeFormat& npt_range);

OSCL_IMPORT_REF bool compose_SDP_string(char *str, unsigned int max_len,
                                        const RtspRangeType& range,
                                        int& len_used);


OSCL_IMPORT_REF bool compose_RTSP_string(char *str, unsigned int max_len,
        const RtspRangeType& range,
        int& len_used);



#endif
