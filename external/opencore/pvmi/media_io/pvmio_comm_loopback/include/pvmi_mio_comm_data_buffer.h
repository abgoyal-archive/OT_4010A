

#ifndef PVMI_MIO_COMM_DATA_BUFFER_H_INCLUDED
#define PVMI_MIO_COMM_DATA_BUFFER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif
#ifndef PVMI_MEDIA_TRANSFER_H_INCLUDED
#include "pvmi_media_transfer.h"
#endif
#ifndef PVMF_MEDIA_DATA_IMPL_H_INCLUDED
#include "pvmf_media_data_impl.h"
#endif

class PvmiMIOCommDataBufferCleanup : public OsclDestructDealloc
{
    public:
        PvmiMIOCommDataBufferCleanup(Oscl_DefAlloc* in_gen_alloc, PvmiMediaTransfer* aMediaInput,
                                     PVMFCommandId aCmdId, OsclAny* aContext = NULL);
        virtual ~PvmiMIOCommDataBufferCleanup() {};
        virtual void destruct_and_dealloc(OsclAny* ptr);

    private:
        Oscl_DefAlloc* gen_alloc;

        // Variables needed to call writeComplete
        PvmiMediaTransfer* iMediaInput;
        PVMFCommandId iCmdId;
        OsclAny* iContext;

        PVLogger *iLogger;
};

class PvmiMIOCommDataBufferAlloc
{
    public:
        PvmiMIOCommDataBufferAlloc(Oscl_DefAlloc* opt_gen_alloc = 0);
        virtual OsclSharedPtr<PVMFMediaDataImpl> allocate(PvmiMediaTransfer* aMediaInput, uint8* aData,
                uint32 aDataLength, PVMFCommandId aCmdId, OsclAny* aContext = NULL);

    private:
        Oscl_DefAlloc* gen_alloc;
        PVLogger* iLogger;
};

#endif // PVMIO_COMM_DATA_BUFFER_H_INCLUDED

