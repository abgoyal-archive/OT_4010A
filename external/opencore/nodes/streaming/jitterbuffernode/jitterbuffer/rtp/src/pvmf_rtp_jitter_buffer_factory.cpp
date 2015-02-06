
#ifndef PVMF_RTP_JITTER_BUFFER_FACTORY_H_INCLUDED
#include "pvmf_rtp_jitter_buffer_factory.h"
#endif

#ifndef PVMF_RTP_JITTER_BUFFER_IMPL_H_INCLUDED
#include "pvmf_rtp_jitter_buffer_impl.h"
#endif

OSCL_EXPORT_REF PVMFJitterBuffer* RTPJitterBufferFactory::Create(const PVMFJitterBufferConstructParams& aJBCreationData)
{
    return PVMFRTPJitterBufferImpl::New(aJBCreationData);
}

OSCL_EXPORT_REF void RTPJitterBufferFactory::Destroy(PVMFJitterBuffer*& aJitterBuffer)
{
    OSCL_DELETE(aJitterBuffer);
    aJitterBuffer = NULL;
}
