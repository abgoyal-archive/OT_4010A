
#ifndef OSCL_SHARED_LIB_INTERFACE_H_INCLUDED
#define OSCL_SHARED_LIB_INTERFACE_H_INCLUDED

#ifndef OSCL_UUID_H_INCLUDED
#include "oscl_uuid.h"
#endif

class OsclSharedLibraryInterface
{
    public:
        virtual OsclAny* SharedLibraryLookup(const OsclUuid& aInterfaceId) = 0;
        virtual ~OsclSharedLibraryInterface() {}
};
#endif // OSCL_SHARED_LIB_INTERFACE_H_INCLUDED

