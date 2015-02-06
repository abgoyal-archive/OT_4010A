


#include "oscl_registry_client.h"
#include "oscl_registry_client_impl.h"

#ifndef OSCL_COMBINED_DLL
#include "oscl_dll.h"
OSCL_DLL_ENTRY_POINT_DEFAULT()
#endif

OSCL_EXPORT_REF OsclRegistryClient::OsclRegistryClient()
{
    iGlobalImpl = NULL;
    iTlsImpl = NULL;
}

OSCL_EXPORT_REF OsclRegistryClient::~OsclRegistryClient()
{
    Close();
}

OSCL_EXPORT_REF int32 OsclRegistryClient::Connect(bool aPerThread)
{
    if (aPerThread)
    {
        //Prevent mixing session types.
        if (iGlobalImpl)
            return OsclErrInvalidState;

        //Create TLS-based session.
        if (!iTlsImpl)
        {
            int32 err;
            OSCL_TRY(err, iTlsImpl = OSCL_NEW(OsclRegistryClientTlsImpl, ()););
            if (err != OsclErrNone)
                return err;
        }
        return iTlsImpl->Connect();
    }
    else
    {
        //Prevent mixing session types.
        if (iTlsImpl)
            return OsclErrInvalidState;

        //Create global session
        if (!iGlobalImpl)
        {
            int32 err;
            OSCL_TRY(err, iGlobalImpl = OSCL_NEW(OsclRegistryClientImpl, ()););
            if (err != OsclErrNone)
                return err;
        }
        return iGlobalImpl->Connect();
    }
}

OSCL_EXPORT_REF int32 OsclRegistryClient::Register(OSCL_String& aComp, OsclComponentFactory aFac)
{
    if (iTlsImpl)
        return iTlsImpl->Register(aComp, aFac);

    else if (iGlobalImpl)
        return iGlobalImpl->Register(aComp, aFac);

    else
        return OsclErrInvalidState;
}

OSCL_EXPORT_REF int32 OsclRegistryClient::UnRegister(OSCL_String& aComp)
{
    if (iTlsImpl)
        return iTlsImpl->UnRegister(aComp);

    else if (iGlobalImpl)
        return iGlobalImpl->UnRegister(aComp);

    else
        return OsclErrInvalidState;
}

OSCL_EXPORT_REF void OsclRegistryClient::Close()
{
    if (iTlsImpl)
    {
        iTlsImpl->Close();
        OSCL_DELETE(iTlsImpl);
        iTlsImpl = NULL;
    }
    else if (iGlobalImpl)
    {
        iGlobalImpl->Close();
        OSCL_DELETE(iGlobalImpl);
        iGlobalImpl = NULL;
    }
}



