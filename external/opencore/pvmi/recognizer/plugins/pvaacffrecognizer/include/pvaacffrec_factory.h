
#ifndef PVAACFFREC_FACTORY_H_INCLUDED
#define PVAACFFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVAACFFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVAACFFRecognizerFactory()
        {
        };

        ~PVAACFFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVAACFFREC_FACTORY_H_INCLUDED


