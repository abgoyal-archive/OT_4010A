
#ifndef PVMF_JITTER_BUFFER_FACTORY_H
#define PVMF_JITTER_BUFFER_FACTORY_H

#ifndef OSCL_BASE_MACROS_H_INCLUDED
#include "oscl_base_macros.h"
#endif

class PVMFJitterBuffer;
class PVMFJitterBufferConstructParams;

class JitterBufferFactory
{
    public:
        virtual PVMFJitterBuffer* Create(const PVMFJitterBufferConstructParams& aJBCreationData) = 0;

        virtual void Destroy(PVMFJitterBuffer*& aJitterBuffer)  = 0;
};

#endif
