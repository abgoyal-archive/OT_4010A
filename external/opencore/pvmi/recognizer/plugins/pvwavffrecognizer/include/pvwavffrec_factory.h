
#ifndef PVWAVFFREC_FACTORY_H_INCLUDED
#define PVWAVFFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVWAVFFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVWAVFFRecognizerFactory()
        {
        };

        ~PVWAVFFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVWAVFFREC_FACTORY_H_INCLUDED


