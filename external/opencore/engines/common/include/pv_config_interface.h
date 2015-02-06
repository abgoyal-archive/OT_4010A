
#ifndef PV_CONFIG_INTERFACE_H_INCLUDED
#define PV_CONFIG_INTERFACE_H_INCLUDED

#include "oscl_base.h"
#include "oscl_vector.h"

class PVConfigInterface
{
        /**
         * This command provides a list of UUIDs supported by this configuration object.
         * The client can then check whether it also supports the interfaces
         * implemented by the configuration object.  If so, the client needs to call
         * QueryInterface to retrieve the interface object of the specified UID.
         *
         * @param aUuids Output parameter to hold a vector containing the supported UUIDs
         * @param aExactUuidsOnly Turns on/off the retrival of UUIDs with aMimeType as a base type
         * @param aContextData Optional opaque data to be passed back to user with the command response
         * @returns A unique command id for asynchronous completion
         */
        virtual PVCommandId QueryUUID(Oscl_Vector<PVUUID>& aUuids, OsclAny* aContextData = NULL) = 0;

        /**
         * This command provides a pointer to the configuration interface of the specified UUID.
         *
         * @param aUuid The UUID of the desired interface
         * @param aInterfacePtr Output pointer to the desired interface
         * @param aContextData Optional opaque data to be passed back to user with the command response
         * @returns A unique command id for asynchronous completion
         */
        virtual PVCommandId QueryInterface(const PVUUID& aUuid,
                                           PVInterfacePtr& aInterfacePtr,
                                           OsclAny* aContextData = NULL) = 0;
}

#endif // PV_CONFIG_INTERFACE_H_INCLUDED




