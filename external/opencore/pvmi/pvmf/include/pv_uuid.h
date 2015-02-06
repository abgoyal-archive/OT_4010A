

#ifndef PV_UUID_H_INCLUDED
#define PV_UUID_H_INCLUDED

#ifndef OSCL_UUID_H_INCLUDED
#include "oscl_uuid.h"
#endif

typedef uint32 PVUid32;

/** PV UUID structure used for unique identification of PV SDK modules and interfaces. */
typedef OsclUuid PVUuid;

#endif

