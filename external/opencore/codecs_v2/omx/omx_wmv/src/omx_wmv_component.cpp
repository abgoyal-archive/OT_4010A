
 /*****************************************************************************
 * OMX WMV component
 * Description:
 * ------------
 *   The file implements OMX WMV component (for VC1 playback)
 *
 * Author:
 * -------
 *   Morris Yang (mtk03147)
 *
 ****************************************************************************/
#include "oscl_base.h"
#include "pv_omxdefs.h"
#include "omx_wmv_component.h"

#ifdef MHAL_H_INCLUDED
#include "MediaHal.h"
#endif

#if PROXY_INTERFACE
#include "omx_proxy_interface.h"
#endif

// Use default DLL entry point
#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

#include <linux/rtpm_prio.h>

#if 1
#include "oscl_profile.h"
#include "oscl_file_cfg.h"
#endif

#ifdef TRICKY_DISABLE_MEMCPY
#include "oscl_pmem.h"
#endif


#include "../../../../../../frameworks/base/media/libstagefright/pmem_util/pmem_util.h"
#include <cutils/properties.h>


// ---------------------------------------------------------------------
//
#undef LOG_TAG
#define LOG_TAG "OMX_WMV_COMPONENT"

#ifndef _WIN32
#include <utils/Log.h>

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

static bool bShowLogProfile = false;
#define LOG_PROFILE(...) \
{\
    if (bShowLogProfile)\
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

#ifdef MT6516_MP4DEC_OMX_PROFILING
#include <time.h>
#include <sys/time.h>
#endif

#ifdef MT6516_MP4DEC_OMX_PROFILING
FILE *fp_mt6516_mp4dec_omx_profiling = NULL;
#endif

unsigned int g_input_seq_num = 0;
unsigned int g_output_seq_num = 0;

OSCL_DLL_ENTRY_POINT_DEFAULT()


// This function is called by OMX_GetHandle and it creates an instance of the mpeg4 component AO
OSCL_EXPORT_REF OMX_ERRORTYPE WmvOmxComponentFactory(OMX_OUT OMX_HANDLETYPE* pHandle, OMX_IN  OMX_PTR pAppData, OMX_IN OMX_PTR pProxy , OMX_STRING aOmxLibName, OMX_PTR &aOmxLib, OMX_PTR aOsclUuid, OMX_U32 &aRefCount)
{
    OSCL_UNUSED_ARG(aOmxLibName);
    OSCL_UNUSED_ARG(aOmxLib);
    OSCL_UNUSED_ARG(aOsclUuid);
    OSCL_UNUSED_ARG(aRefCount);


    OpenmaxWmvAO* pOpenmaxAOType;
    OMX_ERRORTYPE Status;

    // move InitMpeg4OmxComponentFields content to actual constructor

    pOpenmaxAOType = (OpenmaxWmvAO*) OSCL_NEW(OpenmaxWmvAO, ());

    if (NULL == pOpenmaxAOType)
    {
        return OMX_ErrorInsufficientResources;
    }

    //Call the construct component to initialize OMX types
    Status = pOpenmaxAOType->ConstructComponent(pAppData, pProxy);

    *pHandle = pOpenmaxAOType->GetOmxHandle();

    return Status;
    ///////////////////////////////////////////////////////////////////////////////////////
}

// This function is called by OMX_FreeHandle when component AO needs to be destroyed
OSCL_EXPORT_REF OMX_ERRORTYPE WmvOmxComponentDestructor(OMX_IN OMX_HANDLETYPE pHandle, OMX_PTR &aOmxLib, OMX_PTR aOsclUuid, OMX_U32 &aRefCount)
{
    OSCL_UNUSED_ARG(aOmxLib);
    OSCL_UNUSED_ARG(aOsclUuid);
    OSCL_UNUSED_ARG(aRefCount);

    // get pointer to component AO
    OpenmaxWmvAO* pOpenmaxAOType = (OpenmaxWmvAO*)((OMX_COMPONENTTYPE*)pHandle)->pComponentPrivate;

    // clean up decoder, OMX component stuff
    pOpenmaxAOType->DestroyComponent();

    // destroy the AO class
    OSCL_DELETE(pOpenmaxAOType);

    return OMX_ErrorNone;
}


#if (DYNAMIC_LOAD_OMX_WMV_COMPONENT)
class WmvOmxSharedLibraryInterface: public OsclSharedLibraryInterface,
        public OmxSharedLibraryInterface

{
    public:
        OsclAny *QueryOmxComponentInterface(const OsclUuid& aOmxTypeId, const OsclUuid& aInterfaceId)
        {
            if (PV_OMX_WMVDEC_UUID == aOmxTypeId)
            {
                if (PV_OMX_CREATE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(&WmvOmxComponentFactory));
                }
                else if (PV_OMX_DESTROY_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(&WmvOmxComponentDestructor));
                }
            }

            return NULL;
        };
        OsclAny *SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == PV_OMX_SHARED_INTERFACE)
            {
                return OSCL_STATIC_CAST(OmxSharedLibraryInterface*, this);
            }
            return NULL;
        };
        WmvOmxSharedLibraryInterface() {};
};

// function to obtain the interface object from the shared library
extern "C"
{
    OSCL_EXPORT_REF OsclAny* PVGetInterface()
    {
        return (OsclAny*) OSCL_NEW(WmvOmxSharedLibraryInterface, ());
    }

    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        WmvOmxSharedLibraryInterface* module = (WmvOmxSharedLibraryInterface*)aInstance;
        OSCL_DELETE(module);
    }
}

#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

OMX_ERRORTYPE OpenmaxWmvAO::ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy)
{
    iName = "OpenmaxMpeg4AO";
    
    g_input_seq_num = 0;
    g_output_seq_num = 0;

    ComponentPortType *pInPort, *pOutPort;
    OMX_ERRORTYPE Status;

    iNumPorts = 2;
    iOmxComponent.nSize = sizeof(OMX_COMPONENTTYPE);
    iOmxComponent.pComponentPrivate = (OMX_PTR) this;  // pComponentPrivate points to THIS component AO class
    ipComponentProxy = pProxy;
    iOmxComponent.pApplicationPrivate = pAppData; // init the App data


#if PROXY_INTERFACE
    iPVCapabilityFlags.iIsOMXComponentMultiThreaded = OMX_TRUE;

    iOmxComponent.SendCommand = OpenmaxWmvAO::BaseComponentProxySendCommand;
    iOmxComponent.GetParameter = OpenmaxWmvAO::BaseComponentProxyGetParameter;
    iOmxComponent.SetParameter = OpenmaxWmvAO::BaseComponentProxySetParameter;
    iOmxComponent.GetConfig = OpenmaxWmvAO::BaseComponentProxyGetConfig;
    iOmxComponent.SetConfig = OpenmaxWmvAO::BaseComponentProxySetConfig;
    iOmxComponent.GetExtensionIndex = OpenmaxWmvAO::BaseComponentProxyGetExtensionIndex;
    iOmxComponent.GetState = OpenmaxWmvAO::BaseComponentProxyGetState;
    iOmxComponent.UseBuffer = OpenmaxWmvAO::BaseComponentProxyUseBuffer;
    iOmxComponent.AllocateBuffer = OpenmaxWmvAO::BaseComponentProxyAllocateBuffer;
    iOmxComponent.FreeBuffer = OpenmaxWmvAO::BaseComponentProxyFreeBuffer;
    iOmxComponent.EmptyThisBuffer = OpenmaxWmvAO::BaseComponentProxyEmptyThisBuffer;
    iOmxComponent.FillThisBuffer = OpenmaxWmvAO::BaseComponentProxyFillThisBuffer;

#else
    iPVCapabilityFlags.iIsOMXComponentMultiThreaded = OMX_FALSE;

    iOmxComponent.SendCommand = OpenmaxWmvAO::BaseComponentSendCommand;
    iOmxComponent.GetParameter = OpenmaxWmvAO::BaseComponentGetParameter;
    iOmxComponent.SetParameter = OpenmaxWmvAO::BaseComponentSetParameter;
    iOmxComponent.GetConfig = OpenmaxWmvAO::BaseComponentGetConfig;
    iOmxComponent.SetConfig = OpenmaxWmvAO::BaseComponentSetConfig;
    iOmxComponent.GetExtensionIndex = OpenmaxWmvAO::BaseComponentGetExtensionIndex;
    iOmxComponent.GetState = OpenmaxWmvAO::BaseComponentGetState;
    iOmxComponent.UseBuffer = OpenmaxWmvAO::BaseComponentUseBuffer;
    iOmxComponent.AllocateBuffer = OpenmaxWmvAO::BaseComponentAllocateBuffer;
    iOmxComponent.FreeBuffer = OpenmaxWmvAO::BaseComponentFreeBuffer;
    iOmxComponent.EmptyThisBuffer = OpenmaxWmvAO::BaseComponentEmptyThisBuffer;
    iOmxComponent.FillThisBuffer = OpenmaxWmvAO::BaseComponentFillThisBuffer;
#endif

    iOmxComponent.SetCallbacks = OpenmaxWmvAO::BaseComponentSetCallbacks;
    iOmxComponent.nVersion.s.nVersionMajor = SPECVERSIONMAJOR;
    iOmxComponent.nVersion.s.nVersionMinor = SPECVERSIONMINOR;
    iOmxComponent.nVersion.s.nRevision = SPECREVISION;
    iOmxComponent.nVersion.s.nStep = SPECSTEP;

    // PV capability
    iPVCapabilityFlags.iOMXComponentSupportsExternalInputBufferAlloc = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsExternalOutputBufferAlloc = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsMovableInputBuffers = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsPartialFrames = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentUsesNALStartCodes = OMX_FALSE;
    iPVCapabilityFlags.iOMXComponentCanHandleIncompleteFrames = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentUsesFullAVCFrames = OMX_FALSE;

    if (ipAppPriv)
    {
        oscl_free(ipAppPriv);
        ipAppPriv = NULL;
    }

    ipAppPriv = (ComponentPrivateType*) oscl_malloc(sizeof(ComponentPrivateType));

    if (NULL == ipAppPriv)
    {
        return OMX_ErrorInsufficientResources;
    }

    //Construct base class now
    Status = ConstructBaseComponent(pAppData);

    if (OMX_ErrorNone != Status)
    {
        return Status;
    }

    /** Domain specific section for the ports. */
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nPortIndex = OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.eDomain = OMX_PortDomainVideo;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.cMIMEType = (OMX_STRING)"video/x-ms-wmv";
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.eCompressionFormat = OMX_VIDEO_CodingWMV;

    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.pNativeRender = 0;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.bFlagErrorConcealment = OMX_FALSE;

    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.eColorFormat = OMX_COLOR_FormatUnused;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.nFrameWidth = 176;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.nFrameHeight = 144;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.nBitrate = 64000;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.video.xFramerate = (15 << 16);
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.eDir = OMX_DirInput;
    //Set to a default value, will change later during setparameter call
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferCountActual = NUMBER_INPUT_BUFFER_WMV;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferCountMin = 1;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferSize = INPUT_BUFFER_SIZE_WMV;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.bEnabled = OMX_TRUE;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.bPopulated = OMX_FALSE;


    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nPortIndex = OMX_PORT_OUTPUTPORT_INDEX;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.eDomain = OMX_PortDomainVideo;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.cMIMEType = (OMX_STRING)"raw";
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.pNativeRender = 0;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.bFlagErrorConcealment = OMX_FALSE;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
    // Ryan Lin !!
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.eColorFormat = OMX_COLOR_FormatYUV420Planar;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nFrameWidth = 176; //320; //176;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nStride = 176;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nFrameHeight = 144; //240; //144;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nSliceHeight = 144;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nBitrate = 64000;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.xFramerate = (15 << 16);
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.eDir = OMX_DirOutput;
    //Set to a default value, will change later during setparameter call
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferCountActual = NUMBER_OUTPUT_BUFFER_WMV;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferCountMin = 1;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferSize = OUTPUT_BUFFER_SIZE_WMV;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.bEnabled = OMX_TRUE;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.bPopulated = OMX_FALSE;

    //Default values for mpeg4 video param port
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->VideoWmv.nPortIndex = OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->VideoWmv.eFormat = OMX_VIDEO_WMVFormat9;

    ipPorts[OMX_PORT_INPUTPORT_INDEX]->ProfileLevel.nPortIndex = OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->ProfileLevel.nProfileIndex = 0;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->ProfileLevel.eProfile = 0;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->ProfileLevel.eLevel = 0;
    oscl_strncpy((OMX_STRING)iComponentRole, (OMX_STRING)"video_decoder.wmv", OMX_MAX_STRINGNAME_SIZE);

    iPortTypesParam.nPorts = 2;
    iPortTypesParam.nStartPortNumber = 0;

    pInPort = (ComponentPortType*) ipPorts[OMX_PORT_INPUTPORT_INDEX];
    pOutPort = (ComponentPortType*) ipPorts[OMX_PORT_OUTPUTPORT_INDEX];

    pInPort->ActualNumPortFormatsSupported = 1;

    //OMX_VIDEO_PARAM_PORTFORMATTYPE INPUT PORT SETTINGS
    //On input port for index 0
    SetHeader(&pInPort->VideoParam[0], sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
    pInPort->VideoParam[0].nPortIndex = 0;
    pInPort->VideoParam[0].nIndex = 0;

    pInPort->VideoParam[0].eCompressionFormat = OMX_VIDEO_CodingWMV;

    pInPort->VideoParam[0].eColorFormat = OMX_COLOR_FormatUnused;

    pOutPort->ActualNumPortFormatsSupported = 1;

    //OMX_VIDEO_PARAM_PORTFORMATTYPE OUTPUT PORT SETTINGS
    //On output port for index 0
    SetHeader(&pOutPort->VideoParam[0], sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
    pOutPort->VideoParam[0].nPortIndex = 1;
    pOutPort->VideoParam[0].nIndex = 0;
    pOutPort->VideoParam[0].eCompressionFormat = OMX_VIDEO_CodingUnused;
    pOutPort->VideoParam[0].eColorFormat = OMX_COLOR_FormatYUV420Planar;

    iUseExtTimestamp = OMX_TRUE;

    if (ipWmvDecoderObject)
    {
        OSCL_DELETE(ipWmvDecoderObject);
        ipWmvDecoderObject = NULL;
    }
    ipWmvDecoderObject = OSCL_NEW(WmvDecoder_OMX, ());
    oscl_memset(ipWmvDecoderObject, 0, sizeof(WmvDecoder_OMX));

#if PROXY_INTERFACE
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSendCommand = BaseComponentSendCommand;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetParameter = BaseComponentGetParameter;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSetParameter = BaseComponentSetParameter;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetConfig = BaseComponentGetConfig;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSetConfig = BaseComponentSetConfig;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetExtensionIndex = BaseComponentGetExtensionIndex;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetState = BaseComponentGetState;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentUseBuffer = BaseComponentUseBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentAllocateBuffer = BaseComponentAllocateBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentFreeBuffer = BaseComponentFreeBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentEmptyThisBuffer = BaseComponentEmptyThisBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentFillThisBuffer = BaseComponentFillThisBuffer;

#endif

    return OMX_ErrorNone;
}



OMX_ERRORTYPE OpenmaxWmvAO::DestroyComponent()
{
    if (OMX_FALSE != iIsInit)
    {
        ComponentDeInit();
    }

    //Destroy the base class now
    DestroyBaseComponent();

    if (ipWmvDecoderObject)
    {
        OSCL_DELETE(ipWmvDecoderObject);
        ipWmvDecoderObject = NULL;
    }

    if (ipAppPriv)
    {
        ipAppPriv->CompHandle = NULL;

        oscl_free(ipAppPriv);
        ipAppPriv = NULL;
    }

    return OMX_ErrorNone;
}



void OpenmaxWmvAO::ComponentBufferMgmtWithoutMarker()
{
    //This common routine has been written in the base class
    TempInputBufferMgmtWithoutMarker();
}


void OpenmaxWmvAO::ProcessData()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ProcessData IN"));
    if (!iEndOfFrameFlag)
    {
        DecodeWithoutMarker();
    }
    else
    {
        DecodeWithMarker();
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ProcessData OUT"));
}


void OpenmaxWmvAO::DecodeWithoutMarker()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker IN"));
    //LOGE ("@@ OpenmaxWmvAO : DecodeWithoutMarker IN");

    QueueType* pInputQueue = ipPorts[OMX_PORT_INPUTPORT_INDEX]->pBufferQueue;
    QueueType* pOutputQueue = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->pBufferQueue;
    ComponentPortType*  pOutPort = ipPorts[OMX_PORT_OUTPUTPORT_INDEX];
    OMX_COMPONENTTYPE  *pHandle = &iOmxComponent;

    OMX_U8*                 pOutBuffer;
    OMX_U32                 OutputLength;
    OMX_U8*                 pTempInBuffer;
    OMX_U32                 TempInLength;
    OMX_BOOL                DecodeReturn;
    OMX_BOOL                MarkerFlag = OMX_FALSE;
    OMX_BOOL                ResizeNeeded = OMX_FALSE;

    OMX_U32 TempInputBufferSize = (2 * sizeof(uint8) * (ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferSize));

    OMX_U32 CurrWidth =  ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nFrameWidth;
    OMX_U32 CurrHeight = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nFrameHeight;

    if ((!iIsInputBufferEnded) || iEndofStream)
    {
#ifdef MT6573_MFV_HW		
		//LOGD("DEC_WITHOUT_MARKER:ENTER");
		if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
		{
			if(OMX_FALSE == ipWmvDecoderObject->bBufferProcessDone)
			{
				OMX_BUFFERHEADERTYPE*	ipOutBuf = ipWmvDecoderObject->GetDisplayBuffer(); 
				if(ipOutBuf)
				{
					//Send the output buffer back after decode
					//LOGD("[OUT_QUEUE]1 RETURN OUTPUT:%08x", ipOutBuf);
					ReturnOutputBuffer(ipOutBuf, pOutPort);
					ipOutBuf = NULL;
					RunIfNotReady();
					return;
				}

				while(NULL != ipWmvDecoderObject->GetFreeBuffer())
					;
				
				ipWmvDecoderObject->bBufferProcessDone = OMX_TRUE;
			}
			
			//Check whether a new output buffer is available or not
			if(NULL == ipOutputBuffer)
			{				
			if (0 == (GetQueueNumElem(pOutputQueue)))
			{
				PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker OUT output buffer unavailable"));
				//Store the mark data for output buffer, as it will be overwritten next time
				if (NULL != ipTargetComponent)
				{
					ipTempTargetComponent = ipTargetComponent;
					iTempTargetMarkData = iTargetMarkData;
					iMarkPropagate = OMX_TRUE;
				}
				return;
			}
			OMX_U32 r4Count = NUMBER_OUTPUT_BUFFER_WMV; 		
			while(r4Count != 0)
			{
				ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
				if (NULL == ipOutputBuffer)
				{
					PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker Error, output buffer dequeue returned NULL, OUT"));
					iNewInBufferRequired = OMX_FALSE;
					return;
				}
				if(OMX_TRUE == ipWmvDecoderObject->isFreeBuffer(ipOutputBuffer))
				{
					break;
				}
				else
				{
					Queue(pOutputQueue, (void*)ipOutputBuffer);
					r4Count--;
				}
			}
			if (r4Count == 0)
			{
				LOGE("[WARNING_WITH_MARKER] return and wait for another ipOutputBuffer");
				iNewInBufferRequired = OMX_FALSE;
				return;
			}			
				//LOGD("[OUT_QUEUE]GET OUTPUT:%08x", ipOutputBuffer);
			}
		}
		else
		{
				//LOGD("DEC_WITHOUT_MARKER:ENTER ELSE");
#endif	
        //Check whether a new output buffer is available or not
        if (0 == (GetQueueNumElem(pOutputQueue)))
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker OUT output buffer unavailable"));
            //Store the mark data for output buffer, as it will be overwritten next time
            if (NULL != ipTargetComponent)
            {
                ipTempTargetComponent = ipTargetComponent;
                iTempTargetMarkData = iTargetMarkData;
                iMarkPropagate = OMX_TRUE;
            }
            return;
        }

        ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
        if (NULL == ipOutputBuffer)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker Error, output buffer dequeue returned NULL, OUT"));
            return;
        }

#ifdef MT6573_MFV_HW
		}
#endif

        //Do not proceed if the output buffer can't fit the YUV data
        if ((ipOutputBuffer->nAllocLen < (OMX_U32)((((CurrWidth + 15) >> 4) << 4) *(((CurrHeight + 15) >> 4) << 4) * 3 / 2)) && (OMX_TRUE == ipWmvDecoderObject->WmvInitCompleteFlag))
        {
            ipOutputBuffer->nFilledLen = 0;
			//LOGD("[OUT_QUEUE]2 RETURN OUTPUT:%08x", ipOutputBuffer);
            ReturnOutputBuffer(ipOutputBuffer, pOutPort);

getReturnOutputBufferTime(ipOutputBuffer);

            ipOutputBuffer = NULL;
            return;
        }

getOutputDeQueueTime(ipOutputBuffer);
        
        ipOutputBuffer->nFilledLen = 0;


        /* Code for the marking buffer. Takes care of the OMX_CommandMarkBuffer
         * command and hMarkTargetComponent as given by the specifications
         */
        if (ipMark != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent = ipMark->hMarkTargetComponent;
            ipOutputBuffer->pMarkData = ipMark->pMarkData;
            ipMark = NULL;
        }

        if ((OMX_TRUE == iMarkPropagate) && (ipTempTargetComponent != ipTargetComponent))
        {
            ipOutputBuffer->hMarkTargetComponent = ipTempTargetComponent;
            ipOutputBuffer->pMarkData = iTempTargetMarkData;
            ipTempTargetComponent = NULL;
            iMarkPropagate = OMX_FALSE;
        }
        else if (ipTargetComponent != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent = ipTargetComponent;
            ipOutputBuffer->pMarkData = iTargetMarkData;
            ipTargetComponent = NULL;
            iMarkPropagate = OMX_FALSE;

        }
        //Mark buffer code ends here

        pOutBuffer = ipOutputBuffer->pBuffer;
        OutputLength = 0;

        pTempInBuffer = ipTempInputBuffer + iTempConsumedLength;
        TempInLength = iTempInputBufferLength;

        //Output buffer is passed as a short pointer
        // Ryan Lin !!! for performace calculation

        if (iFrameCount > 0)
        {

//getMp4DecodeVideoInTime();

        }
#if 0
        long _starttime = OsclTickCount::getTimeMs();
#endif
			//LOGD("DEC_WITHOUT_MARKER:DEC");

        DecodeReturn = ipWmvDecoderObject->WmvDecodeVideo(pOutBuffer, (OMX_U32*) & OutputLength,
                       &(pTempInBuffer),
                       &TempInLength,
                       &(ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam),
                       &iFrameCount,
                       MarkerFlag,
		       &ResizeNeeded,
		       &(ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam),
		       iFrameTimestamp,
		       ipOutputBuffer);
#if 0
        LOG_PROFILE("dec frame use %lld ms", OsclTickCount::getTimeMs() - _starttime);
#endif        
			if (iFrameCount > 1)
			{				 
		getWmvDecodeVideoOutTime();
			}

		if(iFrameCount == 1)
		{
			if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
			{
				ResizeNeeded = OMX_TRUE;
				ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.eColorFormat = OMX_COLOR_FormatVendorMTKYUV;
				ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->VideoParam[0].eColorFormat = OMX_COLOR_FormatVendorMTKYUV;
				LOGD("[WithoutMarker] HW_Supported = TRUE");
			}
			else
			{
				LOGD("[WithoutMarker] HW_Supported = FALSE");
			}
		}

        ipOutputBuffer->nFilledLen = OutputLength;

        //offset not required in our case, set it to zero
        ipOutputBuffer->nOffset = 0;

        //If decoder returned error, report it to the client via a callback
        if (!DecodeReturn && OMX_FALSE == ipWmvDecoderObject->WmvInitCompleteFlag)
        {
            // initialization error, stop playback
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker ErrorBadParameter callback send"));

            (*(ipCallbacks->EventHandler))
            (pHandle,
             iCallbackData,
             OMX_EventError,
             OMX_ErrorBadParameter,
             0,
             NULL);
        }
        else if (!DecodeReturn && OMX_FALSE == iEndofStream)
        {
            // decoding error
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker ErrorStreamCorrupt callback send"));

            (*(ipCallbacks->EventHandler))
            (pHandle,
             iCallbackData,
             OMX_EventError,
             OMX_ErrorStreamCorrupt,
             0,
             NULL);
        }

        if(iFrameCount == 1)
        {
            struct sched_param sched_p;
            sched_getparam(0, &sched_p);
                
            if (OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
            {
                // [Honda] use Keene's RR setting
                //sched_p.sched_priority = 75;
                sched_p.sched_priority = RTPM_PRIO_OMX_VIDEO;

                if (0 != sched_setscheduler(0, SCHED_RR, &sched_p)) 
                {
                    LOGE("[%s] failed, errno: %d", __func__, errno);
                } 
                else 
                {
                    sched_p.sched_priority = 0;
                    sched_getparam(0, &sched_p);
                    LOGD("!!!!!*****************************************************************!!!!!");
                    LOGD("!!!!!    [WMV_DEC] sched_setscheduler ok, RR priority: %d", sched_p.sched_priority);
                    LOGD("!!!!!*****************************************************************!!!!!");
                }
#if 0
				char thread_name[20]="WMV_DEC";
				int result = 0;
				 
				result = mtk_AdjustPrio(thread_name);
#endif
            }
            else
            {
                sched_p.sched_priority = 0;

                if (0 != sched_setscheduler(0, SCHED_NORMAL, &sched_p)) 
                {
                    LOGE("[%s] failed, errno: %d", __func__, errno);
                } 
                else 
                {
                    sched_p.sched_priority = 100;
                    sched_getparam(0, &sched_p);
                    LOGD("!!!!!*****************************************************************!!!!!");
                    LOGD("!!!!!    [WMV_DEC] sched_setscheduler ok, NORMAL priority: %d", sched_p.sched_priority);
                    LOGD("!!!!!*****************************************************************!!!!!");
                }
            }
        }

        if (ResizeNeeded == OMX_TRUE)
        {
            g_output_seq_num = 0; 
            // send port settings changed event
            OMX_COMPONENTTYPE* pHandle = (OMX_COMPONENTTYPE*) ipAppPriv->CompHandle;
				LOGD("----OMX_EventPortSettingsChanged----");

            // set the flag to disable further processing until Client reacts to this
            //  by doing dynamic port reconfiguration
            iResizePending = OMX_TRUE;

            (*(ipCallbacks->EventHandler))
            (pHandle,
             iCallbackData,
             OMX_EventPortSettingsChanged, //The command was completed
             OMX_PORT_OUTPUTPORT_INDEX,
             0,
             NULL);
			LOGD("----OMX_EventPortSettingsChanged done----");
        }
		/*
        else if((iFrameCount == 1) && (OMX_TRUE == ipMpegDecoderObject->MP4DecHwSupported()))
        {
            g_output_seq_num = 0;
            // send port settings changed event
            OMX_COMPONENTTYPE* pHandle = (OMX_COMPONENTTYPE*) ipAppPriv->CompHandle;

            // set the flag to disable further processing until Client reacts to this
            //	by doing dynamic port reconfiguration
            iResizePending = OMX_TRUE;
            
            LOGD("----OMX_EventPortSettingsChanged----");
            (*(ipCallbacks->EventHandler))
             (pHandle,
             iCallbackData,
             OMX_EventPortSettingsChanged, //The command was completed
             OMX_PORT_OUTPUTPORT_INDEX,
             0,
             NULL);
        }
		*/
        
        //Set the timestamp equal to the input buffer timestamp
        ipOutputBuffer->nTimeStamp = iFrameTimestamp;

        iTempConsumedLength += (iTempInputBufferLength - TempInLength);
        iTempInputBufferLength = TempInLength;

        //Do not decode if big buffer is less than half the size
        if (TempInLength < (TempInputBufferSize >> 1))
        {
            iIsInputBufferEnded = OMX_TRUE;
            iNewInBufferRequired = OMX_TRUE;
        }

#ifdef MT6573_MFV_HW
		/* If EOS flag has come from the client & there are no more
		 * input buffers to decode, send the callback to the client
		 */
		if (OMX_TRUE == iEndofStream)
		{
						LOGE("DEC_WITH_MARKER:EOS");
			if ((0 == iTempInputBufferLength) || (!DecodeReturn))
			{
				//handle final decode				
								if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
				{
					LOGE ("DEC_WITH_MARKER:EOS HW");
					if(OMX_FALSE == ipWmvDecoderObject->bBufferProcessDone)
					{
						OMX_BUFFERHEADERTYPE*	ipOutBuf = ipWmvDecoderObject->GetDisplayBuffer(); 
						if(ipOutBuf)
						{
							//Send the output buffer back after decode
							//LOGD("[OUT_QUEUE]3 RETURN OUTPUT:%08x", ipOutBuf);
							ReturnOutputBuffer(ipOutBuf, pOutPort);
							ipOutBuf = NULL;
							RunIfNotReady();
							return;
						}

						while(NULL != ipWmvDecoderObject->GetFreeBuffer())
							;
						
						ipWmvDecoderObject->bBufferProcessDone = OMX_TRUE;
					}
					if(OMX_FALSE == bFinalDecDone )
					{
						//decode final					
						if(OMX_FALSE == ipWmvDecoderObject->WmvDecodeVideoFinal())
						{
							while(1)
								LOGE("decode final fail");	
							
						}
						bFinalDecDone = OMX_TRUE;
						RunIfNotReady();
						return;
					}

					/*
					if(OMX_FALSE == ipMpegDecoderObject->bBufferProcessDone)
					{
						while(1)
						{
							ipOutputBuffer = ipMpegDecoderObject->GetDisplayBuffer();	
							if(ipOutputBuffer)
							{
								//Send the output buffer back after decode
								ReturnOutputBuffer(ipOutputBuffer, pOutPort);
								ipOutputBuffer = NULL;
							}
							else
							{
								break;
							}
						}
						
						while(NULL != ipMpegDecoderObject->GetFreeBuffer())
							;

						ipMpegDecoderObject->bBufferProcessDone = OMX_TRUE;
					}
					*/
				}
				
				PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker EOS callback send"));
				LOGW("OpenmaxWmvAO : DecodeWithoutMarker EOS callback send");
				(*(ipCallbacks->EventHandler))
				(pHandle,
				 iCallbackData,
				 OMX_EventBufferFlag,
				 1,
				 OMX_BUFFERFLAG_EOS,
				 NULL);
				LOGD("OpenmaxWmvAO : DecodeWithoutMarker EOS callback send done");

				iNewInBufferRequired = OMX_TRUE;

//Jackal: put here instead of put after ReturnOutputBuffer(ipOutputBuffer, pOutPort);
//because we don't need use this profiling time and we need use OMX_TRUE == iEndofStream
getReturnOutputBufferTime(ipOutputBuffer);

				iEndofStream = OMX_FALSE;
				//LOGD("return final output buffer");
				//LOGD("[OUT_QUEUE]4 RETURN OUTPUT:%08x", ipOutputBuffer);

				ipOutputBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
				ReturnOutputBuffer(ipOutputBuffer, pOutPort);
				ipOutputBuffer = NULL;
				//LOGD("return final output buffer done");

				PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker OUT"));

				return;
			}
		}
	
		if(OMX_FALSE == ipWmvDecoderObject->WMVDecHwSupported() || iFrameCount == 1)
		{
			//Send the output buffer back after decode
			//LOGD("[OUT_QUEUE]5 RETURN OUTPUT:%08x", ipOutputBuffer);
			ReturnOutputBuffer(ipOutputBuffer, pOutPort);
		}

getReturnOutputBufferTime(ipOutputBuffer);

		ipOutputBuffer = NULL;

#else	
        /* If EOS flag has come from the client & there are no more
         * input buffers to decode, send the callback to the client
         */
        if (OMX_TRUE == iEndofStream)
        {
            if ((0 == iTempInputBufferLength) || (!DecodeReturn))
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker EOS callback send"));

                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventBufferFlag,
                 1,
                 OMX_BUFFERFLAG_EOS,
                 NULL);

                iNewInBufferRequired = OMX_TRUE;

//Jackal: put here instead of put after ReturnOutputBuffer(ipOutputBuffer, pOutPort);
//because we don't need use this profiling time and we need use OMX_TRUE == iEndofStream
getReturnOutputBufferTime(ipOutputBuffer);

                iEndofStream = OMX_FALSE;

                ipOutputBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
                ReturnOutputBuffer(ipOutputBuffer, pOutPort);
                ipOutputBuffer = NULL;

                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker OUT"));

                return;
            }
        }

        //Send the output buffer back after decode
        ReturnOutputBuffer(ipOutputBuffer, pOutPort);

getReturnOutputBufferTime(ipOutputBuffer);
        
        ipOutputBuffer = NULL;
#endif


        /* If there is some more processing left with current buffers, re-schedule the AO
         * Do not go for more than one round of processing at a time.
         * This may block the AO longer than required.
         */
        if ((TempInLength != 0 || GetQueueNumElem(pInputQueue) > 0) && (GetQueueNumElem(pOutputQueue) > 0) && (ResizeNeeded == OMX_FALSE))
        {
            RunIfNotReady();
        }
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker OUT"));
    return;
}


void OpenmaxWmvAO::DecodeWithMarker()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker IN"));
    //LOGE ("@@ OpenmaxWmvAO : DecodeWithMarker IN");

    QueueType* pInputQueue = ipPorts[OMX_PORT_INPUTPORT_INDEX]->pBufferQueue;
    QueueType* pOutputQueue = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->pBufferQueue;

    ComponentPortType*  pInPort = ipPorts[OMX_PORT_INPUTPORT_INDEX];
    ComponentPortType*  pOutPort = ipPorts[OMX_PORT_OUTPUTPORT_INDEX];

    OMX_U8*                 pOutBuffer;
    OMX_U32                 OutputLength;
    OMX_BOOL                DecodeReturn = OMX_FALSE;
    OMX_BOOL                MarkerFlag = OMX_TRUE;
    OMX_COMPONENTTYPE *     pHandle = &iOmxComponent;
    OMX_BOOL                ResizeNeeded = OMX_FALSE;

    OMX_U32 CurrWidth =  ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nFrameWidth;
    OMX_U32 CurrHeight = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.nFrameHeight;

    if ((!iIsInputBufferEnded) || (iEndofStream))
    {
#ifdef MT6573_MFV_HW
		//LOGD("DEC_WITH_MARKER:ENTER");
		if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
		{
			if(OMX_FALSE == ipWmvDecoderObject->bBufferProcessDone)
			{
				OMX_BUFFERHEADERTYPE*   ipOutBuf = ipWmvDecoderObject->GetDisplayBuffer();	
				if(ipOutBuf)
				{
			        //Send the output buffer back after decode
       				//LOGD("[OUT_QUEUE]1 RETURN OUTPUT:%08x", ipOutBuf);
			        ReturnOutputBuffer(ipOutBuf, pOutPort);
					ipOutBuf = NULL;
					RunIfNotReady();
					return;
				}

				while(NULL != ipWmvDecoderObject->GetFreeBuffer())
					;
				
				ipWmvDecoderObject->bBufferProcessDone = OMX_TRUE;
			}
			
		    //Check whether a new output buffer is available or not
		    if(NULL == ipOutputBuffer)
		    {				
		        if (0 == (GetQueueNumElem(pOutputQueue)))
		        {
		            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker OUT output buffer unavailable"));
		            iNewInBufferRequired = OMX_FALSE;
		            return;
		        }
				OMX_U32 r4Count = NUMBER_OUTPUT_BUFFER_WMV;			
				while(r4Count != 0)
				{
					ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
					if (NULL == ipOutputBuffer)
					{
						PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker Error, output buffer dequeue returned NULL, OUT"));
						iNewInBufferRequired = OMX_FALSE;
						return;
					}
					if(OMX_TRUE == ipWmvDecoderObject->isFreeBuffer(ipOutputBuffer))
					{
						break;
					}
					else
					{
						Queue(pOutputQueue, (void*)ipOutputBuffer);
						r4Count--;
					}
				}
				if (r4Count == 0)
		        {
		            LOGE("[WARNING_WITH_MARKER] return and wait for another ipOutputBuffer");
					iNewInBufferRequired = OMX_FALSE;
					return;
		        }
				//LOGD("[OUT_QUEUE]GET OUTPUT:%08x", ipOutputBuffer);
		    }
		}
		else
		{
			//LOGD("DEC_WITH_MARKER:ENTER ELSE");
#endif	

        //Check whether a new output buffer is available or not
        if (0 == (GetQueueNumElem(pOutputQueue)))
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker OUT output buffer unavailable"));
            iNewInBufferRequired = OMX_FALSE;
            return;
        }

        ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
        if (NULL == ipOutputBuffer)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker Error, output buffer dequeue returned NULL, OUT"));
            iNewInBufferRequired = OMX_FALSE;
            return;
        }

#ifdef MT6573_MFV_HW
		}
#endif

        //Do not proceed if the output buffer can't fit the YUV data
        if ((ipOutputBuffer->nAllocLen < (OMX_U32)((((CurrWidth + 15) >> 4) << 4) *(((CurrHeight + 15) >> 4) << 4) * 3 / 2)) && (OMX_TRUE == ipWmvDecoderObject->WmvInitCompleteFlag))
        {
            ipOutputBuffer->nFilledLen = 0;
			//LOGD("[OUT_QUEUE]2 RETURN OUTPUT:%08x", ipOutputBuffer);
            ReturnOutputBuffer(ipOutputBuffer, pOutPort);

getReturnOutputBufferTime(ipOutputBuffer);

            ipOutputBuffer = NULL;
            return;
        }

getOutputDeQueueTime(ipOutputBuffer);
        
        ipOutputBuffer->nFilledLen = 0;

        /* Code for the marking buffer. Takes care of the OMX_CommandMarkBuffer
         * command and hMarkTargetComponent as given by the specifications
         */
        if (ipMark != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent = ipMark->hMarkTargetComponent;
            ipOutputBuffer->pMarkData = ipMark->pMarkData;
            ipMark = NULL;
        }

        if (ipTargetComponent != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent = ipTargetComponent;
            ipOutputBuffer->pMarkData = iTargetMarkData;
            ipTargetComponent = NULL;

        }
        //Mark buffer code ends here

        if (iInputCurrLength > 0)
        {
            pOutBuffer = ipOutputBuffer->pBuffer;
            OutputLength = 0;

            //Output buffer is passed as a short pointer
            // Ryan Lin !!! for performace calculation
            if (iFrameCount > 0)
            {

//getMp4DecodeVideoInTime();

            }
#if 0
            long _starttime = OsclTickCount::getTimeMs();
#endif
			//LOGD("DEC_WITH_MARKER:DEC");

            DecodeReturn = ipWmvDecoderObject->WmvDecodeVideo(pOutBuffer, (OMX_U32*) & OutputLength,
                           &(ipFrameDecodeBuffer),
                           &(iInputCurrLength),
                           &(ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam),
                           &iFrameCount,
                           MarkerFlag,
                           &ResizeNeeded,
                           &(ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam),
                           iFrameTimestamp,
                           ipOutputBuffer);
#if 0
            LOG_PROFILE("dec frame use %lld ms", OsclTickCount::getTimeMs() - _starttime);
#endif
            if (iFrameCount > 1)
            {                
				getWmvDecodeVideoOutTime();
            }

            if(iFrameCount == 1)
            {
                if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported() )
                {
	            ResizeNeeded = OMX_TRUE;
                    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.video.eColorFormat = OMX_COLOR_FormatVendorMTKYUV;
                    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->VideoParam[0].eColorFormat = OMX_COLOR_FormatVendorMTKYUV;
                    LOGD("[WithMarker] HW_Supported = TRUE");
                }
                else
                {
                    LOGD("[WithMarker] HW_Supported = FALSE");
                }
            }

            ipOutputBuffer->nFilledLen = OutputLength;
            //offset not required in our case, set it to zero
            ipOutputBuffer->nOffset = 0;

            //If decoder returned error, report it to the client via a callback
            if (!DecodeReturn && OMX_FALSE == ipWmvDecoderObject->WmvInitCompleteFlag)
            {
                // initialization error, stop playback
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithoutMarker ErrorBadParameter callback send"));

                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventError,
                 OMX_ErrorBadParameter,
                 0,
                 NULL);
            }
            else if (!DecodeReturn && OMX_FALSE == iEndofStream)
            {
                // decode error
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker ErrorStreamCorrupt callback send"));

                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventError,
                 OMX_ErrorStreamCorrupt,
                 0,
                 NULL);
            }

            if(iFrameCount == 1)
            {
                struct sched_param sched_p;
                sched_getparam(0, &sched_p);
                            
                if (OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
                {
                    // [Honda] use Keene's RR setting
                    //sched_p.sched_priority = 75;
                    sched_p.sched_priority = RTPM_PRIO_OMX_VIDEO;
            
                    if (0 != sched_setscheduler(0, SCHED_RR, &sched_p)) 
                    {
                        LOGE("[%s] failed, errno: %d", __func__, errno);
                    } 
                    else 
                    {
                        sched_p.sched_priority = 0;
                        sched_getparam(0, &sched_p);
                        LOGD("!!!!!*****************************************************************!!!!!");
                        LOGD("!!!!!    [WMV_DEC] sched_setscheduler ok, RR priority: %d", sched_p.sched_priority);
                        LOGD("!!!!!*****************************************************************!!!!!");
                    }
#if 0
					char thread_name[20]="WMV_DEC";
					int result = 0;
					 
					result = mtk_AdjustPrio(thread_name);
#endif                    
                }
                else
                {
                    sched_p.sched_priority = 0;
          
                    if (0 != sched_setscheduler(0, SCHED_NORMAL, &sched_p)) 
                    {
                        LOGE("[%s] failed, errno: %d", __func__, errno);
                    } 
                    else 
                    {
                        sched_p.sched_priority = 100;
                        sched_getparam(0, &sched_p);
                        LOGD("!!!!!*****************************************************************!!!!!");
                        LOGD("!!!!!    [WMV_DEC] sched_setscheduler ok, NORMAL priority: %d", sched_p.sched_priority);
                        LOGD("!!!!!*****************************************************************!!!!!");
                    }
                }
            }

            if (ResizeNeeded == OMX_TRUE)
            {
                g_output_seq_num = 0;
                // send port settings changed event
                OMX_COMPONENTTYPE* pHandle = (OMX_COMPONENTTYPE*) ipAppPriv->CompHandle;
				LOGD("----OMX_EventPortSettingsChanged----");

                iResizePending = OMX_TRUE;
                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventPortSettingsChanged, //The command was completed
                 OMX_PORT_OUTPUTPORT_INDEX,
                 0,
                 NULL);
				 LOGD("----OMX_EventPortSettingsChanged done----");

            }
            /*
            else if((iFrameCount == 1) && (OMX_TRUE == ipMpegDecoderObject->MP4DecHwSupported()))
            {
                g_output_seq_num = 0;
                // send port settings changed event
                OMX_COMPONENTTYPE* pHandle = (OMX_COMPONENTTYPE*) ipAppPriv->CompHandle;

                iResizePending = OMX_TRUE;
                
                LOGD("----OMX_EventPortSettingsChanged----");
                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventPortSettingsChanged, //The command was completed
                 OMX_PORT_OUTPUTPORT_INDEX,
                 0,
                 NULL);
            }
	            */
            
            //Set the timestamp equal to the input buffer timestamp
            if (OMX_TRUE == iUseExtTimestamp)
            {
                ipOutputBuffer->nTimeStamp = iFrameTimestamp;
            }

            /* Discard the input frame if it is with the marker bit & decoder fails*/
            if (iInputCurrLength == 0 || !DecodeReturn)
            {
                ipInputBuffer->nFilledLen = 0;
				//LOGD("[IN_QUEUE]1 RETURN INPUT:%08x", ipInputBuffer);
                ReturnInputBuffer(ipInputBuffer, pInPort);

getReturnInputBufferTime(ipInputBuffer);
                
                ipInputBuffer = NULL;
                iNewInBufferRequired = OMX_TRUE;
                iIsInputBufferEnded = OMX_TRUE;
                iUseExtTimestamp = OMX_TRUE;
                iInputCurrLength = 0;
            }
            else
            {
                iNewInBufferRequired = OMX_FALSE;
                iIsInputBufferEnded = OMX_FALSE;
                iUseExtTimestamp = OMX_FALSE;
            }
        }
        else if (iEndofStream == OMX_FALSE)
        {
            // it's possible that after partial frame assembly, the input buffer still remains empty (due to
            // client erroneously sending such buffers). This code adds robustness in the sense that it returns such buffer to the client
			//LOGD("[IN_QUEUE]2 RETURN INPUT:%08x", ipInputBuffer);
            ipInputBuffer->nFilledLen = 0;
            ReturnInputBuffer(ipInputBuffer, pInPort);

getReturnInputBufferTime(ipInputBuffer);
            
            ipInputBuffer = NULL;
            iNewInBufferRequired = OMX_TRUE;
            iIsInputBufferEnded = OMX_TRUE;
            iUseExtTimestamp = OMX_TRUE;
        }


#ifdef MT6573_MFV_HW	
        /* If EOS flag has come from the client & there are no more
         * input buffers to decode, send the callback to the client
         */
        if (OMX_TRUE == iEndofStream)
        {
        	LOGD("DEC_WITH_MARKER:EOS");
            if (!DecodeReturn)
            {
            	LOGD("!DecodeReturn");
	            //handle final decode				
				if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported())
				{
					LOGD("DEC_WITH_MARKER:EOS HW");
					if(OMX_FALSE == ipWmvDecoderObject->bBufferProcessDone)
					{
						OMX_BUFFERHEADERTYPE*   ipOutBuf = ipWmvDecoderObject->GetDisplayBuffer();	
						if(ipOutBuf)
						{
					        //Send the output buffer back after decode
               				//LOGD("[OUT_QUEUE]3 RETURN OUTPUT:%08x", ipOutBuf);
					        ReturnOutputBuffer(ipOutBuf, pOutPort);
							ipOutBuf = NULL;
							RunIfNotReady();
							return;
						}

						while(NULL != ipWmvDecoderObject->GetFreeBuffer())
							;
						
						ipWmvDecoderObject->bBufferProcessDone = OMX_TRUE;
					}
					if(OMX_FALSE == bFinalDecDone )
					{
						//decode final					
						if(OMX_FALSE == ipWmvDecoderObject->WmvDecodeVideoFinal())
						{
							while(1)
								LOGE("decode final fail");	
							
						}
						bFinalDecDone = OMX_TRUE;
						RunIfNotReady();
						return;
					}

					/*
					if(OMX_FALSE == ipMpegDecoderObject->bBufferProcessDone)
					{
						while(1)
						{
							ipOutputBuffer = ipMpegDecoderObject->GetDisplayBuffer();	
							if(ipOutputBuffer)
							{
						        //Send the output buffer back after decode
						        ReturnOutputBuffer(ipOutputBuffer, pOutPort);
								ipOutputBuffer = NULL;
							}
							else
							{
								break;
							}
						}
						
						while(NULL != ipMpegDecoderObject->GetFreeBuffer())
							;

						ipMpegDecoderObject->bBufferProcessDone = OMX_TRUE;
					}
					*/
				}
				
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker EOS callback send"));
				LOGD("OpenmaxWmvAO : DecodeWithMarker EOS callback send");

                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventBufferFlag,
                 1,
                 OMX_BUFFERFLAG_EOS,
                 NULL);
				LOGD("OpenmaxWmvAO : DecodeWithMarker EOS callback send done");

                iNewInBufferRequired = OMX_TRUE;

				//Jackal: put here instead of put after ReturnOutputBuffer(ipOutputBuffer, pOutPort);
				//because we don't need use this profiling time and we need use OMX_TRUE == iEndofStream
				getReturnOutputBufferTime(ipOutputBuffer);                

                //Mark this flag false once the callback has been send back
                iEndofStream = OMX_FALSE;
				//LOGD("return final output buffer");
				//LOGD("[OUT_QUEUE]4 RETURN OUTPUT:%08x", ipOutputBuffer);

                ipOutputBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
                ReturnOutputBuffer(ipOutputBuffer, pOutPort);
                ipOutputBuffer = NULL;
				//LOGD("return final output buffer done");

                if ((iNumInputBuffer != 0) && (NULL != ipInputBuffer))
                {
                	//LOGD("return final input buffer");
					//LOGD("[IN_QUEUE]3 RETURN INPUT:%08x", ipInputBuffer);
                    ReturnInputBuffer(ipInputBuffer, pInPort);
					//LOGD("return final input buffer done");

					getReturnInputBufferTime(ipInputBuffer);
                    
                    ipInputBuffer = NULL;
                    iIsInputBufferEnded = OMX_TRUE;
                    iInputCurrLength = 0;
                }

                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker OUT"));
                return;
            }
			else if (DecodeReturn)
			{
	            LOGD("DecodeReturn");
			}
        }

		if(OMX_FALSE == ipWmvDecoderObject->WMVDecHwSupported() || iFrameCount == 1)
		{
			//Send the output buffer back after decode
			//LOGD("[OUT_QUEUE]5 RETURN OUTPUT:%08x", ipOutputBuffer);
			ReturnOutputBuffer(ipOutputBuffer, pOutPort);
		}

		getReturnOutputBufferTime(ipOutputBuffer);

        ipOutputBuffer = NULL;

#else
        /* If EOS flag has come from the client & there are no more
         * input buffers to decode, send the callback to the client
         */
        if (OMX_TRUE == iEndofStream)
        {
            if (!DecodeReturn)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker EOS callback send"));

                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventBufferFlag,
                 1,
                 OMX_BUFFERFLAG_EOS,
                 NULL);

                iNewInBufferRequired = OMX_TRUE;

//Jackal: put here instead of put after ReturnOutputBuffer(ipOutputBuffer, pOutPort);
//because we don't need use this profiling time and we need use OMX_TRUE == iEndofStream
getReturnOutputBufferTime(ipOutputBuffer);                

                //Mark this flag false once the callback has been send back
                iEndofStream = OMX_FALSE;

                ipOutputBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
                ReturnOutputBuffer(ipOutputBuffer, pOutPort);
                ipOutputBuffer = NULL;

                if ((iNumInputBuffer != 0) && (NULL != ipInputBuffer))
                {
                    ReturnInputBuffer(ipInputBuffer, pInPort);

getReturnInputBufferTime(ipInputBuffer);
                    
                    ipInputBuffer = NULL;
                    iIsInputBufferEnded = OMX_TRUE;
                    iInputCurrLength = 0;
                }

                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker OUT"));
                return;
            }

        }

        //Send the output buffer back after decode
        ReturnOutputBuffer(ipOutputBuffer, pOutPort);

getReturnOutputBufferTime(ipOutputBuffer);

        ipOutputBuffer = NULL;
#endif


        /* If there is some more processing left with current buffers, re-schedule the AO
         * Do not go for more than one round of processing at a time.
         * This may block the AO longer than required.
         */
        if ((iInputCurrLength != 0 || GetQueueNumElem(pInputQueue) > 0)
                && (GetQueueNumElem(pOutputQueue) > 0) && (ResizeNeeded == OMX_FALSE))
        {
            RunIfNotReady();
        }
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : DecodeWithMarker OUT"));
    return;
}


//Not implemented & supported in case of base profile components

void OpenmaxWmvAO::ComponentGetRolesOfComponent(OMX_STRING* aRoleString)
{
    *aRoleString = (OMX_STRING)"video_decoder.wmv";
}


//Component constructor
OpenmaxWmvAO::OpenmaxWmvAO()
{
#if 0    
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_omxwmv");
    bShowLogProfile = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_wmvdectime");
#endif
    
    iUseExtTimestamp = OMX_TRUE;
    ipWmvDecoderObject = NULL;

    mIsStageFrightPlayer = false;
    char value[PROPERTY_VALUE_MAX];
    if (property_get("media.stagefright.enable-player", value, NULL)
        && (!strcmp(value, "1") || !strcasecmp(value, "true"))) {
        mIsStageFrightPlayer = true;
    }

    if (!IsAdded())
    {
        AddToScheduler();
    }
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : constructed"));
}


//Active object destructor
OpenmaxWmvAO::~OpenmaxWmvAO()
{
    if (IsAdded())
    {
        RemoveFromScheduler();
    }


    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : destructed"));
}


OMX_ERRORTYPE OpenmaxWmvAO::ComponentInit()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ComponentInit IN"));

    OMX_ERRORTYPE Status = OMX_ErrorNone;

    if (OMX_TRUE == iIsInit)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ComponentInit error incorrect operation"));
        return OMX_ErrorIncorrectStateOperation;
    }
    iIsInit = OMX_TRUE;


    if (!iCodecReady)
    {
        Status = ipWmvDecoderObject->WmvDecInit();

        iCodecReady = OMX_TRUE;
    }

    iUseExtTimestamp = OMX_TRUE;
    iInputCurrLength = 0;
	ipOutputBuffer = NULL;
#ifdef MT6573_MFV_HW
	bFinalDecDone = OMX_FALSE;
#endif
    //Used in dynamic port reconfiguration
    iFrameCount = 0;
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ComponentInit OUT"));

    return Status;

}

OMX_ERRORTYPE OpenmaxWmvAO::ComponentDeInit()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ComponentDeInit IN"));
    //LOGD("OpenmaxMpeg4AO : ComponentDeInit+");
    OMX_ERRORTYPE Status = OMX_ErrorNone;

    iIsInit = OMX_FALSE;

    if (iCodecReady)
    {
        Status = ipWmvDecoderObject->WmvDecDeinit();
        iCodecReady = OMX_FALSE;
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxWmvAO : ComponentDeInit OUT"));
    //LOGD("OpenmaxMpeg4AO : ComponentDeInit-");

    return Status;

}

OMX_ERRORTYPE OpenmaxWmvAO::GetConfig(
    OMX_IN  OMX_HANDLETYPE hComponent,
    OMX_IN  OMX_INDEXTYPE nIndex,
    OMX_INOUT OMX_PTR pComponentConfigStructure)
{
    OSCL_UNUSED_ARG(hComponent);
    OSCL_UNUSED_ARG(nIndex);
    OSCL_UNUSED_ARG(pComponentConfigStructure);
    return OMX_ErrorNotImplemented;
}


OMX_ERRORTYPE OpenmaxWmvAO::ReAllocatePartialAssemblyBuffers(OMX_BUFFERHEADERTYPE* aInputBufferHdr)
{
    //LOGE ("@@ ReAllocatePartialAssemblyBuffers aInputBufferHdr->nFilledLen=%d", aInputBufferHdr->nFilledLen);
    
    return OMX_ErrorNone;
}



bool OpenmaxWmvAO::getInputQueueStartTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
    return 0;
}

bool OpenmaxWmvAO::getInputQueueEndTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if (iFrameCount > 0)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getInputQueueEndTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-1));
        }
    }
#endif

    if ((a_prBufHdr->nFlags & OMX_BUFFERFLAG_CODECCONFIG) == 0)
    {
        //OsclProfile::Push(OsclProfile::igetInputQueueEndTime, g_input_seq_num);
        g_input_seq_num++;
    }
    
    return 0;
}

bool OpenmaxWmvAO::getOutputQueueStartTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
    return 0;
}

bool OpenmaxWmvAO::getOutputQueueEndTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{    
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if (iFrameCount > 0)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getOutputQueueEndTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-1));
        }
    }
#endif  
   
    //OsclProfile::Push(OsclProfile::igetOutputQueueEndTime, g_output_seq_num);
    g_output_seq_num++;
  
    return 0;
}

bool OpenmaxWmvAO::getInputDeQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if (iFrameCount > 0)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getInputDeQueueTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-1));
        }
    }
#endif
    if (iFrameCount > 0)
    {
        //OsclProfile::Push(OsclProfile::igetInputDeQueueTime, iFrameCount - 1);
    }
    return 0;
}

bool OpenmaxWmvAO::getOutputDeQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if (iFrameCount > 0)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getOutputDeQueueTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-1));
        }   
    }
#endif   
    if (iFrameCount > 0)
    {
        //OsclProfile::Push(OsclProfile::igetOutputDeQueueTime, iFrameCount - 1);
    }
    return 0;
}

bool OpenmaxWmvAO::getWmvDecodeVideoInTime(void)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if (iFrameCount > 0)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getMp4DecodeVideoInTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-1));
        }
    }
#endif   
    if (iFrameCount > 0)
    {
#if 1        
//        OsclProfile::Push(OsclProfile::iWmvDecodeVideoInTime, iFrameCount - 1);
#endif
    }
    return 0;
}

bool OpenmaxWmvAO::getWmvDecodeVideoOutTime(void)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if (iFrameCount > 1)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getWmvDecodeVideoOutTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-2));
        }
    }   
#endif
    if (iFrameCount > 1)
    {
#if 1        
        OsclProfile::Push(OsclProfile::iMp4DecodeVideoOutTime, iFrameCount - 2);
#endif
    }
    return 0;
}

bool OpenmaxWmvAO::getReturnInputBufferTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if ((iFrameCount > 1) && (OMX_TRUE != iEndofStream))
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getReturnInputBufferTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-2));
        }
    }
#endif
    if ((iFrameCount > 1) && (OMX_TRUE != iEndofStream))
    {
        //OsclProfile::Push(OsclProfile::igetReturnInputBufferTime, iFrameCount - 2);
    }
    return 0;
}

bool OpenmaxWmvAO::getReturnOutputBufferTime(OMX_BUFFERHEADERTYPE *a_prBufHdr)
{
#ifdef MT6516_MP4DEC_OMX_PROFILING
    if ((iFrameCount > 1) && (OMX_TRUE != iEndofStream))
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);
        if(fp_mt6516_mp4dec_omx_profiling)
        {
            fprintf(fp_mt6516_mp4dec_omx_profiling, "VDEC_getReturnOutputBufferTime %u %u %d\n", 
                t1.tv_sec, t1.tv_usec, (iFrameCount-2));
        }
    } 
#endif
    if ((iFrameCount > 1) && (OMX_TRUE != iEndofStream))
    {
        //OsclProfile::Push(OsclProfile::igetReturnOutputBufferTime, iFrameCount - 2);
    }
    return 0;
}

OMX_ERRORTYPE OpenmaxWmvAO::FlushPortFinal(OMX_S32 PortIndex)
{
	//LOGD("~~OpenmaxMpeg4AO::FlushPortFinal+, IDX:%d", PortIndex);
#ifdef MT6573_MFV_HW
	OMX_BOOL bDecDone = OMX_FALSE;
	//handle final decode				
	if(OMX_TRUE == ipWmvDecoderObject->WMVDecHwSupported() && (OMX_PORT_OUTPUTPORT_INDEX == PortIndex || OMX_PORT_ALLPORT_INDEX == PortIndex))
	{
		//LOGD("FlushPortFinal HW");
FlushPortFinal_CHECK:
		if(OMX_FALSE == ipWmvDecoderObject->bBufferProcessDone)
		{
		    ComponentPortType*  pOutPort = ipPorts[OMX_PORT_OUTPUTPORT_INDEX];			
			while(1)
			{
				OMX_BUFFERHEADERTYPE*   ipOutBuf = ipWmvDecoderObject->GetDisplayBuffer();	
				if(ipOutBuf)
				{
			        //Send the output buffer back after decode
	   				//LOGD("[OUT_QUEUE]3 RETURN OUTPUT:%08x", ipOutBuf);
			        ReturnOutputBuffer(ipOutBuf, pOutPort);
					ipOutBuf = NULL;
					//RunIfNotReady();
					//return;
				}
				else
				{
					//LOGD("[OUT_QUEUE]3 RETURN OUTPUT done");
					break;
				}
			}
						
			while(NULL != ipWmvDecoderObject->GetFreeBuffer())
				;
						
			ipWmvDecoderObject->bBufferProcessDone = OMX_TRUE;
		}

		if(OMX_FALSE == bDecDone )
		{
			//LOGD("PROCESSING bFinalDecDone, 1");
			//decode final					
			if(OMX_FALSE == ipWmvDecoderObject->WmvDecodeVideoFinal())
			{
				while(1)
					LOGE("decode final fail");	
				
			}
			//bFinalDecDone = OMX_FALSE;
			bDecDone = OMX_TRUE;
			//RunIfNotReady();
			//return;
			goto FlushPortFinal_CHECK;
		}

		ipWmvDecoderObject->CheckFreeBuffer();
	}
#endif
	//LOGD("OpenmaxWmvAO::FlushPortFinal-, IDX:%d", PortIndex);
	return OMX_ErrorNone;
}


