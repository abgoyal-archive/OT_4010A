
#ifndef PVMF_METADATA_INFOMESSAGE_H_INCLUDED
#define PVMF_METADATA_INFOMESSAGE_H_INCLUDED

#ifndef PVMF_METADATAINFOMESSAGE_EXTENSION_H_INCLUDED
#include "pvmf_metadatainfomessage_extension.h"
#endif

class PVMFMetadataInfoMessage : public PVMFMetadataInfoMessageInterface
{
    public:
        /*
         Default constructor. Ref count is initialized to 1.
         This object should always be created with OSCL_NEW.

         Event code is initialized to 0.
         Event UUId is initialized to 0.
         Event data is NULL.
         Next message pointer is NULL
         */
        OSCL_IMPORT_REF PVMFMetadataInfoMessage();

        /*
         Constructor with initialization parameters. Ref count is initialized to 1
         This object should always be created with OSCL_NEW.

         @param aCode Event code value
         @param aUuid UUID for the event code's group
         @param aData Pointer to the event's opaque data (optional)
         @param aNextMsg Pointer to the next message in the list (optional)
         */
        OSCL_IMPORT_REF PVMFMetadataInfoMessage(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aKvpList, int32 aCode = 0, PVUuid aUuid = PVMFMetadataInfoMessageInterfaceUUID);

        /*
         A derived class should override this in order to destroy itself
         properly by calling OSCL_DELETE with its own type name.
         */
        OSCL_IMPORT_REF virtual void destroy();

        /*
         Method to set the event code and UUID

         @param aCode Event code value
         @param aUuid UUID for the event code's group
         */
        OSCL_IMPORT_REF void SetEventCodeUUID(int32 aCode, PVUuid aUuid);

        // From PVMFMetadataInfoMessageInterface
        OSCL_IMPORT_REF void GetCodeUUID(int32& aCode, PVUuid& aUuid);
        OSCL_IMPORT_REF Oscl_Vector<PvmiKvp, OsclMemAllocator>& GetMetadataVector();

        // From PVInterface
        OSCL_IMPORT_REF virtual void addRef();
        OSCL_IMPORT_REF virtual void removeRef();
        OSCL_IMPORT_REF virtual bool queryInterface(const PVUuid& uuid, PVInterface*& iface);

    protected:
        /* Destructor
        */
        OSCL_IMPORT_REF virtual ~PVMFMetadataInfoMessage();

    private:
        int32 iEventCode;
        PVUuid iEventUuid;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataVector;
        int32 iRefCount;
};

#endif // PVMF_METADATA_INFOMESSAGE_H_INCLUDED


