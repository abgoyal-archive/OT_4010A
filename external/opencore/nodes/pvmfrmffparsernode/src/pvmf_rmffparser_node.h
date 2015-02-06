 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_node.h
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
#ifndef PVMF_RMFFPARSERNODE_H_INCLUDED
#define PVMF_RMFFPARSERNODE_H_INCLUDED


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PVMF_MEDIA_CLOCK_H_INCLUDED
#include "pvmf_media_clock.h"
#endif

#ifndef OSCL_TIMER_H_INCLUDED
#include "oscl_timer.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

#ifndef PVLOGGER_FILE_APPENDER_H_INCLUDED
#include "pvlogger_file_appender.h"
#endif

#ifndef OSCL_BIN_STREAM_H_INCLUDED
#include "oscl_bin_stream.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

#ifndef PVMF_NODE_UTILS_H_INCLUDED
#include "pvmf_node_utils.h"
#endif

#ifndef OSCL_PRIQUEUE_H_INCLUDED
#include "oscl_priqueue.h"
#endif

#ifndef PVMF_MEDIA_DATA_H_INCLUDED
#include "pvmf_media_data.h"
#endif

#ifndef OSCL_MEM_MEMPOOL_H_INCLUDED
#include "oscl_mem_mempool.h"
#endif

#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#include "pvmf_simple_media_buffer.h"
#endif

#ifndef PVMF_MEDIA_FRAG_GROUP_H_INCLUDED
#include "pvmf_media_frag_group.h"
#endif

#ifndef PVMF_RMFFPARSER_DEFS_H_INCLUDED
#include "pvmf_rmffparser_defs.h"
#endif

#ifndef PVMF_DATA_SOURCE_PLAYBACK_CONTROL_H_INCLUDED
#include "pvmf_data_source_playback_control.h"
#endif

#ifndef PVMF_META_DATA_EXTENSION_H_INCLUDED
#include "pvmf_meta_data_extension.h"
#endif

#ifndef PVMF_DATA_SOURCE_INIT_EXTENSION_H_INCLUDED
#include "pvmf_data_source_init_extension.h"
#endif

#ifndef PVMF_TRACK_SELECTION_EXTENSION_H_INCLUDED
#include "pvmf_track_selection_extension.h"
#endif

#ifndef PVMF_MEDIA_PRESENTATION_INFO_H_INCLUDED
#include "pvmf_media_presentation_info.h"
#endif

#ifndef PVMF_TRACK_LEVEL_INFO_EXTENSION_H_INCLUDED
#include "pvmf_track_level_info_extension.h"
#endif

#ifndef PVMF_MP4_PROGDOWNLOAD_SUPPORT_EXTENSION_H_INCLUDED
#include "pvmf_mp4_progdownload_support_extension.h"
#endif



#ifndef PVMF_LOCAL_DATA_SOURCE_H_INCLUDED
#include "pvmf_local_data_source.h"
#endif

#ifndef PVMF_MEMPOOL_H_INCLUDED
#include "pvmf_mempool.h"
#endif

#ifndef PVMF_TIMEDTEXT_H_INCLUDED
#include "pvmf_timedtext.h"
#endif


#ifndef PVFILE_H_INCLUDED
#include "pvfile.h"
#endif

#ifndef PV_GAU_H_
#include "pv_gau.h"
#endif

#ifndef PVMF_FORMAT_PROGDOWNLOAD_SUPPORT_EXTENSION_H_INCLUDED
#include "pvmf_format_progdownload_support_extension.h"
#endif

#ifndef PVMF_DOWNLOAD_PROGRESS_EXTENSION_H
#include "pvmf_download_progress_interface.h"
#endif

#ifndef PVMI_DATASTREAMUSER_INTERFACE_H_INCLUDED
#include "pvmi_datastreamuser_interface.h"
#endif

#ifndef PVMF_RMFFPARSER_OUTPORT_H_INCLUDED
#include "pvmf_rmffparser_outport.h"
#endif

#ifndef PVMF_SOURCE_CONTEXT_DATA_H_INCLUDED
#include "pvmf_source_context_data.h"
#endif


#ifndef PVMI_KVP_UTIL_H_INCLUDED
#include "pvmi_kvp_util.h"
#endif

#ifndef PVMF_BASIC_ERRORINFOMESSAGE_H_INCLUDED
#include "pvmf_basic_errorinfomessage.h"
#endif

#ifndef PVMF_DATA_SOURCE_DIRECTION_CONTROL_H_INCLUDED
#include "pvmf_data_source_direction_control.h"
#endif

#include "pvmf_pmem_alloc.h"


#include "rm_parse.h"
#include "rv_depack.h"
#include "ra_depack.h"
#include "rv_decode.h"
#include "rm_packet.h"


extern "C"{
#include "cookdec_exp.h"
}

typedef struct _tagMTK_RV_FRAME{
	uint32 _data_length;
	uint8* _data;
	uint32 _timestamp;
	uint16 _seqno;
	uint16 _flags;
	bool _bLastPacket;
	uint32 _numseg;
	rv_segment* _segments;
} MTK_RV_FRAME;


typedef struct _tagMTK_RA_BLOCK{
	uint32 _data_length;
	uint8* _data;
	uint32 _timestamp;
	uint32 _flags;
} MTK_RA_BLOCK;


typedef Oscl_Vector<MTK_RA_BLOCK, OsclMemAllocator> RaBlockVector;
typedef Oscl_Vector<MTK_RV_FRAME, OsclMemAllocator> RvFrameVector;

typedef struct _tagRV_DEPACK_INFO {
	uint32 ulRvCodec4cc;
	rv_depack* pRvDepack;
	RvFrameVector* pRvFrames;
} RV_DEPACK_INFO;


#define GECKO_VERSION                 ((1L<<24)|(0L<<16)|(0L<<8)|(3L))

typedef struct ra8lbr_data_struct
{
    INT32   version;
    INT16   nSamples;
    INT16   nRegions;
    INT32   delay;
    UINT16  cplStart;
    UINT16  cplQBits;
    UINT32  channelMask;
} ra8lbr_data;


typedef struct _tagRA_DEPACK_INFO {
	uint32 ulRaCodec4cc;
	ra_depack* pRaDepack;
	RaBlockVector* pRaBlocks;
} RA_DEPACK_INFO;


class PVMFRMFFParserNodeLoggerDestructDealloc : public OsclDestructDealloc
{
    public:
        void destruct_and_dealloc(OsclAny* ptr)
        {
            PVLoggerAppender* p = OSCL_REINTERPRET_CAST(PVLoggerAppender*, ptr);
            BinaryFileAppender* binPtr = OSCL_REINTERPRET_CAST(BinaryFileAppender*, p);
            if (!binPtr)
                return;
            OSCL_DELETE(binPtr);
        }
};

#define PVMF_RMFFPARSERNODE_UNDERFLOW_STATUS_TIMER_ID 1


#define NORMAL_PLAYRATE 100000

// <-- Honda
#define _CHECK_UNNORMAL_VIDEO_
#define _MAX_VIDEO_DIMENSION_   (720*480)
#define _MAX_VIDEO_W_H_         (720)
#define _MAX_VIDEO_FPS_         (120)
// -->

typedef PVMFGenericNodeCommand<OsclMemAllocator> PVMFRMFFParserNodeCommandBase;

enum PVMFRMFFParserNodeCommandType
{
    PVMFRMFFPARSER_NODE_CMD_SETDATASOURCEPOSITION = PVMF_GENERIC_NODE_COMMAND_LAST
    , PVMFRMFFPARSER_NODE_CMD_QUERYDATASOURCEPOSITION
    , PVMFRMFFPARSER_NODE_CMD_SETDATASOURCERATE
    , PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAKEYS
    , PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAVALUES
    , PVMFRMFFPARSER_NODE_CMD_GET_LICENSE_W
    , PVMFRMFFPARSER_NODE_CMD_GET_LICENSE
    , PVMFRMFFPARSER_NODE_CMD_CANCEL_GET_LICENSE
    , PVMFRMFFPARSER_NODE_CAPCONFIG_SETPARAMS
    , PVMFRMFFPARSER_NODE_CMD_SETDATASOURCEDIRECTION
};

class PVMFRMFFParserNodeCommand : public PVMFRMFFParserNodeCommandBase
{
    public:

        // Constructor and parser for GetNodeMetadataKeys
        void Construct(PVMFSessionId s, int32 cmd
                       , PVMFMetadataList& aKeyList
                       , uint32 aStartingIndex
                       , int32 aMaxEntries
                       , char* aQueryKey
                       , const OsclAny* aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aKeyList;
            iParam2 = (OsclAny*)aStartingIndex;
            iParam3 = (OsclAny*)aMaxEntries;
            if (aQueryKey)
            {
                //allocate a copy of the query key string.
                Oscl_TAlloc<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> str;
                iParam4 = str.ALLOC_AND_CONSTRUCT(aQueryKey);
            }
        }
        void Parse(PVMFMetadataList*& MetaDataListPtr, uint32 &aStartingIndex, int32 &aMaxEntries, char*& aQueryKey)
        {
            MetaDataListPtr = (PVMFMetadataList*)iParam1;
            aStartingIndex = (uint32)iParam2;
            aMaxEntries = (int32)iParam3;
            aQueryKey = NULL;
            if (iParam4)
            {
                OSCL_HeapString<OsclMemAllocator>* keystring = (OSCL_HeapString<OsclMemAllocator>*)iParam4;
                aQueryKey = keystring->get_str();
            }
        }

        // Constructor and parser for GetNodeMetadataValue
        void Construct(PVMFSessionId s, int32 cmd, PVMFMetadataList& aKeyList, Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartIndex, int32 aMaxEntries, const OsclAny* aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aKeyList;
            iParam2 = (OsclAny*) & aValueList;
            iParam3 = (OsclAny*)aStartIndex;
            iParam4 = (OsclAny*)aMaxEntries;

        }
        void Parse(PVMFMetadataList* &aKeyList, Oscl_Vector<PvmiKvp, OsclMemAllocator>* &aValueList, uint32 &aStartingIndex, int32 &aMaxEntries)
        {
            aKeyList = (PVMFMetadataList*)iParam1;
            aValueList = (Oscl_Vector<PvmiKvp, OsclMemAllocator>*)iParam2;
            aStartingIndex = (uint32)iParam3;
            aMaxEntries = (int32)iParam4;
        }

        /* Constructor and parser for setParametersAsync */
        void Construct(PVMFSessionId s, int32 cmd, PvmiMIOSession aSession,
                       PvmiKvp* aParameters, int num_elements,
                       PvmiKvp*& aRet_kvp, OsclAny* aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aSession;
            iParam2 = (OsclAny*)aParameters;
            iParam3 = (OsclAny*)num_elements;
            iParam4 = (OsclAny*) & aRet_kvp;
        }
        void Parse(PvmiMIOSession& aSession, PvmiKvp*& aParameters,
                   int &num_elements, PvmiKvp** &ppRet_kvp)
        {
            aSession = (PvmiMIOSession)iParam1;
            aParameters = (PvmiKvp*)iParam2;
            num_elements = (int)iParam3;
            ppRet_kvp = (PvmiKvp**)iParam4;
        }

        // Constructor and parser for SetDataSourcePosition
        void Construct(PVMFSessionId s, int32 cmd, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT, PVMFTimestamp& aActualMediaDataTS,
                       bool aSeekToSyncPoint, uint32 aStreamID, const OsclAny*aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aTargetNPT;
            iParam2 = (OsclAny*) & aActualNPT;
            iParam3 = (OsclAny*) & aActualMediaDataTS;
            iParam4 = (OsclAny*)aSeekToSyncPoint;
            iParam5 = (OsclAny*)aStreamID;
        }
        void Parse(PVMFTimestamp& aTargetNPT, PVMFTimestamp*& aActualNPT, PVMFTimestamp*& aActualMediaDataTS, bool& aSeekToSyncPoint, uint32& aStreamID)
        {
            aTargetNPT = (PVMFTimestamp)iParam1;
            aActualNPT = (PVMFTimestamp*)iParam2;
            aActualMediaDataTS = (PVMFTimestamp*)iParam3;
            aSeekToSyncPoint = (iParam4) ? true : false;
            aStreamID = (uint32)iParam5;
        }

        // Constructor and parser for QueryDataSourcePosition
        void Construct(PVMFSessionId s, int32 cmd, PVMFTimestamp aTargetNPT, PVMFTimestamp &aActualNPT,
                       bool aSeekToSyncPoint, const OsclAny*aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aTargetNPT;
            iParam2 = (OsclAny*) & aActualNPT;
            iParam3 = (OsclAny*)aSeekToSyncPoint;
            iParam4 = NULL;
            iParam5 = NULL;
        }

        // Constructor and parser for QueryDataSourcePosition with aSeekPointBeforeTargetNPT and aSeekPointAfterTargetNPT
        void Construct(PVMFSessionId s, int32 cmd, PVMFTimestamp aTargetNPT,
                       PVMFTimestamp& aSeekPointBeforeTargetNPT, PVMFTimestamp& aSeekPointAfterTargetNPT,
                       const OsclAny*aContext, bool aSeekToSyncPoint)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aTargetNPT;
            iParam3 = (OsclAny*)aSeekToSyncPoint;
            iParam4 = (OsclAny*) & aSeekPointBeforeTargetNPT;
            iParam5 = (OsclAny*) & aSeekPointAfterTargetNPT;
        }


        void Parse(PVMFTimestamp& aTargetNPT, PVMFTimestamp*& aActualNPT, bool& aSeekToSyncPoint)
        {
            aTargetNPT = (PVMFTimestamp)iParam1;
            aActualNPT = (PVMFTimestamp*)iParam2;
            aSeekToSyncPoint = (iParam3) ? true : false;
        }

        void Parse(PVMFTimestamp& aTargetNPT, PVMFTimestamp*& aSeekPointBeforeTargetNPT,
                   bool& aSeekToSyncPoint, PVMFTimestamp*& aSeekPointAfterTargetNPT)
        {
            aTargetNPT = (PVMFTimestamp)iParam1;
            aSeekPointBeforeTargetNPT = (PVMFTimestamp*)iParam4;
            aSeekPointAfterTargetNPT = (PVMFTimestamp*)iParam5;
            aSeekToSyncPoint = (iParam3) ? true : false;
        }
        /* Constructor and parser for SetDataSourceDirection */
        void Construct(PVMFSessionId s, int32 cmd, int32 aDirection,
                       PVMFTimestamp& aActualNPT, PVMFTimestamp& aActualMediaDataTS,
                       PVMFTimebase* aTimebase, OsclAny* aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aDirection;
            iParam2 = (OsclAny*) & aActualNPT;
            iParam3 = (OsclAny*) & aActualMediaDataTS;
            iParam4 = (OsclAny*)aTimebase;
            iParam5 = NULL;
        }
        void Parse(int32& aDirection,
                   PVMFTimestamp*& aActualNPT,
                   PVMFTimestamp*& aActualMediaDataTS,
                   PVMFTimebase*& aTimebase)
        {
            aDirection = (int32)iParam1;
            aActualNPT = (PVMFTimestamp*)iParam2;
            aActualMediaDataTS = (PVMFTimestamp*)iParam3;
            aTimebase = (PVMFTimebase*)iParam4;
        }

        // Constructor and parser for SetDataSourceRate
        void Construct(PVMFSessionId s, int32 cmd, int32 aRate, PVMFTimebase* aTimebase, const OsclAny*aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aRate;
            iParam2 = (OsclAny*)aTimebase;
            iParam3 = NULL;
            iParam4 = NULL;
            iParam5 = NULL;
        }
        void Parse(int32& aRate, PVMFTimebase*& aTimebase)
        {
            aRate = (int32)iParam1;
            aTimebase = (PVMFTimebase*)iParam2;
        }

        /* Constructor and parser for GetLicenseW */
        void Construct(PVMFSessionId s,
                       int32 cmd,
                       OSCL_wString& aContentName,
                       OsclAny* aLicenseData,
                       uint32 aDataSize,
                       int32 aTimeoutMsec,
                       OsclAny* aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aContentName;
            iParam2 = (OsclAny*)aLicenseData;
            iParam3 = (OsclAny*)aDataSize;
            iParam4 = (OsclAny*)aTimeoutMsec;
            iParam5 = NULL;
        }
        void Parse(OSCL_wString*& aContentName,
                   OsclAny*& aLicenseData,
                   uint32& aDataSize,
                   int32& aTimeoutMsec)
        {
            aContentName = (OSCL_wString*)iParam1;
            aLicenseData = (PVMFTimestamp*)iParam2;
            aDataSize = (uint32)iParam3;
            aTimeoutMsec = (int32)iParam4;
        }

        /* Constructor and parser for GetLicense */
        void Construct(PVMFSessionId s,
                       int32 cmd,
                       OSCL_String& aContentName,
                       OsclAny* aLicenseData,
                       uint32 aDataSize,
                       int32 aTimeoutMsec,
                       OsclAny* aContext)
        {
            PVMFRMFFParserNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aContentName;
            iParam2 = (OsclAny*)aLicenseData;
            iParam3 = (OsclAny*)aDataSize;
            iParam4 = (OsclAny*)aTimeoutMsec;
            iParam5 = NULL;
        }
        void Parse(OSCL_String*& aContentName,
                   OsclAny*& aLicenseData,
                   uint32& aDataSize,
                   int32& aTimeoutMsec)
        {
            aContentName = (OSCL_String*)iParam1;
            aLicenseData = (PVMFTimestamp*)iParam2;
            aDataSize = (uint32)iParam3;
            aTimeoutMsec = (int32)iParam4;
        }


        // Need to overlaod the base Destroy routine to cleanup metadata key.
        void Destroy()
        {
            PVMFGenericNodeCommand<OsclMemAllocator>::Destroy();
            switch (iCmd)
            {
                case PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAKEYS:
                    if (iParam4)
                    {
                        //cleanup the allocated string
                        Oscl_TAlloc<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> str;
                        str.destruct_and_dealloc(iParam4);
                    }
                    break;
                default:
                    break;
            }
        }

        // Need to overlaod the base Copy routine to copy metadata key.
        void Copy(const PVMFGenericNodeCommand<OsclMemAllocator>& aCmd)
        {
            PVMFGenericNodeCommand<OsclMemAllocator>::Copy(aCmd);
            switch (aCmd.iCmd)
            {
                case PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAKEYS:
                    if (aCmd.iParam4)
                    {
                        //copy the allocated string
                        OSCL_HeapString<OsclMemAllocator>* aStr = (OSCL_HeapString<OsclMemAllocator>*)aCmd.iParam4;
                        Oscl_TAlloc<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> str;
                        iParam4 = str.ALLOC_AND_CONSTRUCT(*aStr);
                    }
                    break;
                default:
                    break;
            }
        }

};
typedef PVMFNodeCommandQueue<PVMFRMFFParserNodeCommand, OsclMemAllocator> PVMFRMFFParserNodeCmdQueue;

//Command queue type
typedef PVMFNodeCommandQueue<PVMFRMFFParserNodeCommand, OsclMemAllocator> PVMFRMFFParserNodeCmdQueue;


class PVMFRMFFParserOutPort;
class PVMFRMFFParserPortIter;
class PVLogger;
class PVMFMediaClock;

enum BaseKeys_SelectionType
{
    INVALID = 0,
    NET,
    FILE_IO
};

#define PVMFFFPARSERNODE_MAX_NUM_TRACKS 6

class PVMFRMFFParserNode : public OsclTimerObject,
        public PVMFNodeInterface,
        public PVMFDataSourceInitializationExtensionInterface,
        public PVMFTrackSelectionExtensionInterface,
        public PvmfDataSourcePlaybackControlInterface,
        public PVMFMetadataExtensionInterface,
        public OsclTimerObserver,
        public PvmiCapabilityAndConfig,
        public PVMFMediaClockStateObserver // For observing the playback clock states
{
    public:
        PVMFRMFFParserNode(int32 aPriority = OsclActiveObject::EPriorityNominal);
        virtual ~PVMFRMFFParserNode();

        // From PVMFNodeInterface
        PVMFStatus ThreadLogon();
        PVMFStatus ThreadLogoff();
        PVMFStatus GetCapability(PVMFNodeCapability& aNodeCapability);
        PVMFPortIter* GetPorts(const PVMFPortFilter* aFilter = NULL);

        PVMFCommandId QueryUUID(PVMFSessionId aSessionId,
                                const PvmfMimeString& aMimeType,
                                Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                bool aExactUuidsOnly = false,
                                const OsclAny* aContext = NULL);
        PVMFCommandId QueryInterface(PVMFSessionId aSessionId,
                                     const PVUuid& aUuid,
                                     PVInterface*& aInterfacePtr,
                                     const OsclAny* aContext = NULL);

        PVMFCommandId RequestPort(PVMFSessionId aSessionId, int32 aPortTag,
                                  const PvmfMimeString* aPortConfig = NULL,
                                  const OsclAny* aContext = NULL);
        PVMFStatus ReleasePort(PVMFSessionId aSessionId, PVMFPortInterface& aPort,
                               const OsclAny* aContext = NULL);

        PVMFCommandId Init(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Prepare(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Start(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Stop(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Flush(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Pause(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Reset(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);

        PVMFCommandId CancelAllCommands(PVMFSessionId aSessionId, const OsclAny* aContextData = NULL);
        PVMFCommandId CancelCommand(PVMFSessionId aSessionId, PVMFCommandId aCmdId, const OsclAny* aContextData = NULL);
        PVMFStatus QueryInterfaceSync(PVMFSessionId aSession,
                                      const PVUuid& aUuid,
                                      PVInterface*& aInterfacePtr);
        void HandlePortActivity(const PVMFPortActivity& aActivity);

        /* cap config interface */
        virtual void setObserver(PvmiConfigAndCapabilityCmdObserver* aObserver)
        {
            ciObserver = aObserver;
        }

        virtual PVMFStatus getParametersSync(PvmiMIOSession aSession,
                                             PvmiKeyType aIdentifier,
                                             PvmiKvp*& aParameters,
                                             int& aNumParamElements,
                                             PvmiCapabilityContext aContext);
        virtual PVMFStatus releaseParameters(PvmiMIOSession aSession,
                                             PvmiKvp* aParameters,
                                             int num_elements);
        virtual void createContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);
        virtual void setContextParameters(PvmiMIOSession aSession, PvmiCapabilityContext& aContext,
                                          PvmiKvp* aParameters, int num_parameter_elements);
        virtual void DeleteContext(PvmiMIOSession aSession,
                                   PvmiCapabilityContext& aContext);
        virtual void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                       int num_elements, PvmiKvp * & aRet_kvp);
        virtual PVMFCommandId setParametersAsync(PvmiMIOSession aSession,
                PvmiKvp* aParameters,
                int num_elements,
                PvmiKvp*& aRet_kvp,
                OsclAny* context = NULL);
        virtual uint32 getCapabilityMetric(PvmiMIOSession aSession);
        virtual PVMFStatus verifyParametersSync(PvmiMIOSession aSession,
                                                PvmiKvp* aParameters,
                                                int num_elements);

        PVMFStatus GetConfigParameter(PvmiKvp*& aParameters, int& aNumParamElements,
                                      int32 aIndex, PvmiKvpAttr reqattr);
        PVMFStatus VerifyAndSetConfigParameter(int index, PvmiKvp& aParameter, bool set);


        // From PVInterface
        void addRef();
        void removeRef();
        bool queryInterface(const PVUuid& uuid, PVInterface*& iface);

        // From PVMFDataSourceInitializationExtensionInterface
        PVMFStatus SetSourceInitializationData(OSCL_wString& aSourceURL, PVMFFormatType& aSourceFormat, OsclAny* aSourceData);
        PVMFStatus SetClientPlayBackClock(PVMFMediaClock* aClientClock);
        PVMFStatus SetEstimatedServerClock(PVMFMediaClock* aClientClock);

        // From PVMFTrackSelectionExtensionInterface
        PVMFStatus GetMediaPresentationInfo(PVMFMediaPresentationInfo& aInfo);
        PVMFStatus SelectTracks(PVMFMediaPresentationInfo& aInfo);

        // From PVMFMetadataExtensionInterface
        uint32 GetNumMetadataKeys(char* aQueryKeyString = NULL);
        uint32 GetNumMetadataValues(PVMFMetadataList& aKeyList);
        uint32 GetNumImotionMetadataValues(PVMFMetadataList& aKeyList);
        PVMFCommandId GetNodeMetadataKeys(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList, uint32 aStartingKeyIndex, int32 aMaxKeyEntries,
                                          char* aQueryKeyString = NULL, const OsclAny* aContextData = NULL);
        PVMFCommandId GetNodeMetadataValues(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList,
                                            Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartingValueIndex, int32 aMaxValueEntries, const OsclAny* aContextData = NULL);
        PVMFStatus ReleaseNodeMetadataKeys(PVMFMetadataList& aKeyList, uint32 aStartingKeyIndex, uint32 aEndKeyIndex);
        PVMFStatus ReleaseNodeMetadataValues(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartingValueIndex, uint32 aEndValueIndex);

        // From PvmfDataSourcePlaybackControlInterface
        PVMFCommandId SetDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT,
                                            PVMFTimestamp& aActualMediaDataTS, bool aSeekToSyncPoint = true, uint32 aStreamID = 0, OsclAny* aContext = NULL);
        PVMFCommandId QueryDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT,
                                              bool aSeekToSyncPoint = true, OsclAny* aContext = NULL);
        PVMFCommandId QueryDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT,
                                              PVMFTimestamp& aSeekPointBeforeTargetNPT, PVMFTimestamp& aSeekPointAfterTargetNPT,  OsclAny* aContext = NULL, bool aSeekToSyncPoint = true);

        PVMFCommandId SetDataSourceRate(PVMFSessionId aSession, int32 aRate, PVMFTimebase* aTimebase = NULL, OsclAny* aContext = NULL);
        PVMFCommandId SetDataSourceDirection(PVMFSessionId aSessionId, int32 aDirection, PVMFTimestamp& aActualNPT,
                                             PVMFTimestamp& aActualMediaDataTS, PVMFTimebase* aTimebase, OsclAny* aContext);


        // From OsclTimer
        void TimeoutOccurred(int32 timerID, int32 timeoutInfo);

        //from PVMFMediaClockStateObserver
        void ClockStateUpdated();
        void NotificationsInterfaceDestroyed();

    private:
        // from OsclTimerObject
        void Run();

        //Command processing
        PVMFCommandId QueueCommandL(PVMFRMFFParserNodeCommand& aCmd);
        void MoveCmdToCurrentQueue(PVMFRMFFParserNodeCommand& aCmd);
        void MoveCmdToCancelQueue(PVMFRMFFParserNodeCommand& aCmd);
        void ProcessCommand();
        void CommandComplete(PVMFRMFFParserNodeCmdQueue& aCmdQueue, PVMFRMFFParserNodeCommand& aCmd, PVMFStatus aStatus, OsclAny* aData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        bool FlushPending();
        PVMFRMFFParserNodeCmdQueue iInputCommands;
        PVMFRMFFParserNodeCmdQueue iCurrentCommand;
        PVMFRMFFParserNodeCmdQueue iCancelCommand;

        // Event reporting
        void ReportRMFFParserErrorEvent(PVMFEventType aEventType, OsclAny* aEventData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        void ReportRMFFParserInfoEvent(PVMFEventType aEventType, OsclAny* aEventData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        void ChangeNodeState(TPVMFNodeInterfaceState aNewState);

        // Node command handlers
        PVMFStatus DoQueryUuid(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoQueryInterface(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoRequestPort(PVMFRMFFParserNodeCommand& aCmd, PVMFPortInterface*&);
        void GetTrackMaxParameters(PVMFFormatType aFormatType, uint32& aMaxDataSize, uint32& aMaxQueueDepth);
        PVMFStatus DoReleasePort(PVMFRMFFParserNodeCommand& aCmd);

        PVMFStatus DoInit(PVMFRMFFParserNodeCommand& aCmd);
        void PushToAvailableMetadataKeysList(const char* aKeystr, char* aOptionalParam = NULL);
        PVMFStatus InitMetaData();
        PVMFStatus InitImotionMetaData();
        uint32 CountImotionMetaDataKeys();
        int32 CountMetaDataKeys();
        void CompleteInit(PVMFRMFFParserNodeCmdQueue& aCmdQ,
                          PVMFRMFFParserNodeCommand& aCmd);
        void CompleteCancelAfterInit();


        PVMFStatus DoPrepare(PVMFRMFFParserNodeCommand& aCmd);
        void CompletePrepare(PVMFStatus aStatus);
        PVMFStatus DoStart(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoStop(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoFlush(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoPause(PVMFRMFFParserNodeCommand& aCmd);

        PVMFStatus DoReset(PVMFRMFFParserNodeCommand& aCmd);
        void CompleteReset(PVMFRMFFParserNodeCmdQueue& aCmdQ, PVMFRMFFParserNodeCommand& aCmd);

        PVMFStatus DoCancelAllCommands(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoCancelCommand(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoCancelCurrentCommand(PVMFRMFFParserNodeCommand& aCmd);

        // For metadata extention interface
        PVMFStatus DoGetMetadataKeys(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus CompleteGetMetadataKeys(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoGetImotionMetadataValues(PVMFRMFFParserNodeCommand& aCmd, int32 &numentriesadded);
        PVMFStatus DoGetMetadataValues(PVMFRMFFParserNodeCommand& aCmd);
        void CompleteGetMetaDataValues();
        int32 AddToValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, PvmiKvp& aNewValue);
        PVMFStatus GetIndexParamValues(const char* aString, uint32& aStartIndex, uint32& aEndIndex);
        void DeleteAPICStruct(PvmfApicStruct*& aAPICStruct);
        PVMFStatus GetVideoFrameWidth(uint32 aId, int32& aWidth, int32& aDisplayWidth);
        PVMFStatus GetVideoFrameHeight(uint32 aId, int32& aHeight, int32& aDisplayHeight);
        int32 FindVideoWidth(uint32 aId);
        int32 FindVideoHeight(uint32 aId);
        int32 FindVideoDisplayWidth(uint32 aId);
        int32 FindVideoDisplayHeight(uint32 aId);
        PVMFStatus PopulateVideoDimensions(uint32 aId);
        uint32 GetNumAudioChannels(uint32 aId);
        uint32 GetAudioSampleRate(uint32 aId);
        uint32 GetAudioBitsPerSample(uint32 aId);

        // For data source position extension interface
        PVMFStatus DoSetDataSourcePosition(PVMFRMFFParserNodeCommand& aCmd, PVMFStatus &aEventCode, PVUuid &aEventUuid);
        PVMFStatus DoQueryDataSourcePosition(PVMFRMFFParserNodeCommand& aCmd);
        PVMFStatus DoSetDataSourceRate(PVMFRMFFParserNodeCommand& aCmd);

        PVMFStatus DoSetDataSourceDirection(PVMFRMFFParserNodeCommand& aCmd);

        void HandleTrackState();
        bool RetrieveTrackConfigInfo(uint32 aTrackId,
                                     PVMFFormatType aFormatType,
                                     OsclRefCounterMemFrag &aConfig);
        bool RetrieveTrackConfigInfoAndFirstSample(uint32 aTrackId,
                PVMFFormatType aFormatType,
                OsclRefCounterMemFrag &aConfig);
        bool RetrieveTrackData(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo);
        bool SendTrackData(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo);
        bool UpdateTextSampleEntry(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo, uint32 aEntryIndex, PVMFTimedTextMediaData& aTextMediaData);
        bool SendEndOfTrackCommand(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo);
        bool SendBeginOfMediaStreamCommand(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo);

        // Port processing
        void ProcessPortActivity();
        void QueuePortActivity(const PVMFPortActivity& aActivity);
        PVMFStatus ProcessIncomingMsg(PVMFPortInterface* aPort);
        PVMFStatus ProcessOutgoingMsg(PVMFPortInterface* aPort);
        Oscl_Vector<PVMFPortActivity, OsclMemAllocator> iPortActivityQueue;

        friend class PVMFRMFFParserOutPort;

        PVMFFormatType GetFormatTypeFromMIMEType(PvmfMimeString* aMIMEString);

        void ResetAllTracks();
        bool ReleaseAllPorts();
        void RemoveAllCommands();
        void CleanupFileSource();

        bool GetTrackPortInfoForTrackID(PVRMFFParserNodeTrackPortInfo*& aInfo,
                                        uint32 aTrackID);

        bool GetTrackPortInfoForPort(PVRMFFParserNodeTrackPortInfo*& aInfo,
                                     PVMFPortInterface* aPort);

        bool ParseRM();

        OSCL_wHeapString<OsclMemAllocator> iFilename;
        PVMFFormatType iSourceFormat;
        PVMFMediaClock* iClientPlayBackClock;
        PVMFMediaClockNotificationsInterface *iClockNotificationsInf;
        bool iUseCPMPluginRegistry;
        bool iPreviewMode;
        PVMFLocalDataSource iCPMSourceData;
        PVMFCPMPluginAccessInterfaceFactory* iCPMContentAccessFactory;
        PVMFSourceContextData iSourceContextData;
        OsclFileHandle* iFileHandle;
        Oscl_FileServer iFileServer;
        PVFile iFile;
        uint32 iParsingMode;
        bool iProtectedFile;
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iAvailableMetadataKeys;
        uint32 iRMParserNodeMetadataValueCount;
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iCPMMetadataKeys;
        Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator> iNodeTrackPortList;
        Oscl_Vector<PVMFTrackInfo, OsclMemAllocator> iSelectedTrackInfoList;
        Oscl_Vector<VideoTrackDimensionInfo, OsclMemAllocator> iVideoDimensionInfoVec;

        PVMFRMFFParserPortIter* iPortIter;

        // stream id
        uint32 iStreamID;

        PVMFNodeCapability iCapability;

        // Reference counter for extension
        uint32 iExtensionRefCount;

        // variables to support download autopause
        GAU iGau;

        bool iThumbNailMode;


        PVMFStatus CheckForUnderFlow(PVRMFFParserNodeTrackPortInfo* aInfo);

        void getLanguageCode(uint16 langcode, int8 *LangCode);
        void getBrand(uint32 langcode, char *LangCode);


        int32 CreateErrorInfoMsg(PVMFBasicErrorInfoMessage** aErrorMsg, PVUuid aEventUUID, int32 aEventCode);
        void CreateDurationInfoMsg(uint32 adurationms);
        PVMFStatus PushKVPToMetadataValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>* aVecPtr, PvmiKvp& aKvpVal);
        PVMFStatus CreateNewArray(uint32** aTrackidList, uint32 aNumTracks);
        PVMFStatus PushValueToList(Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> &aRefMetadataKeys,
                                   PVMFMetadataList *&aKeyListPtr,
                                   uint32 aLcv);

        uint32 iLastNPTCalcInConvertSizeToTime;
        uint32 iFileSizeLastConvertedToTime;

        /* External PseudoStreaming related */
        bool iExternalDownload;

        bool iUnderFlowEventReported;
        PVMFStatus ReportUnderFlow();
        OsclTimer<OsclMemAllocator> *iUnderFlowCheckTimer;

        /* bitstream logging */
        void LogMediaData(PVMFSharedMediaDataPtr data,
                          PVMFPortInterface* aPort);
		void LogMediaDataToFile(PVMFSharedMediaDataPtr data,
                          PVMFPortInterface* aPort, const char* prefix);
	    void WriteGAUtoFile(GAU* gau);
	 int32 GetNextSamples(uint8* pBuffer, uint32& numSamples, uint32& length, uint32& ts, uint32& deltaTs, uint32 trackId);
	    uint32 iNumMediaFrame;

        char iLogFileIndex;
        OSCL_HeapString<OsclMemAllocator> portLogPath;

        uint32 minTime;
        uint32 avgTime;
        uint32 maxTime;
        uint32 sumTime;
        bool iDiagnosticsLogged;
        void LogDiagnostics();
        uint32 iTimeTakenInReadMP4File;
        bool iBackwardReposFlag; /* To avoid backwardlooping :: A flag to remember backward repositioning */
        bool iForwardReposFlag;
        uint32 iCurPos;
        bool iEOTForTextSentToMIO;

        bool iSetTextSampleDurationZero;

        /* To take into account if we get negative TS for text track after repositionings*/
        bool iTextInvalidTSAfterReposition;

        uint32 iDelayAddToNextTextSample;

        uint32 iCacheSize;
        uint32 iAsyncReadBuffSize;
        bool iPVLoggerEnableFlag;
        bool iPVLoggerStateEnableFlag;
        uint32 iNativeAccessMode;

        BaseKeys_SelectionType iBaseKey;
        uint32 iJitterBufferDurationInMs;
        bool iDataStreamRequestPending;
        bool iCPMSequenceInProgress;
        bool oIsAACFramesFragmented;

        int32 iPlayBackDirection;
        int32 iStartForNextTSSearch;
        int32 iPrevSampleTS;
        bool iParseAudioDuringFF;
        bool iParseAudioDuringREW;
        bool iParseVideoOnly;
        bool iOpenFileOncePerTrack;
        int32 iDataRate;

        int32 minFileOffsetTrackID;

        rm_parser* iRmParser;
        rv_depack* iRvDepack;
        ra_depack* iRaDepack;
        rm_packet* iRmPacket;
        uint16	     iRvStreamNum;
        uint16	     iRaStreamNum; 
        rv_format_info* iRvFormatInfo;
        ra_format_info* iRaFormatInfo;
        RV_DEPACK_INFO iRvDepackInfo;
        RA_DEPACK_INFO iRaDepackInfo;
        PVMFStatus PushVideoDimensions(uint32 aId, int32 width, int32 height);
        RaBlockVector iRaBlocks;
        RvFrameVector iRvFrames;
        bool DequeueRaBlock(uint8* pBuffer, uint32& numSamples, uint32& length, uint32& ts, uint32& deltaTs);
        bool CleanRvFrameQueue();
        bool CleanRaBlockQueue();
        bool RMSeekTo(uint32 targetNPT);

    public:
        PVMFStatus ResetNodeParam();
        PVMFStatus ClearBuffer();

    private:
        // For Pmem  
        PVMFPmemMemAlloc iInBufMemoryAlloc;
        
        long long start_retrieve_time;

        // for check track format
        bool IsVideoTrack(char* FormatType);
};


class PVMFRMFFParserPortIter : public PVMFPortIter
{
    public:
        PVMFRMFFParserPortIter(Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>& aTrackList)
        {
            iIndex = 0;
            iTrackList = &aTrackList;
        };

        virtual ~PVMFRMFFParserPortIter() {};

        uint16 NumPorts()
        {
            if (iTrackList->size() < 0xFFFF)
            {
                return (uint16)(iTrackList->size());
            }
            else
            {
                return 0xFFFF;
            }
        };

        PVMFPortInterface* GetNext()
        {
            if (iIndex < iTrackList->size())
            {
                PVMFPortInterface* portiface = (*iTrackList)[iIndex].iPortInterface;
                ++iIndex;
                return portiface;
            }
            else
            {
                return NULL;
            }
        };

        void Reset()
        {
            iIndex = 0;
        };

    private:
        Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>* iTrackList;
        uint32 iIndex;
};

#endif // PVMF_RMFFPARSER_NODE_H_INCLUDED

