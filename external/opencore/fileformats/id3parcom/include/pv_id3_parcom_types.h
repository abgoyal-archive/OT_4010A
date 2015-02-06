

#ifndef PV_ID3_PARCOM_TYPES_H_INCLUDED
#define PV_ID3_PARCOM_TYPES_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif
#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif
#ifndef OSCL_SHARED_PTR_H_INCLUDED
#include "oscl_shared_ptr.h"
#endif
#ifndef PVMI_KVP_INCLUDED
#include "pvmi_kvp.h"
#endif

/** Enumerated list of supported ID3 versions */
typedef enum
{
    PV_ID3_INVALID_VERSION = -1,    /**< Invalid ID3 version */
    PV_ID3_V1 = 0,      /**< ID3 Version 1 */
    PV_ID3_V1_1 = 1,    /**< ID3 Version 1.1 */
    PV_ID3_V2_2 = 2,    /**< ID3 Version 2.2 */
    PV_ID3_V2_3 = 3,    /**< ID3 Version 2.3 */
    PV_ID3_V2_4 = 4     /**< ID3 Version 2.4 */
} PVID3Version;

/** Shared pointer of a key-value pair */
typedef OsclSharedPtr<PvmiKvp> PvmiKvpSharedPtr;

/** Vector of shared pointer of a key-value pair */
typedef Oscl_Vector<PvmiKvpSharedPtr, OsclMemAllocator> PvmiKvpSharedPtrVector;

#endif // PV_ID3_PARCOM_TYPES_H_INCLUDED


