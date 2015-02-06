
#ifndef PVMF_CPMPLUGIN_ACCESS_INTERFACE_FACTORY_H_INCLUDED
#define PVMF_CPMPLUGIN_ACCESS_INTERFACE_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif
#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif
#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif
#ifndef PV_INTERFACE_H_INCLUDED
#include "pv_interface.h"
#endif
#ifndef PVMF_RETURN_CODES_H_INCLUDED
#include"pvmf_return_codes.h"
#endif

#define PVMF_CPMPLUGIN_ACCESS_INTERFACE_FACTORY_MIMETYPE "pvxxx/pvmf/cpm/plugin/access_interface_factory"
#define PVMFCPMPluginAccessInterfaceFactoryUuid PVUuid(0x8b0ccfbb,0x48d6,0x4f1d,0xa9,0x9f,0x41,0xb8,0xdb,0xe8,0xf9,0x1c)

class PVMFCPMPluginAccessInterface;
class PVMFDataStreamReadCapacityObserver;

class PVMFCPMPluginAccessInterfaceFactory : public PVInterface
{
    public:
        virtual PVMFStatus QueryAccessInterfaceUUIDs(Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids) = 0;
        virtual PVInterface* CreatePVMFCPMPluginAccessInterface(PVUuid& aUuid) = 0;
        virtual void DestroyPVMFCPMPluginAccessInterface(PVUuid& aUuid,
                PVInterface* aPtr) = 0;
        /**
        * Sometimes datastream implementations need to rely on an external source
        * to provide them with updated stream size (take a case of progressive dowmnload
        * where in module A is doing the download and module B is doing the reads). This
        * API is one way to make these datastreams connect with each other.
        */
        virtual void SetStreamReadCapacityObserver(PVMFDataStreamReadCapacityObserver* aObs)
        {
            OSCL_UNUSED_ARG(aObs);
            OSCL_LEAVE(OsclErrNotSupported);
        }
};

#endif //PVMF_CPMPLUGIN_ACCESS_INTERFACE_FACTORY_H_INCLUDED

