
#ifndef PVMP3FFREC_FACTORY_H_INCLUDED
#define PVMP3FFREC_FACTORY_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVMP3FFRecognizerFactory : public PVMFRecognizerPluginFactory
{
    public:
        PVMP3FFRecognizerFactory()
        {
        };

        ~PVMP3FFRecognizerFactory()
        {
        };

        OSCL_IMPORT_REF PVMFRecognizerPluginInterface* CreateRecognizerPlugin();
        OSCL_IMPORT_REF void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn);
};

#endif // PVMP3FFREC_FACTORY_H_INCLUDED


