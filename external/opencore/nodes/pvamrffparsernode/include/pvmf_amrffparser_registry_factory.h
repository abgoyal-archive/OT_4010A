
#ifndef PVMF_AMRFFPARSER_REGISTRY_FACTORY_H_INCLUDED
#define PVMF_AMRFFPARSER_REGISTRY_FACTORY_H_INCLUDED

#ifndef PVMF_AMRFFPARSER_FACTORY_H_INCLUDED
#include "pvmf_amrffparser_factory.h"
#endif
#ifndef PVMF_NODE_REGISTRY_H_INCLUDED
#include "pvmf_node_registry.h"
#endif
#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability.h"
#endif


class PVMFAMRFFParserNodeRegistryFactory: public PVMFNodeCreationInterface
        , public PvmiCapabilityAndConfig
{
    public:
        /*
        ** Get the default registration info for this node.
        **  @param info (output): node registration info.
        */
        void GetDefaultNodeRegistrationInfo(PVMFNodeRegistrationInfo& info)
        {
            info.iUuid = KPVMFAmrFFParserNodeUuid;

            info.iMediaCategory = "audio";
            info.iComponentType = "file_parser";
            info.iHasHardwareAssist = false;

            info.iNodeCreationInterface = this;
            info.iNodeInstance = NULL;

            info.iQueryInterface = this;

            //Just register the source type.
            //Output format info isn't available until the file is parsed.
            info.iCapability.AddInputFormat("multiplexed/x-amr-ff");
        }

        //from PVMFNodeCreationInterface
        PVMFNodeInterface* CreateNode(OsclAny*aParam = NULL)
        {
            OSCL_UNUSED_ARG(aParam);
            return PVMFAMRFFParserNodeFactory::CreatePVMFAMRFFParserNode();
        }
        void ReleaseNode(PVMFNodeInterface* aNode)
        {
            PVMFAMRFFParserNodeFactory::DeletePVMFAMRFFParserNode(aNode);
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

#endif // PVMF_AMRFFPARSER_REGISTRY_FACTORY_H_INCLUDED

