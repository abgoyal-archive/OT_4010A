

#include "pvmf_timestamp.h"


OSCL_EXPORT_REF bool lessthan(PVMFTimestamp& a, PVMFTimestamp& b,
                              PVMFTimestamp threshold = PVMFTIMESTAMP_LESSTHAN_THRESHOLD)
{
    PVMFTimestamp c = b;
    c -= a;
    if (c < threshold)
    {
        return true;
    }
    return false;
}
