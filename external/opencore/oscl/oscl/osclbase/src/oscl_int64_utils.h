

#ifndef OSCL_INT64_UTILS_H_INCLUDED
#define OSCL_INT64_UTILS_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

//! The Oscl_Int64_Utils class provides a wrapper for commonly used int64/uint64 operations
class Oscl_Int64_Utils
{
    public:
        OSCL_IMPORT_REF static void set_int64(int64& input_value, const int32 upper, const int32 lower);

        OSCL_IMPORT_REF static int32 get_int64_upper32(const int64& input_value);

        OSCL_IMPORT_REF static int32 get_int64_lower32(const int64& input_value);

        OSCL_IMPORT_REF static int32 get_int64_middle32(const int64& input_value);

        OSCL_IMPORT_REF static void set_uint64(uint64& input_value, const uint32 upper, const uint32 lower);

        OSCL_IMPORT_REF static uint32 get_uint64_upper32(const uint64& input_value);

        OSCL_IMPORT_REF static uint32 get_uint64_lower32(const uint64& input_value);

        OSCL_IMPORT_REF static uint32 get_uint64_middle32(const uint64& input_value);
};

typedef struct OsclInteger64Transport
{
    uint32 iHigh;
    uint32 iLow;
} _OsclInteger64Transport;

#endif

