
#ifndef PVRMFFREC_FACTORY_H_INCLUDED
#define PVRMFFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVRMFFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVRMFFRecognizerFactory()
        {
        };

        ~PVRMFFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVRMFFREC_FACTORY_H_INCLUDED


