 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_node_tunables.h
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
#ifndef PVMF_RMFFPARSERNODE_TUNEABLES_H_INCLUDED
#define PVMF_RMFFPARSERNODE_TUNEABLES_H_INCLUDED

// Track data size and buffer depth values
#define MPEG4_AUDIO_MAXTRACKDATASIZE    8192
#define MPEG4_AUDIO_MAXTRACKQUEUEDEPTH  2

#define RM_AUDIO_MAXTRACKDATASIZE    8192
#define RM_AUDIO_MAXTRACKQUEUEDEPTH  2

#define PVRMFF_SEEK_BOUNDARY_PROTECTION_IN_MS 2

// PVMF media data memory pool settings
#define PVMF_RMFFPARSER_MEDIADATA_POOLNUM 8
#define PVMF_RMFFPARSER_MEDIADATA_CHUNKSIZE 128



// Number of samples to retrieve per media data
#define MPEG4_AUDIO_NUMSAMPLES  2  // modified by Ryan for performance tuning
#define UNKNOWN_NUMSAMPLES      1

//max number of attempts at growing the resizable mem pool
#define PVMF_RMFFPARSER_NODE_MEM_POOL_GROWTH_LIMIT 2

//optimized parsing mode
#define PVMF_RMFFPARSER_NODE_ENABLE_PARSER_OPTIMIZATION 1
#define PVMF_RMFFPARSER_NODE_ENABLE_PARSING_MOOF_UPFRONT 0

// EOS duration in seconds to prevent EOS timestamp overlapping with beginning of next playback
#define PVMF_RMFFPARSER_DEFAULT_EOS_DURATION_IN_SEC 1

#define PVMF_RMFFPARSER_UNDERFLOW_THRESHOLD_IN_MS   3000
#define PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_FREQUENCY 10 //every 100 ms
#define PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_CYCLES 1

#define PVMFFF_DEFAULT_THUMB_NAIL_SAMPLE_NUMBER 2

#define PVMF_RMFFPARSER_OMA2_DECRYPTION_BUFFER_SIZE 20*1024

// Number of samples to be retrieved for Best Thumbnail Mode
#define NUMSAMPLES_BEST_THUMBNAIL_MODE 10

//flag, enable(1) or disable(0) functionality to break up aac frames into multiple media messages
#define PVMFRMFFPARSER_BREAKUP_AAC_FRAMES_INTO_MULTIPLE_MEDIA_FRAGS 1

#endif // PVMF_RMFFPARSERNODE_TUNEABLES_H_INCLUDED





