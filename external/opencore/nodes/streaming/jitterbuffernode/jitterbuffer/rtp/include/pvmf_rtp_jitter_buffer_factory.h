
#ifndef PVMF_RTP_JITTER_BUFFER_FACTORY_H_INCLUDED
#define PVMF_RTP_JITTER_BUFFER_FACTORY_H_INCLUDED

#ifndef PVMF_JITTER_BUFFER_FACTORY_H
#include "pvmf_jitter_buffer_factory.h"
#endif

class RTPJitterBufferFactory: public JitterBufferFactory
{
    public:
        OSCL_IMPORT_REF virtual PVMFJitterBuffer* Create(const PVMFJitterBufferConstructParams& aJBCreationData);
        OSCL_IMPORT_REF virtual void Destroy(PVMFJitterBuffer*& aJitterBuffer);
};
#endif


