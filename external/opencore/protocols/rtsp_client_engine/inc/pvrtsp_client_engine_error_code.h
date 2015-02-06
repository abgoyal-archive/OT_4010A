
#ifndef PVRTSP_CLIENT_ENGINE_ERROR_CODE_H
#define PVRTSP_CLIENT_ENGINE_ERROR_CODE_H
//#include "pvrtsp_client_engine_error_code.h"

///////////////////////////////////////////////
// Error Events
///////////////////////////////////////////////

enum TPVMFRTSPNodeErrors
{

    PVMF_RTSPNODE_ERROR_OUT_OF_MEM = 3000000,

    PVMF_RTSPNODE_ERROR_URL,
    PVMF_RTSPNODE_ERROR_RTSP_REQUEST_TOO_BIG,
    PVMF_RTSPNODE_ERROR_RTSP_PARSER,
    PVMF_RTSPNODE_ERROR_SOCKET_SERVER,
    PVMF_RTSPNODE_ERROR_DNS_LOOKUP,

    PVMF_RTSPNODE_ERROR_RTSP_CONNECT,
    PVMF_RTSPNODE_ERROR_RTSP_COMPOSE,
    PVMF_RTSPNODE_ERROR_RTSP_PARSE,
    PVMF_RTSPNODE_ERROR_RTSP_SERVER,
    PVMF_RTSPNODE_ERROR_SEND,
    PVMF_RTSPNODE_ERROR_RECV,

    PVMF_RTSPNODE_ERROR_SERVER_REJECT,//for all non 2xx and 3xx
    PVMF_RTSPNODE_ERROR_SERVER  //for faulty servers, like wrong seq number etc
};

///////////////////////////////////////////////
// Information Events
///////////////////////////////////////////////

enum TPVMFRTSPNodeInfo
{
    PVMF_RTSPNODE_INFO_REDIRECT,
    PVMF_RTSPNODE_INFO_EOS
};

#endif //PVRTSP_CLIENT_ENGINE_ERROR_CODE_H


