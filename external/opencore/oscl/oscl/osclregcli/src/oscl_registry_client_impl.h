
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               Oscl Registry Client Impl

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_REGISTRY_CLIENT_IMPL_H_INCLUDED
#define OSCL_REGISTRY_CLIENT_IMPL_H_INCLUDED

#include "oscl_base.h"
#include "osclconfig_proc.h"
#if (OSCL_HAS_SINGLETON_SUPPORT)
//1st choice implementation-- uses Oscl singleton

#include "oscl_registry_serv_impl_global.h"

//client is the same as server
class OsclRegistryClientImpl: public OsclRegistryServImpl
{
};

//access client is also the same as server
class OsclRegistryAccessClientImpl: public OsclRegistryServImpl
{
};

#else //OSCL_HAS_ ...
//3rd choice implementation -- Non-functional stubs.

#include "oscl_vector.h"
#include "oscl_string.h"
#include "oscl_registry_types.h"

class OsclRegistryClientImpl
{
    protected:

        int32 Connect()
        {
            return OsclErrNotSupported;
        }
        void Close() {}

        int32 Register(OSCL_String& , OsclComponentFactory)
        {
            return OsclErrNotSupported;
        }
        int32 UnRegister(OSCL_String&)
        {
            return OsclErrNotSupported;
        }

        //for access client.
        OsclComponentFactory GetFactory(OSCL_String&)
        {
            return NULL;
        }
        void GetFactories(OSCL_String& , Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>&) {}

        friend class OsclRegistryClient;
        friend class OsclRegistryAccessClient;
};

class OsclRegistryAccessClientImpl: public OsclRegistryClientImpl
{
};

#endif //OSCL_HAS_ ...


//TLS-based registry
#include "oscl_registry_serv_impl_tls.h"

//client is the same as server
class OsclRegistryClientTlsImpl: public OsclRegistryServTlsImpl
{
};

//access client is also the same as server
class OsclRegistryAccessClientTlsImpl: public OsclRegistryServTlsImpl
{
};

#endif //OSCL_REGISTRY_IMPL_H_INCLUDED
/*! @} */


