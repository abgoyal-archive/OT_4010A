
#include "pvrmffrec_plugin.h"
#include "pvrmffrec_factory.h"

#include "oscl_error_codes.h"
#include "oscl_exception.h"

// Use default DLL entry point for Symbian
#include "oscl_dll.h"
OSCL_DLL_ENTRY_POINT_DEFAULT()


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFRecognizerPluginInterface* PVRMFFRecognizerFactory::CreateRecognizerPlugin()
{
    PVMFRecognizerPluginInterface* plugin = NULL;
    plugin = OSCL_STATIC_CAST(PVMFRecognizerPluginInterface*, OSCL_NEW(PVRMFFRecognizerPlugin, ()));
    if (plugin == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return plugin;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVRMFFRecognizerFactory::DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugin)
{
    if (aPlugin)
    {
        OSCL_DELETE(((PVRMFFRecognizerPlugin*)aPlugin));
    }
}

