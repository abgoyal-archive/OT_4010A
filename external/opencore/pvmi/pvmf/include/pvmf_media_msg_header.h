

#ifndef PVMF_MEDIA_MSG_HEADER_H_INCLUDED
#define PVMF_MEDIA_MSG_HEADER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_REFCOUNTER_MEMFRAG_H_INCLUDED
#include "oscl_refcounter_memfrag.h"
#endif

#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif

struct PVMFMediaMsgHeader
{
    PVMFMediaMsgHeader(): timestamp(0),
            duration(0),
            seqnum(0),
            stream_id(0),
            format_id(0)
    {};

    PVMFTimestamp timestamp;
    uint32 duration;
    uint32 seqnum;
    uint32 stream_id;
    PVUid32 format_id;
    OsclRefCounterMemFrag format_spec_info;

};


#endif
