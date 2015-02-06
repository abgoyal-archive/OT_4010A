
/*                                                                               */
/*********************************************************************************/


#ifndef RTPRTCP_H
#define RTPRTCP_H


#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif

typedef uint32 RtpSsrc;
typedef uint32 RtpTimeStamp;
typedef uint16 RtpPayloadType;
typedef uint16 RtpSeqType;


const int16 NUM_BYTES_IN_UINT_32 = 4;
const uint8 DEFAULT_RTPRTCP_VERSION = 2;
const int16 RTPRTCP_VERSION_BIT_POSITION = 6;
const int16 RTPRTCP_PAD_FLAG_BIT_POSITION = 5;
const uint16 MAX_STR_COUNT = 256;

#endif
