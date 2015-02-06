 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_defs.h
 *
 * Project:
 * --------
 *   YUSU
 *
 * Description:
 * ------------
 *   Implement RMFF parser node
 *
 * Author:
 * -------
 *   Morris Yang (mtk03147)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime $
 * $Log     $  
 *
 * 01 13 2011 donglei.ji
 * [ALPS00139474] [Need Patch] [Volunteer Patch] Android2.3 opencore migration
 * Android2.3 opencore migration.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef PVMF_RMFFPARSER_DEFS_H_INCLUDED
#define PVMF_RMFFPARSER_DEFS_H_INCLUDED

///////////////////////////////////////////////
// Error Events
///////////////////////////////////////////////
//None--node uses the PVMF event codes.

///////////////////////////////////////////////
// Information Events
///////////////////////////////////////////////
//None--node uses the PVMF event codes.

///////////////////////////////////////////////
// Port type tags
///////////////////////////////////////////////
typedef enum
{
    PVMF_RMFFPARSERNODE_PORT_TYPE_OUTPUT = 1
} PVMFRMFFParserNodePortType;


// Capability mime strings
#define PVMF_RMFFPARSER_PORT_INPUT_FORMATS "x-pvmf/parser/mtkff/input_formats"
#define PVMF_RMFFPARSER_PORT_INPUT_FORMATS_VALTYPE "x-pvmf/port/formattype;valtype=char*"

#define PVMF_RMFFPARSER_NODE_PSEUDO_STREAMING_BUFFER_DURATION_IN_MS 4000

#define PVMF_RMFFPARSER_PARSER_NODE_TS_DELTA_DURING_REPOS_IN_MS 10 // 10ms

#define DEFAULT_CAHCE_SIZE 8*1024
#define DEFAULT_ASYNC_READ_BUFFER_SIZE 8*1024
#define DEFAULT_NATIVE_ACCESS_MODE 0
#define MIN_CACHE_SIZE 1024
#define MAX_CACHE_SIZE 64*1024
#define MIN_ASYNC_READ_BUFFER_SIZE 1024
#define MAX_ASYNC_READ_BUFFER_SIZE 64*1024
#define DEFAULT_FILE_HANDLE NULL

#define MIN_JITTER_BUFFER_DURATION_IN_MS       1000
#define MAX_JITTER_BUFFER_DURATION_IN_MS       10000

// Format Type integer defines
#define PVMF_RMFFPARSER_NODE_FORMAT_UNKNOWN     0
#define PVMF_RMFFPARSER_NODE_AMR                1
#define PVMF_RMFFPARSER_NODE_AMRWB              2
#define PVMF_RMFFPARSER_NODE_AMR_IETF           3
#define PVMF_RMFFPARSER_NODE_AMRWB_IETF         4
#define PVMF_RMFFPARSER_NODE_AMR_IF2            5
#define PVMF_RMFFPARSER_NODE_ADIF               6
#define PVMF_RMFFPARSER_NODE_ADTS               7
#define PVMF_RMFFPARSER_NODE_MPEG4_AUDIO        8
#define PVMF_RMFFPARSER_NODE_M4V                9
#define PVMF_RMFFPARSER_NODE_H2631998           10
#define PVMF_RMFFPARSER_NODE_H2632000           11
#define PVMF_RMFFPARSER_NODE_H264_RAW           12
#define PVMF_RMFFPARSER_NODE_H264_MP4           13
#define PVMF_RMFFPARSER_NODE_H264               14
#define PVMF_RMFFPARSER_NODE_3GPP_TIMED_TEXT    15

#endif // PVMF_RMFFPARSER_DEFS_H_INCLUDED

