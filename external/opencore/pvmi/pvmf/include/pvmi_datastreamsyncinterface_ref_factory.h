
#ifndef PVMI_DATASTREAMSYNCINTERFACE_REF_FACTORY_H_INCLUDED
#define PVMI_DATASTREAMSYNCINTERFACE_REF_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif
#ifndef PVMI_DATASTREAMUSER_INTERFACE_H_INCLUDED
#include "pvmi_datastreamuser_interface.h"
#endif

class PVMIDataStreamSyncInterfaceRefFactory : public PVMFDataStreamFactory
{
    public:
        OSCL_IMPORT_REF PVMIDataStreamSyncInterfaceRefFactory(OSCL_wString& aFileName);
        OSCL_IMPORT_REF PVMIDataStreamSyncInterfaceRefFactory(OsclFileHandle* aFileHandle);
        OSCL_IMPORT_REF virtual ~PVMIDataStreamSyncInterfaceRefFactory();

        // From PVMFDataStreamFactory(currently typedef'ed to PVMFCPMPluginAccessInterfaceFactory)
        PVMFStatus QueryAccessInterfaceUUIDs(Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids);
        PVInterface* CreatePVMFCPMPluginAccessInterface(PVUuid& aUuid);
        void DestroyPVMFCPMPluginAccessInterface(PVUuid& aUuid, PVInterface* aPtr);

        // From PVInterface
        void addRef();
        void removeRef();
        bool queryInterface(const PVUuid& uuid, PVInterface*& iface);

    private:
        OSCL_wHeapString<OsclMemAllocator> iFileName;
        OsclFileHandle* iFileHandle;
};

#endif // PVMI_DATASTREAMSYNCINTERFACE_REF_FACTORY_H_INCLUDED




