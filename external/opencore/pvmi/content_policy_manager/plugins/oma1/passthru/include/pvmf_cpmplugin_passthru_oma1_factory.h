
#ifndef PVMF_CPMPLUGIN_PASSTHRU_OMA1_FACTORY_H_INCLUDED
#define PVMF_CPMPLUGIN_PASSTHRU_OMA1_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef PVMF_CPMPLUGIN_INTERFACE_H_INCLUDED
#include "pvmf_cpmplugin_interface.h"
#endif

//Oma1 Passthru plugin factory.
class PVMFOma1PassthruPluginFactory : public PVMFCPMPluginFactory
{
    public:
        OSCL_IMPORT_REF PVMFOma1PassthruPluginFactory();
        OSCL_IMPORT_REF PVMFOma1PassthruPluginFactory(bool aAuthorizeUsage, bool aCancelAcquireLicense,
                bool aSourceInitDataNotSupported,
                PVMFCPMContentType aCPMContentType);
        //from PVMFCPMPluginFactory
        OSCL_IMPORT_REF PVMFCPMPluginInterface* CreateCPMPlugin();
        OSCL_IMPORT_REF void DestroyCPMPlugin(PVMFCPMPluginInterface* aPlugIn);
        bool iFailAuthorizeUsage;
        bool iCancelAcquireLicense;
        bool iSourceInitDataNotSupported;
        PVMFCPMContentType iCPMContentType;
    private:

};

#endif //PVMF_CPMPLUGIN_PASSTHRU_OMA1_FACTORY_H_INCLUDED



