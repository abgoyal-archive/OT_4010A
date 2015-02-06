
#include "pvmf_recognizer_registry.h"
#include "pvmf_recognizer_registry_impl.h"

#include "oscl_dll.h"
OSCL_DLL_ENTRY_POINT_DEFAULT()


// Need a registry ...
#include "oscl_error.h"
// Platform has TLS support
#define PVMFRECOGNIZER_REGISTRY OsclTLSRegistryEx
#define PVMFRECOGNIZER_REGISTRY_ID OSCL_TLS_ID_PVMFRECOGNIZER
#define PVMFRECOGNIZER_REGISTRY_WRAPPER OsclTLSEx



OSCL_EXPORT_REF PVMFStatus PVMFRecognizerRegistry::Init()
{
    // Check that there is no existing registry
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        // Registry is already present so no need to instantiate again
        // Just increment the refcount
        (pvrecregimpl->iRefCount)++;
        return PVMFSuccess;
    }

    // Instantiate the registry implementation
    Oscl_TAlloc<PVMFRecognizerRegistryImpl, OsclMemAllocator> talloc;
    pvrecregimpl = OSCL_ALLOC_NEW(talloc, PVMFRecognizerRegistryImpl, ());
    // Save it on singleton or TLS
    PVMFRECOGNIZER_REGISTRY::registerInstance(pvrecregimpl, PVMFRECOGNIZER_REGISTRY_ID);
    return PVMFSuccess;
}


OSCL_EXPORT_REF void PVMFRecognizerRegistry::Cleanup()
{
    // Retrieve the registry implementation instance from singleton or TLS and destroy it
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        // First decrement the refcount
        --(pvrecregimpl->iRefCount);
        // If the resulting refcount is 0, then delete the instance
        if ((pvrecregimpl->iRefCount) <= 0)
        {
            Oscl_TAlloc<PVMFRecognizerRegistryImpl, OsclMemAllocator> talloc;
            OSCL_ALLOC_DELETE(pvrecregimpl, talloc, PVMFRecognizerRegistryImpl);
            // Unregister by putting NULL pointer in singleton or TLS
            PVMFRECOGNIZER_REGISTRY::registerInstance(NULL, PVMFRECOGNIZER_REGISTRY_ID);
        }
    }
    else
    {
        // Registry has already been cleaned up so nothing to do
    }
}


OSCL_EXPORT_REF PVMFStatus PVMFRecognizerRegistry::RegisterPlugin(PVMFRecognizerPluginFactory& aPluginFactory)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        return pvrecregimpl->RegisterPlugin(aPluginFactory);
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        return PVMFErrNotReady;
    }
}


OSCL_EXPORT_REF PVMFStatus PVMFRecognizerRegistry::RemovePlugin(PVMFRecognizerPluginFactory& aPluginFactory)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        return pvrecregimpl->RemovePlugin(aPluginFactory);
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        return PVMFErrNotReady;
    }
}


OSCL_EXPORT_REF PVMFStatus PVMFRecognizerRegistry::OpenSession(PVMFSessionId& aSessionId, PVMFRecognizerCommmandHandler& aCmdHandler)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        return pvrecregimpl->OpenSession(aSessionId, aCmdHandler);
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        return PVMFErrNotReady;
    }
}


OSCL_EXPORT_REF PVMFStatus PVMFRecognizerRegistry::CloseSession(PVMFSessionId aSessionId)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        return pvrecregimpl->CloseSession(aSessionId);
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        return PVMFErrNotReady;
    }
}


OSCL_EXPORT_REF PVMFCommandId PVMFRecognizerRegistry::Recognize(PVMFSessionId aSessionId, PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
        Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult, OsclAny* aCmdContext, uint32 aTimeout)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        return pvrecregimpl->Recognize(aSessionId, aSourceDataStreamFactory, aFormatHint, aRecognizerResult, aCmdContext, aTimeout);
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        OSCL_LEAVE(OsclErrNotReady);
        return 0;
    }
}


OSCL_EXPORT_REF PVMFCommandId PVMFRecognizerRegistry::CancelCommand(PVMFSessionId aSessionId, PVMFCommandId aCommandToCancelId, OsclAny* aCmdContext)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        return pvrecregimpl->CancelCommand(aSessionId, aCommandToCancelId, aCmdContext);
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        OSCL_LEAVE(OsclErrNotReady);
        return 0;
    }
}


// <--- Morris Yang ALPS00126974
OSCL_EXPORT_REF bool PVMFRecognizerRegistry::SetMaxRequiredSizeForRecognitionOverride(int32 iMaxSizeOverride)
{
    PVMFRecognizerRegistryImpl* pvrecregimpl = OSCL_STATIC_CAST(PVMFRecognizerRegistryImpl*, PVMFRECOGNIZER_REGISTRY::getInstance(PVMFRECOGNIZER_REGISTRY_ID));
    if (pvrecregimpl != NULL)
    {
        pvrecregimpl->iMaxRequiredSizeForRecognitionOverride = iMaxSizeOverride;
        return true;
    }
    else
    {
        // Registry hasn't been initialized yet. Assert
        OSCL_ASSERT(false);
        OSCL_LEAVE(OsclErrNotReady);
        return false;
    }
}
// --->



