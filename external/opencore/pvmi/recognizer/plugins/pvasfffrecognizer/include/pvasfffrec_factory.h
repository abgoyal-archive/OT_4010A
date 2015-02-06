
#ifndef PVASFFFREC_FACTORY_H_INCLUDED
#define PVASFFFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVASFFFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVASFFFRecognizerFactory()
        {
        };

        ~PVASFFFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVASFFFREC_FACTORY_H_INCLUDED


