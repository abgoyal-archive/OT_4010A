
#ifndef PVMF_AMRFFPARSER_DEFS_H_INCLUDED
#define PVMF_AMRFFPARSER_DEFS_H_INCLUDED

///////////////////////////////////////////////
// Port tags
///////////////////////////////////////////////

typedef enum
{
    PVMF_AMRFFPARSER_NODE_PORT_TYPE_SOURCE
} PVMFAMRFFParserOutPortType;

// Capability mime strings
#define PVMF_AMRFFPARSER_PORT_OUTPUT_FORMATS "x-pvmf/port/formattype"
#define PVMF_AMRFFPARSER_PORT_OUTPUT_FORMATS_VALTYPE "x-pvmf/port/formattype;valtype=int32"

#define AMR_MIN_DATA_SIZE_FOR_RECOGNITION  512
#define AMR_DATA_OBJECT_PARSING_OVERHEAD   512
#define AMR_HEADER_SIZE 32

#define PVMF_AMR_PARSER_NODE_MAX_AUDIO_DATA_MEM_POOL_SIZE   1024*1024
#define PVMF_AMR_PARSER_NODE_DATA_MEM_POOL_GROWTH_LIMIT     1
#define PVMF_AMR_PARSER_NODE_MAX_NUM_OUTSTANDING_MEDIA_MSGS 4
#define PVMF_AMR_PARSER_NODE_MEDIA_MSG_SIZE                 128
#define PVMF_AMR_PARSER_NODE_TS_DELTA_DURING_REPOS_IN_MS    10

#endif

