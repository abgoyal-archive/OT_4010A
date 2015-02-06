

#ifndef WCHAR_SIZE_UTILS_H_INCLUDED
#define WCHAR_SIZE_UTILS_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

OSCL_IMPORT_REF void PackWChar4BytesTo2Bytes(uint8* dest, oscl_wchar* src, int nChars);

OSCL_IMPORT_REF void ExpandWChar2BytesTo4Bytes(oscl_wchar* dest, uint16* src, int nChars);

#endif // WCHAR_SIZE_UTILS_H_INCLUDED
