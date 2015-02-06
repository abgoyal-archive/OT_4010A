
///////////////////////////////////////////////////////////////////////////////
//
// pvmf_streaming_real_interfaces.h
//
// Defines abstract interfaces that will be used by streaming nodes to talk to
// modules providing real media or real data transport functionality.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PVMF_STREAMING_REAL_INTERFACES_INCLUDED
#define PVMF_STREAMING_REAL_INTERFACES_INCLUDED

#include "oscl_mem.h"
#include "oscl_string_containers.h"


class IRealChallengeGen
{
    public:
        IRealChallengeGen() {};
        virtual ~IRealChallengeGen() {};

        virtual bool GetRealChallenge2(OSCL_HeapString<OsclMemAllocator> rc1,
                                       OSCL_HeapString<OsclMemAllocator> &rc2,
                                       OSCL_HeapString<OsclMemAllocator> &sd) = 0;
};

#endif // PVMF_STREAMING_REAL_INTERFACES_INCLUDED


