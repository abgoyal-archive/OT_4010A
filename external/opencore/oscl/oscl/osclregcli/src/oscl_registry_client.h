
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               Oscl Registry Client

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_REGISTRY_CLIENT_H_INCLUDED
#define OSCL_REGISTRY_CLIENT_H_INCLUDED

#ifndef OSCL_REGISTRY_TYPES_H_INCLUDED
#include "oscl_registry_types.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

class OsclRegistryClientImpl;
class OsclRegistryClientTlsImpl;

class OsclRegistryClient : public HeapBase
{
    public:
        OSCL_IMPORT_REF OsclRegistryClient();
        OSCL_IMPORT_REF ~OsclRegistryClient();

        /*!
        ** Create a session.
        ** @param aPerThread: Select per-thread registry instead of global registry.
        ** @return OsclErrNone on success.
        */
        OSCL_IMPORT_REF int32 Connect(bool aPerThread = false);

        /*!
        ** Register a component factory by registry ID and component ID.
        **
        ** @param aComponentID: registry + component mime-string.
        ** @param aFactory: factory function pointer.
        ** @param aParam: component Create param.
        ** @return OsclErrNone on success.
        */
        OSCL_IMPORT_REF int32 Register(OSCL_String& aComponentID, OsclComponentFactory aFactory);

        /*!
        ** Unregister a previously registered component.
        ** @return OsclErrNone on success.
        */
        OSCL_IMPORT_REF int32 UnRegister(OSCL_String& aComponentID);

        /*!
        ** Close and cleanup.  All components registered in this session are
        ** are automatically unregistered.
        */
        OSCL_IMPORT_REF void Close();

    private:
        OsclRegistryClientImpl* iGlobalImpl;
        OsclRegistryClientTlsImpl* iTlsImpl;

};


#endif //OSCL_REGISTRY_CLIENT_H_INCLUDED
/*! @} */


