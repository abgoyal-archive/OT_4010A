


#ifndef OSCL_AOSTATUS_H_INCLUDED
#define OSCL_AOSTATUS_H_INCLUDED

#ifndef OSCLCONFIG_PROC_H_INCLUDED
#include "osclconfig_proc.h"
#endif


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

//Request status values.  These are negative so that
//they won't conflict with system error codes.
const int32 OSCL_REQUEST_ERR_NONE = 0;
const int32 OSCL_REQUEST_PENDING = (-0x7fffffff);
const int32 OSCL_REQUEST_ERR_CANCEL = (-1);
const int32 OSCL_REQUEST_ERR_GENERAL = (-2);


//Request status type
class OsclAOStatus
{
    public:
        OSCL_INLINE OsclAOStatus();
        OSCL_INLINE OsclAOStatus(int32 aStatus);
        OSCL_INLINE int32 operator=(int32 aStatus);
        OSCL_INLINE int32 operator==(int32 aStatus) const;
        OSCL_INLINE int32 operator!=(int32 aStatus) const;
        OSCL_INLINE int32 operator>=(int32 aStatus) const;
        OSCL_INLINE int32 operator<=(int32 aStatus) const;
        OSCL_INLINE int32 operator>(int32 aStatus) const;
        OSCL_INLINE int32 operator<(int32 aStatus) const;
        OSCL_INLINE int32 Value() const;
    private:
        int32 iStatus;
};


#if !(OSCL_DISABLE_INLINES)
#include "oscl_aostatus.inl"
#endif


#endif

/*! @} */
