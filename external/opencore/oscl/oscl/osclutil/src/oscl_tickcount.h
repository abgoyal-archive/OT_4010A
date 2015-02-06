
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ T I C K C O U N T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_TICKCOUNT_H_INCLUDED
#define OSCL_TICKCOUNT_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MAP_H_INCLUDED
#include "oscl_map.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#define OSCLTICKCOUNT_MAX_TICKS 0xffffffff

class OsclTickCount
{
    public:
        /**
         * This function returns the current system tick count
         *
         * @return returns the tick count
         */
        static uint32 TickCount(bool bResetTickCount = 0);

        /**
         * This function returns the tick frequency in ticks
         * per second
         *
         * @return ticks per second
         */
        static uint32 TickCountFrequency();

        /**
         * This function returns the tick period in
         * microseconds per tick
         *
         * @return microseconds per tick
         */
        static uint32 TickCountPeriod();

        /**
         * This function converts ticks to milliseconds
         *
         * @return milliseconds
         */
        static uint32 TicksToMsec(uint32 ticks);

        /**
         * This function converts milliseconds to ticks
         *
         * @return ticks
         */
        static uint32 MsecToTicks(uint32 msec);

        static long long getTimeMs();
        static long long getTimeUs();
};

#if !OSCL_DISABLE_INLINES
#include "oscl_tickcount.inl"
#endif

#endif // OSCL_TICK_UTILS_H_INCLUDED

/*! @} */
