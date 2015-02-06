
#if !defined(AU_UTILS_H)
#define AU_UTILS_H
#include "oscl_types.h"
#include "oscl_media_status.h"
#include "access_unit.h"
#include "pv_gau.h"

OSCL_IMPORT_REF
AccessUnit* Gau2AU(const GAU* gau, AUImplAllocator * au_impl_alloc , MediaStatusClass::status_t& media_status,
                   bool ignore_sample_info = false);

#endif
