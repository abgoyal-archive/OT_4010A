

#ifndef PVMF_MEDIA_INPUT_DATA_BUFFER_H_INCLUDED
#define PVMF_MEDIA_INPUT_DATA_BUFFER_H_INCLUDED

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
#ifndef OSCL_MEM_MEMPOOL_H_INCLUDED
#include "oscl_mem_mempool.h"
#endif

class PvmfMediaInputDataBufferCleanup : public OsclDestructDealloc
{
    public:
        PvmfMediaInputDataBufferCleanup(OsclMemPoolFixedChunkAllocator* in_gen_alloc, PvmiMediaTransfer* aMediaInput,
                                        PVMFCommandId aCmdId, OsclAny* aContext = NULL);
        virtual ~PvmfMediaInputDataBufferCleanup() {};
        virtual void destruct_and_dealloc(OsclAny* ptr);

    private:
        OsclMemPoolFixedChunkAllocator* gen_alloc;

        // Variables needed to call writeComplete
        PvmiMediaTransfer* iMediaInput;
        PVMFCommandId iCmdId;
        OsclAny* iContext;

        PVLogger *iLogger;
};

class PvmfMediaInputDataBufferAlloc
{
    public:
        PvmfMediaInputDataBufferAlloc(OsclMemPoolFixedChunkAllocator* opt_gen_alloc = 0);
        virtual OsclSharedPtr<PVMFMediaDataImpl> allocate(PvmiMediaTransfer* aMediaInput, uint8* aData,
                uint32 aDataLength, PVMFCommandId aCmdId, OsclAny* aContext = NULL);

    private:
        OsclMemPoolFixedChunkAllocator* gen_alloc;
        PVLogger* iLogger;
};

#endif // PVMF_MEDIA_INPUT_DATA_BUFFER_H_INCLUDED

