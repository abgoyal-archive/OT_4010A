

#include "oscl_registry_serv_impl_global.h"

#include "osclconfig_proc.h"
#include "oscl_base.h"
#if (OSCL_HAS_SINGLETON_SUPPORT)

//Global variable implementation.

#include "oscl_singleton.h"
#include "oscl_registry_serv_impl.h"

OsclComponentRegistry* OsclRegistryServImpl::GetOsclComponentRegistry()
{
    return (OsclComponentRegistry*)OsclSingletonRegistryEx::getInstance(OSCL_SINGLETON_ID_OSCLREGISTRY);
}

OsclRegistryServImpl::OsclRegistryServImpl()
{
    iIsOpen = false;
}

OsclRegistryServImpl::~OsclRegistryServImpl()
{
    if (iIsOpen)
        Close();
}

int32 OsclRegistryServImpl::Connect()
{
    if (iIsOpen)
        return OsclErrInvalidState;
    //Create the registry on the first session.
    if (!GetOsclComponentRegistry())
    {
        OsclComponentRegistry* reg = NULL;
        int32 err;
        OSCL_TRY(err, reg = OSCL_NEW(OsclComponentRegistry, ()));
        if (err != OsclErrNone)
            return err;

        OsclSingletonRegistryEx::registerInstance(reg, OSCL_SINGLETON_ID_OSCLREGISTRY);
    }
    iIsOpen = true;
    GetOsclComponentRegistry()->OpenSession();
    return OsclErrNone;
}

void OsclRegistryServImpl::Close()
{
    //session cleanup
    if (!iIdVec.empty())
    {
        //unregister all comps that were registered by this session
        for (uint32 i = 0; i < iIdVec.size(); i++)
            GetOsclComponentRegistry()->Unregister(iIdVec[i]);
        //clear our comp list.
        iIdVec.destroy();
    }

    if (iIsOpen)
    {
        GetOsclComponentRegistry()->CloseSession();
        iIsOpen = false;
        //Delete the registry when the session count goes to zero.
        if (GetOsclComponentRegistry()->iNumSessions == 0)
        {
            OSCL_DELETE(GetOsclComponentRegistry());
            OsclSingletonRegistryEx::registerInstance(NULL, OSCL_SINGLETON_ID_OSCLREGISTRY);
        }
    }
}

int32 OsclRegistryServImpl::Register(OSCL_String& aComp, OsclComponentFactory aFac)
{
    if (!IsOpen())
        return OsclErrInvalidState;

    int32 err;
    OSCL_TRY(err, iIdVec.reserve(iIdVec.size() + 1););
    if (err != OsclErrNone)
        return err;

    uint32 id;
    int32 result = GetOsclComponentRegistry()->Register(id, aComp, aFac);

    //save all comp IDs in our session data
    if (result == OsclErrNone)
        iIdVec.push_back(id);//can't leave, already reserved space.

    return result;
}

int32 OsclRegistryServImpl::UnRegister(OSCL_String& aComp)
{
    if (!IsOpen())
        return OsclErrInvalidState;

    return GetOsclComponentRegistry()->Unregister(aComp);
}

OsclComponentFactory OsclRegistryServImpl::GetFactory(OSCL_String& aComp)
{
    if (!IsOpen())
    {
        return NULL;
    }

    return GetOsclComponentRegistry()->FindExact(aComp);
}

void OsclRegistryServImpl::GetFactories(OSCL_String& aReg, Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>& aVec)
{
    if (!IsOpen())
        return;

    GetOsclComponentRegistry()->FindHierarchical(aReg, aVec);
}

#endif //oscl config









