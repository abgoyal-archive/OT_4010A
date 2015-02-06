
#ifndef PVMI_MEDIA_IO_FILEOUTPUT_REGISTRY_FACTORY_H_INCLUDED
#define PVMI_MEDIA_IO_FILEOUTPUT_REGISTRY_FACTORY_H_INCLUDED

#ifndef PVMF_NODE_REGISTRY_H_INCLUDED
#include "pvmf_node_registry.h"
#endif
#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability.h"
#endif

#define PVMFMediaFileOutputUuid PVUuid(0x71032c90,0xf24c,0x11d9,0x8c,0xd6,0x08,0x00,0x20,0x0c,0x9a,0x66)


class PVMFMediaFileOutputRegistryFactory: public PVMFMediaIOCreationInterface
        , public PvmiCapabilityAndConfig
{
    public:
        /*
        ** Get the default registration info for this component
        **  @param info (output): component registration info.
        */
        void GetDefaultMediaIORegistrationInfo(PVMFMediaIORegistrationInfo& info)
        {
            info.iUuid = PVMFMediaFileOutputUuid;

            info.iMediaCategory = "multi";
            info.iComponentType = "media_output/file";
            info.iHasHardwareAssist = false;

            info.iMediaIOCreationInterface = this;
            info.iMediaIOInstance = NULL;

            info.iQueryInterface = this;

            //this component can handle any audio or video format.
            info.iCapability.AddInputFormat("audio");
            info.iCapability.AddInputFormat("video");
        }

        //from PVMFMediaIOCreationInterface
        OSCL_IMPORT_REF PvmiMIOControl* CreateMediaIO(OsclAny* aParam);
        OSCL_IMPORT_REF void ReleaseMediaIO(PvmiMIOControl* aNode);

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


#endif //PVMI_MEDIA_IO_FILEOUTPUT_REGISTRY_FACTORY_H_INCLUDED





