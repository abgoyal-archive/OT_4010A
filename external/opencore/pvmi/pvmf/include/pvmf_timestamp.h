
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     P V M F _ T I M E S T A M P

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


#ifndef PVMF_TIMESTAMP_H_INCLUDED
#define PVMF_TIMESTAMP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


typedef uint32 PVMFTimestamp;

const PVMFTimestamp PVMFTIMESTAMP_LESSTHAN_THRESHOLD = 0x80000000;

OSCL_IMPORT_REF bool lessthan(const PVMFTimestamp& a, const PVMFTimestamp& b,
                              PVMFTimestamp threshold = PVMFTIMESTAMP_LESSTHAN_THRESHOLD);


typedef struct PVMFTimeValue
{
    uint32 time_val;
    uint32 time_units;
} _PVMFTimeValue;

#endif
