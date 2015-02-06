
#ifndef PVMF_COMMON_AUDIO_DECNODE_H_INCLUDE
#define PVMF_COMMON_AUDIO_DECNODE_H_INCLUDE

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

#ifndef OSCL_PRIQUEUE_H_INCLUDED
#include "oscl_priqueue.h"
#endif

#ifndef PVMF_MEDIA_DATA_H_INCLUDED
#include "pvmf_media_data.h"
#endif


struct channelSampleInfo
{
    uint32 desiredChannels;
    uint32 samplingRate;
    uint32 bitsPerSample;
    uint32 num_buffers;
    uint32 buffer_size;
};

#endif
