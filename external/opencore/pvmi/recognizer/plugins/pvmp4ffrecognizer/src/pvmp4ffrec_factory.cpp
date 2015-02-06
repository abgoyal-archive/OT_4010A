
#include "pvmp4ffrec_plugin.h"
#include "pvmp4ffrec_factory.h"

#include "oscl_error_codes.h"
#include "oscl_exception.h"

// Use default DLL entry point for Symbian
#include "oscl_dll.h"
OSCL_DLL_ENTRY_POINT_DEFAULT()


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFRecognizerPluginInterface* PVMP4FFRecognizerFactory::CreateRecognizerPlugin()
{
    PVMFRecognizerPluginInterface* plugin = NULL;
    plugin = OSCL_STATIC_CAST(PVMFRecognizerPluginInterface*, OSCL_NEW(PVMP4FFRecognizerPlugin, ()));
    if (plugin == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return plugin;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVMP4FFRecognizerFactory::DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugin)
{
    if (aPlugin)
    {
        OSCL_DELETE(((PVMP4FFRecognizerPlugin*)aPlugin));
    }
}

