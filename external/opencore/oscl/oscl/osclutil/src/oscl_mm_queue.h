
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ M M _ Q U E U E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MM_QUEUE_H_INCLUDED
#define OSCL_MM_QUEUE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

class Oscl_MM_Queue
{
    public:
        //open driver
        static uint32 Open();
        //close driver
        static uint32 Close();
        //FLUSH_MM_QUEUE
        static uint32 Flush();
        //PUSH_INFO_TO_MM_QUEUE
        static uint32 PushInfo(uint8* aVAddr, uint32 aPAddr);
        //SHOW_MM_QUEUE
        static uint32 ShowQueueInfo();
        //make pmem from cached to non-cached
        static uint32 MakePmemToNonCached(uint8* aVAddr);
    private:
        static int32 m_mm_queue_fd;
        // return value
        static int32 retVal;
};

#endif // OSCL_MM_QUEUE_H_INCLUDED

/*! @} */
