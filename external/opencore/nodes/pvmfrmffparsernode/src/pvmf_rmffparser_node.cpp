 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_node.cpp
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
 
#include "pvmf_rmffparser_node.h"

#include "pvmf_rmffparser_node_tuneables.h"

#include "pvmf_rmffparser_outport.h"

#include "media_clock_converter.h"

#include "pvlogger.h"

#include "oscl_error_codes.h"

#include "oscl_tickcount.h"

#include "oscl_base.h"

#include "pv_mime_string_utils.h"

#include "oscl_snprintf.h"

#include "pvmf_timestamp.h"

#include "pvmf_fileformat_events.h"

#include "pvmf_rmffparser_events.h"

#include "pvmf_errorinfomessage_extension.h"

#include "pvmf_download_progress_interface.h"

#include "pvmf_duration_infomessage.h"

#include "pvmf_durationinfomessage_extension.h"

#include "oscl_int64_utils.h"

#include "pvmf_media_cmd.h"

#include "pvmf_media_msg_format_ids.h"

#include "pvmf_local_data_source.h"

#include "pvmi_kvp_util.h"

#include "oscl_string_containers.h"

#include "oscl_string_utils.h"

#include "oscl_bin_stream.h"

#include "m4v_config_parser.h"

#include "getactualaacconfig.h"

#include "oscl_exclusive_ptr.h"

#include "oscl_profile.h"

#include "oscl_file_cfg.h"



#define MAX_TRACK_NO 256

#define MILLISECOND_TIMESCALE 1000

#undef LOG_TAG
#define LOG_TAG "RMFFParser"

#ifndef _WIN32
#include <utils/Log.h>
#define LOG_PROFILE(...) LOGV(__VA_ARGS__)

static bool bShowLog = false;
#define LOGP(...) \
{\
    if (bShowLog)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

#undef PVLOGGER_LOGMSG
#define PVLOGGER_LOGMSG(IL, LOGGER, LEVEL, MESSAGE)\
{\
    if (LEVEL == PVLOGMSG_ERR)\
    {\
        ErrMSG MESSAGE;\
    }\
    else\
    {\
        MSG MESSAGE;\
    }\
}
#define ErrMSG(x, ...) LOGE(__VA_ARGS__)
#define MSG(x, ...) LOGP(__VA_ARGS__)
#else

#define LOGD(...)
#define LOGV(...)
#define LOGE(...)

#endif
// ---------------------------------------------------------------------

static uint32 iDebugTest = 0;
#define debugger_test(x) \
{\
    if (x == iDebugTest)\
    {\
        LOGD("iDebugTest = %d", iDebugTest);\
        _OSCL_Abort();\
    }\
}

#ifndef OSCL_MAP_H_INCLUDED
#include "oscl_map.h"
#endif

#include "helix_mime_types.h"
#include "rm_parse.h"

#define RM_EVERYTHING_FINE 0
#define RM_END_OF_TRACK 1
#define RM_INSUFFICIENT_DATA 2
#define RM_INSUFFICIENT_BUFFER_SIZE 3
#define RM_READ_FAILED 4

#define RV8_4CC			0x52563330
#define RV9_4CC			0x52563430
#define RV89COMBO_4CC	0x54524F4D
#define COOK_4CC		0x636F6F6B
#define RAAC_4CC		0x72616163
#define RACP_4CC		0x72616370

#define RV_INTRAPIC				0	// I frame
#define RV_FORCEED_INTRAPIC	1	// I frame
#define RV_INTERPIC				2	// P frame
#define RV_TRUEBPIC				3	// B frame

void rm_error(void* pError, HX_RESULT result, const char* pszMsg){ LOGE("rm_error pError=0x%08x result=0x%08x msg=%s\n", pError, result, pszMsg);}
UINT32 rm_io_read_func (void* pUserRead,
                                     BYTE* pBuf, /* Must be at least ulBytesToRead long */
                                     UINT32  ulBytesToRead)
{
    PVFile* _pFile = (PVFile*)pUserRead;
    if (_pFile->IsOpen())
    {
        uint32 bytesRead = _pFile->Read(pBuf, 1, ulBytesToRead);
        //LOGE ("rm_io_read_func ulBytesToRead(%d), bytesRead(%d) ", ulBytesToRead, bytesRead);
        return bytesRead;
    }
    
    return 0;
}

void rm_io_seek_func (void*  pUserRead,
                                  UINT32 ulOffset,
                                  UINT32 ulOrigin)
{
      PVFile* _pFile = (PVFile*)pUserRead;
      //LOGE ("rm_io_seek_func ulOffset(%u), ulOrigin(%u)", ulOffset, ulOrigin);
      if (_pFile->IsOpen())
      {
          if (ulOrigin == HX_SEEK_ORIGIN_SET)
          {
		_pFile->Seek(ulOffset, Oscl_File::SEEKSET);
          }
          else if (ulOrigin == HX_SEEK_ORIGIN_CUR)
          {
       	_pFile->Seek(ulOffset, Oscl_File::SEEKCUR);
          }
          else if (ulOrigin == HX_SEEK_ORIGIN_END)
          {
              _pFile->Seek(ulOffset, Oscl_File::SEEKEND);
          }
    }
}

HX_RESULT rv_frame_available(void* pAvail, UINT32 ulSubStreamNum, rv_frame* pFrame)
{
	HX_RESULT retVal = HXR_FAIL;
	if (pAvail && pFrame && pFrame->pData && pFrame->ulDataLen)
	{
		RV_DEPACK_INFO* pDepackInfo = (RV_DEPACK_INFO*)pAvail;
		RvFrameVector* pRvFrameVec = (RvFrameVector*)pDepackInfo->pRvFrames;
	       //LOGE ("rv_frame_available ts=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);

	       uint8 *_ptr = pFrame->pData;      
              uint _pic_type = 0;
      
	       if(pDepackInfo->ulRvCodec4cc == RV8_4CC)
	       {
			_pic_type = ( (*_ptr>>3) & 0x3);
			switch(_pic_type)
			{
			case RV_INTRAPIC:
			case RV_FORCEED_INTRAPIC: // I frame
				LOGE ("RV8 I frame ts=%u, len=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulDataLen, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);
				break;
			case RV_INTERPIC: // P frame
				LOGE ("RV8 P frame ts=%u, len=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulDataLen, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);
				break;            
			case RV_TRUEBPIC: // B frame  
				LOGE ("RV8 B frame ts=%u, len=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulDataLen, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);
				break;            
			}         
	       }
	       else if (pDepackInfo->ulRvCodec4cc == RV9_4CC)
	       {
			_pic_type = ( (*_ptr>>1) & 0x3);
			switch(_pic_type)
			{
			case RV_INTRAPIC:
			case RV_FORCEED_INTRAPIC: // I frame
				LOGE ("RV9 I frame ts=%u, len=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulDataLen, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);
				break;
			case RV_INTERPIC: // P frame
				LOGE ("RV9 P frame ts=%u, len=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulDataLen, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);
				break;            
			case RV_TRUEBPIC: // B frame  
				LOGE ("RV9 B frame ts=%u, len=%u, num_seg=%u, seq=%u, last=%d", pFrame->ulTimestamp, pFrame->ulDataLen, pFrame->ulNumSegments, pFrame->usSequenceNum, pFrame->bLastPacket);
				break;            
			}
	       }

		rv_depack_destroy_frame(pDepackInfo->pRvDepack, &pFrame);
		retVal = HXR_OK;
	}
	else
	{
		LOGE ("rv_frame_available return FAIL pAvail(0x%x), pFrame(0x%x), pFrame->pData(0x%x), pFrame->ulDataLen(%u)", pAvail, pFrame, pFrame->pData, pFrame->ulDataLen);
	}
	
	return retVal;
}

HX_RESULT ra_block_available(void* pAvail, UINT32 ulSubStream, ra_block* pBlock)
{    
	HX_RESULT retVal = HXR_FAIL;

	if (pAvail && pBlock && pBlock->pData && pBlock->ulDataLen)
	{
		RA_DEPACK_INFO* pDepackInfo = (RA_DEPACK_INFO*)pAvail;
		RaBlockVector* pRaBlockVec = (RaBlockVector*)pDepackInfo->pRaBlocks;
		//LOGE ("ra_block_available ts=%u, len=%u, iRaBlocks.size()=%d, 4cc=0x%x", pBlock->ulTimestamp, pBlock->ulDataLen, pRaBlockVec->size(), pDepackInfo->ulRaCodec4cc);
		// queue ra block
		MTK_RA_BLOCK audio_block;
		audio_block._data_length = pBlock->ulDataLen;
		audio_block._data = new uint8[pBlock->ulDataLen];
		memcpy (audio_block._data, pBlock->pData, pBlock->ulDataLen);
		audio_block._timestamp = pBlock->ulTimestamp;
		audio_block._flags = pBlock->ulDataFlags;
		pRaBlockVec->push_back(audio_block);

		// NOTE: Don't need to destroy the block, ra depack will do this internally.
		retVal = HXR_OK;
	}
	else
	{
		LOGE ("ra_block_available return FAIL pAvail(0x%x), pBlock(0x%x), pBlock->pData(0x%x), pBlock->ulDataLen(%u)", pAvail, pBlock, pBlock->pData, pBlock->ulDataLen);
	}
	
	return retVal;
}

PVMFRMFFParserNode::PVMFRMFFParserNode(int32 aPriority) :
        OsclTimerObject(aPriority, "PVMFRMFFParserNode"),
        iPortIter(NULL),
        iBackwardReposFlag(false), /* To avoid backwardlooping :: A flag to remember backward repositioning */
        iForwardReposFlag(false),
        iPlayBackDirection(PVMF_DATA_SOURCE_DIRECTION_FORWARD),
        iParseAudioDuringFF(false),
        iParseAudioDuringREW(false),
        iParseVideoOnly(false),
        iDataRate(NORMAL_PLAYRATE),
        start_retrieve_time(0),
        minFileOffsetTrackID(0),
        iRmParser(0),
        iRvDepack(0),
        iRaFormatInfo(HXNULL),
        iRvFormatInfo(HXNULL)
{
    LOGE ("PVMFRMFFParserNode constructor In\n");
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_mtkrawsource");
    iDebugTest = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "debug_test");
    
    iClientPlayBackClock = NULL;
    iClockNotificationsInf = NULL;
    iExtensionRefCount = 0;
    iThumbNailMode = false;
    iProtectedFile = false;


    iInterfaceState = EPVMFNodeCreated;
    iParsingMode = PVMF_RMFFPARSER_NODE_ENABLE_PARSER_OPTIMIZATION;
    oIsAACFramesFragmented = PVMFRMFFPARSER_BREAKUP_AAC_FRAMES_INTO_MULTIPLE_MEDIA_FRAGS;

    iFileHandle = NULL;

    iLastNPTCalcInConvertSizeToTime = 0;
    iFileSizeLastConvertedToTime = 0;

    iExternalDownload = false;
    iUnderFlowEventReported = false;
    iUnderFlowCheckTimer = NULL;

    minTime = 0;
    avgTime = 0;
    maxTime = 0;
    sumTime = 0;
    iDiagnosticsLogged = false;
    iCurPos = 0;
    iDelayAddToNextTextSample = 0;
    iTextInvalidTSAfterReposition = false;
    iEOTForTextSentToMIO = false;
    iSetTextSampleDurationZero = false;

    iCacheSize = DEFAULT_CAHCE_SIZE;
    iAsyncReadBuffSize = DEFAULT_ASYNC_READ_BUFFER_SIZE;
    iPVLoggerEnableFlag = false;
    iPVLoggerStateEnableFlag = false;
    iNativeAccessMode = DEFAULT_NATIVE_ACCESS_MODE;

    iStreamID = 0;

    iDataStreamRequestPending = false;

    iJitterBufferDurationInMs = PVMF_RMFFPARSER_NODE_PSEUDO_STREAMING_BUFFER_DURATION_IN_MS;
    iBaseKey = INVALID;
    iJitterBufferDurationInMs = PVMF_RMFFPARSER_NODE_PSEUDO_STREAMING_BUFFER_DURATION_IN_MS;
    iBaseKey = INVALID;

    iRvStreamNum = 0xFFFF;
    iRaStreamNum = 0xFFFF;

    int32 err;
    OSCL_TRY(err,
             //Create the input command queue.  Use a reserve to avoid lots of
             //dynamic memory allocation.
             iInputCommands.Construct(1000/*Starting command ID*/, 10/*Number slots to reserve in queue*/);

             //Create the "current command" queue.  It will only contain one
             //command at a time, so use a reserve of 1.
             iCurrentCommand.Construct(0, 1);
             iCancelCommand.Construct(0, 1);

             //Set the node capability data.
             iCapability.iCanSupportMultipleInputPorts = false;
             iCapability.iCanSupportMultipleOutputPorts = true;
             iCapability.iHasMaxNumberOfPorts = true;
             iCapability.iMaxNumberOfPorts = 6;
             iCapability.iInputFormatCapability.push_back(PVMFFormatType(PVMF_MIME_RMFF));
             iCapability.iOutputFormatCapability.push_back(PVMFFormatType(PVMF_MIME_MPEG4_AUDIO));
             iCapability.iOutputFormatCapability.push_back(PVMFFormatType(PVMF_MIME_REAL_AUDIO));

             iAvailableMetadataKeys.clear();

             iUnderFlowCheckTimer = OSCL_NEW(OsclTimer<OsclMemAllocator>,
                                             ("PVMFRMFFParserNodeUnderFlowTimer"));

             iUnderFlowCheckTimer->SetObserver(this);
             iUnderFlowCheckTimer->SetFrequency(PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_FREQUENCY);
            );

    iPortIter = OSCL_NEW(PVMFRMFFParserPortIter, (iNodeTrackPortList));
    if (iPortIter == NULL)
    {
        err = OsclErrNoMemory;
    }

    if (err != OsclErrNone)
    {
        //if a leave happened, cleanup and re-throw the error
        iInputCommands.clear();
        iCurrentCommand.clear();
        iCancelCommand.clear();
        iCapability.iInputFormatCapability.clear();
        iCapability.iOutputFormatCapability.clear();
        OSCL_CLEANUP_BASE_CLASS(PVMFNodeInterface);
        OSCL_CLEANUP_BASE_CLASS(OsclTimerObject);
        OSCL_LEAVE(err);
    }
}


PVMFRMFFParserNode::~PVMFRMFFParserNode()
{
    //remove the clock observer
    if (iClientPlayBackClock != NULL)
    {
        if (iClockNotificationsInf != NULL)
        {
            iClockNotificationsInf->RemoveClockStateObserver(*this);
            iClientPlayBackClock->DestroyMediaClockNotificationsInterface(iClockNotificationsInf);
            iClockNotificationsInf = NULL;
        }
    }

    LogDiagnostics();
    Cancel();
    if (IsAdded())
    {
        RemoveFromScheduler();
    }

    if (iUnderFlowCheckTimer != NULL)
    {
        iUnderFlowCheckTimer->Clear();
    }
    OSCL_DELETE(iUnderFlowCheckTimer);


    iPortActivityQueue.clear();
    ReleaseAllPorts();
    RemoveAllCommands();

    OSCL_DELETE(iPortIter);
    iPortIter = NULL;

    if (iRvFormatInfo)
    {
    	rv_depack_destroy_codec_init_info(iRvDepack, &iRvFormatInfo);
    	iRvFormatInfo = HXNULL;
    }

    if (iRaFormatInfo)
    {
    	ra_depack_destroy_codec_init_info(iRaDepack, &iRaFormatInfo);
    	iRaFormatInfo = HXNULL;
    }

    if (iRvDepack)
    {
    	rv_depack_destroy(&iRvDepack);
    	iRvDepack = NULL;
    }
    
    if (iRaDepack)
    {
    	ra_depack_destroy(&iRaDepack);
    	iRaDepack = NULL;
    }
	
    if (iRmParser)
    {
    	rm_parser_destroy(&iRmParser);
    	iRmParser = NULL;
    }

    CleanRaBlockQueue();
    CleanRvFrameQueue();

    CleanupFileSource();
    iFileServer.Close();

    if (iFile.IsOpen())
    {
        iFile.Close();
    }

    if (iExtensionRefCount > 0)
    {
        OSCL_ASSERT(false);
    }

    //The command queues are self-deleting, but we want to
    //notify the observer of unprocessed commands.
    while (!iCurrentCommand.empty())
    {
        CommandComplete(iCurrentCommand, iCurrentCommand.front(), PVMFFailure, NULL, NULL);
    }
    while (!iCancelCommand.empty())
    {
        CommandComplete(iCancelCommand, iCancelCommand.front(), PVMFFailure, NULL, NULL);
    }
    while (!iInputCommands.empty())
    {
        CommandComplete(iInputCommands, iInputCommands.front(), PVMFFailure, NULL, NULL);
    }
}


PVMFStatus PVMFRMFFParserNode::ThreadLogon()
{
    if (iInterfaceState == EPVMFNodeCreated)
    {
        if (!IsAdded())
        {
            AddToScheduler();
        }

        iFileServer.Connect();
        ChangeNodeState(EPVMFNodeIdle);
        return PVMFSuccess;
    }

    return PVMFErrInvalidState;
}


PVMFStatus PVMFRMFFParserNode::ThreadLogoff()
{
    if (iInterfaceState == EPVMFNodeIdle)
    {
        CleanupFileSource();
        iFileServer.Close();

        if (IsAdded())
        {
            RemoveFromScheduler();
        }

        ChangeNodeState(EPVMFNodeCreated);
        return PVMFSuccess;
    }

    return PVMFErrInvalidState;
}


PVMFStatus PVMFRMFFParserNode::GetCapability(PVMFNodeCapability& aNodeCapability)
{
    aNodeCapability = iCapability;

    return PVMFSuccess;
}


PVMFPortIter* PVMFRMFFParserNode::GetPorts(const PVMFPortFilter* aFilter)
{
    OSCL_UNUSED_ARG(aFilter);
    iPortIter->Reset();
    return iPortIter;
}


PVMFCommandId PVMFRMFFParserNode::QueryUUID(PVMFSessionId aSessionId, const PvmfMimeString& aMimeType,
        Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids, bool aExactUuidsOnly, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_QUERYUUID, aMimeType, aUuids, aExactUuidsOnly, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::QueryInterface(PVMFSessionId aSessionId, const PVUuid& aUuid,
        PVInterface*& aInterfacePtr, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_QUERYINTERFACE, aUuid, aInterfacePtr, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::RequestPort(PVMFSessionId aSessionId, int32 aPortTag,
        const PvmfMimeString* aPortConfig, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_REQUESTPORT, aPortTag, aPortConfig, aContext);
    return QueueCommandL(cmd);
}


PVMFStatus PVMFRMFFParserNode::ReleasePort(PVMFSessionId aSessionId, PVMFPortInterface& aPort, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_RELEASEPORT, aPort, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Init(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_INIT, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Prepare(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_PREPARE, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Start(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_START, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Stop(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_STOP, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Flush(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_FLUSH, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Pause(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_PAUSE, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::Reset(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_RESET, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::CancelAllCommands(PVMFSessionId aSessionId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_CANCELALLCOMMANDS, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::CancelCommand(PVMFSessionId aSessionId, PVMFCommandId aCmdId, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommandBase::Construct(aSessionId, PVMF_GENERIC_NODE_CANCELCOMMAND, aCmdId, aContext);
    return QueueCommandL(cmd);
}


void PVMFRMFFParserNode::addRef()
{
    ++iExtensionRefCount;
}


void PVMFRMFFParserNode::removeRef()
{
    --iExtensionRefCount;
}


PVMFStatus PVMFRMFFParserNode::QueryInterfaceSync(PVMFSessionId aSession,
        const PVUuid& aUuid,
        PVInterface*& aInterfacePtr)
{
    OSCL_UNUSED_ARG(aSession);
    aInterfacePtr = NULL;
    if (queryInterface(aUuid, aInterfacePtr))
    {
        aInterfacePtr->addRef();
        return PVMFSuccess;
    }
    return PVMFErrNotSupported;
}

bool PVMFRMFFParserNode::queryInterface(const PVUuid& uuid, PVInterface*& iface)
{
    if (uuid == PVMF_TRACK_SELECTION_INTERFACE_UUID)
    {
        PVMFTrackSelectionExtensionInterface* myInterface = OSCL_STATIC_CAST(PVMFTrackSelectionExtensionInterface*, this);
        iface = OSCL_STATIC_CAST(PVInterface*, myInterface);
    }
    else if (uuid == PVMF_DATA_SOURCE_INIT_INTERFACE_UUID)
    {
        PVMFDataSourceInitializationExtensionInterface* myInterface = OSCL_STATIC_CAST(PVMFDataSourceInitializationExtensionInterface*, this);
        iface = OSCL_STATIC_CAST(PVInterface*, myInterface);
    }
    else if (uuid == KPVMFMetadataExtensionUuid)
    {
        PVMFMetadataExtensionInterface* myInterface = OSCL_STATIC_CAST(PVMFMetadataExtensionInterface*, this);
        iface = OSCL_STATIC_CAST(PVInterface*, myInterface);
    }
    else if (uuid == PvmfDataSourcePlaybackControlUuid)
    {
        PvmfDataSourcePlaybackControlInterface* myInterface = OSCL_STATIC_CAST(PvmfDataSourcePlaybackControlInterface*, this);
        iface = OSCL_STATIC_CAST(PVInterface*, myInterface);
    }
    else if (uuid == PVMI_CAPABILITY_AND_CONFIG_PVUUID)
    {
        PvmiCapabilityAndConfig* myInterface =  OSCL_STATIC_CAST(PvmiCapabilityAndConfig*, this);
        iface = OSCL_STATIC_CAST(PVInterface*, myInterface);
    }
    else
    {
        return false;
    }
    return true;
}


PVMFStatus PVMFRMFFParserNode::SetSourceInitializationData(OSCL_wString& aSourceURL,
        PVMFFormatType& aSourceFormat,
        OsclAny* aSourceData)
{
    //LOGE ("PVMFRMFFParserNode::SetSourceInitializationData() called, filename = %s\n", aSourceURL.get_cstr());

    OSCL_UNUSED_ARG(aSourceData);

    //cleanup any prior source.
    CleanupFileSource();

    PVMFFormatType inputFormatType = aSourceFormat;

    if (inputFormatType == PVMF_MIME_RMFF)
    {
        iFilename = aSourceURL;
        iSourceFormat = inputFormatType;

        //LOGE ("PVMFRMFFParserNode::SetSourceInitializationData iFilename=%s", aSourceURL.get_cstr());

        iFile.SetFileHandle(iFileHandle);
        if (iFile.Open(aSourceURL.get_cstr(), (Oscl_File::MODE_READ | Oscl_File::MODE_BINARY), iFileServer) != 0)
        {
            LOGE ("PVMFRMFFParserNode::SetSourceInitializationData() file open failed\n");
        }

    }

    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::SetClientPlayBackClock(PVMFMediaClock* aClock)
{
    if (aClock == NULL)
    {
        return PVMFErrArgument;
    }

    if (iClockNotificationsInf && iClientPlayBackClock)
    {
        iClockNotificationsInf->RemoveClockStateObserver(*this);
        iClientPlayBackClock->DestroyMediaClockNotificationsInterface(iClockNotificationsInf);
        iClockNotificationsInf = NULL;
    }
    iClientPlayBackClock = aClock;
    iClientPlayBackClock->ConstructMediaClockNotificationsInterface(iClockNotificationsInf, *this);

    if (NULL == iClockNotificationsInf)
    {
        return PVMFErrNoMemory;
    }

    iClockNotificationsInf->SetClockStateObserver(*this);

    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::SetEstimatedServerClock(PVMFMediaClock* /*aClientClock*/)
{
    return PVMFErrNotSupported;
}

PVMFStatus PVMFRMFFParserNode::GetMediaPresentationInfo(PVMFMediaPresentationInfo& aInfo)
{
    LOGE ("GetMediaPresentationInfo");

    uint32 numTracks = rm_parser_get_num_streams(iRmParser);

    LOGE ("numTracks = %d", numTracks);

    uint64 movieDuration = rm_parser_get_duration(iRmParser);
    LOGE ("movieDuration=%lld", movieDuration); 
    aInfo.setDurationValue(movieDuration);
    aInfo.setDurationTimeScale(MILLISECOND_TIMESCALE);
       
    HX_RESULT retVal = HXR_OK;

    for (int i = 0 ; i < numTracks ; i++)
    {
        rm_stream_header* pHdr = HXNULL;
        bool _IsVideo = false;
        bool _IsAudio = false;
        retVal = rm_parser_get_stream_header(iRmParser, i, &pHdr);
        if (retVal == HXR_OK)
        {
            if (rm_stream_is_realvideo(pHdr))
            {
                _IsVideo = true;
                iRvStreamNum = i;
                LOGE ("Track %d is realvideo", i);
                // IGNORE VIDEO !!!
                //continue;
            }
            else if (rm_stream_is_realaudio(pHdr))
            {
                LOGE ("Track %d is realaudio", i);
                _IsAudio = true;
                iRaStreamNum = i;
            }
            else
            {
            	  LOGE ("Un-recognized track %d, i");
            }

            // setup media info
            uint64 duration = rm_stream_get_duration(pHdr);
            uint32 stream_id = rm_stream_get_number(pHdr);
            const char* mime = rm_stream_get_mime_type(pHdr);
            uint32 avgBitrate = rm_stream_get_avg_bit_rate(pHdr);
            uint32 maxBitrate = rm_stream_get_max_bit_rate(pHdr);
            uint32 starTime = rm_stream_get_start_time(pHdr);
            uint32 preroll = rm_stream_get_preroll(pHdr);
            const char* name = rm_stream_get_name(pHdr);
            uint32 maxPacketSize = rm_stream_get_max_packet_size(pHdr);
            LOGE ("Mime=%s, Name=%s, StreamId=%u, Duration=%lld, AvgBitrate=%u, MaxBitrage=%u, maxPacketSize=%u, StartTime=%u", mime, name, stream_id, duration, avgBitrate, maxBitrate, maxPacketSize, starTime);

            PVMFTrackInfo tmpTrackInfo;
            tmpTrackInfo.setTrackID(stream_id);
            tmpTrackInfo.setPortTag(stream_id);
            tmpTrackInfo.setTrackBitRate(avgBitrate);
            tmpTrackInfo.setTrackDurationTimeScale(MILLISECOND_TIMESCALE);
            tmpTrackInfo.setTrackDurationValue(duration);

            OSCL_FastString iMime;
            // Morris Yang 2010 rmvb temp hack
            iMime.set((char*)mime, oscl_strlen(mime));
            //iMime.set((char*)PVMF_MIME_YUV420, oscl_strlen(PVMF_MIME_YUV420));
            tmpTrackInfo.setTrackMimeType(iMime);          

            if (_IsVideo)
            {
                // TODO: pass internal structure to rv_depack_create
                iRvDepack = rv_depack_create(&iRvDepackInfo, rv_frame_available, NULL, rm_error);
                if (!iRvDepack)
                {
                    LOGE ("rv_depack_create failed !!!");
                }
                // initialize iRvDepackInfo
                iRvFrames.reserve(10);
                iRvDepackInfo.pRvFrames = (RvFrameVector*)&iRvFrames;
                iRvDepackInfo.pRvDepack = iRvDepack;
                
                retVal = rv_depack_init(iRvDepack, pHdr);
                if (retVal != HXR_OK)
                {
	             LOGE ("rv_depack_init failed !!!");
                }

		 iRvDepackInfo.ulRvCodec4cc = rv_depack_get_codec_4cc(iRvDepack);

		 if (iRvDepackInfo.ulRvCodec4cc == RV8_4CC)
		 {
		 	LOGE ("RealVideo Codec: RV8");
		 }
		 else if (iRvDepackInfo.ulRvCodec4cc == RV9_4CC)
		 {
		 	LOGE ("RealVideo Codec: RV9");
		 }
		 else if (iRvDepackInfo.ulRvCodec4cc == RV89COMBO_4CC)
		 {
		 	LOGE ("RealVideo Codec: RV89COMBO");
		 }
		 
                retVal = rv_depack_get_codec_init_info(iRvDepack, &iRvFormatInfo);
                if (retVal != HXR_OK)
                {
                    LOGE ("rv_depack_get_codec_init_info failed !!!");
                }
                
                //PopulateVideoDimensions(stream_id);
                uint32 _FramesPerSec = iRvFormatInfo->ufFramesPerSecond >> 16;
                LOGE ("WIDTH(%d), HEIGHT(%d), FPS(%d)", iRvFormatInfo->usWidth, iRvFormatInfo->usHeight, _FramesPerSec);
                PushVideoDimensions(stream_id, iRvFormatInfo->usWidth, iRvFormatInfo->usHeight);
                tmpTrackInfo.setTrackFrameRate(_FramesPerSec);

		  // IGNORE VIDEO !!!
                continue;

	         // TODO: retrieve decoder config info
                OsclRefCounterMemFrag config;
                if (!RetrieveTrackConfigInfo(0, REALVIDEO_MIME_TYPE, config))
                {
                    return PVMFFailure;
                }

                tmpTrackInfo.setTrackConfigInfo(config);
            }

            if (_IsAudio)
            {
              // TODO: pass internal struct to ra_depack_create 
            	iRaDepack = ra_depack_create(&iRaDepackInfo, ra_block_available, NULL, rm_error);
              if (!iRaDepack)
              {
              	LOGE ("ra_depack_create failed");
              	 return PVMFFailure;
              }
              // initialize iRaDepackInfo
              iRaBlocks.reserve(10);
              iRaDepackInfo.pRaBlocks = (RaBlockVector*)&iRaBlocks;
              iRaDepackInfo.pRaDepack = iRaDepack;

              retVal = ra_depack_init(iRaDepack, pHdr);
              if (retVal != HXR_OK)
              {
	       	LOGE ("ra_depack_init failed !!!");
	       	 return PVMFFailure;
              }

		iRaDepackInfo.ulRaCodec4cc = ra_depack_get_codec_4cc(iRaDepack, 0);
              
		retVal = ra_depack_get_codec_init_info(iRaDepack, 0, &iRaFormatInfo);
		if (retVal != HXR_OK)
		{
			LOGE ("ra_depack_get_codec_init_info failed !!!");
			 return PVMFFailure;
		}

		if (iRaDepackInfo.ulRaCodec4cc == RAAC_4CC || iRaDepackInfo.ulRaCodec4cc == RACP_4CC)
		{
			LOGE ("RealAudio Codec: AAC/AAC+");
			iMime.set((char*)PVMF_MIME_MPEG4_AUDIO, oscl_strlen(PVMF_MIME_MPEG4_AUDIO));
            		tmpTrackInfo.setTrackMimeType(iMime);          

            	       // TODO: retrieve decoder config info
                     OsclRefCounterMemFrag config;
                     if (!RetrieveTrackConfigInfo(0, PVMF_MIME_MPEG4_AUDIO, config))
                	{
                         return PVMFFailure;
                	}
                     tmpTrackInfo.setTrackConfigInfo(config);
		}
		else if (iRaDepackInfo.ulRaCodec4cc == COOK_4CC)
		{
			iMime.set((char*)PVMF_MIME_REAL_AUDIO, oscl_strlen(PVMF_MIME_REAL_AUDIO));
			tmpTrackInfo.setTrackMimeType(iMime);
			
			LOGE ("RealAudio Codec: COOK");
                     OsclRefCounterMemFrag config;
                     if (!RetrieveTrackConfigInfo(0, PVMF_MIME_REAL_AUDIO, config))
                	{
                         return PVMFFailure;
                	}
                     tmpTrackInfo.setTrackConfigInfo(config);
		}
		else
		{
			LOGE ("Unrecognized audio codec: 0x%x", iRaDepackInfo.ulRaCodec4cc);
		}
   
              LOGE ("ulSampleRate(%u), ulActualRate(%u), usBitsPerSample(%u), usNumChannels(%u), usAudioQuality(%u), ulBitsPerFrame(%u)",
              	iRaFormatInfo->ulSampleRate, iRaFormatInfo->ulActualRate, iRaFormatInfo->usBitsPerSample, iRaFormatInfo->usNumChannels, iRaFormatInfo->usAudioQuality, iRaFormatInfo->ulBitsPerFrame);
            }

            
            aInfo.addTrackInfo(tmpTrackInfo);
            
            // TODO: move to destructor
            rm_parser_destroy_stream_header(iRmParser, &pHdr);
        }
    }


    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::SelectTracks(PVMFMediaPresentationInfo& aInfo)
{
    // Copy the selected tracks to internal list
    iSelectedTrackInfoList.clear();

    uint32 i;
    for (i = 0; i < aInfo.getNumTracks(); ++i)
    {
        iSelectedTrackInfoList.push_back(*(aInfo.getTrackInfo(i)));
    }

    return PVMFSuccess;
}


PVMFCommandId PVMFRMFFParserNode::SetDataSourcePosition(PVMFSessionId aSessionId,
        PVMFTimestamp aTargetNPT,
        PVMFTimestamp& aActualNPT,
        PVMFTimestamp& aActualMediaDataTS,
        bool aSeekToSyncPoint,
        uint32 aStreamID,
        OsclAny* aContext)
{
    //LOGE ("PVMFRMFFParserNode::SetDataSourcePosition: aTargetNPT=%d, aSeekToSyncPoint=%d, aContext=0x%x", aTargetNPT, aSeekToSyncPoint, (uint8*)aContext);

    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommand::Construct(aSessionId, PVMFRMFFPARSER_NODE_CMD_SETDATASOURCEPOSITION, aTargetNPT, aActualNPT,
            aActualMediaDataTS, aSeekToSyncPoint, aStreamID, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::QueryDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT,
        PVMFTimestamp& aActualNPT,
        bool aSeekToSyncPoint,
        OsclAny* aContext)
{
    //LOGE ("PVMFRMFFParserNode::QueryDataSourcePosition: aTargetNPT=%d, aSeekToSyncPoint=%d, aContext=0x%x", aTargetNPT, aSeekToSyncPoint, aContext);

    if (aActualNPT > aTargetNPT) /* eg of backward aActualNPT (CurrentPos) = 10, aTargetNPT (NewPos) = 2 */
    {
        iBackwardReposFlag = true; /* To avoid backwardlooping */
        aActualNPT = 0;
    }
    else
    {
        iForwardReposFlag = true;
        iCurPos = aActualNPT;
        aActualNPT = 0;
    }

    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommand::Construct(aSessionId, PVMFRMFFPARSER_NODE_CMD_QUERYDATASOURCEPOSITION, aTargetNPT, aActualNPT,
            aSeekToSyncPoint, aContext);
    return QueueCommandL(cmd);
}

PVMFCommandId PVMFRMFFParserNode::QueryDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT,
        PVMFTimestamp& aSeekPointBeforeTargetNPT,
        PVMFTimestamp& aSeekPointAfterTargetNPT,
        OsclAny* aContext,
        bool aSeekToSyncPoint)
{
    //LOGE ("PVMFRMFFParserNode::QueryDataSourcePosition: aTargetNPT=%d, aSeekToSyncPoint=%d, aContext=0x%x", aTargetNPT, aSeekToSyncPoint, aContext);

    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommand::Construct(aSessionId, PVMFRMFFPARSER_NODE_CMD_QUERYDATASOURCEPOSITION, aTargetNPT,
            aSeekPointBeforeTargetNPT, aSeekPointAfterTargetNPT, aContext, aSeekToSyncPoint);
    return QueueCommandL(cmd);
}

PVMFCommandId PVMFRMFFParserNode::SetDataSourceRate(PVMFSessionId aSessionId, int32 aRate, PVMFTimebase* aTimebase, OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommand::Construct(aSessionId, PVMFRMFFPARSER_NODE_CMD_SETDATASOURCERATE, aRate, aTimebase, aContext);
    return QueueCommandL(cmd);
}


/////////////////////
// Private Section //
/////////////////////

void PVMFRMFFParserNode::Run()
{
    //Process commands.
    if (!iInputCommands.empty())
    {
        ProcessCommand();
    }

    // Process port activity
    while (!iPortActivityQueue.empty() && (iInterfaceState == EPVMFNodeStarted || FlushPending()))
    {
        ProcessPortActivity();
    }

    // Send out media data when started and not flushing
    if (iInterfaceState == EPVMFNodeStarted && !FlushPending())
    {
        HandleTrackState();
    }

    //Check for completion of a flush command...
    if (FlushPending() && iPortActivityQueue.empty())
    {
        //Flush is complete.
        CommandComplete(iCurrentCommand, iCurrentCommand.front(), PVMFSuccess);
    }
}


PVMFCommandId PVMFRMFFParserNode::QueueCommandL(PVMFRMFFParserNodeCommand& aCmd)
{
    if (IsAdded())
    {
        PVMFCommandId id;
        id = iInputCommands.AddL(aCmd);

        /* Wakeup the AO */
        RunIfNotReady();
        return id;
    }
    LOGE("PVMFRMFFParserNode::QueueCommandL");
    OSCL_LEAVE(OsclErrInvalidState);
    return -1;
}

void PVMFRMFFParserNode::MoveCmdToCurrentQueue(PVMFRMFFParserNodeCommand& aCmd)
{
    //note: the StoreL cannot fail since the queue is never more than 1 deep
    //and we reserved space.
    iCurrentCommand.StoreL(aCmd);
    iInputCommands.Erase(&aCmd);
}

void PVMFRMFFParserNode::MoveCmdToCancelQueue(PVMFRMFFParserNodeCommand& aCmd)
{
    //note: the StoreL cannot fail since the queue is never more than 1 deep
    //and we reserved space.
    iCancelCommand.StoreL(aCmd);
    iInputCommands.Erase(&aCmd);
}


void PVMFRMFFParserNode::CompleteInit(PVMFRMFFParserNodeCmdQueue& aCmdQ,
                          PVMFRMFFParserNodeCommand& aCmd)

{
    LOGE ("CompleteInit");
    if (ParseRM())
    {
        LOGE ("ParseRM successful!!!");
    }

    PVMFStatus status = InitMetaData();
    
    CommandComplete(aCmdQ, aCmd, PVMFSuccess, NULL, NULL, NULL);
    return;
}

void PVMFRMFFParserNode::ProcessCommand()
{
    //can't do anything while an asynchronous cancel is in progress...
    if (!iCancelCommand.empty())
        return;

    PVMFRMFFParserNodeCommand& aCmd = iInputCommands.front();

    //normally this node will not start processing one command
    //until the prior one is finished.  However, a hi priority
    //command such as Cancel must be able to interrupt a command
    //in progress.
    if (!iCurrentCommand.empty() && !aCmd.hipri() && aCmd.iCmd != PVMFRMFFPARSER_NODE_CMD_CANCEL_GET_LICENSE)
    {
        return ;
    }

    PVMFStatus cmdstatus;
    switch (aCmd.iCmd)
    {
        case PVMF_GENERIC_NODE_QUERYUUID:
            cmdstatus = DoQueryUuid(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMF_GENERIC_NODE_QUERYINTERFACE:
            cmdstatus = DoQueryInterface(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMF_GENERIC_NODE_REQUESTPORT:
        {
            PVMFPortInterface*port;
            cmdstatus = DoRequestPort(aCmd, port);
            CommandComplete(iInputCommands, aCmd, cmdstatus, (OsclAny*)port);
        }
        break;

        case PVMF_GENERIC_NODE_RELEASEPORT:
            cmdstatus = DoReleasePort(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMF_GENERIC_NODE_INIT:
            cmdstatus = DoInit(aCmd);
            switch (cmdstatus)
            {
                case PVMFPending:
                    MoveCmdToCurrentQueue(aCmd);
                    //wait on CPM or data stream callback.
                    break;
                case PVMFSuccess:
                    //This means that init can be completed right away
                    //without waiting on either CPM or datastream callbacks
                    //This happens if:
                    //1) Local playback of unprotected content
                    //2) FT / PDL /PPB of unprotected content where in movieatom
                    //is complete when init was processed
                    CompleteInit(iInputCommands, aCmd);
                    break;
                default:
                    CommandComplete(iInputCommands, aCmd, cmdstatus);
                    break;
            }
            break;

        case PVMF_GENERIC_NODE_PREPARE:
            cmdstatus = DoPrepare(aCmd);
            //doprepare may complete synchronously or asynchronously.
            switch (cmdstatus)
            {
                case PVMFPending:
                    //wait on DataStream callback.
                    MoveCmdToCurrentQueue(aCmd);
                    break;
                default:
                    CommandComplete(iInputCommands, aCmd, cmdstatus);
                    break;
            }
            break;

        case PVMF_GENERIC_NODE_START:
            cmdstatus = DoStart(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMF_GENERIC_NODE_STOP:
            cmdstatus = DoStop(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMF_GENERIC_NODE_FLUSH:
            cmdstatus = DoFlush(aCmd);
            switch (cmdstatus)
            {
                case PVMFPending:
                    MoveCmdToCurrentQueue(aCmd);
                    break;
                default:
                    CommandComplete(iInputCommands, aCmd, cmdstatus);
                    break;
            }
            break;

        case PVMF_GENERIC_NODE_PAUSE:
            cmdstatus = DoPause(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMF_GENERIC_NODE_RESET:
            cmdstatus = DoReset(aCmd);
            //reset may complete synchronously or asynchronously.
            switch (cmdstatus)
            {
                case PVMFPending:
                    MoveCmdToCurrentQueue(aCmd);
                    //wait on CPM callback.
                    break;
                case PVMFSuccess:
                    CompleteReset(iInputCommands, aCmd);
                    break;
                default:
                    CommandComplete(iInputCommands, aCmd, cmdstatus);
                    break;
            }
            break;

        case PVMF_GENERIC_NODE_CANCELALLCOMMANDS:
            cmdstatus = DoCancelAllCommands(aCmd);
            switch (cmdstatus)
            {
                case PVMFPending:
                    MoveCmdToCancelQueue(aCmd);
                    //wait on CPM callback.
                    break;
                default:
                    CommandComplete(iInputCommands, aCmd, cmdstatus);
                    break;
            }
            break;

        case PVMF_GENERIC_NODE_CANCELCOMMAND:
            cmdstatus = DoCancelCommand(aCmd);
            switch (cmdstatus)
            {
                case PVMFPending:
                    MoveCmdToCancelQueue(aCmd);
                    //wait on CPM callback.
                    break;
                default:
                    CommandComplete(iInputCommands, aCmd, cmdstatus);
                    break;
            }
            break;

        case PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAKEYS:
            cmdstatus = DoGetMetadataKeys(aCmd);
            if (cmdstatus != PVMFPending)
            {
                CommandComplete(iInputCommands, aCmd, cmdstatus);
            }
            else
            {
                MoveCmdToCurrentQueue(aCmd);
            }
            break;

        case PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAVALUES:
            cmdstatus = DoGetMetadataValues(aCmd);
            if (cmdstatus != PVMFPending)
            {
                CommandComplete(iInputCommands, aCmd, cmdstatus);
            }
            else
            {
                MoveCmdToCurrentQueue(aCmd);
            }
            break;

        case PVMFRMFFPARSER_NODE_CMD_SETDATASOURCEPOSITION:
        {
            PVUuid eventuuid;
            PVMFStatus eventcode;
            cmdstatus = DoSetDataSourcePosition(aCmd, eventcode, eventuuid);
            if (eventcode == PVMFSuccess)
            {
                CommandComplete(iInputCommands, aCmd, cmdstatus);
            }
            else
            {
                CommandComplete(iInputCommands, aCmd, cmdstatus, NULL, &eventuuid, &eventcode);
            }
        }
        break;

        case PVMFRMFFPARSER_NODE_CMD_QUERYDATASOURCEPOSITION:
            cmdstatus = DoQueryDataSourcePosition(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMFRMFFPARSER_NODE_CMD_SETDATASOURCERATE:
            cmdstatus = DoSetDataSourceRate(aCmd);
            CommandComplete(iInputCommands, aCmd, cmdstatus);
            break;

        case PVMFRMFFPARSER_NODE_CAPCONFIG_SETPARAMS:
        {
            PvmiMIOSession session;
            PvmiKvp* aParameters;
            int num_elements;
            PvmiKvp** ppRet_kvp;
            aCmd.Parse(session, aParameters, num_elements, ppRet_kvp);
            setParametersSync(NULL, aParameters, num_elements, *ppRet_kvp);
            CommandComplete(iInputCommands, aCmd, PVMFSuccess);
        }
        break;

        default://unknown command type
            CommandComplete(iInputCommands, aCmd, PVMFErrNotSupported);
            break;
    }
}

void PVMFRMFFParserNode::CommandComplete(PVMFRMFFParserNodeCmdQueue& aCmdQ, PVMFRMFFParserNodeCommand& aCmd, PVMFStatus aStatus, OsclAny* aEventData, PVUuid* aEventUUID, int32* aEventCode)
{
    //LOGE ("PVMFRMFFParserNode::CommandComplete() In Id %d Cmd %d Status %d Context %d Data %d", aCmd.iId, aCmd.iCmd, aStatus, aCmd.iContext, aEventData);

    bool oCompleteCancel = false;
    if ((aCmd.iCmd == PVMF_GENERIC_NODE_INIT) &&
            (iCancelCommand.empty() == false))
    {
        //cancel has been waiting on init
        oCompleteCancel = true;
    }
    //Do standard node command state changes.
    if (aStatus == PVMFSuccess)
    {
        switch (aCmd.iCmd)
        {
            case PVMF_GENERIC_NODE_INIT:
                ChangeNodeState(EPVMFNodeInitialized);
                break;
            case PVMF_GENERIC_NODE_PREPARE:
                ChangeNodeState(EPVMFNodePrepared);
                break;
            case PVMF_GENERIC_NODE_START:
                ChangeNodeState(EPVMFNodeStarted);
                //wakeup the AO when started...
                RunIfNotReady();
                break;
            case PVMF_GENERIC_NODE_PAUSE:
                ChangeNodeState(EPVMFNodePaused);
                break;
            case PVMF_GENERIC_NODE_STOP:
                ChangeNodeState(EPVMFNodePrepared);
                break;
            case PVMF_GENERIC_NODE_FLUSH:
                ChangeNodeState(EPVMFNodePrepared);
                break;
            case PVMF_GENERIC_NODE_RESET:
                ChangeNodeState(EPVMFNodeIdle);
                break;
        }
    }

    if (aStatus != PVMFSuccess)
    {
        //LOGE ("PVMFRMFFParserNode::CommandComplete() In Id %d Cmd %d Status %d Context %d Data %d", aCmd.iId, aCmd.iCmd, aStatus, aCmd.iContext, aEventData);
    }

    PVInterface* extif = NULL;
    PVMFBasicErrorInfoMessage* errormsg = NULL;
    if (aEventUUID && aEventCode)
    {
        int32 leavecode = CreateErrorInfoMsg(&errormsg, *aEventUUID, *aEventCode);
        if (leavecode == 0 && errormsg)
        {
            extif = OSCL_STATIC_CAST(PVInterface*, errormsg);
        }

    }
    //create response
    PVMFCmdResp resp(aCmd.iId, aCmd.iContext, aStatus, extif, aEventData);
    PVMFSessionId session = aCmd.iSession;

    //Erase the command from the queue.
    aCmdQ.Erase(&aCmd);

    //Report completion to the session observer.
    ReportCmdCompleteEvent(session, resp);

    if (errormsg)
    {
        errormsg->removeRef();
    }

    if (oCompleteCancel)
    {
        CompleteCancelAfterInit();
    }

    //Re-schedule if there are additional commands
    if (!iInputCommands.empty()
            && IsAdded())
    {
        RunIfNotReady();
    }
}

void PVMFRMFFParserNode::CompleteCancelAfterInit()
{
    // cancel commands were pending, but not processed. return failed cancel
    while (!iCancelCommand.empty())
    {
        PVMFRMFFParserNodeCommand& cmdCancel = iCancelCommand.front();
        PVMFCmdResp resp(cmdCancel.iId, cmdCancel.iContext, PVMFFailure);
        PVMFSessionId session = cmdCancel.iSession;

        //Erase the command from the queue.
        iCancelCommand.Erase(&cmdCancel);

        //Report completion to the session observer.
        ReportCmdCompleteEvent(session, resp);
    }
}

void PVMFRMFFParserNode::ReportRMFFParserErrorEvent(PVMFEventType aEventType, OsclAny* aEventData, PVUuid* aEventUUID, int32* aEventCode)
{
    if (aEventUUID && aEventCode)
    {
        int32 leavecode = 0;
        PVMFBasicErrorInfoMessage* eventmsg = NULL;
        OSCL_TRY(leavecode, eventmsg = OSCL_NEW(PVMFBasicErrorInfoMessage, (*aEventCode, *aEventUUID, NULL)));
        PVMFAsyncEvent asyncevent(PVMFErrorEvent, aEventType, NULL, OSCL_STATIC_CAST(PVInterface*, eventmsg), aEventData, NULL, 0);
        PVMFNodeInterface::ReportErrorEvent(asyncevent);
        if (eventmsg)
        {
            eventmsg->removeRef();
        }
    }
    else
    {
        PVMFNodeInterface::ReportErrorEvent(aEventType, aEventData);
    }
}


void PVMFRMFFParserNode::ReportRMFFParserInfoEvent(PVMFEventType aEventType, OsclAny* aEventData, PVUuid* aEventUUID, int32* aEventCode)
{
    if (aEventUUID && aEventCode)
    {
        int32 leavecode = 0;
        PVMFBasicErrorInfoMessage* eventmsg = NULL;
        OSCL_TRY(leavecode, eventmsg = OSCL_NEW(PVMFBasicErrorInfoMessage, (*aEventCode, *aEventUUID, NULL)));
        PVMFAsyncEvent asyncevent(PVMFInfoEvent, aEventType, NULL, OSCL_STATIC_CAST(PVInterface*, eventmsg), aEventData, NULL, 0);
        PVMFNodeInterface::ReportInfoEvent(asyncevent);
        if (eventmsg)
        {
            eventmsg->removeRef();
        }
    }
    else
    {
        PVMFNodeInterface::ReportInfoEvent(aEventType, aEventData);
    }
}


void PVMFRMFFParserNode::ChangeNodeState(TPVMFNodeInterfaceState aNewState)
{
    SetState(aNewState);
}


PVMFStatus PVMFRMFFParserNode::DoQueryUuid(PVMFRMFFParserNodeCommand& aCmd)
{
    OSCL_String* mimetype;
    Oscl_Vector<PVUuid, OsclMemAllocator> *uuidvec;
    bool exactmatch;
    aCmd.PVMFRMFFParserNodeCommandBase::Parse(mimetype, uuidvec, exactmatch);

    // @TODO Add MIME string matching
    // For now just return all available extension interface UUID
    uuidvec->push_back(PVMF_TRACK_SELECTION_INTERFACE_UUID);
    uuidvec->push_back(PVMF_DATA_SOURCE_INIT_INTERFACE_UUID);
    uuidvec->push_back(KPVMFMetadataExtensionUuid);
    uuidvec->push_back(PvmfDataSourcePlaybackControlUuid);
    uuidvec->push_back(PVMF_TRACK_LEVEL_INFO_INTERFACE_UUID);
    uuidvec->push_back(PVMF_MP4_PROGDOWNLOAD_SUPPORT_INTERFACE_UUID);
    uuidvec->push_back(PvmfDataSourceDirectionControlUuid);

    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoQueryInterface(PVMFRMFFParserNodeCommand& aCmd)
{
    PVUuid* uuid;
    PVInterface** ptr;
    aCmd.PVMFRMFFParserNodeCommandBase::Parse(uuid, ptr);

    if (queryInterface(*uuid, *ptr))
    {
        (*ptr)->addRef();
        return PVMFSuccess;
    }
    else
    {
        //not supported
        *ptr = NULL;
        return PVMFFailure;
    }
}


PVMFStatus PVMFRMFFParserNode::DoRequestPort(PVMFRMFFParserNodeCommand& aCmd, PVMFPortInterface*&aPort)
{
    LOGE ("PVMFRMFFParserNode::DoRequestPort");

    aPort = NULL;

    //retrieve port tag.
    int32 tag;
    PvmfMimeString* mimetype;
    aCmd.PVMFRMFFParserNodeCommandBase::Parse(tag, mimetype);
    if (!mimetype)
    {
        return PVMFErrArgument;//no mimetype supplied.
    }

    // Allocate a port based on the request
    // Return the pointer to the port in the command complete message

    // Determine the format type from the MIME type string
    PVMFFormatType formattype = mimetype->get_cstr();
    LOGE ("DoRequestPort mimetype = %s", mimetype->get_cstr());
    if (formattype == PVMF_MIME_FORMAT_UNKNOWN)
    {
        // Unknown track type
        return PVMFErrArgument;
    }

    // Determine the maximum track data size and queue depth based on the format type
    uint32 trackmaxdatasize = 0;
    uint32 trackmaxqueuedepth = 0;
    GetTrackMaxParameters(formattype, trackmaxdatasize, trackmaxqueuedepth);
    OSCL_ASSERT(trackmaxdatasize > 0 && trackmaxqueuedepth > 0);

    // Track ID is the port tag
    // @TODO might need validation on the port tag==track ID.
    int32 trackid = tag;
    if (trackid < 0)
    {
        return PVMFErrArgument;
    }

    //timestamp for tracks need not always start with zero
    //initialize the ts value to track timestamp start offset
    uint32 tsStartOffset = 0;

    //set the names for datapath logging
    OSCL_StackString<20> portname;
    OSCL_StackString<20> mempoolname;
    bool oTextTrack = false;
    if (formattype.isAudio())
    {
        portname = "PVMFRMFFParserParOut(Audio)";
        mempoolname = "PVMFRMFFParserPar(Audio)";
    }
    else if (formattype.isVideo())
    {
        portname = "PVMFRMFFParserParOut(Video)";
        mempoolname = "PVMFRMFFParserPar(Video)";
    }
    else if (formattype.isText())
    {
        oTextTrack = true;
        portname = "PVMFRMFFParserParOut(Misc)";
        mempoolname = "PVMFRMFFParserPar(Misc)";
    }

    int32 leavecode = 0;
    PVMFPortInterface* outport = NULL;
    MediaClockConverter* clockconv = NULL;
    OsclMemPoolResizableAllocator* trackdatamempool = NULL;
    PVMFResizableSimpleMediaMsgAlloc* mediadataimplalloc = NULL;
    PVMFTimedTextMediaDataAlloc* textmediadataimplalloc = NULL;
    PVMFMemPoolFixedChunkAllocator* mediadatamempool = NULL;
    OsclMemPoolFixedChunkAllocator* mediadatagroupimplmempool = NULL;
    PVMFMediaFragGroupCombinedAlloc<OsclMemAllocator>* mediadatagroupalloc = NULL;
    if (oTextTrack == false)
    {
        OSCL_TRY(leavecode,
                 outport = OSCL_NEW(PVMFRMFFParserOutPort, (tag, this, portname.get_str()));
                 clockconv = OSCL_NEW(MediaClockConverter, (MILLISECOND_TIMESCALE));

//#define MT6516_OMX_USE_PMEM 1
//#ifdef MT6516_OMX_USE_PMEM
//                 trackdatamempool = OSCL_NEW(OsclMemPoolResizableAllocator, (trackmaxqueuedepth * trackmaxdatasize, PVMF_RMFFPARSER_NODE_MEM_POOL_GROWTH_LIMIT, 0, &iInBufMemoryAlloc));
//#else
                 trackdatamempool = OSCL_NEW(OsclMemPoolResizableAllocator, (trackmaxqueuedepth * trackmaxdatasize, PVMF_RMFFPARSER_NODE_MEM_POOL_GROWTH_LIMIT));
//#endif
                 mediadataimplalloc = OSCL_NEW(PVMFResizableSimpleMediaMsgAlloc, (trackdatamempool));
                 mediadatamempool = OSCL_NEW(PVMFMemPoolFixedChunkAllocator, (mempoolname.get_str(), PVMF_RMFFPARSER_MEDIADATA_POOLNUM, PVMF_RMFFPARSER_MEDIADATA_CHUNKSIZE));
                 mediadatagroupimplmempool = OSCL_NEW(OsclMemPoolFixedChunkAllocator, (PVMF_RMFFPARSER_MEDIADATA_POOLNUM));
                 mediadatagroupalloc = OSCL_NEW(PVMFMediaFragGroupCombinedAlloc<OsclMemAllocator>, (PVMF_RMFFPARSER_MEDIADATA_POOLNUM, 20, mediadatagroupimplmempool));
                );
    }
    else
    {
    }

    bool memerr = false;
    if (oTextTrack == false)
    {
        if (leavecode || !outport || !clockconv || !trackdatamempool || !mediadataimplalloc ||
                !mediadatamempool || !mediadatagroupimplmempool || !mediadatagroupalloc)
        {
            memerr = true;
        }
    }
    else
    {
        if (leavecode || !outport || !clockconv || !trackdatamempool ||
                !textmediadataimplalloc || !mediadatamempool || !mediadatagroupimplmempool || !mediadatagroupalloc)
        {
            memerr = true;
        }
    }
    if (memerr == true)
    {
        if (outport)
        {
            OSCL_DELETE(((PVMFRMFFParserOutPort*)outport));
        }
        if (clockconv)
        {
            OSCL_DELETE(clockconv);
        }
        if (trackdatamempool)
        {
            trackdatamempool->removeRef();
            trackdatamempool = NULL;
        }
        if (mediadataimplalloc)
        {
            OSCL_DELETE(mediadataimplalloc);
        }
        if (textmediadataimplalloc)
        {
            OSCL_DELETE(textmediadataimplalloc);
        }
        if (mediadatamempool)
        {
            OSCL_DELETE(mediadatamempool);
        }
        if (mediadatagroupalloc)
        {
            mediadatagroupalloc->removeRef();
        }
        if (mediadatagroupimplmempool)
        {
            mediadatagroupimplmempool->removeRef();
        }
        return PVMFErrNoMemory;
    }

    mediadatagroupimplmempool->enablenullpointerreturn();
    trackdatamempool->enablenullpointerreturn();
    mediadatamempool->enablenullpointerreturn();

    mediadatagroupalloc->create();

    // Add the selected track/port to track list
    PVRMFFParserNodeTrackPortInfo trackportinfo;
    trackportinfo.iTrackId = trackid;
    trackportinfo.iPortInterface = outport;
    trackportinfo.iFormatType = formattype;
    // assign the integer format type based on the format type recieved
    // these are formats being used during media data flow, so just assign
    // integer values to these types, others defined as unknown.
    if (formattype == PVMF_MIME_MPEG4_AUDIO)
    {
        trackportinfo.iFormatTypeInteger = PVMF_RMFFPARSER_NODE_MPEG4_AUDIO;
    }
    else
    {
        trackportinfo.iFormatTypeInteger = PVMF_RMFFPARSER_NODE_FORMAT_UNKNOWN;
    }
    RetrieveTrackConfigInfo(trackid,
                            formattype,
                            trackportinfo.iFormatSpecificConfig);

    if (formattype == PVMF_MIME_MPEG4_AUDIO)
    {
        RetrieveTrackConfigInfoAndFirstSample(trackid,
                                              formattype,
                                              trackportinfo.iFormatSpecificConfigAndFirstSample);

    }

    trackportinfo.iMimeType = (*mimetype);
    trackportinfo.iClockConverter = clockconv;
    trackportinfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_UNINITIALIZED;
    trackportinfo.iTrackMaxDataSize = trackmaxdatasize;
    trackportinfo.iTrackMaxQueueDepth = trackmaxqueuedepth;
    trackportinfo.iTrackDataMemoryPool = trackdatamempool;
    trackportinfo.iMediaDataImplAlloc = mediadataimplalloc;
    trackportinfo.iTextMediaDataImplAlloc = textmediadataimplalloc;
    trackportinfo.iMediaDataMemPool = mediadatamempool;
    trackportinfo.iMediaDataGroupImplMemPool = mediadatagroupimplmempool;
    trackportinfo.iMediaDataGroupAlloc = mediadatagroupalloc;
    trackportinfo.iNode = OSCL_STATIC_CAST(OsclTimerObject* , this);
    trackportinfo.iTimestamp = tsStartOffset;
    trackportinfo.iSeqNum = 0;

    // TEMP: Number of samples to retrieve should be negotiated between
    // the nodes but for now hardcode the values
    // By default retrieve one bundle of samples from the file format parser
    if (formattype == PVMF_MIME_MPEG4_AUDIO)
    {
        trackportinfo.iNumSamples = MPEG4_AUDIO_NUMSAMPLES;
    }
    else if (formattype == PVMF_MIME_REAL_AUDIO)
    {
    }
    else
    {
        trackportinfo.iNumSamples = UNKNOWN_NUMSAMPLES;
    }

    iNodeTrackPortList.push_back(trackportinfo);
    aPort = outport;

    return PVMFSuccess;
}


void PVMFRMFFParserNode::GetTrackMaxParameters(PVMFFormatType aFormatType, uint32& aMaxDataSize, uint32& aMaxQueueDepth)
{
    if (aFormatType == PVMF_MIME_MPEG4_AUDIO)
    {
        aMaxDataSize = MPEG4_AUDIO_MAXTRACKDATASIZE;
        aMaxQueueDepth = MPEG4_AUDIO_MAXTRACKQUEUEDEPTH;
    }

#ifdef DECODE_COOK_FROM_PARSER
     if (aFormatType == PVMF_MIME_PCM16)
    {
        aMaxDataSize = 15360;
        aMaxQueueDepth = MPEG4_AUDIO_MAXTRACKQUEUEDEPTH;
    }
#endif

     if (aFormatType == PVMF_MIME_REAL_AUDIO)
    {
        aMaxDataSize = RM_AUDIO_MAXTRACKDATASIZE;
        aMaxQueueDepth = RM_AUDIO_MAXTRACKQUEUEDEPTH;
    }

}


PVMFStatus PVMFRMFFParserNode::DoReleasePort(PVMFRMFFParserNodeCommand& aCmd)
{
    LogDiagnostics();
    //Find the port in the port vector
    PVMFPortInterface* port;
    aCmd.PVMFRMFFParserNodeCommandBase::Parse(port);

    // Remove the selected track from the track list
    int32 i = 0;
    int32 maxtrack = iNodeTrackPortList.size();
    while (i < maxtrack)
    {
        if (iNodeTrackPortList[i].iPortInterface == port)
        {
            // Found the element. So erase it
            iNodeTrackPortList[i].iMediaData.Unbind();
            if (iNodeTrackPortList[i].iPortInterface)
            {
                OSCL_DELETE(((PVMFRMFFParserOutPort*)iNodeTrackPortList[i].iPortInterface));
            }
            if (iNodeTrackPortList[i].iClockConverter)
            {
                OSCL_DELETE(iNodeTrackPortList[i].iClockConverter);
            }
            if (iNodeTrackPortList[i].iTrackDataMemoryPool)
            {
                iNodeTrackPortList[i].iTrackDataMemoryPool->CancelFreeChunkAvailableCallback();
                iNodeTrackPortList[i].iTrackDataMemoryPool->removeRef();
                iNodeTrackPortList[i].iTrackDataMemoryPool = NULL;
            }
            if (iNodeTrackPortList[i].iMediaDataImplAlloc)
            {
                OSCL_DELETE(iNodeTrackPortList[i].iMediaDataImplAlloc);
            }
            if (iNodeTrackPortList[i].iTextMediaDataImplAlloc)
            {
                OSCL_DELETE(iNodeTrackPortList[i].iTextMediaDataImplAlloc);
            }
            if (iNodeTrackPortList[i].iMediaDataMemPool)
            {
                iNodeTrackPortList[i].iMediaDataMemPool->CancelFreeChunkAvailableCallback();
                iNodeTrackPortList[i].iMediaDataMemPool->removeRef();
            }
            if (iNodeTrackPortList[i].iMediaDataGroupAlloc)
            {
                iNodeTrackPortList[i].iMediaDataGroupAlloc->removeRef();
            }
            if (iNodeTrackPortList[i].iMediaDataGroupImplMemPool)
            {
                iNodeTrackPortList[i].iMediaDataGroupImplMemPool->removeRef();
            }

            iNodeTrackPortList.erase(iNodeTrackPortList.begin() + i);
            return PVMFSuccess;
        }
        ++i;
    }

    if (i >= maxtrack)
    {
        return PVMFErrBadHandle;
    }

    // Unknown port
    return PVMFFailure;
}



PVMFStatus PVMFRMFFParserNode::DoInit(PVMFRMFFParserNodeCommand& aCmd)
{
    LOGE ("PVMFRMFFParserNode::DoInit()");

    OSCL_UNUSED_ARG(aCmd);

    if (iInterfaceState != EPVMFNodeIdle)
    {
        // Wrong state
        return PVMFErrInvalidState;
    }

    return PVMFSuccess;
}
// ]


PVMFStatus PVMFRMFFParserNode::DoPrepare(PVMFRMFFParserNodeCommand& /*aCmd*/)
{
    LOGE ("PVMFRMFFParserNode::DoPrepare()");
    
    if (iInterfaceState != EPVMFNodeInitialized)
    {
        return PVMFErrInvalidState;
    }
    
    return PVMFSuccess;
}

void PVMFRMFFParserNode::CompletePrepare(PVMFStatus aStatus)
{
    CommandComplete(iCurrentCommand, iCurrentCommand.front(), aStatus);
}


PVMFStatus PVMFRMFFParserNode::DoStart(PVMFRMFFParserNodeCommand& /*aCmd*/)
{
    LOGE ("PVMFRMFFParserNode::DoStart()");
    
    if (iInterfaceState != EPVMFNodePrepared &&
            iInterfaceState != EPVMFNodePaused)
    {
        return PVMFErrInvalidState;
    }

    // If resuming, do not reset the auto-pause variables
    // parser node should send InfoReadyEvent to Engine
    // if in underflow condition.

    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoStop(PVMFRMFFParserNodeCommand& aCmd)
{
    LOGE ("PVMFRMFFParserNode::DoStop()");
    OSCL_UNUSED_ARG(aCmd);

    iStreamID = 0;
    if (iInterfaceState != EPVMFNodeStarted &&
            iInterfaceState != EPVMFNodePaused)
    {
        return PVMFErrInvalidState;
    }

    // stop and reset position to beginning
    ResetAllTracks();

    // reset direction rate state variables
    iPlayBackDirection = PVMF_DATA_SOURCE_DIRECTION_FORWARD;
    iParseAudioDuringFF = false;
    iParseAudioDuringREW = false;
    iParseVideoOnly = false;
    iDataRate = NORMAL_PLAYRATE;

    for (uint32 i = 0; i < iNodeTrackPortList.size(); i++)
    {
         iNodeTrackPortList[i].iTimestamp = 0;
    }

    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoFlush(PVMFRMFFParserNodeCommand& aCmd)
{
    OSCL_UNUSED_ARG(aCmd);

    if (iInterfaceState != EPVMFNodeStarted &&
            iInterfaceState != EPVMFNodePaused)
    {
        return PVMFErrInvalidState;
    }

    //the flush is asynchronous.  Completion is detected in the Run.
    //Make sure the AO is active to finish the flush..
    RunIfNotReady();
    return PVMFPending;
}


bool PVMFRMFFParserNode::FlushPending()
{
    return (iCurrentCommand.size() > 0 && iCurrentCommand.front().iCmd == PVMF_GENERIC_NODE_FLUSH);
}


PVMFStatus PVMFRMFFParserNode::DoPause(PVMFRMFFParserNodeCommand& aCmd)
{
    OSCL_UNUSED_ARG(aCmd);

    if (iInterfaceState != EPVMFNodeStarted)
    {
        return PVMFErrInvalidState;
    }

    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoReset(PVMFRMFFParserNodeCommand& aCmd)
{
    OSCL_UNUSED_ARG(aCmd);

    //remove the clock observer
    if (iClientPlayBackClock != NULL)
    {
        if (iClockNotificationsInf != NULL)
        {
            iClockNotificationsInf->RemoveClockStateObserver(*this);
            iClientPlayBackClock->DestroyMediaClockNotificationsInterface(iClockNotificationsInf);
            iClockNotificationsInf = NULL;
        }
    }

    if (iUnderFlowCheckTimer != NULL)
    {
        iUnderFlowCheckTimer->Clear();
    }

    // reset direction rate state variables
    iPlayBackDirection = PVMF_DATA_SOURCE_DIRECTION_FORWARD;
    iParseAudioDuringFF = false;
    iParseAudioDuringREW = false;
    iParseVideoOnly = false;
    iDataRate = NORMAL_PLAYRATE;


     ReleaseAllPorts();
     CleanupFileSource();
     iSelectedTrackInfoList.clear();
     SetState(EPVMFNodeIdle);
            return PVMFSuccess;
}

void PVMFRMFFParserNode::CompleteReset(PVMFRMFFParserNodeCmdQueue& aCmdQ, PVMFRMFFParserNodeCommand& aCmd)
{
    // stop and cleanup

    ReleaseAllPorts();
    CleanupFileSource();
    iSelectedTrackInfoList.clear();

    CommandComplete(aCmdQ, aCmd, PVMFSuccess);

    return;
}


PVMFStatus PVMFRMFFParserNode::DoCancelAllCommands(PVMFRMFFParserNodeCommand& /*aCmd*/)
{
    // The "current command" queue is used to hold different asynchronous commands
    // 1) Init command during Local Playback or when there is no DS cmd pending, we will have to wait for
    // CPM to be initalised. All asynchronous CPM cmds needs to be completed to complete Init.
    // 2) Init command when partial MOOV is downlaoded during PDL and PS - can be cancelled.
    // 3) Prepare command when parser node requests for 4 secs of data to datastream before
    // sending prepare complete - can be cancelled.
    if (!iCurrentCommand.empty())
    {
        PVMFStatus retVal = PVMFSuccess;
        retVal = DoCancelCurrentCommand(iCurrentCommand[0]);
        if (retVal == PVMFPending)
        {
            return retVal;
        }
    }

    //cancel all queued commands
    //start at element 1 since this cancel command is element 0.
    while (iInputCommands.size() > 1)
    {
        CommandComplete(iInputCommands, iInputCommands[1], PVMFErrCancelled);
    }

    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoCancelCommand(PVMFRMFFParserNodeCommand& aCmd)
{
    //extract the command ID from the parameters.
    PVMFCommandId id;
    aCmd.PVMFRMFFParserNodeCommandBase::Parse(id);

    //first check "current" command if any
    PVMFRMFFParserNodeCommand* cmd = iCurrentCommand.FindById(id);
    // The "current command" queue is used to hold different asynchronous commands
    // 1) Init command during Local Playback or when there is no DS cmd pending, we will have to wait for
    // CPM to be initalised. All asynchronous CPM cmds needs to be completed to complete Init.
    // 2) Init command when partial MOOV is downlaoded during PDL and PS - can be cancelled.
    // 3) Prepare command when parser node requests for 4 secs of data to datastream before
    // sending prepare complete - can be cancelled.
    if (cmd)
    {
        PVMFStatus retVal = PVMFSuccess;
        retVal = DoCancelCurrentCommand(*cmd);
        if (retVal == PVMFPending)
        {
            return retVal;
        }
    }

    //next check input queue.
    //start at element 1 since this cancel command is element 0.
    cmd = iInputCommands.FindById(id, 1);
    if (cmd)
    {
        //cancel the queued command
        CommandComplete(iInputCommands, *cmd, PVMFErrCancelled);
        //report cancel success
        return PVMFSuccess;
    }

    //if we get here the command isn't queued so the cancel fails.
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::DoCancelCurrentCommand(PVMFRMFFParserNodeCommand& aCmd)
{
    if (aCmd.iCmd == PVMF_GENERIC_NODE_INIT)
    {
        {
            // wait on cpm commands completion.
            return PVMFPending;
        }
    }
    else if (aCmd.iCmd == PVMF_GENERIC_NODE_PREPARE)
    {
    }
    else
    {
        return PVMFPending;
    }
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::DoSetDataSourcePosition(PVMFRMFFParserNodeCommand& aCmd, PVMFStatus &aEventCode, PVUuid &aEventUuid)
{
	aEventCode = PVMFSuccess;

	CleanRaBlockQueue();

	uint32 targetNPT = 0;
	uint32* actualNPT = NULL;
	uint32* actualMediaDataTS = NULL;
	bool seektosyncpoint = false;
	uint32 streamID = 0;

	aCmd.PVMFRMFFParserNodeCommand::Parse(targetNPT, actualNPT, actualMediaDataTS, seektosyncpoint, streamID);

	LOGE ("PVMFRMFFParserNode::DoSetDataSourcePosition targetNPT(%u), seektosyncpoint(%d), streamID(%d)\n", targetNPT, seektosyncpoint, streamID);

	for (int i = 0; i < iNodeTrackPortList.size(); ++i)
	{
		iNodeTrackPortList[i].iSendBOS = true;
	}

	iStreamID = streamID;

	// Validate the parameters
	if (actualNPT == NULL || actualMediaDataTS == NULL)
	{
		return PVMFErrArgument;
	}
    
	MediaMetaInfo info;
	int i = 0;
	*actualMediaDataTS = 0;
	for (i = 0; i < iNodeTrackPortList.size(); i++)
	{
		iNodeTrackPortList[i].iClockConverter->set_clock(iNodeTrackPortList[i].iTimestamp, 0);
		uint32 millisecTS = iNodeTrackPortList[i].iClockConverter->get_converted_ts(1000);

		LOGE ("millisecTS[%d]=%u", i, millisecTS);
		// Actual media data TS is the max timestamp of all selected tracks
		if (millisecTS > *actualMediaDataTS)
		{
			*actualMediaDataTS = millisecTS;
		}
	}
    
	uint64 duration64 = rm_parser_get_duration(iRmParser);
	uint32 durationms = Oscl_Int64_Utils::get_uint64_lower32(duration64);

	LOGE ("PVMFRMFFParserNode::DoSetDataSourcePosition targetNPT=%u, duration64=%llu, durationms=%u", targetNPT, duration64, durationms);

	if ((targetNPT >= (durationms - PVRMFF_SEEK_BOUNDARY_PROTECTION_IN_MS)) && (PVMF_DATA_SOURCE_DIRECTION_REVERSE != iPlayBackDirection))
	{      
		if(!RMSeekTo(0))
		{
			return PVMFFailure;
		}

		//report EOT for all streams.
		for (i = 0; i < iNodeTrackPortList.size(); i++)
		{
			iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK;
			// reset all tracks to zero.
			iNodeTrackPortList[i].iClockConverter->set_clock_other_timescale(*actualMediaDataTS, 1000);
			iNodeTrackPortList[i].iTimestamp = iNodeTrackPortList[i].iClockConverter->get_current_timestamp();
		}

		*actualNPT = durationms;
		return PVMFSuccess;
	}

	LOGE("PVMFRMFFParserNode::DoSetDataSourcePosition() *actualMediaDataTS %d", *actualMediaDataTS);

	*actualNPT = targetNPT; // init *actualNPT to targetNPT
	uint32 tempNPT = 0;
	uint32 tempTrackId = 0;

	bool bSeekOverVideoEOS = false;
	int32 minFileOffset = 0x7FFFFFFF;

	if(!RMSeekTo(targetNPT))
	{
		return PVMFFailure;
	}	

	*actualNPT = targetNPT;

	uint32 numSamples = 1;
	uint32 mints = 0xFFFFFFFF;
	int32 retval = RM_EVERYTHING_FINE;
	int32 *retValPerTrack = NULL;
	uint32 *retNumSamplesPerTrack = NULL;
	uint32 *trackTSAfterRepo = NULL;

	trackTSAfterRepo = (uint32*) OSCL_MALLOC(iNodeTrackPortList.size() * sizeof(uint32));
	retValPerTrack = (int32*) OSCL_MALLOC(iNodeTrackPortList.size() * sizeof(int32));
	retNumSamplesPerTrack = (uint32*) OSCL_MALLOC(iNodeTrackPortList.size() * sizeof(uint32));

	if ((trackTSAfterRepo == NULL) || (retValPerTrack == NULL) || (retNumSamplesPerTrack == NULL))
	{
		LOGE ("PVMFRMFFParserNode::DoSetDataSourcePosition() Memory alloc for array to keep the timestamp of the samples failed");
		OSCL_FREE(trackTSAfterRepo);
		trackTSAfterRepo = NULL;
		OSCL_FREE(retValPerTrack);
		retValPerTrack = NULL;
		OSCL_FREE(retNumSamplesPerTrack);
		retNumSamplesPerTrack = NULL;
		return PVMFErrNoMemory;
	}

	for (i = 0; i < iNodeTrackPortList.size(); i++)
	{
		numSamples = 1;
		trackTSAfterRepo[i] = targetNPT;
		retNumSamplesPerTrack[i] = numSamples;
		retValPerTrack[i] = retval;
		iNodeTrackPortList[i].iClockConverter->set_clock(targetNPT, 0);
	}

	mints = targetNPT;
	
	if (mints == 0xFFFFFFFF)
	{
		mints = *actualNPT;
		LOGE ("PVMFRMFFParserNode::DoSetDataSourcePosition() Minimum timestamp could not be determined so using the actual NPT %d", mints);
	}
	else if (mints != *actualNPT)
	{
		*actualNPT = mints;
	}

	for (i = 0; i < iNodeTrackPortList.size(); i++)
	{
		if (retValPerTrack[i] == RM_EVERYTHING_FINE)
		{
            		if (retNumSamplesPerTrack[i] > 0)
			{
				iNodeTrackPortList[i].iClockConverter->set_clock(trackTSAfterRepo[i], 0);
				uint32 trackts = iNodeTrackPortList[i].iClockConverter->get_converted_ts(1000);
				iNodeTrackPortList[i].iClockConverter->set_clock_other_timescale(*actualMediaDataTS, 1000);
			}
			else
			{
				// Since sample is not available, just set the track timestamp to the calculated starting media data TS
				iNodeTrackPortList[i].iClockConverter->set_clock_other_timescale(*actualMediaDataTS, 1000);
			}

			iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;

			iNodeTrackPortList[i].iTimestamp = iNodeTrackPortList[i].iClockConverter->get_current_timestamp();
			iNodeTrackPortList[i].iFirstFrameAfterRepositioning = true;
			iNodeTrackPortList[i].iCurrentTextSampleEntry.Unbind();
			iNodeTrackPortList[i].iTargetNPTInMediaTimeScale = targetNPT;
        	}
	}

     	LOGE("PVMFRMFFParserNode::DoSetDataSourcePosition: targetNPT=%d, actualNPT=%d, actualTS=%d", targetNPT, *actualNPT, *actualMediaDataTS);
 
	OSCL_FREE(trackTSAfterRepo);
	trackTSAfterRepo = NULL;
	OSCL_FREE(retValPerTrack);
	retValPerTrack = NULL;
	OSCL_FREE(retNumSamplesPerTrack);
	retNumSamplesPerTrack = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::DoQueryDataSourcePosition(PVMFRMFFParserNodeCommand& aCmd)
{
    LOGE ("PVMFRMFFParserNode::DoQueryDataSourcePosition\n");
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::DoSetDataSourceRate(PVMFRMFFParserNodeCommand& aCmd)
{
    LOGE ("PVMFRMFFParserNode::DoSetDataSourceRate\n");
    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoSetDataSourceDirection(PVMFRMFFParserNodeCommand& aCmd)
{
    return PVMFSuccess;
}

void PVMFRMFFParserNode::HandleTrackState()
{
    //LOGE ("PVMFRMFFParserNode::HandleTrackState() In");

    for (uint32 i = 0; i < iNodeTrackPortList.size(); ++i)
    {
        switch (iNodeTrackPortList[i].iState)
        {
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_UNINITIALIZED:
                if (RetrieveTrackConfigInfo(iNodeTrackPortList[i].iTrackId, iNodeTrackPortList[i].iFormatType, iNodeTrackPortList[i].iFormatSpecificConfig) == false)
                {
                    // Failed
                    break;
                }
                iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;
                // Continue on to retrieve the first frame

            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA:
            {
                if (iNodeTrackPortList[i].iSendBOS)
                {
                    if (!SendBeginOfMediaStreamCommand(iNodeTrackPortList[i]))
                        break;
                }
                if (iNodeTrackPortList[i].iFirstFrameAfterRepositioning)
                {
                    //after repo, let the track with min file offset retrieve data first
                    uint32 j = 0;
                    for (j = 0; j < iNodeTrackPortList.size(); ++j)
                    {
                        if (minFileOffsetTrackID == iNodeTrackPortList[j].iTrackId)
                        {
                            break;
                        }
                    }
                    if ((i != j) && (iNodeTrackPortList[j].iFirstFrameAfterRepositioning))
                    {
                        //LOGE("Ln %d UGLY? Yes. minFileOffsetTrackID %d Skipped iTrackId %d", __LINE__, minFileOffsetTrackID , iNodeTrackPortList[j].iTrackId);
                        break;
                    }
                }

                uint32 _iSeqNum = iNodeTrackPortList[i].iSeqNum;
                if (start_retrieve_time == 0 && IsVideoTrack((char*)iNodeTrackPortList[i].iMimeType.get_cstr()))
                {
                    start_retrieve_time = OsclTickCount::getTimeMs();
                }
                
                if (!RetrieveTrackData(iNodeTrackPortList[i]))
                {
                    start_retrieve_time = 0;
                    if (iNodeTrackPortList[i].iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK)
                    {
                        RunIfNotReady();
                    }
                    else if (iNodeTrackPortList[i].iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SKIP_CORRUPT_SAMPLE)
                    {
                        iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;
                        RunIfNotReady();
                    }
                     else if (iNodeTrackPortList[i].iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA)
                    {
                        iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;
                        RunIfNotReady();
                    }
                    break;
                }

                if (_iSeqNum != iNodeTrackPortList[i].iSeqNum && IsVideoTrack((char*)iNodeTrackPortList[i].iMimeType.get_cstr()))
                {
                    OsclProfile::Push(OsclProfile::iRetrieveTrackDataInTime, _iSeqNum, start_retrieve_time);
                    OsclProfile::Push(OsclProfile::iRetrieveTrackDataOutTime, _iSeqNum);
                    start_retrieve_time = 0;
                }

                if (iNodeTrackPortList[i].iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRACKMAXDATASIZE_RESIZE)
                {
                    iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;
                    RunIfNotReady();
                    break;
                }
                else
                {
                    iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_SENDDATA;
                }
                // Continue on to send the frame
            }
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_SENDDATA:
                if (SendTrackData(iNodeTrackPortList[i]))
                {
                    iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;
                    RunIfNotReady();
                }
                break;

            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK:
                if (iNodeTrackPortList[i].iSendBOS)
                {
                    if (!SendBeginOfMediaStreamCommand(iNodeTrackPortList[i]))
                        break;
                }
                if (SendEndOfTrackCommand(iNodeTrackPortList[i]))
                {
                    // EOS command sent successfully
                    iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_ENDOFTRACK;
                    ReportRMFFParserInfoEvent(PVMFInfoEndOfData);
                }
                else
                {
                    // EOS command sending failed -- wait on outgoing queue ready notice
                    // before trying again.
                }
                break;

            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRACKDATAPOOLEMPTY:
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_MEDIADATAPOOLEMPTY:
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_INITIALIZED:
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DESTFULL:
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_INSUFFICIENTDATA:
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_ENDOFTRACK:
            case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_ERROR:
            default:
                // Do nothing for these states for now
                break;
        }
    }

}

bool PVMFRMFFParserNode::RetrieveTrackConfigInfo(uint32 aTrackId, PVMFFormatType aFormatType, OsclRefCounterMemFrag &aConfig)
{
        if (aFormatType == REALVIDEO_MIME_TYPE) {
// ]
        	}


//    get audio decoder config 
        if (aFormatType == PVMF_MIME_MPEG4_AUDIO)
        {
       	if (iRaFormatInfo != NULL)
        	{
        	       if ((iRaFormatInfo->pOpaqueData == HXNULL) || (iRaFormatInfo->ulOpaqueDataSize == 0))
        	       {
        	       	LOGE ("RetrieveTrackConfigInfo OpaqueData is NULL (AAC)");
        	       }
        	       
        		// Create mem frag for decoder specific config
		       OsclMemAllocDestructDealloc<uint8> my_alloc;
		       OsclRefCounter* my_refcnt;
        	 	uint aligned_refcnt_size = oscl_mem_aligned_size(sizeof(OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >));
        		uint8* my_ptr = NULL;
        		int32 errcode = 0;
        		OSCL_TRY(errcode, my_ptr = (uint8*) my_alloc.ALLOCATE(aligned_refcnt_size + iRaFormatInfo->ulOpaqueDataSize));
        		OSCL_FIRST_CATCH_ANY(errcode,
                             		LOGE ("PVMFRMFFParserNode::RetrieveTrackConfigInfo() Memory allocation failed size = %d", aligned_refcnt_size + iRaFormatInfo->ulOpaqueDataSize);
                             		return false;
                            		);

        		my_refcnt = OSCL_PLACEMENT_NEW(my_ptr, OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >(my_ptr));
        		my_ptr += aligned_refcnt_size;

        		OsclMemoryFragment memfrag;
        		memfrag.len = iRaFormatInfo->ulOpaqueDataSize;
        		memfrag.ptr = my_ptr;

			int32 specinfosize = iRaFormatInfo->ulOpaqueDataSize;
        		// Copy the decoder specific info to the memory fragment
        		oscl_memcpy(memfrag.ptr, iRaFormatInfo->pOpaqueData+1, specinfosize-1);

        		// Save format specific info
        		aConfig = OsclRefCounterMemFrag(memfrag, my_refcnt, specinfosize-1);        		

                      uint8 audioObjectType;
                      uint8 sampleRateIndex;
                      uint32 num_channels = 0;
                      
			 uint8 cfgType = iRaFormatInfo->pOpaqueData[0];

			 if (cfgType == 2) // MPEG4 audio
			 {
				 uint8* specific = iRaFormatInfo->pOpaqueData+1;
				 specinfosize--;
                      
    			        GetActualAacConfig(specific,
                     				         &audioObjectType,
				                               &specinfosize,
				                               &sampleRateIndex,
				                               &num_channels);
                        
    		        	LOGE ("audioObjectType=%d, sampleRateIndex=%d, num_channels=%d", audioObjectType, sampleRateIndex, num_channels);
			 }
			 else
			 {
			 	LOGE ("Error, MPEG4 audio cfgType != 2 (cfgType=%d)", cfgType);
			 }
        	}
        	else
        	{
        	    LOGE ("ERROR RetrieveTrackConfigInfo iRaFormatInfo is NULL !!!");
        	}
        }
#ifdef DECODE_COOK_FROM_PARSER
	else if (aFormatType == PVMF_MIME_PCM16)
	{
		// Create mem frag for decoder specific config
		OsclMemAllocDestructDealloc<uint8> my_alloc;
		OsclRefCounter* my_refcnt;
		uint aligned_refcnt_size = oscl_mem_aligned_size(sizeof(OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >));
		uint8* my_ptr = NULL;
		int32 errcode = 0;
		OSCL_TRY(errcode, my_ptr = (uint8*) my_alloc.ALLOCATE(aligned_refcnt_size + sizeof(channelSampleInfo)));
		OSCL_FIRST_CATCH_ANY(errcode,
                             		LOGE ("PVMFRMFFParserNode::RetrieveTrackConfigInfo() Memory allocation failed size = %d", aligned_refcnt_size + sizeof(channelSampleInfo));
                             		return false;
                            		);

		my_refcnt = OSCL_PLACEMENT_NEW(my_ptr, OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >(my_ptr));
		my_ptr += aligned_refcnt_size;

		OsclMemoryFragment memfrag;
		memfrag.len = sizeof(channelSampleInfo);
		memfrag.ptr = my_ptr;

		// Copy the decoder specific info to the memory fragment
              ((channelSampleInfo*)memfrag.ptr)->desiredChannels = 2;
              ((channelSampleInfo*)memfrag.ptr)->samplingRate = 22050;
              ((channelSampleInfo*)memfrag.ptr)->bitsPerSample = 16;
              ((channelSampleInfo*)memfrag.ptr)->buffer_size = 12288;

		// Save format specific info
		aConfig = OsclRefCounterMemFrag(memfrag, my_refcnt, memfrag.len);        		
	}
#endif
	else if (aFormatType == PVMF_MIME_REAL_AUDIO)
	{
       	if (iRaFormatInfo != NULL)
        	{
        	       if ((iRaFormatInfo->pOpaqueData == HXNULL) || (iRaFormatInfo->ulOpaqueDataSize == 0))
        	       {
        	       	LOGE ("RetrieveTrackConfigInfo OpaqueData is NULL (PVMF_MIME_REAL_AUDIO)");
        	       	return false;
        	       }

        	       ra8lbr_data unpackedData;
      			BYTE* ptr = iRaFormatInfo->pOpaqueData;
		     	unpackedData.version = ((INT32)*ptr++)<<24;
         		unpackedData.version |= ((INT32)*ptr++)<<16;
         		unpackedData.version |= ((INT32)*ptr++)<<8;
         		unpackedData.version |= ((INT32)*ptr++);

         		unpackedData.nSamples = *ptr++<<8;
         		unpackedData.nSamples |= *ptr++;

         		unpackedData.nRegions = *ptr++<<8;
         		unpackedData.nRegions |= *ptr++;

         		if (unpackedData.version >= GECKO_VERSION)
         		{
         			unpackedData.delay = ((INT32)*ptr++)<<24;
            			unpackedData.delay |= ((INT32)*ptr++)<<16;
            			unpackedData.delay |= ((INT32)*ptr++)<<8;
            			unpackedData.delay |= ((INT32)*ptr++);

            			unpackedData.cplStart = *ptr++<<8;
            			unpackedData.cplStart |= *ptr++;

            			unpackedData.cplQBits = *ptr++<<8;
            			unpackedData.cplQBits |= *ptr++;
         		}
         		else
         		{
           			 /* the fixed point ra8lbr decoder supports dual-mono decoding with
               		a single decoder instance if cplQBits is set to zero. */
            			unpackedData.cplStart = 0;
            			unpackedData.cplQBits = 0;
         		}

                     RA_CODEC_SPECIFIC_DATA _ra_codec_specific;
         		memset(&_ra_codec_specific, 0, sizeof(RA_CODEC_SPECIFIC_DATA));

         		_ra_codec_specific.ulSampleRate = iRaFormatInfo->ulSampleRate;
       		_ra_codec_specific.ulActualRate = iRaFormatInfo->ulActualRate;
       		_ra_codec_specific.usBitsPerSample = iRaFormatInfo->usBitsPerSample;
       		_ra_codec_specific.usNumChannels = iRaFormatInfo->usNumChannels;
       		_ra_codec_specific.usAudioQuality = iRaFormatInfo->usAudioQuality;
       		_ra_codec_specific.usFlavorIndex = iRaFormatInfo->usFlavorIndex;
       		_ra_codec_specific.ulBitsPerFrame = iRaFormatInfo->ulBitsPerFrame;
       		_ra_codec_specific.ulGranularity = iRaFormatInfo->ulGranularity;
       		_ra_codec_specific.nSamples = unpackedData.nSamples;
       		_ra_codec_specific.nChannels = iRaFormatInfo->usNumChannels;
       		_ra_codec_specific.nRegions = unpackedData.nRegions;
       		_ra_codec_specific.nFrameBits = iRaFormatInfo->ulBitsPerFrame*8;
       		_ra_codec_specific.sampRate = iRaFormatInfo->ulSampleRate;
       		_ra_codec_specific.cplStart = unpackedData.cplStart;
       		_ra_codec_specific.cplQbits = unpackedData.cplQBits;

                     /*
                     LOGE ("----------RetrieveTrackConfigInfo--------");
   			LOGE ("_ra_codec_specific.ulSampleRate=%d", _ra_codec_specific.ulSampleRate);
   			LOGE ("_ra_codec_specific.ulActualRate=%d", _ra_codec_specific.ulActualRate);
   			LOGE ("_ra_codec_specific.usBitsPerSample=%d", _ra_codec_specific.usBitsPerSample);
   			LOGE ("_ra_codec_specific.usNumChannels=%d", _ra_codec_specific.usNumChannels);
   			LOGE ("_ra_codec_specific.usAudioQuality=%d", _ra_codec_specific.usAudioQuality);
   			LOGE ("_ra_codec_specific.usFlavorIndex=%d", _ra_codec_specific.usFlavorIndex);
   			LOGE ("_ra_codec_specific.ulBitsPerFrame=%d", _ra_codec_specific.ulBitsPerFrame);
   			LOGE ("_ra_codec_specific.ulGranularity=%d", _ra_codec_specific.ulGranularity);
   			LOGE ("_ra_codec_specific.nSamples=%d", _ra_codec_specific.nSamples);
   			LOGE ("_ra_codec_specific.nChannels=%d", _ra_codec_specific.nChannels);
   			LOGE ("_ra_codec_specific.nRegions=%d", _ra_codec_specific.nRegions);
   			LOGE ("_ra_codec_specific.nFrameBits=%d", _ra_codec_specific.nFrameBits);
   			LOGE ("_ra_codec_specific.sampRate=%d", _ra_codec_specific.sampRate);
   			LOGE ("_ra_codec_specific.cplStart=%d", _ra_codec_specific.cplStart);
  			LOGE ("_ra_codec_specific.cplQbits=%d", _ra_codec_specific.cplQbits);
  			*/


   			int32 _specific_info_size = sizeof(RA_CODEC_SPECIFIC_DATA);
        		// Create mem frag for decoder specific config
		       OsclMemAllocDestructDealloc<uint8> my_alloc;
		       OsclRefCounter* my_refcnt;
        	 	uint aligned_refcnt_size = oscl_mem_aligned_size(sizeof(OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >));
        		uint8* my_ptr = NULL;
        		int32 errcode = 0;
        		OSCL_TRY(errcode, my_ptr = (uint8*) my_alloc.ALLOCATE(aligned_refcnt_size + _specific_info_size));
        		OSCL_FIRST_CATCH_ANY(errcode,
                             		LOGE ("PVMFRMFFParserNode::RetrieveTrackConfigInfo() Memory allocation failed size = %d", aligned_refcnt_size + _specific_info_size);
                             		return false;
                            		);

        		my_refcnt = OSCL_PLACEMENT_NEW(my_ptr, OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >(my_ptr));
        		my_ptr += aligned_refcnt_size;

        		OsclMemoryFragment memfrag;
        		memfrag.len = _specific_info_size;
        		memfrag.ptr = my_ptr;

        		oscl_memcpy(memfrag.ptr, &_ra_codec_specific, _specific_info_size);

        		// Save format specific info
        		aConfig = OsclRefCounterMemFrag(memfrag, my_refcnt, _specific_info_size);        
        	}
        	else
        	{
        	    LOGE ("ERROR RetrieveTrackConfigInfo iRaFormatInfo is NULL (PVMF_MIME_REAL_AUDIO)!!!");
        	}

	}

            return true;
}



bool PVMFRMFFParserNode::RetrieveTrackConfigInfoAndFirstSample(uint32 aTrackId,
        PVMFFormatType aFormatType,
        OsclRefCounterMemFrag &aConfig)
{
	LOGD("RetrieveTrackConfigInfoAndFirstSample: Format Type= %s", aFormatType.getMIMEStrPtr());
	if (aFormatType == PVMF_MIME_MPEG4_AUDIO)
	{
       	if (iRaFormatInfo != NULL)
        	{
        	       if ((iRaFormatInfo->pOpaqueData == HXNULL) || (iRaFormatInfo->ulOpaqueDataSize == 0))
        	       {
        	       	LOGE ("RetrieveTrackConfigInfoAndFirstSample OpaqueData is NULL (AAC)");
        	       }
        	       
        		// Create mem frag for decoder specific config
		       OsclMemAllocDestructDealloc<uint8> my_alloc;
		       OsclRefCounter* my_refcnt;
        	 	uint aligned_refcnt_size = oscl_mem_aligned_size(sizeof(OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >));
        		uint8* my_ptr = NULL;
        		int32 errcode = 0;
        		OSCL_TRY(errcode, my_ptr = (uint8*) my_alloc.ALLOCATE(aligned_refcnt_size + iRaFormatInfo->ulOpaqueDataSize));
        		OSCL_FIRST_CATCH_ANY(errcode,
                             		LOGE ("PVMFRMFFParserNode::RetrieveTrackConfigInfo() Memory allocation failed size = %d", aligned_refcnt_size + iRaFormatInfo->ulOpaqueDataSize);
                             		return false;
                            		);

        		my_refcnt = OSCL_PLACEMENT_NEW(my_ptr, OsclRefCounterSA< OsclMemAllocDestructDealloc<uint8> >(my_ptr));
        		my_ptr += aligned_refcnt_size;

        		OsclMemoryFragment memfrag;
        		memfrag.len = iRaFormatInfo->ulOpaqueDataSize;
        		memfrag.ptr = my_ptr;

			int32 specinfosize = iRaFormatInfo->ulOpaqueDataSize;
        		// Copy the decoder specific info to the memory fragment
        		oscl_memcpy(memfrag.ptr, iRaFormatInfo->pOpaqueData+1, specinfosize-1);

        		// Save format specific info
        		aConfig = OsclRefCounterMemFrag(memfrag, my_refcnt, specinfosize-1);        		

                      uint8 audioObjectType;
                      uint8 sampleRateIndex;
                      uint32 num_channels = 0;
                      
			 uint8 cfgType = iRaFormatInfo->pOpaqueData[0];

			 if (cfgType == 2) // MPEG4 audio
			 {
				 uint8* specific = iRaFormatInfo->pOpaqueData+1;
				 specinfosize--;
                      
    			        GetActualAacConfig(specific,
                     				         &audioObjectType,
				                               &specinfosize,
				                               &sampleRateIndex,
				                               &num_channels);
                        
    		        	LOGE ("audioObjectType=%d, sampleRateIndex=%d, num_channels=%d", audioObjectType, sampleRateIndex, num_channels);
			 }
			 else
			 {
			 	LOGE ("Error, MPEG4 audio cfgType != 2 (cfgType=%d)", cfgType);
			 }
        	}
        	else
        	{
        	    LOGE ("ERROR RetrieveTrackConfigInfo iRaFormatInfo is NULL !!!");
        	}
    	}

    return false;
}


bool PVMFRMFFParserNode::RetrieveTrackData(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo)
{
    //LOGE ("PVMFRMFFParserNode::RetrieveTrackData");
    
    if (aTrackPortInfo.iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
    {
        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::RetrieveTrackData() - Auto Pause"));
        return false;
    }

    // Get the track ID
    uint32 trackid = aTrackPortInfo.iTrackId;

    // Create a data buffer from pool
    int errcode = OsclErrNoResources;

    OsclSharedPtr<PVMFMediaDataImpl> mediaDataImplOut;
    mediaDataImplOut = aTrackPortInfo.iMediaDataImplAlloc->allocate(aTrackPortInfo.iTrackMaxDataSize);

    if (mediaDataImplOut.GetRep() != NULL)
    {
        errcode = OsclErrNone;
    }
    else
    {
        //LOGE ("PVMFRMFFParserNode::RetrieveTrackData() No Resource Found");
        aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRACKDATAPOOLEMPTY;
        aTrackPortInfo.iTrackDataMemoryPool->notifyfreeblockavailable(aTrackPortInfo, aTrackPortInfo.iTrackMaxDataSize);    // Enable flag to receive event when next deallocate() is called on pool
        return false;
    }

    // Now create a PVMF media data from pool
    errcode = OsclErrNoResources;
    PVMFSharedMediaDataPtr mediadataout;
    mediadataout = PVMFMediaData::createMediaData(mediaDataImplOut, aTrackPortInfo.iMediaDataMemPool);
    if (mediadataout.GetRep() != NULL)
    {
        errcode = OsclErrNone;
    }

    else
    {
        LOGE ("PVMFRMFFParserNode::RetrieveTrackData() Memory allocation for media data memory pool failed");
        aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_MEDIADATAPOOLEMPTY;
        aTrackPortInfo.iMediaDataMemPool->notifyfreechunkavailable(aTrackPortInfo);     // Enable flag to receive event when next deallocate() is called on pool
        return false;
    }

    // Try creating a PVMF media frag group if H.264
    OsclSharedPtr<PVMFMediaDataImpl> mediadatafraggroup;
    if ((aTrackPortInfo.iFormatTypeInteger == PVMF_RMFFPARSER_NODE_H264_MP4) ||
            ((aTrackPortInfo.iFormatTypeInteger == PVMF_RMFFPARSER_NODE_MPEG4_AUDIO) &&
             (oIsAACFramesFragmented)))
    {
        errcode = OsclErrNoResources;
        mediadatafraggroup = aTrackPortInfo.iMediaDataGroupAlloc->allocate();
        if (mediadatafraggroup.GetRep() != NULL)
        {
            errcode = OsclErrNone;
        }

        else
        {
            aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_MEDIADATAFRAGGROUPPOOLEMPTY;
            aTrackPortInfo.iMediaDataGroupAlloc->notifyfreechunkavailable(aTrackPortInfo);
            return false;
        }

    }
    uint32 numsamples = aTrackPortInfo.iNumSamples;


    // Retrieve memory fragment to write to
    OsclRefCounterMemFrag refCtrMemFragOut;
    mediadataout->getMediaFragment(0, refCtrMemFragOut);

    // Resets GAU structure.
    oscl_memset(&iGau.buf, 0, sizeof(iGau.buf));
    oscl_memset(&iGau.info, 0, sizeof(iGau.info));
    iGau.free_buffer_states_when_done = 0;

    iGau.numMediaSamples = numsamples;
    iGau.buf.num_fragments = 1;
    iGau.buf.buf_states[0] = NULL;

    iGau.buf.fragments[0].ptr = refCtrMemFragOut.getMemFrag().ptr;
    iGau.buf.fragments[0].len = refCtrMemFragOut.getCapacity();


    uint32 currticks = OsclTickCount::TickCount();
    uint32 StartTime = OsclTickCount::TicksToMsec(currticks);

    // Retrieve the data from the parser
    int32 retval;
    if (iThumbNailMode)
    {
        if (aTrackPortInfo.iThumbSampleDone == false)
        {
             // TODO: get thumbnail frame here
             aTrackPortInfo.iThumbSampleDone = true;
             numsamples = 1;
             aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK;
             return false;
        }
        else
        {
            aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK;
             return false;
        }
    }
    else
    {
		retval = GetNextSamples((uint8*)iGau.buf.fragments[0].ptr, iGau.numMediaSamples,
		                        iGau.info[0].len, iGau.info[0].ts, iGau.info[0].ts_delta, trackid);

              if (retval == RM_INSUFFICIENT_DATA)
              {
              	aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_GETDATA;
              	return false;
              }
              else if (retval == RM_END_OF_TRACK)
              {
              	aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK;
              	return false;
              }
		
    }

    currticks = OsclTickCount::TickCount();
    uint32 EndTime = OsclTickCount::TicksToMsec(currticks);

    aTrackPortInfo.iNumTimesMediaSampleRead++;

    // Determine actual size of the retrieved data by summing each sample length in GAU

    uint32 DeltaTime = EndTime - StartTime;

    if (DeltaTime >= maxTime)
    {
        maxTime = DeltaTime;
    }
    if (DeltaTime <= minTime)
    {
        minTime = DeltaTime;
    }
    sumTime += DeltaTime;

    aTrackPortInfo.iMaxTime = maxTime;
    aTrackPortInfo.iMinTime = minTime;
    aTrackPortInfo.iSumTime = sumTime;

    uint32 actualdatasize = 0;
    uint32 tsDelta = 0;
    uint32 duration_text_msec = 0;
    bool oSetNoRenderBit = false;
    bool textOnlyClip = false;
    for (uint32 i = 0; i < iGau.numMediaSamples; ++i)
    {
        actualdatasize += iGau.info[i].len;
        tsDelta += iGau.info[i].ts_delta;

        if (iGau.info[i].ts < aTrackPortInfo.iTargetNPTInMediaTimeScale)
        {
            //oSetNoRenderBit = true;
        }
        else
        {
            //we are past the no render point, so reset to zero
            aTrackPortInfo.iTargetNPTInMediaTimeScale = 0;
        }
    }

	//LOGE ("[Morris] @@ iGau.numMediaSamples=%d, iGau.info[0].ts=%d, tsDelta=%d, actualdatasize=%d\n", iGau.numMediaSamples, iGau.info[0].ts, tsDelta, actualdatasize);

    // Save the first frame flag for output media data's random access point flag
    bool israndomaccesspt = aTrackPortInfo.iFirstFrameAfterRepositioning;
    if (aTrackPortInfo.iFirstFrameAfterRepositioning)
    {
        // If it is first frame after repositioning, media data timestamp should be
        // at the spot set when SetDataSourcePosition was processed and doesn't need to
        // consider the delta of the current frame from previous one.
        aTrackPortInfo.iFirstFrameAfterRepositioning = false;
    }

    if (retval == RM_EVERYTHING_FINE || retval == RM_END_OF_TRACK)
    {
        // Truly is end of track when return value is END_OF_TRACK and number of samples
        // returned is 0 or length of data is 0
        if (retval == RM_END_OF_TRACK && (numsamples < 1 || actualdatasize == 0))
        {
            if (aTrackPortInfo.iSendBOS == true)
            {
                // If BOS has not been sent, it needs to be send before EOS.
                // E.g., In case the first call to RetrieveTrack produces END_OF_TRACK.
                SendBeginOfMediaStreamCommand(aTrackPortInfo);
            }
            aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK;
            // End of track reached so send EOS media command

            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::RetrieveTrackData() EOS LOC 3 track ID %d", aTrackPortInfo.iTrackId));
            return false;
        }

       
        // Retrieve the output media data impl to set some properties
        OsclSharedPtr<PVMFMediaDataImpl> media_data_impl;
        mediadataout->getMediaDataImpl(media_data_impl);

        
        // Set buffer size
        mediadataout->setMediaFragFilledLen(0, actualdatasize);
        media_data_impl->setCapacity(actualdatasize);
        // Return the unused space from mempool back
        if (refCtrMemFragOut.getCapacity() > actualdatasize)
        {
             // Need to go to the resizable memory pool and free some memory
              aTrackPortInfo.iMediaDataImplAlloc->ResizeMemoryFragment(mediaDataImplOut);
        }

        // Set the random access point flag if this is the first frame after repositioning
        uint32 markerInfo = 0;
        if (israndomaccesspt)
        {
            markerInfo |= PVMF_MEDIA_DATA_MARKER_INFO_RANDOM_ACCESS_POINT_BIT;
        }

        // Save the media data in the trackport info
        aTrackPortInfo.iMediaData = mediadataout;

        // Retrieve duration and convert to milliseconds
        aTrackPortInfo.iClockConverter->set_clock(tsDelta, 0);

        // Set duration bit in marker info
        markerInfo |= PVMF_MEDIA_DATA_MARKER_INFO_DURATION_AVAILABLE_BIT;
        // Set M bit to 1 always - MP4 FF only outputs complete frames
        markerInfo |= PVMF_MEDIA_DATA_MARKER_INFO_M_BIT;
        // Set No Render bit where applicable
        if (oSetNoRenderBit == true)
        {
            markerInfo |= PVMF_MEDIA_DATA_MARKER_INFO_NO_RENDER_BIT;
        }

        media_data_impl->setMarkerInfo(markerInfo);

        // Retrieve timestamp and convert to milliseconds
        aTrackPortInfo.iClockConverter->set_clock(aTrackPortInfo.iTimestamp, 0);
        uint32 timestamp = aTrackPortInfo.iClockConverter->get_converted_ts(1000);

        // Set the media data's timestamp
        aTrackPortInfo.iMediaData->setTimestamp(timestamp);

        // Set the media data sequence number
        aTrackPortInfo.iMediaData->setSeqNum(aTrackPortInfo.iSeqNum);

        // Advance aTrackPortInfo.iTimestamp to the timestamp at the end of the
        // all samples in the group, i.e. current duration.
        aTrackPortInfo.iTimestamp += tsDelta;
        aTrackPortInfo.iClockConverter->set_clock(aTrackPortInfo.iTimestamp, 0);
        uint32 timestamp_next = aTrackPortInfo.iClockConverter->get_converted_ts(1000);
        uint32 duration_msec = timestamp_next - timestamp;
        aTrackPortInfo.iMediaData->setDuration(duration_msec);
        // increment media data sequence number
        aTrackPortInfo.iSeqNum++;
    }
    else if (retval == RM_READ_FAILED)
    {
        LOGE("PVMFRMFFParserNode::RetrieveTrackData - READ_FAILED");
        // Data could not be read from file
        PVUuid erruuid = PVMFFileFormatEventTypesUUID;
        int32 errcode = PVMFFFErrFileRead;
        ReportRMFFParserInfoEvent(PVMFInfoContentTruncated, NULL, &erruuid, &errcode);
        // Treat this as EOS
        aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK;
        return false;
    }
    else if (retval == RM_INSUFFICIENT_DATA)
    {
            aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_INSUFFICIENTDATA;
            return false;

    }
    else if (retval == RM_INSUFFICIENT_BUFFER_SIZE)
    {
        /* Reset the actual size */
        if (actualdatasize > aTrackPortInfo.iTrackMaxDataSize)
        {
            /* We will attempt to retrieve this sample again with a new max size */
            aTrackPortInfo.iTrackMaxDataSize = actualdatasize;
            aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRACKMAXDATASIZE_RESIZE;
            return true;
        }
        /* Error */
        return false;
    }
    else
    {
        // Parsing error
        aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_ERROR;
        PVUuid erruuid;
        int32 errcode;

        LOGE ("PVMFRMFFParserNode::RetrieveTrackData ReportRMFFParserErrorEvent(PVMFErrCorrupt)");

        ReportRMFFParserErrorEvent(PVMFErrCorrupt, NULL, &erruuid, &errcode);
        return false;
    }

    return true;
}

bool PVMFRMFFParserNode::SendTrackData(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo)
{
    // Set the track specific config info
    aTrackPortInfo.iMediaData->setFormatSpecificInfo(aTrackPortInfo.iFormatSpecificConfig);

    aTrackPortInfo.iMediaData->setStreamID(iStreamID);

    // Send frame to downstream node via port
    PVMFSharedMediaMsgPtr mediaMsgOut;
    convertToPVMFMediaMsg(mediaMsgOut, aTrackPortInfo.iMediaData);
    if (aTrackPortInfo.iPortInterface->QueueOutgoingMsg(mediaMsgOut) != PVMFSuccess)
    {
        aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DESTFULL;
        return false;
    }

    // Don't need the ref to iMediaData so unbind it
    aTrackPortInfo.iMediaData.Unbind();

    return true;
}


bool PVMFRMFFParserNode::UpdateTextSampleEntry(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo, uint32 aEntryIndex, PVMFTimedTextMediaData& aTextMediaData)
{
    return true;
}

bool PVMFRMFFParserNode::SendEndOfTrackCommand(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo)
{
    PVMFSharedMediaCmdPtr sharedMediaCmdPtr = PVMFMediaCmd::createMediaCmd();

    sharedMediaCmdPtr->setFormatID(PVMF_MEDIA_CMD_EOS_FORMAT_ID);

    // Set the sequence number
    uint32 seqNum = aTrackPortInfo.iSeqNum;
    sharedMediaCmdPtr->setSeqNum(seqNum);
    aTrackPortInfo.iSeqNum++;

    //set stream id
    sharedMediaCmdPtr->setStreamID(iStreamID);

    // Set the timestamp
    // Retrieve timestamp and convert to milliseconds
    aTrackPortInfo.iClockConverter->update_clock(aTrackPortInfo.iTimestamp);
    uint32 timestamp = aTrackPortInfo.iClockConverter->get_converted_ts(1000);
    sharedMediaCmdPtr->setTimestamp(timestamp);
    //EOS timestamp(aTrackPortInfo.iTimestamp)is considered while deciding the iResumeTimeStamp in the mediaoutput node
    //therefore its length should also be considered while making decision to forward or drop the packet
    //at the mediaoutput node.
    if (aTrackPortInfo.iFormatTypeInteger == PVMF_RMFFPARSER_NODE_3GPP_TIMED_TEXT &&
            iEOTForTextSentToMIO)
    {
        iEOTForTextSentToMIO = false;
        if (iSetTextSampleDurationZero)
        {
            sharedMediaCmdPtr->setDuration(0);
        }
    }
    else
    {
        //EOS timestamp(aTrackPortInfo.iTimestamp)is considered while deciding the iResumeTimeStamp in the mediaoutput node
        //therefore its length should also be considered while making decision to forward or drop the packet
        //at the mediaoutput node.
        sharedMediaCmdPtr->setDuration(PVMF_RMFFPARSER_DEFAULT_EOS_DURATION_IN_SEC *(aTrackPortInfo.iClockConverter->get_timescale()));
    }

    PVMFSharedMediaMsgPtr mediaMsgOut;
    convertToPVMFMediaCmdMsg(mediaMsgOut, sharedMediaCmdPtr);
    if (aTrackPortInfo.iPortInterface->QueueOutgoingMsg(mediaMsgOut) != PVMFSuccess)
    {
        return false;
    }

    aTrackPortInfo.iTimestamp += (PVMF_RMFFPARSER_DEFAULT_EOS_DURATION_IN_SEC * aTrackPortInfo.iClockConverter->get_timescale());

    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::SendEndOfTrackCommand - Mime=%s, StreamID=%d, TrackID=%d, TS=%d, SEQNUM=%d",
                                         aTrackPortInfo.iMimeType.get_cstr(),
                                         iStreamID,
                                         aTrackPortInfo.iTrackId,
                                         timestamp,
                                         seqNum));

    return true;
}


void PVMFRMFFParserNode::QueuePortActivity(const PVMFPortActivity &aActivity)
{
    //queue a new port activity event
    int32 err;
    OSCL_TRY(err, iPortActivityQueue.push_back(aActivity););
    if (err != OsclErrNone)
    {
        ReportRMFFParserErrorEvent(PVMFErrPortProcessing, (OsclAny*)(aActivity.iPort));
    }
    else
    {
        //wake up the AO to process the port activity event.
        RunIfNotReady();
    }
}


void PVMFRMFFParserNode::HandlePortActivity(const PVMFPortActivity &aActivity)
{
    //A port is reporting some activity or state change.  This code
    //figures out whether we need to queue a processing event
    //for the AO, and/or report a node event to the observer.

    switch (aActivity.iType)
    {
        case PVMF_PORT_ACTIVITY_CREATED:
            //Report port created info event to the node.
            ReportRMFFParserInfoEvent(PVMFInfoPortCreated, (OsclAny*)aActivity.iPort);
            break;

        case PVMF_PORT_ACTIVITY_DELETED:
            //Report port deleted info event to the node.
            ReportRMFFParserInfoEvent(PVMFInfoPortDeleted, (OsclAny*)aActivity.iPort);
            //Purge any port activity events already queued
            //for this port.
            {
                for (uint32 i = 0; i < iPortActivityQueue.size();)
                {
                    if (iPortActivityQueue[i].iPort == aActivity.iPort)
                    {
                        iPortActivityQueue.erase(&iPortActivityQueue[i]);
                    }
                    else
                    {
                        i++;
                    }
                }
            }
            break;

        case PVMF_PORT_ACTIVITY_CONNECT:
            //nothing needed.
            break;

        case PVMF_PORT_ACTIVITY_DISCONNECT:
            //nothing needed.
            break;

        case PVMF_PORT_ACTIVITY_OUTGOING_MSG:
            //An outgoing message was queued on this port.
            //We only need to queue a port activity event on the
            //first message.  Additional events will be queued during
            //the port processing as needed.
            if (aActivity.iPort->OutgoingMsgQueueSize() == 1)
            {
                QueuePortActivity(aActivity);
            }
            break;

        case PVMF_PORT_ACTIVITY_INCOMING_MSG:
            // Incoming msg not supported
            break;

        case PVMF_PORT_ACTIVITY_OUTGOING_QUEUE_BUSY:
            //Outgoing queue is now busy.
            //No action is needed here-- the node checks for
            //outgoing queue busy as needed during data processing.
            break;

        case PVMF_PORT_ACTIVITY_OUTGOING_QUEUE_READY:
            //Outgoing queue was previously busy, but is now ready.
            //We may need to schedule new processing events depending on the port type.

            switch (aActivity.iPort->GetPortTag())
            {
                case PVMF_RMFFPARSERNODE_PORT_TYPE_OUTPUT:
                {
                    // Find the trackportinfo for the port
                    int32 i = 0;
                    int32 maxtrack = iNodeTrackPortList.size();
                    while (i < maxtrack)
                    {
                        if (iNodeTrackPortList[i].iPortInterface == aActivity.iPort)
                        {
                            // Found the element.
                            switch (iNodeTrackPortList[i].iState)
                            {
                                case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DESTFULL:
                                {
                                    // Change the track state to only send the data
                                    iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_SENDDATA;
                                    // Activate the node to process the track state change
                                    RunIfNotReady();
                                }
                                break;

                                case PVRMFFParserNodeTrackPortInfo::TRACKSTATE_SEND_ENDOFTRACK:
                                {
                                    //wakeup the AO to try and send the end-of-track again.
                                    RunIfNotReady();
                                }
                                break;

                                default:
                                    break;
                            }
                        }
                        ++i;
                    }
                }
                break;

                default:
                    break;
            }
            break;

        case PVMF_PORT_ACTIVITY_CONNECTED_PORT_BUSY:
            // The connected port has become busy (its incoming queue is
            // busy).
            // No action is needed here-- the port processing code
            // checks for connected port busy during data processing.
            break;

        case PVMF_PORT_ACTIVITY_CONNECTED_PORT_READY:
            // The connected port has transitioned from Busy to Ready.
            // It's time to start processing outgoing messages again.
            if (aActivity.iPort->OutgoingMsgQueueSize() > 0)
            {
                PVMFPortActivity activity(aActivity.iPort, PVMF_PORT_ACTIVITY_OUTGOING_MSG);
                QueuePortActivity(activity);
            }
            break;

        default:
            break;
    }
}


void PVMFRMFFParserNode::ProcessPortActivity()
{
    //called by the AO to process a port activity message
    //Pop the queue...
    PVMFPortActivity activity(iPortActivityQueue.front());
    iPortActivityQueue.erase(&iPortActivityQueue.front());

    PVMFStatus status = PVMFSuccess;
    switch (activity.iType)
    {
        case PVMF_PORT_ACTIVITY_OUTGOING_MSG:
            status = ProcessOutgoingMsg(activity.iPort);
            //Re-queue the port activity event as long as there's
            //more data to process and it isn't in a Busy state.
            if (status != PVMFErrBusy && activity.iPort->OutgoingMsgQueueSize() > 0)
            {
                QueuePortActivity(activity);
            }
            break;

        default:
            break;

    }

    //Report any unexpected failure in port processing...
    //(the InvalidState error happens when port input is suspended,
    //so don't report it.)
    if (status != PVMFErrBusy && status != PVMFSuccess && status != PVMFErrInvalidState)
    {
        LOGE ("PVMFRMFFParserNode::ProcessPortActivity() Error unexpected failure Port=0x%x Type=0x%x", activity.iPort, activity.iType);

        ReportRMFFParserErrorEvent(PVMFErrPortProcessing);
    }
}


PVMFStatus PVMFRMFFParserNode::ProcessIncomingMsg(PVMFPortInterface* /*aPort*/)
{
    // No incoming msg supported
    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::ProcessOutgoingMsg(PVMFPortInterface* aPort)
{
    //Called by the AO to process one message off the outgoing
    //message queue for the given port.  This routine will
    //try to send the data to the connected port.
    LOGE ("PVMFRMFFParserNode::ProcessOutgoingMsg() In aPort=0x%x", aPort);

    PVMFStatus status = aPort->Send();
    if (status == PVMFErrBusy)
    {
        LOGE ("PVMFRMFFParserNode::ProcessOutgoingMsg() Connected port is in busy state");
    }

    return status;
}

void PVMFRMFFParserNode::ResetAllTracks()
{
    for (uint32 i = 0; i < iNodeTrackPortList.size(); ++i)
    {
        iNodeTrackPortList[i].iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_UNINITIALIZED;
        iNodeTrackPortList[i].iMediaData.Unbind();
        iNodeTrackPortList[i].iSeqNum = 0;
        iNodeTrackPortList[i].iPortInterface->ClearMsgQueues();
        iNodeTrackPortList[i].iCurrentTextSampleEntry.Unbind();
    }
    iPortActivityQueue.clear();
}


bool PVMFRMFFParserNode::ReleaseAllPorts()
{
    while (!iNodeTrackPortList.empty())
    {
        iNodeTrackPortList[0].iPortInterface->Disconnect();
        iNodeTrackPortList[0].iMediaData.Unbind();
        if (iNodeTrackPortList[0].iPortInterface)
        {
            OSCL_DELETE(((PVMFRMFFParserOutPort*)iNodeTrackPortList[0].iPortInterface));
        }
        if (iNodeTrackPortList[0].iClockConverter)
        {
            OSCL_DELETE(iNodeTrackPortList[0].iClockConverter);
        }
        if (iNodeTrackPortList[0].iTrackDataMemoryPool)
        {
            iNodeTrackPortList[0].iTrackDataMemoryPool->CancelFreeChunkAvailableCallback();
            iNodeTrackPortList[0].iTrackDataMemoryPool->removeRef();
            iNodeTrackPortList[0].iTrackDataMemoryPool = NULL;
        }
        if (iNodeTrackPortList[0].iMediaDataImplAlloc)
        {
            OSCL_DELETE(iNodeTrackPortList[0].iMediaDataImplAlloc);
        }
        if (iNodeTrackPortList[0].iTextMediaDataImplAlloc)
        {
            OSCL_DELETE(iNodeTrackPortList[0].iTextMediaDataImplAlloc);
        }
        if (iNodeTrackPortList[0].iMediaDataMemPool)
        {
            iNodeTrackPortList[0].iMediaDataMemPool->CancelFreeChunkAvailableCallback();
            iNodeTrackPortList[0].iMediaDataMemPool->removeRef();
        }
        if (iNodeTrackPortList[0].iMediaDataGroupAlloc)
        {
            iNodeTrackPortList[0].iMediaDataGroupAlloc->removeRef();
        }
        if (iNodeTrackPortList[0].iMediaDataGroupImplMemPool)
        {
            iNodeTrackPortList[0].iMediaDataGroupImplMemPool->removeRef();
        }

        iNodeTrackPortList.erase(iNodeTrackPortList.begin());
    }

    return true;
}


void PVMFRMFFParserNode::RemoveAllCommands()
{
    //The command queues are self-deleting, but we want to
    //notify the observer of unprocessed commands.
    while (!iCurrentCommand.empty())
    {
        CommandComplete(iCurrentCommand, iCurrentCommand.front(), PVMFFailure);
    }

    while (!iInputCommands.empty())
    {
        CommandComplete(iInputCommands, iInputCommands.front(), PVMFFailure);
    }
}


void PVMFRMFFParserNode::CleanupFileSource()
{
    LOGE ("PVMFRMFFParserNode::CleanupFileSource()\n");
    iAvailableMetadataKeys.clear();
    iRMParserNodeMetadataValueCount = 0;
    iVideoDimensionInfoVec.clear();

    iProtectedFile = false;
    iExternalDownload = false;
    iThumbNailMode = false;

    if (iFileHandle)
    {
        OSCL_DELETE(iFileHandle);
        iFileHandle = NULL;
    }


}



void PVMFRMFFParserNode::getBrand(uint32 aBrandVal, char *BrandVal)
{
    BrandVal[0] = (aBrandVal >> 24);
    BrandVal[1] = (aBrandVal >> 16);
    BrandVal[2] = (aBrandVal >> 8);
    BrandVal[3] =  aBrandVal;
}

bool PVMFRMFFParserNode::GetTrackPortInfoForTrackID(PVRMFFParserNodeTrackPortInfo*& aInfo,
        uint32 aTrackID)
{
    aInfo = NULL;
    Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;
    for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
    {
        if (it->iTrackId == (int32)aTrackID)
        {
            aInfo = it;
            return true;
        }
    }
    return false;
}

bool PVMFRMFFParserNode::GetTrackPortInfoForPort(PVRMFFParserNodeTrackPortInfo*& aInfo,
        PVMFPortInterface* aPort)
{
    aInfo = NULL;
    Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;
    for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
    {
        if (it->iPortInterface == aPort)
        {
            aInfo = it;
            return true;
        }
    }
    return false;
}




PVMFStatus PVMFRMFFParserNode::CheckForUnderFlow(PVRMFFParserNodeTrackPortInfo* aInfo)
{
    uint32 timebase32 = 0;
    uint32 clientClock32 = 0;
    bool overload = 0;
    if (iClientPlayBackClock != NULL)
    {
        iClientPlayBackClock->GetCurrentTime32(clientClock32, overload, PVMF_MEDIA_CLOCK_MSEC, timebase32);
    }
    else
    {
        return PVMFFailure;
    }

    MediaClockConverter clock_conv(*(aInfo->iClockConverter));
    clock_conv.update_clock(aInfo->iTimestamp);
    uint32 msec = clock_conv.get_converted_ts(MILLISECOND_TIMESCALE);
    if (clientClock32 <= msec)
    {
        uint32 diff32 = (msec - clientClock32);
        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::CheckForUnderFlow - Diff Bet Client Clock(%d) and Track Media TS(%d) = %d", clientClock32, msec, diff32));
        if (diff32 < PVMF_RMFFPARSER_UNDERFLOW_THRESHOLD_IN_MS || aInfo->iFormatType == PVMF_MIME_AMR_IETF || aInfo->iFormatType == PVMF_MIME_MPEG4_AUDIO)
        {
            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::CheckForUnderFlow - Time To Auto Pause"));
            return PVMFSuccess;
        }
        else
        {
            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::CheckForUnderFlow - Setting UnderFlow Timer"));
            iUnderFlowCheckTimer->Request(PVMF_RMFFPARSERNODE_UNDERFLOW_STATUS_TIMER_ID,
                                          0,
                                          PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_CYCLES,
                                          this);
        }
    }
    else
    {
        PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserNode::CheckForUnderFlow - Client Clock Greater than Media TS, Clock=%d, MaxMTS=%d",  clientClock32, msec));
        OSCL_ASSERT(false);
    }

    /* Check if all tracks are autopaused. If so, it is time to autopause the node */
    bool oAllTracksAutoPaused = true;
    Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;
    for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
    {
        if (it->iState != PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
        {
            oAllTracksAutoPaused = false;
        }
    }
    if (oAllTracksAutoPaused == true)
    {
        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::CheckForUnderFlow - All Tracks AutoPaused - Time To Auto Pause"));
        return PVMFSuccess;
    }
    //not yet time to autopause
    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::CheckForUnderFlow - Wait on player clock for Auto Pause"));
    return PVMFPending;
}


PVMFStatus PVMFRMFFParserNode::ReportUnderFlow()
{
    /* Check if all tracks are autopaused. If so, it is time to autopause the node */
    bool oAllTracksAutoPaused = true;
    Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;
    for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
    {
        if (it->iState != PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
        {
            oAllTracksAutoPaused = false;
        }
    }
    if (oAllTracksAutoPaused == true)
    {
        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - Reporting UnderFlow"));
        ReportInfoEvent(PVMFInfoUnderflow);
        iUnderFlowEventReported = true;
        return PVMFSuccess;
    }


    uint32 minTS = 0xFFFFFFFF;
    for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
    {
        if (it->iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
        {
            MediaClockConverter clock_conv(*(it->iClockConverter));
            clock_conv.update_clock(it->iTimestamp);
            uint32 msec = clock_conv.get_converted_ts(MILLISECOND_TIMESCALE);
            if (msec < minTS)
            {
                minTS = msec;
            }
        }
    }
    uint32 timebase32 = 0;
    uint32 clientClock32 = 0;
    bool overload = 0;

    if (iClientPlayBackClock != NULL)
        iClientPlayBackClock->GetCurrentTime32(clientClock32, overload, PVMF_MEDIA_CLOCK_MSEC, timebase32);


    uint32 currentFileSize = 0;
    
    iUnderFlowEventReported = false;
    uint32 currNPT = 0;

    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - ClientClock = %d", clientClock32));
    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - NPTInMS = %d, FileSize = %d", currNPT, currentFileSize));
    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - Min Media TS = %d", minTS));

    if (clientClock32 <= minTS)
    {
        uint32 diff32 = (minTS - clientClock32);
        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - Diff Bet Client Clock and Min Media TS = %d", diff32));
        if (diff32 < PVMF_RMFFPARSER_UNDERFLOW_THRESHOLD_IN_MS)
        {
            /* If parserNode is reporting Underflow, all tracks should be in auto-paused state */
            for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
            {
                if (it->iState != PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
                {
                    it->iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE;
                }
            }
            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - Reporting UnderFlow"));
            ReportInfoEvent(PVMFInfoUnderflow);
            iUnderFlowEventReported = true;
        }
        else
        {
            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::ReportUnderFlow - Setting UnderFlow Timer"));
            iUnderFlowCheckTimer->Request(PVMF_RMFFPARSERNODE_UNDERFLOW_STATUS_TIMER_ID,
                                          0,
                                          PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_CYCLES,
                                          this);
        }
    }
    else
    {
        PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserNode::ReportUnderFlow - Client Clock Greater than Min Media TS, Clock=%d, MaxMTS=%d",  clientClock32, minTS));
        OSCL_ASSERT(false);
    }
    return PVMFSuccess;
}

void PVMFRMFFParserNode::TimeoutOccurred(int32 timerID,
        int32 timeoutInfo)
{
    OSCL_UNUSED_ARG(timeoutInfo);

    if (timerID == PVMF_RMFFPARSERNODE_UNDERFLOW_STATUS_TIMER_ID)
    {
        if (iUnderFlowEventReported == false)
        {
            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - UnderFlow TimeOut - Still AutoPaused"));
            uint32 currentFileSize = 0;

            int code = RM_EVERYTHING_FINE;
 
            if (code == RM_EVERYTHING_FINE)
            {
                uint32 currNPT = 0;

                uint32 minTS = 0xFFFFFFFF;
                Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;
                for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
                {
                    if (it->iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
                    {
                        MediaClockConverter clock_conv(*(it->iClockConverter));
                        clock_conv.update_clock(it->iTimestamp);
                        uint32 msec = clock_conv.get_converted_ts(MILLISECOND_TIMESCALE);
                        if (msec < minTS)
                        {
                            minTS = msec;
                        }
                    }
                }
                uint32 timebase32 = 0;
                uint32 clientClock32 = 0;
                bool overload = 0;
                if (iClientPlayBackClock != NULL)
                    iClientPlayBackClock->GetCurrentTime32(clientClock32, overload, PVMF_MEDIA_CLOCK_MSEC, timebase32);


                PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - ClientClock = %d", clientClock32));
                PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - NPTInMS = %d,  FileSize = %d", currNPT, currentFileSize));
                PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Min Media TS = %d", minTS));

                if (clientClock32 <= minTS)
                {
                    uint32 diff32 = (minTS - clientClock32);
                    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Diff Bet Client Clock and Min Media TS = %d", diff32));
                    if (diff32 < PVMF_RMFFPARSER_UNDERFLOW_THRESHOLD_IN_MS)
                    {
                        /* If parserNode is reporting Underflow, all tracks should be in auto-paused state */
                        for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
                        {
                            if (it->iState != PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE)
                            {
                                it->iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE;
                            }
                        }

                        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Reporting UnderFlow"));
                        ReportInfoEvent(PVMFInfoUnderflow);
                        iUnderFlowEventReported = true;
                    }
                    else
                    {
                        PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Requesting Additional Time Out"));
                        iUnderFlowCheckTimer->Request(PVMF_RMFFPARSERNODE_UNDERFLOW_STATUS_TIMER_ID,
                                                      0,
                                                      PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_CYCLES,
                                                      this);
                    }
                }
                else
                {
                    PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserNode::TimeoutOccurred - Client Clock Greater than Min Media TS, Clock=%d, MaxMTS=%d",  clientClock32, minTS));
                    OSCL_ASSERT(false);
                }

            }
            else
            {
                PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserNode::TimeoutOccurred - GetCurrentFileSize Failed - Ret=%d",  code));
                ReportErrorEvent(PVMFErrResource);
            }
        }
        else if (iUnderFlowEventReported == false)
        {
            uint32 timebase32 = 0;
            uint32 clientClock32 = 0;
            bool overload = 0;
            uint32 msec = 0;
            if (iClientPlayBackClock != NULL)
            {
                iClientPlayBackClock->GetCurrentTime32(clientClock32, overload, PVMF_MEDIA_CLOCK_MSEC, timebase32);
            }

            Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;
            for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
            {
                if (it->iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_INSUFFICIENTDATA)
                {
                    MediaClockConverter clock_conv(*(it->iClockConverter));
                    clock_conv.update_clock(it->iTimestamp);
                    msec = clock_conv.get_converted_ts(MILLISECOND_TIMESCALE);
                }
            }

            if (clientClock32 <= msec)
            {
                uint32 diff32 = (msec - clientClock32);
                PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Diff Bet Client Clock(%d) and Track Media TS(%d) = %d", clientClock32, msec, diff32));
                if (diff32 < PVMF_RMFFPARSER_UNDERFLOW_THRESHOLD_IN_MS)
                {
                    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Time To Auto Pause"));
                    for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
                    {
                        if (it->iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_INSUFFICIENTDATA)
                        {
                            it->iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DOWNLOAD_AUTOPAUSE;
                        }
                    }
                    ReportUnderFlow();
                }
                else
                {
                    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - Setting UnderFlow Timer"));
                    iUnderFlowCheckTimer->Request(PVMF_RMFFPARSERNODE_UNDERFLOW_STATUS_TIMER_ID,
                                                  0,
                                                  PVMF_RMFFPARSER_UNDERFLOW_STATUS_EVENT_CYCLES,
                                                  this);
                }
            }
            else
            {
                PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserNode::TimeoutOccurred - Client Clock Greater than Media TS, Clock=%d, MaxMTS=%d",  clientClock32, msec));
                //should never happen
                OSCL_ASSERT(false);
            }
            PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::TimeoutOccurred - UnderFlow TimeOut - After AutoResume"));
        }
    }
    return;
}

void
PVMFRMFFParserNode::LogMediaData(PVMFSharedMediaDataPtr data,
                                  PVMFPortInterface* aPort)
{
    PVRMFFParserNodeTrackPortInfo* trackInfoPtr = NULL;
    if (!GetTrackPortInfoForPort(trackInfoPtr, aPort))
    {
        return;
    }

    bool ologSizeTS = false;

    /* Get Format Specific Info, if any */
    if (trackInfoPtr->oFormatSpecificInfoLogged == false)
    {
        uint32 size = trackInfoPtr->iFormatSpecificConfig.getMemFragSize();
        if (size > 0)
        {
            PVMF_RMFFPARSERNODE_LOGBIN(trackInfoPtr->iPortLogger, (0, 1, size, trackInfoPtr->iFormatSpecificConfig.getMemFragPtr()));
        }
        trackInfoPtr->oFormatSpecificInfoLogged = true;
    }

    ologSizeTS = false;

    PVMFMediaData* mediaData = data.GetRep();
    if (mediaData != NULL)
    {
        /* Log Media Fragments */
        uint32 numMediaFragments = mediaData->getNumFragments();
        for (uint32 i = 0; i < numMediaFragments; i++)
        {
            OsclRefCounterMemFrag memFrag;
            mediaData->getMediaFragment(i, memFrag);
            if (ologSizeTS)
            {
                PVMF_RMFFPARSERNODE_LOGBIN(trackInfoPtr->iPortLogger, (0, 1, sizeof(uint32), memFrag.getMemFragSize()));
                PVMF_RMFFPARSERNODE_LOGBIN(trackInfoPtr->iPortLogger, (0, 1, sizeof(uint32), data->getTimestamp()));
            }
            PVMF_RMFFPARSERNODE_LOGBIN(trackInfoPtr->iPortLogger, (0, 1, memFrag.getMemFragSize(), memFrag.getMemFragPtr()));
        }
    }
}

// Morris Yang 0415 [
void
PVMFRMFFParserNode::WriteGAUtoFile(GAU* iGau)
{
	//iGau.buf.fragments[0].ptr = refCtrMemFragOut.getMemFrag().ptr;
    //iGau.buf.fragments[0].len = refCtrMemFragOut.getCapacity();
    uint32 _len = iGau->info[0].len;
    uint32 _ts = iGau->info[0].ts;
	uint32 _ts_delta = iGau->info[0].ts_delta;
	uint32 _size = _len + 12;

	FILE *fp;
	fp = fopen("/sdcard/transbin/Samples.mtk", "a");
	if (fp != NULL){
	 fwrite(&_size, 4, 1, fp);
	 fwrite(&_ts, 4, 1, fp);
	 fwrite(&_ts_delta, 4, 1, fp);
	 fwrite(iGau->buf.fragments[0].ptr, 1, _len, fp);
	 fclose(fp);
	 LOGE ("[Morris] WriteGAUtoFile _size=%u, _ts=%u, _ts_delta=%u\n", _size, _ts, _ts_delta);
	}

}

int32 
PVMFRMFFParserNode::GetNextSamples(uint8* pBuffer, uint32& numSamples, uint32& length, uint32& ts, uint32& deltaTs, uint32 trackId)
{
	HX_RESULT retVal = HXR_OK;

	//LOGE ("GetNextSamples trackId=%d, V=%d, A=%d", trackId, iRvStreamNum, iRaStreamNum);

       if (trackId == iRvStreamNum)
       {
       	LOGE ("@@ GetVideoFrames");
       }

       
       if (trackId == iRaStreamNum)
       {
       	//LOGE ("@@ GetAudioBlock, iRaBlocks.size()=%d", iRaBlocks.size());
       	if (iRaBlocks.size() >= 10)	// queue is pretty full, get the block from the queue
       	{
       	    DequeueRaBlock(pBuffer, numSamples, length, ts, deltaTs);
		    return RM_EVERYTHING_FINE;
       	}
       	else if (iRaBlocks.size() >= 2) // fill more blocks to queue
       	{
       		retVal = rm_parser_get_packet(iRmParser, &iRmPacket);
			if (retVal == HXR_OK)
			{  
				// TODO: in case of audio or video only, DON'T add packet for the other stream
				if (iRmPacket->usStream == iRvStreamNum)
				{
				    // leave it
				    retVal = rv_depack_add_packet(iRvDepack, iRmPacket);
				}

	    			if (iRmPacket->usStream == iRaStreamNum)
		   	       {
		   	           retVal = ra_depack_add_packet(iRaDepack, iRmPacket);
	    			}
	    		}
			else  if (retVal == HXR_AT_END)
			{
			
			}

			DequeueRaBlock(pBuffer, numSamples, length, ts, deltaTs);
		   	rm_parser_destroy_packet(iRmParser, &iRmPacket);
			return RM_EVERYTHING_FINE;			
      		}
       	else // iRaBlocks.size() < 2
      		{
      			retVal = rm_parser_get_packet(iRmParser, &iRmPacket);
      			if (retVal == HXR_OK)
			{  
				if (iRmPacket->usStream == iRvStreamNum)
				{
				    // leave it
				     retVal = rv_depack_add_packet(iRvDepack, iRmPacket);
				}

	    			if (iRmPacket->usStream == iRaStreamNum)
		   	       {
		   	           retVal = ra_depack_add_packet(iRaDepack, iRmPacket);
	    			}

		   		rm_parser_destroy_packet(iRmParser, &iRmPacket);
		   		numSamples = 0;
				return RM_INSUFFICIENT_DATA;	
	    		}
			else  if (retVal == HXR_AT_END)
			{
				if (iRaBlocks.size() == 1)
				{
					// return the last block
                  			length = iRaBlocks[0]._data_length;
			              memcpy (pBuffer, iRaBlocks[0]._data, length);
		 	              ts = iRaBlocks[0]._timestamp;
	            			deltaTs = 23;

		    			// clean up
	          			delete [] iRaBlocks[0]._data;
		   			iRaBlocks.erase(&iRaBlocks[0]);
		   			numSamples = 1;
		   			return RM_EVERYTHING_FINE;
				}

				if (iRaBlocks.empty())
				{
					// send EOS
					//LOGE ("RM_END_OF_TRACK");
					return RM_END_OF_TRACK;
				}
			}
      		}
       }

	return RM_EVERYTHING_FINE;
}


bool
PVMFRMFFParserNode::DequeueRaBlock(uint8* pBuffer, uint32& numSamples, uint32& length, uint32& ts, uint32& deltaTs)
{
	// iRaBlocks.size() must >= 2
	numSamples = 1;
	length = iRaBlocks[0]._data_length;
       memcpy (pBuffer, iRaBlocks[0]._data, length);
	ts = iRaBlocks[0]._timestamp;
	deltaTs = iRaBlocks[1]._timestamp - iRaBlocks[0]._timestamp;

	//LOGE ("DequeueRaBlock length=%u", length);

	// clean up
	delete [] iRaBlocks[0]._data;
	iRaBlocks.erase(&iRaBlocks[0]);

	return true;
}

bool 
PVMFRMFFParserNode::CleanRaBlockQueue()
{
	while(!iRaBlocks.empty())
	{
		RaBlockVector::iterator it;
		it = iRaBlocks.begin();
		delete [] it->_data;
		iRaBlocks.erase(it);
	}
	
	return true;
}

bool 
PVMFRMFFParserNode::CleanRvFrameQueue()
{
	while(!iRvFrames.empty())
	{
		RvFrameVector::iterator it;
		it = iRvFrames.begin();
		delete [] it->_data;
		// TODO: free _segments
		iRvFrames.erase(it);
	}
	
	return true;
}

bool PVMFRMFFParserNode::RMSeekTo(uint32 targetNPT)
{
	HX_RESULT retVal = HXR_FAIL;
	
	if (iRmParser)
	{
		LOGE ("PVMFRMFFParserNode::RMSeekTo(%u)", targetNPT);
		// seek video depacketizer
		if (iRvDepack)
		{
			retVal = rv_depack_seek(iRvDepack, targetNPT);
			if (retVal != HXR_OK)
			{
				LOGE ("PVMFRMFFParserNode::RMSeekTo rv_depack_seek failed, retVal=%d", retVal);
			}
		}
		
		// seek audio depacketizer
		if (iRaDepack)
		{
			retVal = ra_depack_seek(iRaDepack, targetNPT);
			if (retVal != HXR_OK)
			{
				LOGE ("PVMFRMFFParserNode::RMSeekTo ra_depack_seek failed, retVal=%d", retVal);
			}
		}
			
		// seek rm parser
		rm_parser_get_packet(iRmParser, &iRmPacket);
		retVal = rm_parser_seek(iRmParser, targetNPT);
		rm_parser_destroy_packet(iRmParser, &iRmPacket);
		
		if (retVal != HXR_OK)
		{
			LOGE ("PVMFRMFFParserNode::RMSeekTo rm_parser_seek failed, retVal=%d", retVal);
			return false;
		}

		return true;
	}
	else
	{
		LOGE ("PVMFRMFFParserNode::RMSeekTo failed due to iRmParser is NULL");
		return false;
	}
	
	return true;
}


void
PVMFRMFFParserNode::LogMediaDataToFile(PVMFSharedMediaDataPtr data,
                                  PVMFPortInterface* aPort, const char* prefix)
{
    PVRMFFParserNodeTrackPortInfo* trackInfoPtr = NULL;
    if (!GetTrackPortInfoForPort(trackInfoPtr, aPort))
    {
        return;
    }

    PVMFMediaData* mediaData = data.GetRep();
    if (mediaData != NULL)
    {
        /* Log Media Fragments */
        uint32 numMediaFragments = mediaData->getNumFragments();
        for (uint32 i = 0; i < numMediaFragments; i++)
        {
            OsclRefCounterMemFrag memFrag;
            mediaData->getMediaFragment(i, memFrag);
 // log frames as seperate file
            LOGE("Writing %d th frame to file, TS = %d (numMediaFragments=%d)\n", iNumMediaFrame, data->getTimestamp(), numMediaFragments);
			char path[255];
			sprintf (path, "/sdcard/transbin/%s/%d.bin", prefix, iNumMediaFrame++);
			FILE *fp;
			fp = fopen(path, "a");
			if (fp != NULL){
			 fwrite(memFrag.getMemFragPtr(), 1, memFrag.getMemFragSize(), fp);
			 fclose(fp);
			}


		 // log frames as a single file
		 /*
					LOGE("Writing %d th frame to file, TS = %d\n", iNumMediaFrame++, data->getTimestamp());
					char path[255];
					sprintf (path, "/sdcard/transbin/full.bin");
					FILE *fp;
					fp = fopen(path, "a");
					if (fp != NULL){
					 uint32 _size = memFrag.getMemFragSize()+8;
					 uint32 _ts = data->getTimestamp();
					 fwrite(&_size, 1, 4, fp);
					 fwrite(&_ts, 1, 4, fp);
					 fwrite(memFrag.getMemFragPtr(), 1, memFrag.getMemFragSize(), fp);
					 fclose(fp);
					}
		*/
		
        }
    }
}

// ]

void PVMFRMFFParserNode::LogDiagnostics()
{
    if (iDiagnosticsLogged == false)
    {
        iDiagnosticsLogged = true;
        Oscl_Vector<PVRMFFParserNodeTrackPortInfo, OsclMemAllocator>::iterator it;

        if (&iNodeTrackPortList)
        {
            it = iNodeTrackPortList.begin();
            for (it = iNodeTrackPortList.begin(); it != iNodeTrackPortList.end(); it++)
            {
                PVMF_RMFFPARSERNODE_LOGDIAGNOSTICS((0, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
                PVMF_RMFFPARSERNODE_LOGDIAGNOSTICS((0, "PVMFRMFFParserNode - Read Media Sample Minimum Time  =%2d", it->iMinTime));
                PVMF_RMFFPARSERNODE_LOGDIAGNOSTICS((0, "PVMFRMFFParserNode - Read Media Sample Maximum Time  =%2d", it->iMaxTime));
                PVMF_RMFFPARSERNODE_LOGDIAGNOSTICS((0, "PVMFRMFFParserNode - Read Media Sample Total Time  =%2d", it->iSumTime));

                uint64 avg_time = 0;
                if ((it->iNumTimesMediaSampleRead) > 0)
                    avg_time = Oscl_Int64_Utils::get_uint64_lower32(it->iSumTime) / (it->iNumTimesMediaSampleRead);

                PVMF_RMFFPARSERNODE_LOGDIAGNOSTICS((0, "PVMFRMFFParserNode - Read Media Sample Avg Time  =%2d", avg_time));
                PVMF_RMFFPARSERNODE_LOGDIAGNOSTICS((0, "PVMFRMFFParserNode - Number of Sample Read each time  =%d", it->iNumSamples));


            }
        }
    }
}

PVMFStatus PVMFRMFFParserNode::GetVideoFrameWidth(uint32 aId, int32& aWidth, int32& aDisplayWidth)
{
    int32 height = 0;
    int32 width = 0;
    int32 display_width = 0;
    int32 display_height = 0;

    aWidth = aDisplayWidth = 176;
    
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::GetVideoFrameHeight(uint32 aId, int32& aHeight, int32& aDisplayHeight)
{
    int32 height = 0;
    int32 width = 0;
    int32 display_width = 0;
    int32 display_height = 0;

    aDisplayHeight = aHeight = 144;

    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::PopulateVideoDimensions(uint32 aId)
{
    VideoTrackDimensionInfo vidDimInfo;
    vidDimInfo.iTrackId = aId;
    if (PVMFSuccess != GetVideoFrameWidth(aId, vidDimInfo.iWidth, vidDimInfo.iDisplayWidth))
    {
        return PVMFFailure;
    }
    if (PVMFSuccess != GetVideoFrameHeight(aId, vidDimInfo.iHeight, vidDimInfo.iDisplayHeight))
    {
        return PVMFFailure;
    }
    iVideoDimensionInfoVec.push_back(vidDimInfo);
    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::PushVideoDimensions(uint32 aId, int32 width, int32 height)
{
    VideoTrackDimensionInfo vidDimInfo;
    vidDimInfo.iTrackId = aId;
    vidDimInfo.iWidth = vidDimInfo.iDisplayWidth = width;
    vidDimInfo.iHeight = vidDimInfo.iDisplayHeight = height;
    iVideoDimensionInfoVec.push_back(vidDimInfo);
    return PVMFSuccess;
}


int32 PVMFRMFFParserNode::FindVideoWidth(uint32 aId)
{
    int32 width = 0;
    Oscl_Vector<VideoTrackDimensionInfo, OsclMemAllocator>::iterator it;
    for (it = iVideoDimensionInfoVec.begin(); it != iVideoDimensionInfoVec.end(); it++)
    {
        if (it->iTrackId == aId)
        {
            width = it->iWidth;
        }
    }
    return width;
}

int32 PVMFRMFFParserNode::FindVideoHeight(uint32 aId)
{
    int32 height = 0;
    Oscl_Vector<VideoTrackDimensionInfo, OsclMemAllocator>::iterator it;
    for (it = iVideoDimensionInfoVec.begin(); it != iVideoDimensionInfoVec.end(); it++)
    {
        if (it->iTrackId == aId)
        {
            height = it->iHeight;
        }
    }
    return height;
}

int32 PVMFRMFFParserNode::FindVideoDisplayWidth(uint32 aId)
{
    int32 display_width = 0;
    Oscl_Vector<VideoTrackDimensionInfo, OsclMemAllocator>::iterator it;
    for (it = iVideoDimensionInfoVec.begin(); it != iVideoDimensionInfoVec.end(); it++)
    {
        if (it->iTrackId == aId)
        {
            display_width = it->iDisplayWidth;
        }
    }
    return display_width;
}

int32 PVMFRMFFParserNode::FindVideoDisplayHeight(uint32 aId)
{
    int32 display_height = 0;
    Oscl_Vector<VideoTrackDimensionInfo, OsclMemAllocator>::iterator it;
    for (it = iVideoDimensionInfoVec.begin(); it != iVideoDimensionInfoVec.end(); it++)
    {
        if (it->iTrackId == aId)
        {
            display_height = it->iDisplayHeight;
        }
    }
    return display_height;
}

uint32 PVMFRMFFParserNode::GetNumAudioChannels(uint32 aId)
{
    OSCL_UNUSED_ARG(aId);
    return 2;
}

uint32 PVMFRMFFParserNode::GetAudioSampleRate(uint32 aId)
{
    OSCL_UNUSED_ARG(aId);
    return 44100;
}

uint32 PVMFRMFFParserNode::GetAudioBitsPerSample(uint32 aId)
{
    OSCL_UNUSED_ARG(aId);
    //always 16 bits per samples
    return 16;
}


bool PVMFRMFFParserNode::SendBeginOfMediaStreamCommand(PVRMFFParserNodeTrackPortInfo& aTrackPortInfo)
{
    PVMFSharedMediaCmdPtr sharedMediaCmdPtr = PVMFMediaCmd::createMediaCmd();
    sharedMediaCmdPtr->setFormatID(PVMF_MEDIA_CMD_BOS_FORMAT_ID);

    aTrackPortInfo.iClockConverter->update_clock(aTrackPortInfo.iTimestamp);
    uint32 timestamp = aTrackPortInfo.iClockConverter->get_converted_ts(1000);
    sharedMediaCmdPtr->setTimestamp(timestamp);

    PVMFSharedMediaMsgPtr mediaMsgOut;
    convertToPVMFMediaCmdMsg(mediaMsgOut, sharedMediaCmdPtr);
    mediaMsgOut->setStreamID(iStreamID);
    mediaMsgOut->setSeqNum(aTrackPortInfo.iSeqNum);

    if (aTrackPortInfo.iPortInterface->QueueOutgoingMsg(mediaMsgOut) != PVMFSuccess)
    {
        // Output queue is busy, so wait for the output queue being ready
        if (aTrackPortInfo.iState == PVRMFFParserNodeTrackPortInfo::TRACKSTATE_TRANSMITTING_SENDDATA)
        {
            aTrackPortInfo.iState = PVRMFFParserNodeTrackPortInfo::TRACKSTATE_DESTFULL;
        }
        return false;
    }
    aTrackPortInfo.iSendBOS = false;
    PVMF_RMFFPARSERNODE_LOGDATATRAFFIC((0, "PVMFRMFFParserNode::SendBeginOfMediaStreamCommand() BOS sent - Mime=%s, StreamId=%d, TS=%d",
                                         aTrackPortInfo.iMimeType.get_cstr(),
                                         iStreamID,
                                         timestamp));
    LOGE ("PVMFRMFFParserNode::SendBeginOfMediaStreamCommand() BOS sent StreamId=%d", iStreamID);
    return true;
}

void PVMFRMFFParserNode::NotificationsInterfaceDestroyed()
{
    iClockNotificationsInf = NULL;
}

void PVMFRMFFParserNode::ClockStateUpdated()
{

}


int32 PVMFRMFFParserNode::CreateErrorInfoMsg(PVMFBasicErrorInfoMessage** aErrorMsg, PVUuid aEventUUID, int32 aEventCode)
{
    int32 leavecode = 0;
    OSCL_TRY(leavecode, *aErrorMsg = OSCL_NEW(PVMFBasicErrorInfoMessage, (aEventCode, aEventUUID, NULL)));
    return leavecode;
}

PVMFStatus PVMFRMFFParserNode::ResetNodeParam()
{
    LOGV("***** ResetNodeParam() *****");


    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::ClearBuffer()
{
    LOGV("***** ClearBuffer() *****");

    ResetAllTracks();

    return PVMFSuccess;
}

bool PVMFRMFFParserNode::IsVideoTrack(char* FormatType)
{
    bool ret = false;
    if (!strcmp(FormatType, PVMF_MIME_M4V) ||
        !strcmp(FormatType, PVMF_MIME_H2631998) ||
        !strcmp(FormatType, PVMF_MIME_H2632000) ||
        !strcmp(FormatType, PVMF_MIME_H264_VIDEO_RAW) ||
        !strcmp(FormatType, PVMF_MIME_H264_VIDEO_MP4) ||
        !strcmp(FormatType, PVMF_MIME_H264_VIDEO))
    {
        ret = true;
    }

    return ret;
}




bool PVMFRMFFParserNode::ParseRM()
{
    LOGE ("+ParseRM");
    HX_RESULT retVal = HXR_OK;

    iRmParser = rm_parser_create(NULL, rm_error);

    if (!iRmParser)
    {
        LOGE ("rm_parser_create failed !!!");
        return false;
    }

    retVal = rm_parser_init_io(iRmParser, (void*)&iFile, rm_io_read_func, rm_io_seek_func);

    if (retVal != HXR_OK)
    {
        LOGE ("rm_parser_init_io failed !!!");
        return false;
    }

    retVal = rm_parser_read_headers(iRmParser);

    if (retVal != HXR_OK)
    {
        LOGE ("rm_parser_read_headers failed !!!");
        return false;
    }

    LOGE ("-ParseRM");   
    return true;
}






