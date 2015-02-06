
#ifndef PVMP4FFREC_FACTORY_H_INCLUDED
#define PVMP4FFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVMP4FFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVMP4FFRecognizerFactory()
        {
        };

        ~PVMP4FFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVMP4FFREC_FACTORY_H_INCLUDED


