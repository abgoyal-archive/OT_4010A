 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_node_outport.cpp
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

#include "pvmf_rmffparser_outport.h"

#include "oscl_mem_basic_functions.h"

#include "pv_mime_string_utils.h"

#include "pvmf_node_interface.h"

#include "pvmf_rmffparser_defs.h"

#include <cutils/log.h>


PVMFRMFFParserOutPort::PVMFRMFFParserOutPort(int32 aTag, PVMFNodeInterface* aNode, const char*name)
        : PvmfPortBaseImpl(aTag, aNode, name)
{
    iRMFFParserNode = OSCL_STATIC_CAST(PVMFRMFFParserNode*, aNode);
    Construct();
}


PVMFRMFFParserOutPort::PVMFRMFFParserOutPort(int32 aTag, PVMFNodeInterface* aNode
        , uint32 aInCapacity
        , uint32 aInReserve
        , uint32 aInThreshold
        , uint32 aOutCapacity
        , uint32 aOutReserve
        , uint32 aOutThreshold
        , const char*name)
        : PvmfPortBaseImpl(aTag, aNode, aInCapacity, aInReserve, aInThreshold, aOutCapacity, aOutReserve, aOutThreshold, name)
{
    Construct();
}


PVMFRMFFParserOutPort::~PVMFRMFFParserOutPort()
{
    Disconnect();
    ClearMsgQueues();
}


void PVMFRMFFParserOutPort::Construct()
{
    iLogger = PVLogger::GetLoggerObject("PVMFRMFFParserOutPort");
    oscl_memset(&iStats, 0, sizeof(PvmfPortBaseImplStats));
    iNumFramesGenerated = 0;
    iNumFramesConsumed = 0;
    PvmiCapabilityAndConfigPortFormatImpl::Construct(PVMF_RMFFPARSER_PORT_INPUT_FORMATS, PVMF_RMFFPARSER_PORT_INPUT_FORMATS_VALTYPE);
}

bool PVMFRMFFParserOutPort::IsFormatSupported(PVMFFormatType aFmt)
{
    if (aFmt == PVMF_MIME_REAL_VIDEO ||
            aFmt == PVMF_MIME_YUV420)
    {
        LOGE ("PVMFRMFFParserOutPort::IsFormatSupported return true");
        return true;
    }
    else
    {
        LOGE ("PVMFRMFFParserOutPort::IsFormatSupported return false");
        return false;
    }
}


void PVMFRMFFParserOutPort::FormatUpdated()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFRMFFParserOutPort::FormatUpdated %s", iFormat.getMIMEStrPtr()));
}

PVMFStatus PVMFRMFFParserOutPort::Connect(PVMFPortInterface* aPort)
{
    //PVMF_RMFFPARSERNODE_LOGINFO((0, "PVMFRMFFParserOutPort::Connect: aPort=0x%x", aPort));
    LOGE ("PVMFRMFFParserOutPort::Connect: aPort=0x%x", aPort);

    if (!aPort)
    {
        PVMF_RMFFPARSERNODE_LOGINFO((0, "PVMFRMFFParserOutPort::Connect: Error - Connecting to invalid port"));
        return PVMFErrArgument;
    }

    if (iConnectedPort)
    {
        PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Already connected"));
        return PVMFFailure;
    }

    OsclAny* temp = NULL;
    aPort->QueryInterface(PVMI_CAPABILITY_AND_CONFIG_PVUUID, temp);
    PvmiCapabilityAndConfig *config = OSCL_STATIC_CAST(PvmiCapabilityAndConfig*, temp);

    if (config != NULL)
    {
        if (!(pvmiSetPortFormatSpecificInfoSync(config, PVMF_FORMAT_SPECIFIC_INFO_KEY)))
        {
            PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Unable To Send Format Specific Info To Peer"));
            return PVMFFailure;
        }
        if (!(pvmiSetPortFormatSpecificInfoSync(config, PVMF_FORMAT_SPECIFIC_INFO_PLUS_FIRST_SAMPLE_KEY)))
        {
            PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Unable To Send Format Specific Info Plus First Sample To Peer"));
            return PVMFFailure;
        }
        if (!(pvmiSetPortFormatSpecificInfoSync(config, MOUT_VIDEO_WIDTH_KEY)))
        {
            PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Unable To Decode Width To Peer"));
            return PVMFFailure;
        }
        if (!(pvmiSetPortFormatSpecificInfoSync(config, MOUT_VIDEO_DISPLAY_WIDTH_KEY)))
        {
            PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Unable To Send Display Width To Peer"));
            return PVMFFailure;
        }
        if (!(pvmiSetPortFormatSpecificInfoSync(config, MOUT_VIDEO_HEIGHT_KEY)))
        {
            PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Unable To Send Decode Height To Peer"));
            return PVMFFailure;
        }
        if (!(pvmiSetPortFormatSpecificInfoSync(config, MOUT_VIDEO_DISPLAY_HEIGHT_KEY)))
        {
            PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Unable To Send Display Height To Peer"));
            return PVMFFailure;
        }
    }

    /*
     * Automatically connect the peer.
     */
    if (aPort->PeerConnect(this) != PVMFSuccess)
    {
        PVMF_RMFFPARSERNODE_LOGERROR((0, "PVMFRMFFParserOutPort::Connect: Error - Peer Connect failed"));
        return PVMFFailure;
    }

    iConnectedPort = aPort;

    PortActivity(PVMF_PORT_ACTIVITY_CONNECT);
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserOutPort::getParametersSync(PvmiMIOSession aSession,
        PvmiKeyType aIdentifier,
        PvmiKvp*& aParameters,
        int& num_parameter_elements,
        PvmiCapabilityContext aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aIdentifier);
    OSCL_UNUSED_ARG(aContext);
    PVMF_RMFFPARSERNODE_LOGINFO((0, "PVMFRMFFParserOutPort::getParametersSync: aSession=0x%x, aIdentifier=%s, aParameters=0x%x, num_parameters_elements=%d, aContext=0x%x",
                                  aSession, aIdentifier, aParameters, num_parameter_elements, aContext));

    num_parameter_elements = 0;

    if (pv_mime_strcmp(aIdentifier, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(PVMF_FORMAT_SPECIFIC_INFO_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    else if (pv_mime_strcmp(aIdentifier, PVMF_FORMAT_SPECIFIC_INFO_PLUS_FIRST_SAMPLE_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(PVMF_FORMAT_SPECIFIC_INFO_PLUS_FIRST_SAMPLE_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    else if (pv_mime_strcmp(aIdentifier, MOUT_VIDEO_WIDTH_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(MOUT_VIDEO_WIDTH_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    else if (pv_mime_strcmp(aIdentifier, MOUT_VIDEO_DISPLAY_WIDTH_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(MOUT_VIDEO_DISPLAY_WIDTH_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    else if (pv_mime_strcmp(aIdentifier, MOUT_VIDEO_HEIGHT_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(MOUT_VIDEO_HEIGHT_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    else if (pv_mime_strcmp(aIdentifier, MOUT_VIDEO_DISPLAY_HEIGHT_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(MOUT_VIDEO_DISPLAY_HEIGHT_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    num_parameter_elements = 1;
    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserOutPort::releaseParameters(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(num_elements);

    PVMF_RMFFPARSERNODE_LOGINFO((0, "PVMFRMFFParserOutPort::releaseParameters: aSession=0x%x, aParameters=0x%x, num_elements=%d",
                                  aSession, aParameters, num_elements));

    if (pv_mime_strcmp(aParameters->key, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));

    }
    if (pv_mime_strcmp(aParameters->key, PVMF_FORMAT_SPECIFIC_INFO_PLUS_FIRST_SAMPLE_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));
    }
    if (pv_mime_strcmp(aParameters->key, MOUT_VIDEO_WIDTH_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));
    }
    if (pv_mime_strcmp(aParameters->key, MOUT_VIDEO_DISPLAY_WIDTH_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));
    }
    if (pv_mime_strcmp(aParameters->key, MOUT_VIDEO_HEIGHT_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));
    }
    if (pv_mime_strcmp(aParameters->key, MOUT_VIDEO_DISPLAY_HEIGHT_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));
    }
    return PVMFSuccess;
}

void PVMFRMFFParserOutPort::setParametersSync(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements,
        PvmiKvp * & aRet_kvp)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aParameters);
    OSCL_UNUSED_ARG(aRet_kvp);
    OSCL_UNUSED_ARG(num_elements);

    PVMF_RMFFPARSERNODE_LOGINFO((0, "PVMFRMFFParserOutPort::getParametersSync: aSession=0x%x, aParameters=0x%x, num_elements=%d, aRet_kvp=0x%x",
                                  aSession, aParameters, num_elements, aRet_kvp));

    //OSCL_LEAVE(OsclErrNotSupported);
}

PVMFStatus PVMFRMFFParserOutPort::verifyParametersSync(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aParameters);
    OSCL_UNUSED_ARG(num_elements);

    PVMF_RMFFPARSERNODE_LOGINFO((0, "PVMFRMFFParserOutPort::verifyParametersSync: aSession=0x%x, aParameters=0x%x, num_elements=%d",
                                  aSession, aParameters, num_elements));

    return PVMFErrNotSupported;
}


bool
PVMFRMFFParserOutPort::pvmiSetPortFormatSpecificInfoSync(PvmiCapabilityAndConfig *aPort,
        const char* aFormatValType)
{
    /*
     * Create PvmiKvp for capability settings
     */
    PVRMFFParserNodeTrackPortInfo* trackInfoPtr = NULL;

    if (!(iRMFFParserNode->GetTrackPortInfoForPort(trackInfoPtr, (OSCL_STATIC_CAST(PVMFPortInterface*, this)))))
    {
        return false;
    }

    if (pv_mime_strcmp(aFormatValType, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        OsclMemAllocator alloc;
        PvmiKvp kvp;
        kvp.key = NULL;
        kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
        kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
        if (kvp.key == NULL)
        {
            return false;
        }
        oscl_strncpy(kvp.key, aFormatValType, kvp.length);
        if (trackInfoPtr->iFormatSpecificConfig.getMemFragSize() == 0)
        {
            kvp.value.key_specific_value = 0;
            kvp.capacity = 0;
        }
        else
        {
            kvp.value.key_specific_value = (OsclAny*)(trackInfoPtr->iFormatSpecificConfig.getMemFragPtr());
            kvp.capacity = trackInfoPtr->iFormatSpecificConfig.getMemFragSize();
        }

        PvmiKvp* retKvp = NULL; // for return value
        int32 err;
        OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
        /* ignore the error for now */
        alloc.deallocate((OsclAny*)(kvp.key));
        return true;
    }
    else if (pv_mime_strcmp(aFormatValType, PVMF_FORMAT_SPECIFIC_INFO_PLUS_FIRST_SAMPLE_KEY) == 0)
    {
        if (trackInfoPtr->iFormatSpecificConfigAndFirstSample.getMemFragSize() > 0)
        {
            OsclMemAllocator alloc;
            PvmiKvp kvp;
            kvp.key = NULL;
            kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
            if (kvp.key == NULL)
            {
                return false;
            }
            oscl_strncpy(kvp.key, aFormatValType, kvp.length);

            kvp.value.pUint8_value = (uint8*)(trackInfoPtr->iFormatSpecificConfigAndFirstSample.getMemFragPtr());
            kvp.capacity = trackInfoPtr->iFormatSpecificConfigAndFirstSample.getMemFragSize();
            PvmiKvp* retKvp = NULL; // for return value
            int32 err;
            OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
            /* ignore the error for now */
            alloc.deallocate((OsclAny*)(kvp.key));
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_WIDTH_KEY) == 0))
    {
        int32 width = iRMFFParserNode->FindVideoWidth(trackInfoPtr->iTrackId);
        if (width > 0)
        {
            OsclMemAllocator alloc;
            PvmiKvp kvp;
            kvp.key = NULL;
            kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
            if (kvp.key == NULL)
            {
                return false;
            }
            oscl_strncpy(kvp.key, aFormatValType, kvp.length);

            kvp.value.uint32_value = (uint32)width;
            PvmiKvp* retKvp = NULL; // for return value
            int32 err;
            OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
            /* ignore the error for now */
            alloc.deallocate((OsclAny*)(kvp.key));
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_HEIGHT_KEY) == 0))
    {
        int32 height = iRMFFParserNode->FindVideoHeight(trackInfoPtr->iTrackId);
        if (height > 0)
        {
            OsclMemAllocator alloc;
            PvmiKvp kvp;
            kvp.key = NULL;
            kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
            if (kvp.key == NULL)
            {
                return false;
            }
            oscl_strncpy(kvp.key, aFormatValType, kvp.length);

            kvp.value.uint32_value = (uint32)height;
            PvmiKvp* retKvp = NULL; // for return value
            int32 err;
            OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
            /* ignore the error for now */
            alloc.deallocate((OsclAny*)(kvp.key));
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_DISPLAY_WIDTH_KEY) == 0))
    {
        int32 width = iRMFFParserNode->FindVideoDisplayWidth(trackInfoPtr->iTrackId);
        if (width > 0)
        {
            OsclMemAllocator alloc;
            PvmiKvp kvp;
            kvp.key = NULL;
            kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
            if (kvp.key == NULL)
            {
                return false;
            }
            oscl_strncpy(kvp.key, aFormatValType, kvp.length);

            kvp.value.uint32_value = (uint32)width;
            PvmiKvp* retKvp = NULL; // for return value
            int32 err;
            OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
            /* ignore the error for now */
            alloc.deallocate((OsclAny*)(kvp.key));
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_DISPLAY_HEIGHT_KEY) == 0))
    {
        int32 height = iRMFFParserNode->FindVideoDisplayHeight(trackInfoPtr->iTrackId);
        if (height > 0)
        {
            OsclMemAllocator alloc;
            PvmiKvp kvp;
            kvp.key = NULL;
            kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
            if (kvp.key == NULL)
            {
                return false;
            }
            oscl_strncpy(kvp.key, aFormatValType, kvp.length);

            kvp.value.uint32_value = (uint32)height;
            PvmiKvp* retKvp = NULL; // for return value
            int32 err;
            OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
            /* ignore the error for now */
            alloc.deallocate((OsclAny*)(kvp.key));
        }
        return true;
    }

    return false;
}

bool
PVMFRMFFParserOutPort::pvmiGetPortFormatSpecificInfoSync(const char* aFormatValType,
        PvmiKvp*& aKvp)
{
    /*
     * Create PvmiKvp for capability settings
     */
    PVRMFFParserNodeTrackPortInfo* trackInfoPtr = NULL;
    if (!(iRMFFParserNode->GetTrackPortInfoForPort(trackInfoPtr, OSCL_STATIC_CAST(PVMFPortInterface*, this))))
    {
        return false;
    }
    if (pv_mime_strcmp(aFormatValType, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        OsclMemAllocator alloc;
        aKvp->key = NULL;
        aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
        aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
        if (aKvp->key == NULL)
        {
            return false;
        }
        oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);
        if (trackInfoPtr->iFormatSpecificConfig.getMemFragSize() == 0)
        {
            aKvp->value.key_specific_value = 0;
            aKvp->capacity = 0;
        }
        else
        {
            aKvp->value.key_specific_value = (OsclAny*)(trackInfoPtr->iFormatSpecificConfig.getMemFragPtr());
            aKvp->capacity = trackInfoPtr->iFormatSpecificConfig.getMemFragSize();
        }
        return true;
    }
    else if (pv_mime_strcmp(aFormatValType, PVMF_FORMAT_SPECIFIC_INFO_PLUS_FIRST_SAMPLE_KEY) == 0)
    {
        if (trackInfoPtr->iFormatSpecificConfigAndFirstSample.getMemFragSize() > 0)
        {
            OsclMemAllocator alloc;
            aKvp->key = NULL;
            aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
            if (aKvp->key == NULL)
            {
                return false;
            }
            oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);

            aKvp->value.pUint8_value = (uint8*)(trackInfoPtr->iFormatSpecificConfigAndFirstSample.getMemFragPtr());
            aKvp->capacity = trackInfoPtr->iFormatSpecificConfigAndFirstSample.getMemFragSize();
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_WIDTH_KEY) == 0))
    {
        int32 width = iRMFFParserNode->FindVideoWidth(trackInfoPtr->iTrackId);
        if (width > 0)
        {
            OsclMemAllocator alloc;
            aKvp->key = NULL;
            aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
            if (aKvp->key == NULL)
            {
                return false;
            }
            oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);

            aKvp->value.uint32_value = (uint32)width;
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_HEIGHT_KEY) == 0))
    {
        int32 height = iRMFFParserNode->FindVideoHeight(trackInfoPtr->iTrackId);
        if (height > 0)
        {
            OsclMemAllocator alloc;
            aKvp->key = NULL;
            aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
            if (aKvp->key == NULL)
            {
                return false;
            }
            oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);

            aKvp->value.uint32_value = (uint32)height;
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_DISPLAY_WIDTH_KEY) == 0))
    {
        int32 width = iRMFFParserNode->FindVideoDisplayWidth(trackInfoPtr->iTrackId);
        if (width > 0)
        {
            OsclMemAllocator alloc;
            aKvp->key = NULL;
            aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
            if (aKvp->key == NULL)
            {
                return false;
            }
            oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);

            aKvp->value.uint32_value = (uint32)width;
        }
        return true;
    }
    else if ((pv_mime_strcmp(aFormatValType, MOUT_VIDEO_DISPLAY_HEIGHT_KEY) == 0))
    {
        int32 height = iRMFFParserNode->FindVideoDisplayHeight(trackInfoPtr->iTrackId);
        if (height > 0)
        {
            OsclMemAllocator alloc;
            aKvp->key = NULL;
            aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
            if (aKvp->key == NULL)
            {
                return false;
            }
            oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);

            aKvp->value.uint32_value = (uint32)height;
        }
        return true;
    }
    return false;
}



