

#include "time_comparison_utils.h"
#include "media_clock_converter.h"


OSCL_EXPORT_REF bool PVTimeComparisonUtils::IsEarlier(uint32 aTimeA, uint32 aTimeB, uint32& delta)
{
    delta = aTimeB - aTimeA;
    if (delta < WRAP_THRESHOLD)
        return true;
    delta = 0 - delta;
    return false;
}

OSCL_EXPORT_REF PVTimeComparisonUtils::MediaTimeStatus PVTimeComparisonUtils::CheckTimeWindow(PVMFTimestamp aTimeStamp, uint32 aClock,
        uint32 aEarlyMargin, uint32 aLateMargin, uint32 &aDelta)
{
    bool flag = IsEarlier(aClock, aTimeStamp, aDelta);

    if (0 == aDelta)
    {
        return MEDIA_ONTIME_WITHIN_WINDOW;
    }

    if (flag)
    {
        if (aDelta < aEarlyMargin)
            return MEDIA_EARLY_WITHIN_WINDOW;
        else
            return MEDIA_EARLY_OUTSIDE_WINDOW;
    }
    else if (aDelta < aLateMargin)
    {
        return MEDIA_LATE_WITHIN_WINDOW;
    }

    return MEDIA_LATE_OUTSIDE_WINDOW;
}
