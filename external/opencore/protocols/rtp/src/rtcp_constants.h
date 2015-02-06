
/*                                                                               */
/*********************************************************************************/


#ifndef RTCP_CONSTANTS_H
#define RTCP_CONSTANTS_H


const uint8 SR_PACKET_TYPE = 200;
const uint8 RR_PACKET_TYPE = 201;
const uint8 SDES_PACKET_TYPE = 202;
const uint8 BYE_PACKET_TYPE = 203;
const uint8 APP_PACKET_TYPE = 204;
const uint8 RECORD_COUNT_MASK = 0x1F;
const int16 FRACTION_LOST_POSITION = 24;
const uint32 FRACTION_LOST_MASK = 0x00FFFFFF;
const uint8 DEFAULT_RTCP_VERSION = 2;
const uint8 RTCP_PAD_BIT_MASK = 0x20;


// size constants
const int32 RTCP_HEADER_SIZE = 4;
const int32 RTCP_REPORT_BLOCK_SIZE = 24;
const int32 RTCP_RR_SENDER_INFO_SIZE = 4;
const int32 RTCP_SR_SENDER_INFO_SIZE = 24;

const int32 RTCP_APP_HEADER_INFO_SIZE = 8;
const int32 RTCP_PVSS_APP_COMMON_SIZE = 24;
const int32 RTCP_PVSS_APP_DRC_SIZE = 20;
const int32 RTCP_PVSS_APP_BUFLOW_SIZE = 4;
const int32 RTCP_PVSS_APP_BUFHIGH_SIZE = 4;


#endif
