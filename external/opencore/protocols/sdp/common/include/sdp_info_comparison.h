
#ifndef __SDP_INFO_COMPARISON_H__
#define __SDP_INFO_COMPARISON_H__

#include "oscl_mem.h"
#include "session_info.h"
#include "media_info.h"
#include "bool_array.h"
#include "mime_registry.h"


//======================================================================
//  CLASS DEFINITIONS and FUNCTION DECLARATIONS
//======================================================================

class SDPInfoComparison
{
    public:
        // Fuction call to compare the 2 sdpInfo passed and put the common results into the
        // first sdpInfo
        // sdp1 - first Input sdp, this is base sdpinfo, meaning local capabiity sdpinfo
        //          the port is used for sdpOut
        // sdp2 - second Input sdp
        // result - int - 0 for success -ve for unsuccessful
        OSCL_IMPORT_REF static int
        compareSDPInfo(SDPInfo* sdp1, SDPInfo* sdp2);
};
#endif // __SDP_INFO_COMPARISON_H__

