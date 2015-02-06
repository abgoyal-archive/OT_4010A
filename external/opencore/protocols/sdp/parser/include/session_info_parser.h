

#ifndef SESSIONINFO_PARSER_H
#define SESSIONINFO_PARSER_H
//#include "sdp_return_code.h"
#include "sdp_error.h"
#include "sdp_memory.h"
#include "session_info.h"
#include "sdp_info.h"
#include "sdp_parsing_utils.h"

SDP_ERROR_CODE parseSDPSessionInfo(const char *sdp_text, int length, SDPInfo *sdp);

#endif //SESSIONINFO_PARSER_H
