
#include "pvwavffrec_plugin.h"
#include "pvwavffrec_factory.h"

#include "oscl_error_codes.h"
#include "oscl_exception.h"

// Use default DLL entry point for Symbian
#include "oscl_dll.h"
OSCL_DLL_ENTRY_POINT_DEFAULT()


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFRecognizerPluginInterface* PVWAVFFRecognizerFactory::CreateRecognizerPlugin()
{
    PVMFRecognizerPluginInterface* plugin = NULL;
    plugin = OSCL_STATIC_CAST(PVMFRecognizerPluginInterface*, OSCL_NEW(PVWAVFFRecognizerPlugin, ()));
    if (plugin == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return plugin;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVWAVFFRecognizerFactory::DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugin)
{
    if (aPlugin)
    {
        OSCL_DELETE(((PVWAVFFRecognizerPlugin*)aPlugin));
    }
}

