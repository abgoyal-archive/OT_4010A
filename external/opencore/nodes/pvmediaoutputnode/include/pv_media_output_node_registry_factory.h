
#ifndef PV_MEDIA_OUTPUT_NODE_REGISTRY_FACTORY_H_INCLUDED
#define PV_MEDIA_OUTPUT_NODE_REGISTRY_FACTORY_H_INCLUDED

#ifndef PV_MEDIA_OUTPUT_NODE_FACTORY_H_INCLUDED
#include "pv_media_output_node_factory.h"
#endif
#ifndef PVMF_NODE_REGISTRY_H_INCLUDED
#include "pvmf_node_registry.h"
#endif
#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability.h"
#endif

class PVMFMediaOutputNodeRegistryFactory: public PVMFNodeCreationInterface
        , public PvmiCapabilityAndConfig
{
    public:
        /*
        ** Get the default registration info for this node.
        **  @param info (output): node registration info.
        */
        void GetDefaultNodeRegistrationInfo(PVMFNodeRegistrationInfo& info)
        {
            info.iUuid = PVMFMediaOutputNodeUUID;

            info.iMediaCategory = "multi";
            info.iComponentType = "media_output";
            info.iHasHardwareAssist = false;

            info.iNodeCreationInterface = this;
            info.iNodeInstance = NULL;

            info.iQueryInterface = this;

            //in general this node can handle any audio or video format.
            //the exact formats aren't known until the component is created.
            info.iCapability.AddFormatPair("audio", "audio");
            info.iCapability.AddFormatPair("video", "video");
        }

        //from PVMFNodeCreationInterface
        PVMFNodeInterface* CreateNode(OsclAny*aParam)
        {
            return PVMediaOutputNodeFactory::CreateMediaOutputNode((PvmiMIOControl*)aParam);
        }
        void ReleaseNode(PVMFNodeInterface* aNode)
        {
            PVMediaOutputNodeFactory::DeleteMediaOutputNode(aNode);
        }

        //from PvmiCapabilityAndConfig interface
        PVMFStatus getParametersSync(PvmiMIOSession , PvmiKeyType ,
                                     PvmiKvp*& , int& , PvmiCapabilityContext)
        {
            return PVMFFailure;//nothing available yet.
        }
        PVMFStatus releaseParameters(PvmiMIOSession , PvmiKvp* , int)
        {
            return PVMFFailure;
        }
        void setParametersSync(PvmiMIOSession , PvmiKvp* aParameters,
                               int , PvmiKvp * & aRet_kvp)
        {
            aRet_kvp = aParameters;//fail.
        }
        PVMFStatus verifyParametersSync(PvmiMIOSession , PvmiKvp* , int)
        {
            return PVMFFailure;
        }
        void setObserver(PvmiConfigAndCapabilityCmdObserver*)
        {}
        void createContext(PvmiMIOSession , PvmiCapabilityContext&)
        {}
        void setContextParameters(PvmiMIOSession , PvmiCapabilityContext& , PvmiKvp* , int)
        {}
        void DeleteContext(PvmiMIOSession , PvmiCapabilityContext&)
        {}
        PVMFCommandId setParametersAsync(PvmiMIOSession , PvmiKvp* , int , PvmiKvp*& , OsclAny* context = NULL)
        {
            OSCL_UNUSED_ARG(context);
            return -1;
        }
        uint32 getCapabilityMetric(PvmiMIOSession)
        {
            return 0;
        }

};

#endif // PV_MEDIA_OUTPUT_NODE_REGISTRY_FACTORY_H_INCLUDED
