
#ifndef PVAMRFFREC_FACTORY_H_INCLUDED
#define PVAMRFFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVAMRFFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVAMRFFRecognizerFactory()
        {
        };

        ~PVAMRFFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVAMRFFREC_FACTORY_H_INCLUDED


