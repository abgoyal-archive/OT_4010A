
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               Oscl Registry Serv Impl

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_REGISTRY_SERV_IMPL_TLS_H_INCLUDED
#define OSCL_REGISTRY_SERV_IMPL_TLS_H_INCLUDED

#include "osclconfig_proc.h"

#include "oscl_registry_serv_impl.h"
#include "oscl_registry_types.h"
#include "oscl_vector.h"
#include "oscl_mem.h"

//TLS-based implementation.
//this handles both the registration client and the access client
//interfaces.
class OsclRegistryServTlsImpl
{
    protected:
        OsclRegistryServTlsImpl();
        virtual ~OsclRegistryServTlsImpl();

        int32 Connect();
        void Close();

        //for registration client
        int32 Register(OSCL_String& aComponentID, OsclComponentFactory aFactory);
        int32 UnRegister(OSCL_String& aComponentID);

        //for access client.
        OsclComponentFactory GetFactory(OSCL_String& aComponent);
        void GetFactories(OSCL_String& aRegistry, Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>& aVec);

        friend class OsclRegistryClient;
        friend class OsclRegistryAccessClient;
    private:
        bool IsOpen() const
        {
            return iIsOpen;
        }
        bool iIsOpen;
        //server data.
        OsclComponentRegistry* GetOsclComponentRegistry();
        //session data.
        Oscl_Vector<uint32, OsclMemAllocator> iIdVec;
};


#endif //OSCL_REGISTRY_IMPL_TLS_H_INCLUDED
/*! @} */


